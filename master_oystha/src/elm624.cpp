/*
 * elm624.cpp
 *
 *  Created on: Feb 5, 2014
 *      Author: oystha
 */

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <mutex>

#include "initXML.h"
#include "elm624.h"

using namespace std;

//mutex mut_ready;
//pthread_mutex_t mut_ready;
//mutex mut_sync;
//pthread_mutex_t mut_sync;

elm624::elm624(string Device, string xmlfile) {
	this->xmlfile = xmlfile;
	fd = 0;
	setCommands();
	Open(Device.c_str());
	setReady(false);
	sync = false;
}
elm624::~elm624() {
	Close();
}

string elm624::getID() {
	string ret;
	mut_ID.lock();
	ret = ID;
	mut_ID.unlock();
	return ret;
}

void elm624::setID(string id) {
	mut_ID.lock();
	this->ID = id;
	mut_ID.unlock();
}

bool elm624::getSync() {
	bool ret;
	mut_sync.lock();
	ret = sync;
	mut_sync.unlock();
	return ret;
}

void elm624::setSync(bool sync) {
	mut_sync.lock();
	this->sync = sync;
	mut_sync.unlock();
}

bool elm624::getReady() {
	bool ret;
	mut_ready.lock();
	ret = ready;
	mut_ready.unlock();
	return ret;
}

void elm624::setReady(bool ready) {
	mut_ready.lock();
	this->ready = ready;
	mut_ready.unlock();
}

/*
 * Enable B9600 uart on Device.
 * Return 0 on success, else fail.
 */
int elm624::Open(const char *Device) {
		/*
		 *Open modem device for reading and writing and not as controlling tty
	     *because we don't want to get killed if linenoise sends CTRL-C.
	     */
		fd = open(Device, O_RDWR | O_NOCTTY );
		if (fd <0){
			cout << "Open failed, do you have sufficient privileges." << endl;
			return -1;
		}
		/* save current serial port settings */
		tcgetattr(fd,&oldtio);
		/* clear struct for new port settings */
		bzero(&newtio, sizeof(newtio));
		/* CONTROL
		 * BAUDRATE	: Set bps rate. You could also use cfsetispeed and cfsetospeed.
	     * CRTSCTS 	: output hardware flow control (only used if the cable has
	     *            all necessary lines. See sect. 7 of Serial-HOWTO)
	     * CS8     	: 8n1 (8bit,no parity,1 stopbit)
	     * CLOCAL	: local connection, no modem contol
	     * CREAD	: enable receiving characters
	     */
		newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
		/* INPUT
		 * IGNPAR  : ignore bytes with parity errors
		 * ICRNL   : map CR to NL (otherwise a CR input on the other computer
		 *			 will not terminate input)
		 *			 otherwise make device raw (no other input processing)
	     */
		newtio.c_iflag = IGNPAR; // | ICRNL; //elm sends both CR and NL
		/* OUTPUT
		 * Raw output.
		 */
		newtio.c_oflag = 0;
		/*
		 * ICANON  : enable canonical input
	     *   		 disable all echo functionality, and don't send signals to calling program
	     */
		newtio.c_lflag = ICANON;
	    /*
	     *	initialize all control characters
	     *  default values can be found in /usr/include/termios.h, and are given
	     *  in the comments, but we don't need them here
	     */
		newtio.c_cc[VINTR]    = 0;     // Ctrl-c
		newtio.c_cc[VQUIT]    = 0;     // Ctrl-'\'
		newtio.c_cc[VERASE]   = 0;     // del
		newtio.c_cc[VKILL]    = 0;     // @
		newtio.c_cc[VEOF]     = '>';   //4;     // Ctrl-d
		newtio.c_cc[VTIME]    = 1;     // inter-character timer unused
		newtio.c_cc[VMIN]     = 1;     // blocking read until 1 character arrives
		newtio.c_cc[VSWTC]    = 0;     // '\0'
		newtio.c_cc[VSTART]   = 0;     // Ctrl-q
		newtio.c_cc[VSTOP]    = 0;     // Ctrl-s
		newtio.c_cc[VSUSP]    = 0;     // Ctrl-z
		newtio.c_cc[VEOL]     = '\n';  // '\0'
		newtio.c_cc[VREPRINT] = 0;     // Ctrl-r
		newtio.c_cc[VDISCARD] = 0;     // Ctrl-u
		newtio.c_cc[VWERASE]  = 0;     // Ctrl-w
		newtio.c_cc[VLNEXT]   = 0;     // Ctrl-v
		newtio.c_cc[VEOL2]    = 0;     // '\0'
		/*
		 * now clean the modem line and activate the settings for the port
		 */
		tcflush(fd, TCIFLUSH);
		tcsetattr(fd,TCSANOW,&newtio);
		return 0;
}

