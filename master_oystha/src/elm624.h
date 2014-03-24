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

class elm624 {
	std::mutex mut_ready;
	std::mutex mut_sync;
	std::mutex mut_ID;
public:

	elm624(std::string Device, std::string xmlfile);
	~elm624();

	int Open(const char*);
	int Close();
	int setCommands();
	int Read(char*);
	int ReadThread(char*);
	void ProcessInfo();
	void ProcessCmd(char*);
	void ProcessLanc(char*);
	void PlainToLanc(char*);
	void LancToPlain(char*);
	int Write(const void*, int);
	int writeString(std::string Buffer);
	int writeBump();
	int writeCommand(std::string);
	std::string getID();
	void setID(std::string);
	bool getSync();
	void setSync(bool b);
	bool getReady();
	void setReady(bool b);
	void printCommandValues();
	void testCommands();

private:
	std::string ID;

	std::string xmlfile;

	bool ready, sync;
	struct termios oldtio, newtio;
	int fd;
	const char end = '\r';

	//Commands
	std::string useVersionTwo;
	std::string sendAll;
	std::string sendOnlyChanges;
	std::string checkSync;
	std::string setDefaults;
	std::string duplicateOff;
	std::string duplicateOn;
	std::string echoOff;
	std::string echoOn;
	std::string sendFormatedData;
	std::string identify;
	std::string linefeedOff;
	std::string linefeedOn;
	std::string monitorAllMessages;
	std::string quietOff;
	std::string quietOn;
	std::string sendRawData;
	std::string repeatCommand;
	std::string sendPulse;
	std::string triggerPulseOnWord;
	std::string resetAll;
};

#endif /* ELM624_H_ */
