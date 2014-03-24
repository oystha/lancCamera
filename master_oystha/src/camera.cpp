/*
 * camera.cpp
 *
 *  Created on: Mar 21, 2014
 *      Author: oystha
 */

#include <iostream>
#include <string>

#include "camera.h"
#include "elm624.h"
#include "initXML.h"

using namespace std;

camera::camera (string model, string xmlfile, elm624* elm) {
	this->elm = elm;
	status = new cameraStatus();
	this->model = model;
	this->xmlfile = xmlfile;
	setCommands();
}

camera::~camera() {
	delete status;
}

int camera::setCommands(){
	int ret = -1;
	initXML* doc = new initXML(xmlfile.c_str());
	if (doc) {
		int childCount = doc->getChildCount("root", 0, "camera");
		for (int i = 0; i<childCount; i++) {
			if(doc->getAttributeValue("camera", i, "model") == model) {
				PLAY				= doc->getValue("camera", i, "play");
				STOP				= doc->getValue("camera", i, "stop");
				RECORD		 		= doc->getValue("camera", i, "record");
				PAUSE				= doc->getValue("camera", i, "pause");
				REW					= doc->getValue("camera", i, "rew");
				FWD					= doc->getValue("camera", i, "fwd");
				TELE0				= doc->getValue("camera", i, "zoomTele0");
				TELE1				= doc->getValue("camera", i, "zoomTele1");
				TELE2				= doc->getValue("camera", i, "zoomTele2");
				TELE3				= doc->getValue("camera", i, "zoomTele3");
				TELE4				= doc->getValue("camera", i, "zoomTele4");
				TELE5				= doc->getValue("camera", i, "zoomTele5");
				TELE6				= doc->getValue("camera", i, "zoomTele6");
				TELE7				= doc->getValue("camera", i, "zoomTele7");
				WIDE0				= doc->getValue("camera", i, "zoomWide0");
				WIDE1				= doc->getValue("camera", i, "zoomWide1");
				WIDE2				= doc->getValue("camera", i, "zoomWide2");
				WIDE3				= doc->getValue("camera", i, "zoomWide3");
				WIDE4				= doc->getValue("camera", i, "zoomWide4");
				WIDE5				= doc->getValue("camera", i, "zoomWide5");
				WIDE6				= doc->getValue("camera", i, "zoomWide6");
				WIDE7				= doc->getValue("camera", i, "zoomWide7");
				ret = 0;
				break;
			}
			if(i == childCount-1) /*Looked through all children without finding "model".*/
			{
				cerr << "WARNING: model '" << model << "' not found, using default camera values." << endl;
				i = doc->getDefault();
				PLAY				= doc->getChildValue("camera", i, "play");
				STOP				= doc->getChildValue("camera", i, "stop");
				RECORD		 		= doc->getChildValue("camera", i, "record");
				PAUSE				= doc->getChildValue("camera", i, "pause");
				REW					= doc->getChildValue("camera", i, "rew");
				FWD					= doc->getChildValue("camera", i, "fwd");
				TELE0				= doc->getChildValue("camera", i, "zoomTele0");
				TELE1				= doc->getChildValue("camera", i, "zoomTele1");
				TELE2				= doc->getChildValue("camera", i, "zoomTele2");
				TELE3				= doc->getChildValue("camera", i, "zoomTele3");
				TELE4				= doc->getChildValue("camera", i, "zoomTele4");
				TELE5				= doc->getChildValue("camera", i, "zoomTele5");
				TELE6				= doc->getChildValue("camera", i, "zoomTele6");
				TELE7				= doc->getChildValue("camera", i, "zoomTele7");
				WIDE0				= doc->getChildValue("camera", i, "zoomWide0");
				WIDE1				= doc->getChildValue("camera", i, "zoomWide1");
				WIDE2				= doc->getChildValue("camera", i, "zoomWide2");
				WIDE3				= doc->getChildValue("camera", i, "zoomWide3");
				WIDE4				= doc->getChildValue("camera", i, "zoomWide4");
				WIDE5				= doc->getChildValue("camera", i, "zoomWide5");
				WIDE6				= doc->getChildValue("camera", i, "zoomWide6");
				WIDE7				= doc->getChildValue("camera", i, "zoomWide7");
				break;
			}
		}
		delete doc;
	}
	return ret;
}
/*
 * Write string to camera. Return 0 if bytes sent is equal to string length or -1 of failure.
 */