int elm624::Close() {
	/* restore the old port settings */
	tcsetattr(fd,TCSANOW,&oldtio);
	return close(fd);
}
/*
 * Return values:
 * 	-1 	read fail,
 * 	 0 	prompt(>) read,
 * 	 number of bytes read before '\n'.
 */
int elm624::Read(char* Buffer){
	int res = read(fd,Buffer,255);
	if(res < 0) return -1;
	Buffer[res] = 0;
	return res;
}

/*
 * Return 0 if command was understood and forwarded successfully.
 * -1 on failure
 */
int elm624::writeCommand(string cmd) {
	string tmp;
	if(cmd.find("elm_") > 0) {
		cerr << "elm624::writeCmd ";
		cerr << "WARNING: command '" << cmd << "' should not have arrived here!" << endl;
		return -1;
	}
	cmd = cmd.substr(4);
	if(cmd == "checkSync") {
		return writeString(checkSync);
	} else if(cmd == "identify") {
		return writeString(identify);
	} else if(cmd == "resetAll") {
		return writeString(resetAll);
	} else if(cmd == "setDefaults") {
		return writeString(setDefaults);
	} else if(cmd == "useVersionTwo") {
		return writeString(useVersionTwo);
	} else if(strncmp(cmd.c_str(), "send", 4) == 0) {
		tmp = cmd.substr(4);
		if(tmp == "Pulse") {
			return writeString(sendPulse);
		} else if(tmp == "OnlyChange") {
			return writeString(sendOnlyChanges);
		} else if(tmp == "All") {
			return writeString(sendAll);
		} else if(tmp == "FormatedData") {
			return writeString(sendFormatedData);
		} else if(tmp == "RawData") {
			return writeString(sendRawData);
		}
	} else if(strncmp(cmd.c_str(), "echo", 4) == 0) {
		tmp = cmd.substr(4);
		if(tmp == "Off") {
			return writeString(echoOff);
		} else if(tmp == "On") {
			return writeString(echoOn);
		}
	} else if(strncmp(cmd.c_str(), "quiet", 5) == 0) {
		tmp = cmd.substr(5);
		if(tmp == "Off") {
			return writeString(quietOff);
		} else if(tmp == "On") {
			return writeString(quietOn);
		}
	} else if(strncmp(cmd.c_str(), "duplicate", 9) == 0) {
		tmp = cmd.substr(9);
		if(tmp == "Off") {
			return writeString(duplicateOff);
		} else if(tmp == "On") {
			return writeString(duplicateOn);
		}
	} else if(strncmp(cmd.c_str(), "linefeed", 8) == 0) {
		tmp = cmd.substr(8);
		if(tmp == "Off") {
			return writeString(linefeedOff);
		} else if(tmp == "On") {
			return writeString(linefeedOn);
		}
	} else if(cmd == "triggerPulseOnWord") {
		return writeString(triggerPulseOnWord);
	} else if(cmd == "repeatCommand") {
		return writeString(repeatCommand);
	} else if(cmd == "monitorAllMessages") {
		return writeString(monitorAllMessages);
	}
	cerr << "elm624::writeCmd ";
	cerr << "WARNING: no match for command '" << cmd << "'." << endl;
	return -1;
}

/*
 * Return number of bytes sent. -1 if failed
 */
int elm624::Write(const void *Buffer, int Length) {
	int ret = 0;
	if(!getReady()){
		ret = writeBump();
		//give chip time to return prompt before continuing
		usleep(100000);
	}
	if((ret = write(fd,Buffer,Length)) != Length) {
		ret = -1;
	}
	ret = writeBump();
	mut_ready.lock();
	ready = false;
	mut_ready.unlock();
	return ret;
}
/*
 * Write '\r' to elm.
 */
int elm624::writeBump() {
	return write(fd,&end,1);
}

/*
 * Return number of bytes sent. -1 if failed
 */
int elm624::writeString(string Buffer) {
	return Write(Buffer.c_str(),Buffer.size());
}

/*
 * Get values from xmlfile
 */
