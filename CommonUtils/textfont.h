#ifndef __TEXTFONT_H
#define __TEXTFONT_H

//#include "global.h"

namespace textfont {
	void DrawChar(int  x,int  y, int  co, char c);
	int TextLength(std::string &str);
	void Write(int x, int y, int jelenx, int co, std::string &str);
	void IanOutText(int x, int y, int co, std::string str);
    void IanOutTextC(int x, int y, int co, std::string str);
	void IanOutTextR(int x, int y, int co, std::string str);
	void IanOutNum1(int x, int y, int c,std::string &str);
	void IanOutNum2(int x, int y, int c,std::string &str);
	void DisplayNum(int x, int y, int c,int i, short count);
}

#endif //__TEXTFONT_H