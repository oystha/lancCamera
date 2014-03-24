/*
 * initXML.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: oystha
 */
#include <iostream>
#include <string>

#include "XmlDomDocument.h"
#include "initXML.h"

initXML::initXML(const char* xmlfile) : XmlDomDocument(xmlfile) {
	DEFAULT = getElementNumber("root", "camera", "model", "default");
}

int initXML::getElementNumber(const char* parentTag, const char* childTag, const char* attributeTag, const char* attributeValue) {
	for (int i = 0; i < this->getChildCount(parentTag, 0, childTag); i++) {
		if(this->getAttributeValue(childTag, i, attributeTag) == attributeValue){
			return i;
		}
	}
	return -1;
}
/*
 * Return value of childTag. If child doesn't have give tag function return value of tag from default class.
 * If tag doesn't exist return "NULL".
 */
string initXML::getValue(const char* parentTag, int childIndex, const char* childTag) {
	string tmp = getChildValue(parentTag, childIndex, childTag);
	if(tmp.size() > 0) {
		return tmp;
	}
	tmp = getChildValue(parentTag, DEFAULT, childTag);
	if(tmp.size() > 0) {
		return tmp;
	} else {
		return "NULL";
	}
}


