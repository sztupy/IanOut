#include "windows.h"
#include "ddraw.h"

#include "textutil.h"
#include "textfont.h"

int textutil::page = 0;
TIniStr	*LINIStr[75];

std::string szoveg[50];
int szovegnum = 0;

void textutil::DisplayFrameRate(void)
{
    DWORD               time2;
    char                buff[256];
    
    dwFrameCount++;
    time2 = GetTickCount() - dwFrameTime;
    if( time2 > 1000 )
    {
        dwFrames = (dwFrameCount*1000)/time2;
        dwFrameTime = GetTickCount();
        dwFrameCount = 0;
    }
    if( dwFrames == 0 )
    {
        return;
    }
    
    if (dwFrames != dwFramesLast)
    {
        dwFramesLast = dwFrames;
    }
    
    if( dwFrames > 999 )
    {
        dwFrames = 999;
    }
	 
	//textfont::DisplayNum(0,20,3,dwFrames,3);
	buff[0] = (char)((dwFramesLast / 100) + '0');dwFramesLast-=dwFramesLast/100*100;
	buff[1] = (char)((dwFramesLast / 10) + '0');
    buff[2] = (char)((dwFramesLast % 10) + '0');
	buff[3] = ' ';
	buff[4] = 'F';
	buff[5] = 'P';
	buff[6] = 'S';
    buff[7] = '\0';
	std::string x;
	x = buff;
	textfont::IanOutText(0,0,3,x);
}

void textutil::WriteFromProf(int x,int y,int c,int i3,std::string fnam,std::string master,std::string slave)
{
	int i,i2;
	char buf[200];
	std::string xx;
	i2=-1;
	for (i=0; i<75; i++)
	{
		if (LINIStr[i]->FindIt(fnam,master,slave) == 0) i2=i;
	}
	if (i2!=-1)
	{
	 wsprintf(buf,"%s",LINIStr[i2]->Get().c_str());
	 xx = buf;
	 if (i3==1) textfont::IanOutText(x,y,c,xx);
	 if (i3==2) textfont::IanOutTextC(x,y,c,xx);
	 if (i3==3) textfont::IanOutTextR(x,y,c,xx);
	} else
	{
	  GetPrivateProfileString(master.c_str(),slave.c_str(),"",buf,200,fnam.c_str());
	  for (i=1; i<75; i++)
	  {
		LINIStr[i-1]->Set(LINIStr[i]->fname,LINIStr[i]->master,LINIStr[i]->slave,LINIStr[i]->buffer);
	  }
	  xx = buf;
	  LINIStr[74]->Set(fnam,master,slave,buf);
	  if (i3==1) textfont::IanOutText(x,y,c,xx);
	 if (i3==2) textfont::IanOutTextC(x,y,c,xx);
	 if (i3==3) textfont::IanOutTextR(x,y,c,xx);
	}
}

std::string textutil::GetFromProf(std::string fnam,std::string master,std::string slave)
{
	int i,i2;
	char buf[200];
	i2=-1;
	for (i=0; i<75; i++)
	{
		if (LINIStr[i]->FindIt(fnam,master,slave) == 0) i2=i;
	}
	if (i2!=-1)
	{
	 wsprintf(buf,"%s",LINIStr[i2]->Get().c_str());
	 return buf;
	} else
	{
	  GetPrivateProfileString(master.c_str(),slave.c_str(),"",buf,200,fnam.c_str());
	  for (i=1; i<75; i++)
	  {
		LINIStr[i-1]->Set(LINIStr[i]->fname,LINIStr[i]->master,LINIStr[i]->slave,LINIStr[i]->buffer);
	  }
	  LINIStr[74]->Set(fnam,master,slave,buf);
	  return buf;
	}
}

void textutil::AddString(std::string duma,int i2)
{
	int i,i3;
	for (i=0; i<49; i++)
	{
		szoveg[i]=szoveg[i+1];
	}
	if (textfont::TextLength(duma)>160) {
		std::string duma2;
		std::string duma3;
		duma2="";
		duma3="";
		i=0;i3=0;
		while (textfont::TextLength(duma2+duma3)<150){
		  duma3.push_back(duma[i]);
		  if (duma[i]==' ')
		  {
			  duma2+=duma3;
			  duma3="";
			  i3=i;
		  }
		  i++;
		}
		duma.erase(0,i3+1);
		char dumaelo = ' ';
		if (i2) dumaelo = '~'+1;
		szoveg[49]=dumaelo+duma2;
		AddString(duma,0);
	} else {
	 char dumaelo = ' ';
	 if (i2) dumaelo = '~'+1;
	 szoveg[49]=dumaelo+duma;
	}
}

void textutil::DrawStuff()
{
 int i;
 for (i=44-page; i<50-page; i++)
 {
	 textfont::IanOutText((GetMaxX-640)/2+28+(i-50+page),GetMaxY-18+(i-50+page)*10,1,szoveg[i]);
 }
}

