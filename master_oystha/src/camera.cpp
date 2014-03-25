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
	if(this->xmlfile.length() > 0) {
		setCommands();
	}
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
				commands.MODE				= doc->getValue("camera", i, "mode");
				commands.ENTER				= doc->getValue("camera", i, "enter");
				commands.POWEROFF			= doc->getValue("camera", i, "powerOff");
				commands.EJECT				= doc->getValue("camera", i, "eject");
				commands.STOP				= doc->getValue("camera", i, "stop");
				commands.PAUSE				= doc->getValue("camera", i, "pause");
				commands.STARTSTOP			= doc->getValue("camera", i, "startStop");
				commands.PLAY				= doc->getValue("camera", i, "play");
				commands.REW				= doc->getValue("camera", i, "rew");
				commands.FWD				= doc->getValue("camera", i, "fwd");
				commands.PHOTOCAPTURE		= doc->getValue("camera", i, "photoCapture");
				commands.RECORD		 		= doc->getValue("camera", i, "record");
				commands.EXECUTE			= doc->getValue("camera", i, "execute");
				commands.MENU				= doc->getValue("camera", i, "menu");
				commands.MENUUP				= doc->getValue("camera", i, "menuUp");
				commands.MENUDOWN			= doc->getValue("camera", i, "menuDown");
				commands.MENURIGHT			= doc->getValue("camera", i, "menuRight");
				commands.MENULEFT			= doc->getValue("camera", i, "menuLeft");
				commands.TELE0				= doc->getValue("camera", i, "zoomTele0");
				commands.TELE1				= doc->getValue("camera", i, "zoomTele1");
				commands.TELE2				= doc->getValue("camera", i, "zoomTele2");
				commands.TELE3				= doc->getValue("camera", i, "zoomTele3");
				commands.TELE4				= doc->getValue("camera", i, "zoomTele4");
				commands.TELE5				= doc->getValue("camera", i, "zoomTele5");
				commands.TELE6				= doc->getValue("camera", i, "zoomTele6");
				commands.TELE7				= doc->getValue("camera", i, "zoomTele7");
				commands.WIDE0				= doc->getValue("camera", i, "zoomWide0");
				commands.WIDE1				= doc->getValue("camera", i, "zoomWide1");
				commands.WIDE2				= doc->getValue("camera", i, "zoomWide2");
				commands.WIDE3				= doc->getValue("camera", i, "zoomWide3");
				commands.WIDE4				= doc->getValue("camera", i, "zoomWide4");
				commands.WIDE5				= doc->getValue("camera", i, "zoomWide5");
				commands.WIDE6				= doc->getValue("camera", i, "zoomWide6");
				commands.WIDE7				= doc->getValue("camera", i, "zoomWide7");
				ret = 0;
				break;
			}
			if(i == childCount-1) /*Looked through all children without finding "model".*/
			{
				cerr << "WARNING: model '" << model << "' not found, using default camera values." << endl;
				i = doc->getDefault();
				commands.MODE				= doc->getChildValue("camera", i, "mode");
				commands.ENTER				= doc->getChildValue("camera", i, "enter");
				commands.POWEROFF			= doc->getChildValue("camera", i, "powerOff");
				commands.EJECT				= doc->getChildValue("camera", i, "eject");
				commands.STOP				= doc->getChildValue("camera", i, "stop");
				commands.PAUSE				= doc->getChildValue("camera", i, "pause");
				commands.STARTSTOP			= doc->getChildValue("camera", i, "startStop");
				commands.PLAY				= doc->getChildValue("camera", i, "play");
				commands.REW				= doc->getChildValue("camera", i, "rew");
				commands.FWD				= doc->getChildValue("camera", i, "fwd");
				commands.PHOTOCAPTURE		= doc->getChildValue("camera", i, "photoCapture");
				commands.RECORD		 		= doc->getChildValue("camera", i, "record");
				commands.EXECUTE			= doc->getChildValue("camera", i, "execute");
				commands.MENU				= doc->getChildValue("camera", i, "menu");
				commands.MENUUP				= doc->getChildValue("camera", i, "menuUp");
				commands.MENUDOWN			= doc->getChildValue("camera", i, "menuDown");
				commands.MENURIGHT			= doc->getChildValue("camera", i, "menuRight");
				commands.MENULEFT			= doc->getChildValue("camera", i, "menuLeft");
				commands.TELE0				= doc->getChildValue("camera", i, "zoomTele0");
				commands.TELE1				= doc->getChildValue("camera", i, "zoomTele1");
				commands.TELE2				= doc->getChildValue("camera", i, "zoomTele2");
				commands.TELE3				= doc->getChildValue("camera", i, "zoomTele3");
				commands.TELE4				= doc->getChildValue("camera", i, "zoomTele4");
				commands.TELE5				= doc->getChildValue("camera", i, "zoomTele5");
				commands.TELE6				= doc->getChildValue("camera", i, "zoomTele6");
				commands.TELE7				= doc->getChildValue("camera", i, "zoomTele7");
				commands.WIDE0				= doc->getChildValue("camera", i, "zoomWide0");
				commands.WIDE1				= doc->getChildValue("camera", i, "zoomWide1");
				commands.WIDE2				= doc->getChildValue("camera", i, "zoomWide2");
				commands.WIDE3				= doc->getChildValue("camera", i, "zoomWide3");
				commands.WIDE4				= doc->getChildValue("camera", i, "zoomWide4");
				commands.WIDE5				= doc->getChildValue("camera", i, "zoomWide5");
				commands.WIDE6				= doc->getChildValue("camera", i, "zoomWide6");
				commands.WIDE7				= doc->getChildValue("camera", i, "zoomWide7");
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
		cerr << "camera::writeCommand WARNING: command '" << cmd << "' should not have arrived here!" << endl;
		return -1;
	}
	cmd = cmd.substr(4);
	if(cmd == "mode") {
		value = commands.MODE;
	} else if(cmd == "enter") {
		value = commands.ENTER;
	} else if(cmd == "powerOff") {
		value = commands.POWEROFF;
	} else if(cmd == "eject") {
		value = commands.EJECT;
	} else if(cmd == "stop") {
		value = commands.STOP;
	} else if(cmd == "pause") {
		value = commands.PAUSE;
	} else if(cmd == "startStop") {
		value = commands.STARTSTOP;
	} else if(cmd == "play") {
		value = commands.PLAY;
	} else if(cmd == "fwd") {
		value = commands.FWD;
	} else if(cmd == "rew") {
		value = commands.REW;
	} else if(cmd == "photoCapture") {
		value = commands.PHOTOCAPTURE;
	} else if(cmd == "record") {
		value = commands.RECORD;
	} else if(cmd == "execute") {
		value = commands.EXECUTE;
	} else if(strncmp(cmd.c_str(), "menu", 4) == 0) {
		if(cmd.size() == 4) {
			value = commands.MENU;
		} else {
			cmd = cmd.substr(4);
			if(cmd == "Up") {
				value = commands.MENUUP;
			} else if(cmd == "Down") {
				value = commands.MENUDOWN;
			} else if(cmd == "Right") {
				value = commands.MENURIGHT;
			} else if(cmd == "Left") {
				value = commands.MENULEFT;
			} else {
				cerr << "camera::writeCommand WARNING: command '" << cmd << "'not found." << endl;
				return -1;
			}
		}
	} else if(strncmp(cmd.c_str(),"zoom",4) == 0 && cmd.size() == 9) {
		value = zoom(cmd);
	} else {
		cerr << "camera::writeCommand WARNING: command '" << cmd << "'not found." << endl;
		return -1;
	}
	if(value != "NULL") {
		return Write(value);
	} else {
		cerr << "camera::writeCommand WARNING: command '" << cmd << "' not available." << endl;
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
			return commands.TELE0;
		case '1':
			return commands.TELE1;
		case '2':
			return commands.TELE2;
		case '3':
			return commands.TELE3;
		case '4':
			return commands.TELE4;
		case '5':
			return commands.TELE5;
		case '6':
			return commands.TELE6;
		case '7':
			return commands.TELE7;
		default:
			break;
		}
		break;
	case 'W':
		switch(cmd[8]) {
		case '0':
			return commands.WIDE0;
		case '1':
			return commands.WIDE1;
		case '2':
			return commands.WIDE2;
		case '3':
			return commands.WIDE3;
		case '4':
			return commands.WIDE4;
		case '5':
			return commands.WIDE5;
		case '6':
			return commands.WIDE6;
		case '7':
			return commands.WIDE7;
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
	cout << "Available commands and their LANC values for camera:" << endl;
	cout << "  mode:         " << commands.MODE				<< endl;
	cout << "  enter:        " << commands.ENTER			<< endl;
	cout << "  powerOff:     " << commands.POWEROFF			<< endl;
	cout << "  eject:        " << commands.EJECT			<< endl;
	cout << "  stop:         " << commands.STOP 			<< endl;
	cout << "  pause:        " << commands.PAUSE			<< endl;
	cout << "  startStop:    " << commands.STARTSTOP	 	<< endl;
	cout << "  play:         " << commands.PLAY 			<< endl;
	cout << "  rew:          " << commands.REW 				<< endl;
	cout << "  fwd:          " << commands.FWD 				<< endl;
	cout << "  photoCapture: " << commands.PHOTOCAPTURE 	<< endl;
	cout << "  record:       " << commands.RECORD 			<< endl;
	cout << "  execute:      " << commands.EXECUTE 			<< endl;
	cout << "  menu:         " << commands.MENU 			<< endl;
	cout << "  menuUp:       " << commands.MENUUP 			<< endl;
	cout << "  menuDown:     " << commands.MENUDOWN			<< endl;
	cout << "  menuRight:    " << commands.MENURIGHT		<< endl;
	cout << "  menuLeft:     " << commands.MENULEFT			<< endl;
	cout << "  zoomTele0:    " << commands.TELE0			<< endl;
	cout << "  zoomTele1:    " << commands.TELE1			<< endl;
	cout << "  zoomTele2:    " << commands.TELE2			<< endl;
	cout << "  zoomTele3:    " << commands.TELE3			<< endl;
	cout << "  zoomTele4:    " << commands.TELE4			<< endl;
	cout << "  zoomTele5:    " << commands.TELE5			<< endl;
	cout << "  zoomTele6:    " << commands.TELE6			<< endl;
	cout << "  zoomTele7:    " << commands.TELE7			<< endl;
	cout << "  zoomWide0:    " << commands.WIDE0			<< endl;
	cout << "  zoomWide1:    " << commands.WIDE1			<< endl;
	cout << "  zoomWide2:    " << commands.WIDE2			<< endl;
	cout << "  zoomWide3:    " << commands.WIDE3			<< endl;
	cout << "  zoomWide4:    " << commands.WIDE4			<< endl;
	cout << "  zoomWide5:    " << commands.WIDE5			<< endl;
	cout << "  zoomWide6:    " << commands.WIDE6			<< endl;
	cout << "  zoomWide7:    " << commands.WIDE7			<< endl;
}

