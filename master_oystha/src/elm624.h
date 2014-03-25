/*
 * elm624.h
 *
 *  Created on: Feb 5, 2014
 *      Author: oystha
 */

#ifndef ELM624_H_
#define ELM624_H_

#define BAUDRATE B9600

#include <string>
#include <mutex>

#include <termios.h>

int charToInt(char);
/*
 *Commands for elm624 with default values
 */
struct elm624Commands_t {
	std::string useVersionTwo 		= "AT2D";
	std::string sendAll 			= "ATC0";
	std::string sendOnlyChanges 	= "ATC1";
	std::string checkSync 			= "ATCS";
	std::string setDefaults 		= "ATD";
	std::string duplicateOff 		= "ATD0";
	std::string duplicateOn 		= "ATD1";
	std::string echoOff 			= "ATE0";
	std::string echoOn 				= "ATE1";
	std::string sendFormatedData 	= "ATFD";
	std::string identify 			= "ATI";
	std::string linefeedOff 		= "ATL0";
	std::string linefeedOn 			= "ATL1";
	std::string monitorAllMessages 	= "ATMA";
	std::string quietOff 			= "ATQ0";
	std::string quietOn 			= "ATQ1";
	std::string sendRawData 		= "ATRD";
	std::string repeatCommand 		= "ATR5";
	std::string sendPulse 			= "ATSP";
	std::string triggerPulseOnWord 	= "ATTP0";
	std::string resetAll 			= "ATZ";
};


class elm624 {
	std::mutex mut_ready;
	std::mutex mut_sync;
	std::mutex mut_ID;
public:

	elm624(std::string Device, std::string xmlfile);
	~elm624();

	int Open(const char* Device);
	int Close();
	int setCommands();
	int Read(char*);
	int writeString(std::string Buffer);
	int writeCommand(std::string cmd);
	std::string getID();
	void setID(std::string);
	bool getSync();
	void setSync(bool b);
	bool getReady();
	void setReady(bool b);
	void printCommandValues();
	void testCommands();

private:
	elm624Commands_t commands;

	std::string ID;
	std::string xmlfile;
	bool ready, sync;
	struct termios oldtio, newtio;
	int fd;
	const char end = '\r';

	int Write(const void* Buffer, int Length);
	int writeBump();
};

#endif /* ELM624_H_ */
