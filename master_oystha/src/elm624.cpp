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

elm624::elm624(string Device, string xmlfile) {
	this->xmlfile = xmlfile;
	fd = 0;
	if(this->xmlfile.length() > 0) {
		setCommands();
	}
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
			cerr << "Open of '" << Device << "' failed, do you have sufficient privileges?" << endl;
			exit(2);
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
	     *  in the comments, but we don't need them here.
	     *  '>' is used as end of file since this is the response when the elm chip is ready for input.
	     */
		newtio.c_cc[VINTR]    = 0;     // Ctrl-c
		newtio.c_cc[VQUIT]    = 0;     // Ctrl-'\'
		newtio.c_cc[VERASE]   = 0;     // del
		newtio.c_cc[VKILL]    = 0;     // @
		newtio.c_cc[VEOF]     = '>';   // Ctrl-d
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
 * 	 number of bytes read.
 */
int elm624::Read(char* Buffer){
	int res = read(fd,Buffer,255);
	if(res < 0) return -1;
	Buffer[res] = 0;
	return res;
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
		return writeString(commands.checkSync);
	} else if(cmd == "identify") {
		return writeString(commands.identify);
	} else if(cmd == "resetAll") {
		return writeString(commands.resetAll);
	} else if(cmd == "setDefaults") {
		return writeString(commands.setDefaults);
	} else if(cmd == "useVersionTwo") {
		return writeString(commands.useVersionTwo);
	} else if(strncmp(cmd.c_str(), "send", 4) == 0) {
		tmp = cmd.substr(4);
		if(tmp == "Pulse") {
			return writeString(commands.sendPulse);
		} else if(tmp == "OnlyChange") {
			return writeString(commands.sendOnlyChanges);
		} else if(tmp == "All") {
			return writeString(commands.sendAll);
		} else if(tmp == "FormatedData") {
			return writeString(commands.sendFormatedData);
		} else if(tmp == "RawData") {
			return writeString(commands.sendRawData);
		}
	} else if(strncmp(cmd.c_str(), "echo", 4) == 0) {
		tmp = cmd.substr(4);
		if(tmp == "Off") {
			return writeString(commands.echoOff);
		} else if(tmp == "On") {
			return writeString(commands.echoOn);
		}
	} else if(strncmp(cmd.c_str(), "quiet", 5) == 0) {
		tmp = cmd.substr(5);
		if(tmp == "Off") {
			return writeString(commands.quietOff);
		} else if(tmp == "On") {
			return writeString(commands.quietOn);
		}
	} else if(strncmp(cmd.c_str(), "duplicate", 9) == 0) {
		tmp = cmd.substr(9);
		if(tmp == "Off") {
			return writeString(commands.duplicateOff);
		} else if(tmp == "On") {
			return writeString(commands.duplicateOn);
		}
	} else if(strncmp(cmd.c_str(), "linefeed", 8) == 0) {
		tmp = cmd.substr(8);
		if(tmp == "Off") {
			return writeString(commands.linefeedOff);
		} else if(tmp == "On") {
			return writeString(commands.linefeedOn);
		}
	} else if(cmd == "triggerPulseOnWord") {
		return writeString(commands.triggerPulseOnWord);
	} else if(cmd == "repeatCommand") {
		return writeString(commands.repeatCommand);
	} else if(cmd == "monitorAllMessages") {
		return writeString(commands.monitorAllMessages);
	}
	cerr << "elm624::writeCmd ";
	cerr << "WARNING: no match for command '" << cmd << "'." << endl;
	return -1;
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
				commands.useVersionTwo		= doc->getChildValue("elm", i, "useVersionTwo");
				commands.sendAll			= doc->getChildValue("elm", i, "sendAll");
				commands.sendOnlyChanges	= doc->getChildValue("elm", i, "sendOnlyChanges");
				commands.checkSync			= doc->getChildValue("elm", i, "checkSync");
				commands.setDefaults		= doc->getChildValue("elm", i, "setDefaults");
				commands.duplicateOff		= doc->getChildValue("elm", i, "duplicateOff");
				commands.duplicateOn		= doc->getChildValue("elm", i, "duplicateOn");
				commands.echoOff		 	= doc->getChildValue("elm", i, "echoOff");
				commands.echoOn				= doc->getChildValue("elm", i, "echoOn");
				commands.sendFormatedData	= doc->getChildValue("elm", i, "sendFormatedData");
				commands.identify			= doc->getChildValue("elm", i, "identify");
				commands.linefeedOff		= doc->getChildValue("elm", i, "linefeedOff");
				commands.linefeedOn	 		= doc->getChildValue("elm", i, "linefeedOn");
				commands.monitorAllMessages	= doc->getChildValue("elm", i, "monitorAllMessages");
				commands.quietOff			= doc->getChildValue("elm", i, "quietOff");
				commands.quietOn			= doc->getChildValue("elm", i, "quietOn");
				commands.sendRawData		= doc->getChildValue("elm", i, "sendRawData");
				commands.repeatCommand	 	= doc->getChildValue("elm", i, "repeatCommand");
				commands.sendPulse			= doc->getChildValue("elm", i, "sendPulse");
				commands.triggerPulseOnWord	= doc->getChildValue("elm", i, "triggerPulseOnWord");
				commands.resetAll			= doc->getChildValue("elm", i, "resetAll");
				ret = 0;
				break;
			}
		}
		delete doc;
	}
	return ret;
}

