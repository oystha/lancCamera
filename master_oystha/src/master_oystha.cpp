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

string device 	= "/dev/ttyO2";
string model 	= "default";
string xmlfile 	= "sample.xml";
string cmdfile 	= "/usr/share/nginx/html/test.txt";

string username = "root";
string password = "root";
string dataBase = "statusLanc";
string table 	= "Camera";

MYSQL *con;
elm624* elm;
camera* cam;

/*
 * Reads file when the file is modified. Forwards content of file to camera or elm624.
 * Creates file if it doesn't exist.
 */
void commandUpdate(void* file) {
	//cout << "----------------Thread " << this_thread::get_id() << " start----------------" << endl;
	cout << "----------------Thread commandUpdate start----------------" << endl;
	const char* path = (const char*)file;
	int fd, wd, len, i, res;
	/*
	 * Create file 'path' if not existing. Read block calling thread until file is opened by other thread.
	 */
	int handler = open(path, O_RDONLY | O_CREAT );
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
						//cout << "cmd: '" << cmd << "'." << endl;
						elm->writeCommand(cmd);
					} else if(strcmp(cmd, "EXIT") == 0) {
						elm->writeString("EXIT");
						close(handler);
						//exit(0);
						cout << "----------------Thread commandUpdate end------------------" << endl;
						pthread_exit(NULL);
					} else {
						cerr << "Unknown command: '" << cmd << "'." << endl;
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
	cout << "----------------Thread cameraUpdate start-----------------" << endl;
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
			//status message
			if(cam->status->LancToPlain(Buffer)) {
				//cout << "UPDATE" << endl;
				status = cam->status->getStatusString();
				writeStatusToDB(con, table, status);
			}
			break;
		default:
			if(res > 2) {
				Buffer[res-2] = 0;
				if(strcmp(Buffer, "EXIT") == 0){
					//cout << "EXIT" << endl;
					stop=true;
				} else if(strcmp(Buffer, "SYNC OK") == 0){
					//cout << "SYNC OK" << endl;
					pelm->setSync(true);
				} else if(strcmp(Buffer, "NO SYNC") == 0){
					//cout << "NO SYNC" << endl;
					pelm->setSync(false);
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
	cout << "----------------Thread cameraUpdate end-------------------" << endl;
	pthread_exit(NULL);
}

void Test2(){
	cout << "Thread start" << endl;
	for(int i=0; i<MAX_LENGTH; i++) cout << i << endl;
	cout << "Thread end" << endl;
}

/*
 * INPUT ARGUMENTS
 * user SQL
 * pwd SQL
 * PATH command file
 */
int main(int argc, const char **argv){
	//create camera and elm
	//thread receive commands from user, forward to ELM
	//thread read from elm, insert into SQL database
	//join threads
	//delete camera and elm

	cout << "START, argc='" << argc << "'" << endl;
	for(int i=0; i<argc; i++){
		cout << "argv[" << i <<"]: '" << argv[i] << "'." << endl;
	}
	if(argc == 2){
		model = argv[1];
	}
	elm = new elm624(device, xmlfile);
	cam = new camera(model, xmlfile, elm);

	initDB(&con, username, password, dataBase, table);
	if(con == NULL) {
		cerr << "con == NULL" << endl;
	}

	//cam->printCommandValues();
	//elm->printCommandValues();

	thread readUser (commandUpdate, (void*)cmdfile.c_str());
	thread readCam	(cameraUpdate, (void*)elm);

	readUser.join();
	readCam.join();

	//power off camera
	elm->writeString("END OF MAIN");
	elm->writeString("EXIT");
	elm->writeString("185E");
	delete cam;
	delete elm;
	mysql_close(con);
	pthread_exit(NULL);
}
