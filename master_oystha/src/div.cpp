/*
 * div.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: oystha
 */

#include <iostream>
#include <string>
#include <cstring>

#include "div.h"

using namespace std;

/*
 * Return HEX value of char
 */
int charToInt(char chr){
	if((int)chr >= 48 && (int)chr <= 57){
		return (int)chr - 48;
	} else if((int)chr >= 65 && (int)chr <= 70){
		return (int)chr - 55;
	} else {
		std::cerr << "Not a hex char" << std::endl;
		return -1;
	}
}
/*
 * Return HEX value of char
 */
char charToByte(char chr){
	chr = toupper(chr);
	if((int)chr >= 48 && (int)chr <= 57){
		return (int)chr - 48;
	} else if((int)chr >= 65 && (int)chr <= 70){
		return (int)chr - 55;
	} else {
		std::cerr << "Not a HEX char" << std::endl;
		return -1;
	}
}
/*
 * Return 1 if string is a valid hex digit
 */
int isHex(std::string str){
	for(unsigned int i = 0; i<str.size(); i++){
		if(charToInt(str[i]) == -1) return 0;
	}
	return 1;
}

void help() {
	cout << "Argument is mandatory for option if used." << endl;
	cout << "  --dev=PATH                     path for serial communication. E.g. dev=/dev/ttyO2" << endl;
	cout << "  --cmd=PATH                     path for command file. Program read changes in this file and input content" << endl;
	cout << "  --xml=PATH                     path to xmlfile containing LANC values of commands" << endl;
	cout << "  --model=MODEL                  model of camera. Used when getting values from xmlfile" << endl;
	cout << "  --SQL-host=HOST                host for SQL database" << endl;
	cout << "  --SQL-user=USER                user for SQL database" << endl;
	cout << "  --SQL-password=PASSWORD        password for SQL database" << endl;
	cout << "  --SQL-database=DATABASE        name of SQL database" << endl;
	cout << "  --SQL-table=TABLE              name of SQL table in DATABASE" << endl;
	cout << endl;
	cout << "Additional flags" << endl;
	cout << "  --dump                         print value of options" << endl;
	cout << "  --print[=WORD]                 print values of LANC commands for elm624 or camera, default both" << endl;
	cout << "  --help                         display this help and exit" << endl;
	cout << endl;
}

void setOptions(int argc, const char** argv, options_t* options, printValues_t* printValues) {
string tmp;
		for(int i=1; i<argc; i++){
			//cout << "argv[" << i << "]: '" << argv[i] << "'" << endl;
			if(strncmp(argv[i], "--", 2) != 0) {
				cout << "Unknown input value: '" << argv[i] << "'" << endl;
				help();
				exit(1);
			} else if(strcmp(argv[i],"--help") == 0) {
				cout << "MUST BE RUN AS ROOT TO GET ACCESS TO TTYOn." << endl;
				cout << "Program will use default values for unspecified options. Values of options can be shown with --dump."  << endl;
				cout << "To end program write 'EXIT' to command file." << endl;
				help();
				cout << endl;
				cout << "Exit status:" << endl;
				cout << " 0 if OK," << endl;
				cout << " 1 if minor problems (e.g., unknown input)" << endl;
				cout << " 2 if major problems (e.g., cannot access files)" << endl;
				exit(0);
			} else if(strncmp(argv[i], "--dev=", 6) == 0) {
				tmp = argv[i];
				options->device = tmp.substr(6);
			} else if(strncmp(argv[i], "--SQL-", 6) == 0) {
				tmp = argv[i];
				tmp = tmp.substr(6);
				if(strncmp(tmp.c_str(), "host=", 5) == 0) {
					options->SQL.username = tmp.substr(5);
				} else if(strncmp(tmp.c_str(), "user=", 5) == 0) {
					options->SQL.username = tmp.substr(5);
				} else if(strncmp(tmp.c_str(), "password=", 9) == 0) {
					options->SQL.password = tmp.substr(9).c_str();
				} else if(strncmp(tmp.c_str(), "database=", 9) == 0) {
					options->SQL.dataBase = tmp.substr(9);
				} else if(strncmp(tmp.c_str(), "table=", 6) == 0) {
					options->SQL.table = tmp.substr(6);
				} else {
					cout << "Unknown input value: '" << argv[i] << "'" << endl;
					help();
					exit(1);
				}
			} else if(strncmp(argv[i], "--cmd=", 6) == 0) {
				tmp = argv[i];
				options->cmdfile = tmp.substr(6);
			} else if(strncmp(argv[i], "--xml=", 6) == 0) {
				tmp = argv[i];
				tmp = tmp.substr(6);
				if(tmp.length() > 0 && tmp.find(".xml") == string::npos) {
					cerr << "WARNING: "<< tmp << " is not a valid xml file, using default values" << endl;
				} else {
					options->xmlfile = tmp;
				}
			} else if(strncmp(argv[i], "--model=", 8) == 0) {
				tmp = argv[i];
				options->model = tmp.substr(8);
			} else if(strncmp(argv[i], "--dump", 6) == 0) {
				if(strlen(argv[i]) == 6) {
					printValues->options = true;
				}
			} else if(strncmp(argv[i], "--print", 7) == 0) {
				if(strlen(argv[i]) == 7) {
					printValues->elm = true;
					printValues->camera = true;
				} else {
					if(strncmp(argv[i], "--print=elm", 11) == 0) {
						printValues->elm = true;
					} else if(strncmp(argv[i], "--print=cam", 11) == 0) {
						printValues->camera = true;
					} else {
						cout << "Unknown input value: '" << argv[i] << "'" << endl;
						help();
						exit(1);
					}
				}
			} else {
				cout << "Unknown input value: '" << argv[i] << "'" << endl;
				help();
				exit(1);
			}
		}
}