void elm624::printCommandValues() {
	cout << "Available commands and their LANC values for elm624:" << endl;
	cout << "  useVersionTwo:      " << commands.useVersionTwo 		<< endl;
	cout << "  sendAll:            " << commands.sendAll 			<< endl;
	cout << "  sendOnlyChanges:    " << commands.sendOnlyChanges 	<< endl;
	cout << "  checkSync:          " << commands.checkSync 			<< endl;
	cout << "  setDefaults:        " << commands.setDefaults 		<< endl;
	cout << "  duplicateOff:       " << commands.duplicateOff 		<< endl;
	cout << "  duplicateOn:        " << commands.duplicateOn 		<< endl;
	cout << "  echoOff:            " << commands.echoOff 			<< endl;
	cout << "  echoOn:             " << commands.echoOn 			<< endl;
	cout << "  sendFormatedData:   " << commands.sendFormatedData 	<< endl;
	cout << "  identify:           " << commands.identify 			<< endl;
	cout << "  linefeedOff:        " << commands.linefeedOff		<< endl;
	cout << "  linefeedOn:         " << commands.linefeedOn 		<< endl;
	cout << "  monitorAllMessages: " << commands.monitorAllMessages << endl;
	cout << "  quietOff:           " << commands.quietOff 			<< endl;
	cout << "  quietOn:            " << commands.quietOn 			<< endl;
	cout << "  sendRawData:        " << commands.sendRawData 		<< endl;
	cout << "  repeatCommand:      " << commands.repeatCommand 		<< endl;
	cout << "  sendPulse:          " << commands.sendPulse 			<< endl;
	cout << "  triggerPulseOnWord: " << commands.triggerPulseOnWord << endl;
	cout << "  resetAll:           " << commands.resetAll 			<< endl;
}
/*
 * Function for testing, send commands all commands to chip.
 * Sleep between commands to allow chip to handle request.
 */
void elm624::testCommands() {
	writeCommand("elm_useVersionTwo");
	sleep(1);
	writeCommand("elm_sendAll");
	sleep(1);
	writeCommand("elm_sendOnlyChange");
	sleep(1);
	writeCommand("elm_checkSync");
	sleep(1);
	writeCommand("elm_setDefaults");
	sleep(1);
	writeCommand("elm_duplicateOff");
	sleep(1);
	writeCommand("elm_duplicateOn");
	sleep(1);
	writeCommand("elm_echoOff");
	sleep(1);
	writeCommand("elm_echoOn");
	sleep(1);
	writeCommand("elm_sendFormatedData");
	sleep(1);
	writeCommand("elm_identify");
	sleep(1);
	writeCommand("elm_linefeedOff");
	sleep(1);
	writeCommand("elm_linefeedOff");
	sleep(1);
	writeCommand("elm_monitorAllMessages");
	sleep(1);
	writeCommand("elm_quietOff");
	sleep(1);
	writeCommand("elm_quietOn");
	sleep(1);
	writeCommand("elm_sendRawData");
	sleep(1);
	writeCommand("elm_repeatCommand");
	sleep(1);
	writeCommand("elm_sendPulse");
	sleep(1);
	writeCommand("elm_triggerPulseOnWord");
	sleep(1);
	writeCommand("elm_resetAll");
	sleep(1);
}