int elm624::setCommands(){
	int ret = -1;
	initXML* doc = new initXML(xmlfile.c_str());
	if (doc) {
		for (int i = 0; i < doc->getChildCount("root", 0, "elm"); i++) {
			if(doc->getAttributeValue("elm", i, "type") == "624"){
				useVersionTwo		= doc->getChildValue("elm", i, "useVersionTwo");
				sendAll				= doc->getChildValue("elm", i, "sendAll");
				sendOnlyChanges		= doc->getChildValue("elm", i, "sendOnlyChanges");
				checkSync			= doc->getChildValue("elm", i, "checkSync");
				setDefaults			= doc->getChildValue("elm", i, "setDefaults");
				duplicateOff		= doc->getChildValue("elm", i, "duplicateOff");
				duplicateOn			= doc->getChildValue("elm", i, "duplicateOn");
				echoOff		 		= doc->getChildValue("elm", i, "echoOff");
				echoOn				= doc->getChildValue("elm", i, "echoOn");
				sendFormatedData	= doc->getChildValue("elm", i, "sendFormatedData");
				identify			= doc->getChildValue("elm", i, "identify");
				linefeedOff			= doc->getChildValue("elm", i, "linefeedOff");
				linefeedOn	 		= doc->getChildValue("elm", i, "linefeedOn");
				monitorAllMessages	= doc->getChildValue("elm", i, "monitorAllMessages");
				quietOff			= doc->getChildValue("elm", i, "quietOff");
				quietOn				= doc->getChildValue("elm", i, "quietOn");
				sendRawData			= doc->getChildValue("elm", i, "sendRawData");
				repeatCommand	 	= doc->getChildValue("elm", i, "repeatCommand");
				sendPulse			= doc->getChildValue("elm", i, "sendPulse");
				triggerPulseOnWord	= doc->getChildValue("elm", i, "triggerPulseOnWord");
				resetAll			= doc->getChildValue("elm", i, "resetAll");
				ret = 0;
				break;
			}
		}
		delete doc;
	}
	return ret;
}

void elm624::printCommandValues() {
	cout << "LANC value of available commands:" 			<< endl;
	cout << "useVersionTwo:      " << useVersionTwo 		<< endl;
	cout << "sendAll:            " << sendAll 				<< endl;
	cout << "sendOnlyChanges:    " << sendOnlyChanges 		<< endl;
	cout << "checkSync:          " << checkSync 			<< endl;
	cout << "setDefaults:        " << setDefaults 			<< endl;
	cout << "duplicateOff:       " << duplicateOff 			<< endl;
	cout << "duplicateOn:        " << duplicateOn 			<< endl;
	cout << "echoOff:            " << echoOff 				<< endl;
	cout << "echoOn:             " << echoOn 				<< endl;
	cout << "sendFormatedData:   " << sendFormatedData 		<< endl;
	cout << "identify:           " << identify 				<< endl;
	cout << "linefeedOff:        " << linefeedOff			<< endl;
	cout << "linefeedOn:         " << linefeedOn 			<< endl;
	cout << "monitorAllMessages: " << monitorAllMessages 	<< endl;
	cout << "quietOff:           " << quietOff 				<< endl;
	cout << "quietOn:            " << quietOn 				<< endl;
	cout << "sendRawData:        " << sendRawData 			<< endl;
	cout << "repeatCommand:      " << repeatCommand 		<< endl;
	cout << "sendPulse:          " << sendPulse 			<< endl;
	cout << "triggerPulseOnWord: " << triggerPulseOnWord 	<< endl;
	cout << "resetAll:           " << resetAll 				<< endl;
}

void elm624::testCommands() {
	writeCommand("elm_useVersionTwo");
	//sleep(1);
	writeCommand("elm_sendAll");
	//sleep(1);
	writeCommand("elm_sendOnlyChange");
	//sleep(1);
	writeCommand("elm_checkSync");
	//sleep(1);
	writeCommand("elm_setDefaults");
	//sleep(1);
	writeCommand("elm_duplicateOff");
	//sleep(1);
	writeCommand("elm_duplicateOn");
	//sleep(1);
	writeCommand("elm_echoOff");
	//sleep(1);
	writeCommand("elm_echoOn");
	//sleep(1);
	writeCommand("elm_sendFormatedData");
	//sleep(1);
	writeCommand("elm_identify");
	//sleep(1);
	writeCommand("elm_linefeedOff");
	//sleep(1);
	writeCommand("elm_linefeedOff");
	//sleep(1);
	writeCommand("elm_monitorAllMessages");
	//sleep(1);
	writeCommand("elm_quietOff");
	//sleep(1);
	writeCommand("elm_quietOn");
	//sleep(1);
	writeCommand("elm_sendRawData");
	//sleep(1);
	writeCommand("elm_repeatCommand");
	//sleep(1);
	writeCommand("elm_sendPulse");
	//sleep(1);
	writeCommand("elm_triggerPulseOnWord");
	//sleep(1);
	writeCommand("elm_resetAll");
	//sleep(1);
}

