/*
 * camera.h
 *
 *  Created on: Feb 23, 2014
 *      Author: oystha
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <string>

#include "elm624.h"
#include "cameraStatus.h"

/*
 * Commands for camera with default values
 */
struct cameraCommands_t {
	std::string MODE		= "1805";
	std::string ENTER		= "1816";
	std::string POWEROFF	= "182A";
	std::string EJECT		= "18C2";
	std::string STOP 		= "1830";
	std::string PAUSE		= "1832";
	std::string STARTSTOP 	= "1833";
	std::string PLAY		= "1834";
	std::string REW			= "1836";
	std::string FWD			= "1838";
	std::string PHOTOCAPTURE= "1839";
	std::string RECORD		= "183A";
	std::string EXECUTE		= "18A2";
	//Menu
	std::string MENU		= "189A";
	std::string MENUUP		= "1884";
	std::string MENUDOWN	= "1886";
	std::string MENURIGHT	= "18C2";
	std::string MENULEFT	= "18C4";
	//Variable speed zoom Tele
	std::string TELE0		= "2800";
	std::string TELE1		= "2802";
	std::string TELE2		= "2804";
	std::string TELE3		= "2806";
	std::string TELE4		= "2808";
	std::string TELE5		= "280A";
	std::string TELE6		= "280C";
	std::string TELE7		= "280E";
	//Variable speed zoom Wide
	std::string WIDE0		= "2810";
	std::string WIDE1		= "2812";
	std::string WIDE2		= "2814";
	std::string WIDE3		= "2816";
	std::string WIDE4		= "2818";
	std::string WIDE5		= "281A";
	std::string WIDE6		= "281C";
	std::string WIDE7		= "281E";
};


class camera {
	elm624* elm;
public:
	cameraStatus* status;
	camera(std::string model, std::string xmlfile, elm624* elm);
	~camera();
	int setCommands();
	void printCommandValues();
	int writeCommand(std::string cmd);
	void getStatus(cameraStatus* status) {status = this->status;};
	int setStatus(cameraStatus* status);
private:
	std::string model;
	std::string xmlfile;
	cameraCommands_t commands;

	int Write(std::string str);
	std::string zoom(std::string cmd);
};


#endif /* CAMERA_H_ */
