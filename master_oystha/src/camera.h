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
	int Write(std::string str);
	std::string zoom(std::string cmd);

	std::string model;
	std::string xmlfile;

	std::string PLAY;
	std::string STOP;
	std::string RECORD;
	std::string PAUSE;
	std::string REW;
	std::string FWD;

	std::string TELE0;
	std::string TELE1;
	std::string TELE2;
	std::string TELE3;
	std::string TELE4;
	std::string TELE5;
	std::string TELE6;
	std::string TELE7;

	std::string WIDE0;
	std::string WIDE1;
	std::string WIDE2;
	std::string WIDE3;
	std::string WIDE4;
	std::string WIDE5;
	std::string WIDE6;
	std::string WIDE7;
};


#endif /* CAMERA_H_ */
