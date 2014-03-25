/*
 * div.h
 *
 *  Created on: Mar 11, 2014
 *      Author: oystha
 */

#ifndef DIV_H_
#define DIV_H_

#include <string>

struct SQL_options_t {
	std::string host		= "localhost";
	std::string username 	= "root";
	std::string password 	= "root";
	std::string dataBase 	= "statusLanc";
	std::string table 		= "Camera";
};

struct options_t {
	std::string device 		= "/dev/ttyO2";
	std::string model 		= "default";
	std::string xmlfile 	= "";
	std::string cmdfile 	= "commands.txt";

	SQL_options_t SQL;
};

struct printValues_t {
	bool camera = false;
	bool elm = false;
	bool options = false;
};


//helper functions

int charToInt(char);
char charToByte(char chr);
int isHex(std::string);
void setOptions(int argc, const char** argv, options_t* options, printValues_t* printValues);

#endif /* DIV_H_ */
