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
#include "div.h"

using namespace std;

camera::camera (string model, string xmlfile, elm624* elm) {
	this->elm = elm;
	status = new cameraStatus();
	this->model = model;
	this->xmlfile = xmlfile;
	if(this->xmlfile.length() > 0) {
		setCommandValues();
	}
}

camera::~camera() {
	delete status;
}

int camera::setCommandValues(){
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

/*
 * Input is the 4 byte returned from camera in the LANC message structure.
 * Compares values with current states and return true if status has been updated.
 */
bool camera::LancToPlain(char* Buffer){
	status_t copy 		= this->status->getStatus();
	char _5LN 			= charToByte(Buffer[3]);
	char _6HN 			= charToByte(Buffer[4]);
	char _6LN 			= charToByte(Buffer[5]);
	char _7HN 			= charToByte(Buffer[6]);
	char _7LN 			= charToByte(Buffer[7]);
	bool update 		= false;
	string mode 		= copy.mode;
	string tapeSpeed 	= copy.tapeSpeed;
	string audio		= copy.audio;
	string input 		= copy.DV_input;
	string type 		= copy.DV_type;
	int line 			= copy.DV_line;
	switch(Buffer[0]){
	case '0':
		switch(Buffer[1]){
		case '0':
			//initial
			mode = "INITIAL";
			break;
		case '1':
			//is eject
			mode = "IS EJECT";
			break;
		case '2':
			//stop
			mode = "STOP";
			break;
		case '3':
			//fwd
			mode = "FWD";
			break;
		case '4':
			//rec
			mode = "REC";
			break;
		case '6':
			//play
			mode = "PLAY";
			break;
		case '7':
			//play/pause fwd
			mode = "PLAY/PAUSE FWD";
			break;
		case '8':
			//AL insert
			mode = "AL INSERT";
			break;
		default:
			mode = "";
			//cerr << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case '1':
		switch(Buffer[1]){
		case '1':
			//dew cass. out
			mode = "DEW CASS: OUT";
			break;
		case '2':
			//load
			mode = "LOAD";
			break;
		case '4':
			//rec/pause
			mode = "REC/PAUSE";
			break;
		case '7':
			//frame fwd
			mode = "FRAME FWD";
			break;
		case '8':
			//AL ins-pause
			mode = "AL INSERT-PAUSE";
			break;
		default:
			mode = "";
			//cerr << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case '2':
		switch(Buffer[1]){
		case '1':
			//ejecting
			mode = "EJECTING";
			break;
		case '2':
			//cassett busy
			mode = "CASSETT BUSY";
			break;
		case '4':
			//timer-rec
			mode = "TIMER-REC";
			break;
		case '6':
			//x1 fwd
			mode = "X1 FWD";
			break;
		case '7':
			//1/5 fwd
			mode = "1/5 FWD";
			break;
		case '8':
			//AR insert
			mode = "AR INSERT";
			break;
		default:
			mode = "";
			//cerr << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case '3':
		switch(Buffer[1]){
		case '1':
			//unload
			mode = "UNLOAD";
			break;
		case '2':
			//low-battery
			mode = "LOW BATTERY";
			break;
		case '3':
			//go zero/play f.
			mode = "GO ZERO/PLAY F.";
			break;
		case '4':
			//timer-rec s.
			mode = "TIMER-REC S.";
			break;
		case '6':
			//x1 rev
			mode = "X1 REV";
			break;
		case '7':
			//1/5 rev
			mode = "1/5 REV";
			break;
		case '8':
			//AR ins-pause
			mode = "AR INSERT-PAUSE";
			break;
		default:
			mode = "";
			//cerr << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case '4':
		switch(Buffer[1]){
		case '2':
			//dew stop
			mode = "DEW STOP";
			break;
		case '3':
			//fwd mem stop
			mode = "FWD MEM STOP";
			break;
		case '4':
			//AV insert
			mode = "AV INSERT";
			break;
		case '6':
			//cue
			mode = "CUE";
			break;
		case '7':
			//1/10 fwd
			mode = "1/10 FWD";
			break;
		case '8':
			//AL+V insert
			mode = "AL+V INSERT";
			break;
		default:
			mode = "";
			//cerr << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case '5':
		switch(Buffer[1]){
		case '2':
			//emergency
			mode = "EMERGENCY";
			break;
		case '4':
			//AV ins.-pause
			mode = "AV INSERT-PAUSE";
			break;
		case '6':
			//rev
			mode = "REV";
			break;
		case '7':
			//1/10 rev
			mode = "1/10 REV";
			break;
		case '8':
			//AL+V ins-ps
			mode = "AL+V INSERT-PAUSE";
			break;
		default:
			mode = "";
			//cerr << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case '6':
		switch(Buffer[1]){
		case '2':
			//tape end
			mode = "TAPE END";
			break;
		case '4':
			//video insert
			mode = "VIDEO INSERT";
			break;
		case '6':
			//x2/x3 fwd
			mode = "X2/X3 FWD";
			break;
		case '7':
			//frame fwd
			mode = "FRAME FWD";
			break;
		case '8':
			//AR+V insert
			mode = "AR+V INSERT";
			break;
		default:
			mode = "";
			//cerr << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case '7':
		switch(Buffer[1]){
		case '2':
			//tape top
			mode = "TAPE TOP";
			break;
		case '4':
			//video ins-ps
			mode = "VIDEO INSERT-PAUSE";
			break;
		case '6':
			//x2/x3 rev
			mode = "X2/X3 REV";
			break;
		case '7':
			//frame rev
			mode = "FRAME REV";
			break;
		case '8':
			//AL+R ins-ps
			mode = "AL+R INSERT-PAUSE";
			break;
		default:
			//cerr << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case '8':
		switch(Buffer[1]){
		case '3':
			//rew
			mode = "REW";
			break;
		case '4':
			//audio dub
			mode = "AUDIO DUB";
			break;
		case '5':
			//edit search+
			mode = "EDIT SEARCH+";
			break;
		case '6':
			//x9 fwd
			mode = "X9 FWD";
			break;
		default:
			mode = "";
			//cerr << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case '9':
		switch(Buffer[1]){
		case '2':
			//stp after zero
			mode = "STP AFTER ZERO";
			break;
		case '4':
			//a.dub pause
			mode = "AUDIO DUB PAUSE";
			break;
		case '5':
			//edit search-
			mode = "EDIT SEARCH-";
			break;
		case '6':
			//x9 rev
			mode = "X9 REV";
			break;
		case '7':
			//play/pause rev
			mode = "PLAY/PAUSE REV";
			break;
		default:
			mode = "";
			//cerr << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case 'A':
		switch(Buffer[1]){
		case '2':
			//load emer.
			mode = "LOAD EMERGENCY";
			break;
		case '3':
			//auto-play
			mode = "AUTO-PLAY";
			break;
		case '4':
			//cam rec
			mode = "CAM REC";
			break;
		case '5':
			//edit-s fwd
			mode = "EDIT SEARCH FWD";
			break;
		case '6':
			//frame sea. cue
			mode = "FRAME SEARCH CUE";
			break;
		default:
			mode = "";
			//cerr << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case 'B':
		switch(Buffer[1]){
		case '1':
			//unload emerg.
			mode = "UNLOAD EMERGENCY";
			break;
		case '2':
			//stop emerg. 1
			mode = "STOP EMERGENCY 1";
			break;
		case '3':
			//go zero/play r.
			mode = "GO ZERO/PLAY R.";
			break;
		case '4':
			//cam stby
			mode = "CAM STBY";
			break;
		case '5':
			//edit-s rev
			mode = "EDIT SEARCH REV";
			break;
		case '6':
			//frame sea rev.
			mode = "FRAME SEARCH REV";
			break;
		default:
			mode = "";
			//cerr << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case 'C':
		switch(Buffer[1]){
		case '2':
			//stop emerg. 2
			mode = "STOP EMERGENCY 2";
			break;
		case '3':
			//rew mem stop
			mode = "REW MEM STOP";
			break;
		case '6':
			//x14 fwd
			mode = "X14 FWD";
			break;
		default:
			mode = "";
			//cerr << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case 'D':
		switch(Buffer[1]){
		case '3':
			//hi-speed rew
			mode = "HI-SPEED REW";
			break;
		case '6':
			//x14 rev
			mode = "X14 REV";
			break;
		default:
			mode = "";
			//cerr << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case 'E':
		switch(Buffer[1]){
		case '2':
			//stop NC
			mode = "STOP NC";
			break;
		default:
			mode = "";
			//cerr << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	case 'F':
		switch(Buffer[1]){
		case '5':
			//edit pause
			mode = "EDIT PAUSE";
			break;
		default:
			mode = "";
			//cerr << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
			break;
		}
		break;
	default:
		mode = "";
		//cerr << "Unknown value for Byte 4: " << Buffer[0] << Buffer[1] << ", ";
		break;
	}
	if(copy.mode != mode){
		update = true;
		copy.mode = mode;
	}
	switch(Buffer[2]){
	case '1':
		//Status V8 and Hi8
		if((_6LN & 0b11) == 0b00) {
			//tape speed SP (Standard Play)
			tapeSpeed = "SP";
		} else if ((_6LN & 0b11) == 0b01) {
			//tape speed LP (Long Play)
			tapeSpeed = "LP";
		}
		if(copy.tapeSpeed != tapeSpeed){
			update = true;
			copy.tapeSpeed = tapeSpeed;
		}
		if(_6LN & 0b0100) {
			//standard FM-sound
			audio = "standard FM-sound";
		}
		if(_6LN &0b1000) {
			//PCM-sound
			audio = "PCM-sound";
		}
		if(copy.audio != audio){
			update = true;
			copy.audio = audio;
		}
		if(_6HN & 0b0001){
			//camera-mode
		}
		if(_6HN & 0b0010){
			//rec protection / tape pre-end
		}
		if(_6HN & 0b0100){
			//ME
		} else {
			//MP
		}
		if(_6HN & 0b1000){
			//13 micro-m
		} else {
			//10 micro-m
		}
		if(_7LN & 0b0010){
			//camera-mode
		}
		if(_7LN & 0b1000){
			//servo / mechanics on
		}
		if((_7HN & 0b0101) == 0b0000) {
			//tuner
		} else if((_7HN & 0b0101) == 0b0001) {
			//tuner
		} else if ((_7HN & 0b0101) == 0b0101) {
			//sim
		}
		break;
	case '2':
		//decimal counter
		//_6LN 	One's
		//_6HN	Ten's
		//_7LN 	Hundred's
		//_7HN 	Thousand's
		break;
	case '3':
		//real time counter
		//picture counter
		//_6LN 	Seconds One's / Picture One's
		//_6HN	Seconds Ten's / Picture Ten's
		//_7LN 	Minutes One's / Picture Hundred's
		//_7HN 	Minutes Ten's / Picture Thousand's
		if(copy.rt_minutesTens != (int)_7HN || copy.rt_minutesOnes != (int)_7LN){
			update = true;
			copy.rt_minutesTens = (int)_7HN;
			copy.rt_minutesOnes = (int)_7LN;
		}
		if(copy.rt_secondsTens != (int)_6HN || copy.rt_secondsOnes != (int)_6LN){
			update = true;
			copy.rt_secondsTens = (int)_6HN;
			copy.rt_secondsOnes = (int)_6LN;
		}
		break;
	case '4':
		//real time counter
		//_6LN 	Hours One's
		//_6HN	Hours Ten's
		//_7LN	Days One's
		//_7HN 0+1	Days Ten's
		//_7HN 2	RCTC (Rewritable consumer timecode)
		//_7HN 3	sign(1=negativ)(or 0000/1111)

		if(copy.RCTC != (bool)(_7HN & 0b0100)){
			update = true;
			copy.RCTC = (bool)(_7HN & 0b0100);
		}
		//byte 7 is probably not days, changes rapidly...
		//if(copy.rt_daysTens != (_7HN & 0b0011) || copy.rt_daysOnes != _7LN){
		//	update = true;
		//	copy.rt_daysTens = (_7HN & 0b0011);
		//	copy.rt_daysOnes = _7LN;
		//}
		if(copy.rt_hoursTens != (int)_6HN || copy.rt_hoursOnes != (int)_6LN){
			update = true;
			copy.rt_hoursTens = (int)_6HN;
			copy.rt_hoursOnes = (int)_6LN;
		}
		break;
	case '5':
		//remain time
		//_6LN 	Minutes One's
		//_6HN	Minutes Ten's
		//_7LN 	Hours One's
		if(!_7HN & 0b0100){
			//calculating
		}
		break;
	case '7':
		//status Betamax or DV
		if(copy.betamaxDV != (bool)(_6HN & 0b0001)){
			update = true;
			copy.betamaxDV = (bool)(_6HN & 0b0001);
		}
		if(copy.recordProtection != (bool)(_6HN & 0b0010)){
			update = true;
			copy.recordProtection = (bool)(_6HN & 0b0010);
		}
		//_6HN 2+3 Byte 7 DV mode
		switch(_6HN >> 2){
		//DV mode
		case 0b00:
			switch(_7LN){
			case 0b0000:
				//input tuner
				input = "TUNER";
				break;
			case 0b0101:
				//input line
				input = "LINE";
				break;
			case 0b1010:
				//input DV
				input = "DV";
				break;
			}
			switch(_7HN){
			case 0b0000:
				//Line 1
				line = 1;
				break;
			case 0b0101:
				//Line 2
				line = 2;
				break;
			case 0b1010:
				//Line 3
				line = 3;
				break;
			case 0b1111:
				//Line 4
				line = 4;
				break;
			}
			break;
		case 0b01:
			switch(_7LN){
			case 0b0000:
				//player
				type = "PLAYER";
				break;
			case 0b0011:
				//camera
				type = "CAMERA";
				break;
			}
			break;
		default:
			//unknown DV value
			break;
		}
		if(copy.DV_input != input){
			update = true;
			copy.DV_input = input;
		}
		if(copy.DV_line != line){
			update = true;
			copy.DV_line = line;
		}
		if(copy.DV_type != type){
			update = true;
			copy.DV_type = type;
		}
		if((_6LN & 0b11) == 0b00) {
			tapeSpeed = "SP";
		} else if ((_6LN & 0b11) == 0b01) {
			tapeSpeed = "LP";
		}
		if(copy.tapeSpeed != tapeSpeed){
			update = true;
			copy.tapeSpeed = tapeSpeed;
		}
		if(_6LN & 0b0100) {
			audio = "16bit";
		} else {
			audio = "12bit";
		}
		if(copy.audio != audio){
			update = true;
			copy.audio = audio;
		}
		if(copy.servo != (bool)(_6LN & 0b1000)) {
			update = true;
			copy.servo = (bool)(_6LN & 0b1000);
		}
		break;
	case '8':
		//Data Guide
		//_6LN 4
		//_6HN 1
		//_7LN 3=Date, 4=Time
		this->status->setDate_Time(_7LN);
		//_7HN 0
		break;
	case '9':
		//Data Code
		//(Tape + Picture)
		if(copy.date_Time == 3) {
			if(copy.d_monthTens != (int)_7HN || copy.d_monthOnes != (int)_7LN){
				update = true;
				copy.d_monthTens = (int)_7HN;
				copy.d_monthOnes = (int)_7LN;
			}
			if(copy.d_yearTens != (int)_6HN || copy.d_yearOnes != (int)_6LN){
				update = true;
				copy.d_yearTens = (int)_6HN;
				copy.d_yearOnes = (int)_6LN;
			}
		} else if(copy.date_Time == 4) {
			if(copy.t_minutesTens != (int)_7HN || copy.t_minutesOnes != (int)_7LN){
				update = true;
				copy.t_minutesTens = (int)_7HN;
				copy.t_minutesOnes = (int)_7LN;
			}
			if(copy.t_hoursTens != (int)_6HN || copy.t_hoursOnes != (int)_6LN){
				update = true;
				copy.t_hoursTens = (int)_6HN;
				copy.t_hoursOnes = (int)_6LN;
			}
		}
		break;
	case 'A':
		//Data Code
		//(Tape + Picture)
		if(copy.date_Time == 3) {
			if(copy.d_dayTens != (int)_6HN || copy.d_dayOnes != (int)_6LN){
				update = true;
				copy.d_dayTens = (int)_6HN;
				copy.d_dayOnes = (int)_6LN;
			}
		} else if(copy.date_Time == 4) {
			if(copy.t_secondsTens != (int)_6HN || copy.t_secondsOnes != (int)_6LN){
				update = true;
				copy.t_secondsTens = (int)_6HN;
				copy.t_secondsOnes = (int)_6LN;
			}
		}
		break;
	case 'B':
		//Status AVCHD
		if(_6LN & 0b0001){
			//AVCHD lanc mode
		}
		if(_6LN & 0b0010){
			//AVCHD photo mode
		}
		break;
	case 'C':
		//Status Hi8, DV (+GC1)
		if(_7LN & 0b0001) {
			//camera mode / CTL found
		}
		if(_7LN & 0b0100) {
			//edit on
		}
		if(_7LN & 0b1000) {
			//auto Hi8
		}
		break;
	case 'E':
		//Status Photo and Memory Stick
		//photo mode selected
		if(copy.pm_selected != (bool)(_6LN & 0b0001)){
			update = true;
			copy.pm_selected = (bool)(_6LN & 0b0001);
		}
		//memory stick inserted
		if(copy.ms_inserted != (bool)(_6LN & 0b0010)){
			update = true;
			copy.ms_inserted = (bool)(_6LN & 0b0010);
		}
		//memory stick write-protected
		if(copy.ms_writeProtected != (bool)(_6LN & 0b0100)){
			update = true;
			copy.ms_writeProtected = (bool)(_6LN &0b0100);
		}
		//memory stick play
		if(copy.ms_play != (bool)(_6HN & 0b0001)){
			update = true;
			copy.ms_play = (bool)(_6HN & 0b0001);
		}
		//memory stick search
		if(copy.ms_search != (bool)(_6HN & 0b0010)){
			update = true;
			copy.ms_search = (bool)(_6HN & 0b0010);
		}
		//memory stick full
		if(copy.ms_full != (bool)(_6HN & 0b0100)){
			update = true;
			copy.ms_full = (bool)(_6HN & 0b0100);
		}
		//camera with memory stick
		if(copy.ms_available != (bool)(_6HN & 0b1000)){
			update = true;
			copy.ms_available = (bool)(_6HN & 0b1000);
		}
		//photo mode available
		if(copy.pm_available != (bool)(_7LN & 0b0001)){
			update = true;
			copy.pm_available = (bool)(_7LN & 0b0001);
		}
		break;
	default:
		//cout << "Unknown value for Byte 2: " << Buffer[2];
		break;
	}
	if(_5LN & 0b0001){
		//invalid transmitted code
		//not seen yet, error handling?
	}
	//rec protection
	if(copy.recordProtection != (bool)(_5LN & 0b0010)){
		update = true;
		copy.recordProtection = (bool)(_5LN & 0b0010);
	}
	//battery low
	if(copy.battery != (bool)(_5LN & 0b0100)){
		update = true;
		copy.battery = (bool)(_5LN & 0b0100);
	}
	if(_5LN & 0b1000){
		//zero mem / zero found
	}
	if(update){
		status->setStatus(copy);
		//printStatus();
	}
	return update;
}