std::string textutil::CutString(std::string duma, int length, int number)
{
	int i,i3;
	if (textfont::TextLength(duma)>length) {
		std::string duma2;
		std::string duma3;
		duma2="";
		duma3="";
		i=0;i3=0;
		while (textfont::TextLength(duma2+duma3)<length-10){
		  duma3.push_back(duma[i]);
		  if (duma[i]==' ')
		  {
			  duma2+=duma3;
			  duma3="";
			  i3=i;
		  }
		  i++;
		}
		duma.erase(0,i3+1);
		if (number==1) return duma2; else
		return CutString(duma,length,number-1);		
	} else {
		if (number==1) return duma; else return "";
	}
}

const TiXmlElement *textutil::XMLSearchNode(const TiXmlElement* iXML, std::string location)
{
	char buf[200];
	if (location[0]=='/') {
		location.erase(0,1);
		for (int i=0;i<location.length();i++) {
			if ((location[i]=='.') || (location[i]==':') || (location[i]=='/')) {
				TiXmlNode *nXML;
				nXML = iXML->FirstChild(location.substr(0,i).c_str());
				location.erase(0,i);
				if (nXML) return XMLSearchNode(nXML->ToElement(),location); else return NULL;
			}
		}
		return NULL;
	} else return iXML;
}

std::string XMLSearch(const TiXmlElement* iXML, std::string location)
{
	char buf[200];
	if (location[0]=='.') {
		location.erase(0,1);
		if (iXML->Attribute(location.c_str())) sprintf(buf,"%s",iXML->Attribute(location.c_str())); else sprintf(buf,"");
		return buf;
	} else if (location[0]=='/') {
		location.erase(0,1);
		for (int i=0;i<location.length();i++) {
			if ((location[i]=='.') || (location[i]==':') || (location[i]=='/')) {
				TiXmlNode *nXML;
				nXML = iXML->FirstChild(location.substr(0,i).c_str());
				location.erase(0,i);
				if (nXML) return XMLSearch(nXML->ToElement(),location); else return "";
			}
			if ((location[i]=='-')) {
				std::string locname = location.substr(0,i).c_str();
				location.erase(0,i+1);
				//AddToLog(locname.c_str());
				for (int i=0;i<location.length();i++) {
					if ((location[i]=='=')) {
						std::string attribname = location.substr(0,i).c_str();
						location.erase(0,i+1);
						//AddToLog(attribname.c_str());
						for (int i=0;i<location.length();i++) {
							if ((location[i]=='.') || (location[i]==':') || (location[i]=='/')) {
								std::string attribvalue = location.substr(0,i).c_str();
								//AddToLog(attribvalue.c_str());
								TiXmlNode *nXML;
								//nXML = iXML->FirstChild(locname.c_str());
								location.erase(0,i);
								while (nXML = iXML->IterateChildren(locname.c_str(),nXML)) {
								//	AddToLog("1");
									if (nXML->ToElement()->Attribute(attribname.c_str())) {
										std::string atv = nXML->ToElement()->Attribute(attribname.c_str());
									//	AddToLog("x %s",atv.c_str());
										if (atv == attribvalue) {
										//	AddToLog("2");
											return XMLSearch(nXML->ToElement(),location);
										}
									}
								}
								return "";
							}
						}
						return "";
					}
				}
				return "";
			}
		}
		return "";
	} else if (location[0]==':') {
		TiXmlNode *nXML;
		nXML = iXML->FirstChild();
		if ((nXML) && (nXML->ToText())) return nXML->ToText()->Value(); else return "";
	} else return "";
}

std::string textutil::GetFromXML(std::string fname,std::string location)
{
	std::string realname;
	realname = GetFile(fname);

	int i,i2;
	i2=-1;
	for (i=0; i<75; i++)
	{
		if (LINIStr[i]->FindIt(realname,location,"") == 0) i2=i;
	}
	if (i2==-1) {
		TiXmlDocument XMLD(realname.c_str());
		XMLD.LoadFile();
		for (i=1; i<75; i++)
		{
			LINIStr[i-1]->Set(LINIStr[i]->fname,LINIStr[i]->master,LINIStr[i]->slave,LINIStr[i]->buffer);
		}
		LINIStr[74]->Set(realname,location,"",XMLSearch(XMLD.RootElement(),location));
		return LINIStr[74]->Get();
	} else {
		return LINIStr[i2]->Get();
	}
}

void textutil::WriteFromProf(int x,int y,int c,int i3,std::string fnam,std::string location)
{
	if (i3==1) textfont::IanOutText(x,y,c,GetFromXML(fnam,location));
	if (i3==2) textfont::IanOutTextC(x,y,c,GetFromXML(fnam,location));
	if (i3==3) textfont::IanOutTextR(x,y,c,GetFromXML(fnam,location));
}