int camera::Write(string str) {
	return (elm->writeString(str) == str.size()) ? 0 : -1;
}



/*
 * Translates command from plain to LANC and writes the LANC command to the camera.
 * Return 0 if bytes sent is equal to length of LANC command.
 */
int camera::writeCommand(string cmd) {
	string value;
	string tmp;
	if(cmd.find("cam_") > 0) {
		cerr << "camera::writeCommand ";
		cerr << "WARNING: command '" << cmd << "' should not have arrived here!" << endl;
		return -1;
	}
	cmd = cmd.substr(4);
	if(cmd == "play") {
		value = PLAY;
	} else if(cmd == "stop") {
		value = STOP;
	} else if(cmd == "record") {
		value = RECORD;
	} else if(cmd == "fwd") {
		value = FWD;
	} else if(cmd == "rew") {
		value = REW;
	} else if(cmd == "pause") {
		value = PAUSE;
	} else if(strncmp(cmd.c_str(),"zoom",4) == 0 && cmd.size() == 9) {
		value = zoom(cmd);
	} else {
		cerr << "WARNING: command '" << cmd << "'not found." << endl;
		return -1;
	}
	if(value != "NULL") {
		//cout << "WRITE: command '" << cmd << "' with value '"<< value << "'." << endl;
		return Write(value);
	} else {
		cerr << "WARNING: command '" << cmd << "' not available." << endl;
		return -1;
	}
}

/*
 * Translates plain commands beginning with 'zoom' to LANC commands. Returns corresponding LANC command.
 */
string camera::zoom(string cmd) {
	switch(cmd[4]) {
	case 'T':
		switch(cmd[8]) {
		case '0':
			return TELE0;
		case '1':
			return TELE1;
		case '2':
			return TELE2;
		case '3':
			return TELE3;
		case '4':
			return TELE4;
		case '5':
			return TELE5;
		case '6':
			return TELE6;
		case '7':
			return TELE7;
		default:
			break;
		}
		break;
	case 'W':
		switch(cmd[8]) {
		case '0':
			return WIDE0;
		case '1':
			return WIDE1;
		case '2':
			return WIDE2;
		case '3':
			return WIDE3;
		case '4':
			return WIDE4;
		case '5':
			return WIDE5;
		case '6':
			return WIDE6;
		case '7':
			return WIDE7;
		default:
			break;
		}
		break;
	default:
		break;
	}
	cerr << "WARNING: command '" << cmd << "'not found." << endl;
	return "NULL";
}

/*
 * Print LANC values of available commands
 */
void camera::printCommandValues() {
	cout << "LANC value of available commands:" << endl;
	cout << "play:       " 	<< PLAY 	<< endl;
	cout << "stop:       " 	<< STOP 	<< endl;
	cout << "record:     " 	<< RECORD 	<< endl;
	cout << "pause:      "	<< PAUSE	<< endl;
	cout << "rew:        " 	<< REW		<< endl;
	cout << "fwd:        "	<< FWD		<< endl;
	cout << "zoomTele0:  "  << TELE0	<< endl;
	cout << "zoomTele1:  "  << TELE1	<< endl;
	cout << "zoomTele2:  "  << TELE2	<< endl;
	cout << "zoomTele3:  "  << TELE3	<< endl;
	cout << "zoomTele4:  "  << TELE4	<< endl;
	cout << "zoomTele5:  "  << TELE5	<< endl;
	cout << "zoomTele6:  "  << TELE6	<< endl;
	cout << "zoomTele7:  "  << TELE7	<< endl;
	cout << "zoomWide0:  "  << WIDE0	<< endl;
	cout << "zoomWide1:  "  << WIDE1	<< endl;
	cout << "zoomWide2:  "  << WIDE2	<< endl;
	cout << "zoomWide3:  "  << WIDE3	<< endl;
	cout << "zoomWide4:  "  << WIDE4	<< endl;
	cout << "zoomWide5:  "  << WIDE5	<< endl;
	cout << "zoomWide6:  "  << WIDE6	<< endl;
	cout << "zoomWide7:  "  << WIDE7	<< endl;
}

