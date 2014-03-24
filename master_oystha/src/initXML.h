/*
 * initXML.h
 *
 *  Created on: Mar 20, 2014
 *      Author: oystha
 */

#ifndef INITXML_H_
#define INITXML_H_

#include "XmlDomDocument.h"

class initXML : public XmlDomDocument
{
public:
	initXML(const char* xmlfile);
	int getDefault() {return DEFAULT;}
	int getElementNumber(const char* parentTag, const char* childTag, const char* attributeTag, const char* attributeValue);
	string getValue(const char* parentTag, int childIndex, const char* childTag);

private:
	initXML();
	initXML(const initXML&);

	int DEFAULT;
};


#endif /* INITXML_H_ */
