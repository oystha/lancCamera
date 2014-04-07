//============================================================================
// Name        : master_oystha.cpp
// Author      : Øystein Børslid Haugsdal
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/inotify.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

#include <mysql.h>
#include <my_global.h>

#include "camera.h"
#include "elm624.h"
#include "cameraStatus.h"
#include "statusSQL.h"
#include "div.h"
#include "initXML.h"

#define MAX_LENGTH 255

using namespace std;

MYSQL *con;
elm624* elm;
camera* cam;
options_t options;
printValues_t printValues;

/*
 * Reads file when the file is modified. Forwards content of file to camera or elm624.
 * Creates file if it doesn't exist.
 */
void commandUpdate(void* file) {
	//cout << "----------------Thread commandUpdate start----------------" << endl;
	const char* path = (const char*)file;
	int fd, wd, len, i, res;
	/*
	 * Create file 'path' if not existing. Read block calling thread until file is opened by other thread.
	 */
	int handler = open(path, O_RDONLY | O_CREAT );
	if (handler <0){
		cerr << "commandoUpdate ERROR: Open of '" << path << "' failed." << endl;
		exit(2);
	}
	char buf[sizeof(struct inotify_event) + PATH_MAX];
	char cmd[MAX_LENGTH];
	if((fd = inotify_init()) < 0) {
		error(EXIT_FAILURE, errno, "failed to initialize inotify instance");
	}
	if((wd = inotify_add_watch(fd, path, IN_MODIFY)) < 0) {
		error(EXIT_FAILURE, errno, "failed to add inotify watch '%s'", path);
	}
	while((len = read(fd, buf, sizeof(buf))) > 0) {
		i = 0;
		while(i < len) {
			struct inotify_event *ie = (struct inotify_event*) &buf[i];
			//printf("event occured ");
			if(ie->mask & IN_MODIFY) {
				//printf("%s was modified\n", ie->len ? ie->name : "file");
				if((res = read(handler, cmd, MAX_LENGTH)) > 0) {
					cmd[res-1] = 0;
					if(lseek(handler, 0, SEEK_SET) == -1) {
						cerr << "lseek failed, error: '" << errno << "'." << endl;
					}
					//forward command
					if(strncmp(cmd, "cam_", 4) == 0) {
						cam->writeCommand(cmd);
					} else if(strncmp(cmd, "elm_", 4) == 0) {
						elm->writeCommand(cmd);
					} else if(strcmp(cmd, "EXIT") == 0) {
						elm->writeString("EXIT");
						close(handler);
						if(inotify_rm_watch(fd, wd) < 0) {
							error(EXIT_FAILURE, errno, "failed to remove inotify watch '%s'", path);
						}
						//cout << "----------------Thread commandUpdate end------------------" << endl;
						pthread_exit(NULL);
					} else {
						cerr << "commandoUpdate WARNING: Unknown command: '" << cmd << "'." << endl;
					}
				}
			}
			else
				printf("unexpected event\n");
			i += sizeof(struct inotify_event) + ie->len;
		}
	}
	error(EXIT_FAILURE, len == 0 ? 0 : errno, "failed to read inotify event");
}


void cameraUpdate(void* elm) {
	//cout << "----------------Thread cameraUpdate start-----------------" << endl;
	elm624* pelm = (elm624*)elm;
	int res;
	char Buffer[MAX_LENGTH];
	string status;
	bool stop = false;
	while(!stop){
		res = pelm->Read(Buffer);
		switch(res){
		case -1:
			cerr << "WARNING: Read failed!" << endl;
			break;
		case 0:
			//prompt, ready to receive new command
			pelm->setReady(true);
			break;
		case 10:
			//status message received, SYNC OK
			if(!pelm->getSync()) {
				pelm->setSync(true);
			}
			if(cam->LancToPlain(Buffer)) {
				//status change, UPDATE
				status = cam->getStatusString();
				writeStatusToDB(con, options.SQL.table, status);
			}
			break;
		default:
			if(res > 2) {
				Buffer[res-2] = 0;
				if(strcmp(Buffer, "EXIT") == 0){
					stop=true;
				} else if(strcmp(Buffer, "SYNC OK") == 0){
					pelm->setSync(true);
				} else if(strcmp(Buffer, "NO SYNC") == 0){
					pelm->setSync(false);
					cerr << "NO SYNC" << endl;
				} else if(strncmp(Buffer, "ELM", 3) == 0){
					pelm->setID(Buffer);
				} else {
					//All commands sent to chip should end here as it echos all transmitted commands
				}
			} else {
				/*
				 * Check if line is none empty
				 */
				if(strcmp(Buffer, "\r\n") != 0) {
					cerr << "WARNING cameraUpdate Buffer: '" << Buffer << "'." << endl;
				}
			}
		}
	}
	//cout << "----------------Thread cameraUpdate end-------------------" << endl;
	pthread_exit(NULL);
}

/*
 * MUST BE RUN AS ROOT TO GET ACCESS TO TTYO*
 * DEFAULT VALUES:
 * --dev=/dev/ttyO2
 * --SQL-user=root
 * --SQL-password=root
 * --SQL-database=statusLanc
 * --SQL-table=Camera
 * --cmd=command.txt
 * --xml=LANC.xml
 * --model="model"
 */
int main(int argc, const char **argv){
	//create camera and elm
	//thread receive commands from user, forward to ELM
	//thread read from elm, insert into SQL database
	//join threads
	//delete camera and elm
	if(argc <= 1) {
		cout << "No input given, using default option values. To change values see --help." << endl;
	} else {
		setOptions(argc, argv, &options, &printValues);
	}

	if(printValues.options) {
		cout << "Value of options" 							<< endl;
		cout << "  --dev=" 			<< options.device 		<< endl;
		cout << "  --cmd=" 			<< options.cmdfile 		<< endl;
		if(options.xmlfile.length() > 0 && options.xmlfile.find(".xml") != string::npos) {
			cout << "  --xml=" 		<< options.xmlfile 		<< endl;
		}
		cout << "  --model=" 		<< options.model 		<< endl;
		cout << "  --SQL-host=" 	<< options.SQL.host 	<< endl;
		cout << "  --SQL-user=" 	<< options.SQL.username << endl;
		cout << "  --SQL-password=" << options.SQL.password << endl;
		cout << "  --SQL-database=" << options.SQL.dataBase << endl;
		cout << "  --SQL-table=" 	<< options.SQL.table 	<< endl;
		cout << endl;
	}

	elm = new elm624(options.device, options.xmlfile);
	cam = new camera(options.model, options.xmlfile, elm);

	if(initDB(&con, &options.SQL)) {
		cerr << "ERROR: Failed establishing connection to SQL." << endl;
		cerr << "       user      = " << options.SQL.username 	<< endl;
		cerr << "       password  = " << options.SQL.password 	<< endl;
		cerr << "       database  = " << options.SQL.dataBase 	<< endl;
		cerr << "       table     = " << options.SQL.table 		<< endl;
		exit(2);
	}
	if(printValues.camera) {
		cam->printCommandValues();
	}
	if(printValues.elm) {
		elm->printCommandValues();
	}
	//set default values
	elm->writeString("ATD");
	sleep(1);

	thread readUser (commandUpdate, (void*)options.cmdfile.c_str());
	thread readCam	(cameraUpdate, (void*)elm);

	readUser.join();
	readCam.join();

	//power off camera
	elm->writeString("EXIT");
	elm->writeString("182A");
	elm->writeString("END OF MAIN");
	delete cam;
	delete elm;
	mysql_close(con);
	//cout << "END OF MAIN" << endl;
	exit(0);
}
