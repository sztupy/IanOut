#ifndef __TEXTUTIL_H
#define __TEXTUTIL_H

#include "engine.h"
#include <string>
#include "utils.h"

#include "../TinyXML/tinyxml.h"

extern TIniStr *LINIStr[75];

namespace textutil {
	extern int page;

	std::string CutString(std::string duma, int length, int number);
	void DisplayFrameRate(void);
	void WriteFromProf(int x,int y,int c,int i3,std::string fnam,std::string master,std::string slave);
	std::string GetFromProf(std::string fnam,std::string master,std::string slave);
	void AddString(std::string duma,int i2);
	void DrawStuff();

	const TiXmlElement *XMLSearchNode(const TiXmlElement* iXML, std::string location);
	std::string GetFromXML(std::string fname,std::string location);
	void WriteFromProf(int x,int y,int c,int i3,std::string fnam,std::string location);
}

#endif //__TEXTUTIL_H