#ifndef __IANOS_H
#define __IANOS_H

#include "../commonutils/ItemObj.h"
#include <string>
#include <map>
#include <iostream>
#include "../frmobject/player.h"

#undef ERROR
#undef TRUE
#undef FALSE
#undef max

namespace IanOs {

	namespace GVar {
		extern std::map<std::string,int> GlobalVars;
		int Get(std::string i);
		void Set(std::string i,int i2);
	}

	namespace Text {
		extern std::map<int,std::string> TextValue;
		extern std::map<int,int> ToDoValue;
		extern std::map<int,int> ColorValue;
		extern std::string TextFile;
		extern std::string MainValue;
		extern int TextCount;
		extern int TextPosition;


		void ClearTexts();
		void SetMain(std::string newmain);
		void SetFile(std::string newmain);
		void AddLine(std::string newtext,int newposition, int newcolor);
		void SetPosition(int newpos);
		int GetPosition();
	}

	extern BlockType		*Block;
	extern BlockType		*BigBlock;
	extern PFRMPlayer		ActualPlayer;
	extern PLocationDat	ActualItem;

	void RunStat(std::string statement);
	void f_RunFile(std::string filename);
	
};

#endif //__IANOS_H