/*
 * div.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: oystha
 */

#include <iostream>

#include "div.h"

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
	if((int)chr >= 48 && (int)chr <= 57){
		return (int)chr - 48;
	} else if((int)chr >= 65 && (int)chr <= 70){
		return (int)chr - 55;
	} else {
		std::cerr << "Not a HEX char" << std::endl;
		return -1;
	}
}

int isHex(std::string str){
	for(unsigned int i = 0; i<str.size(); i++){
		if(charToInt(str[i]) == -1) return 0;
	}
	return 1;
}

