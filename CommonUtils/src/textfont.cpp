#include <string>
#include "textfont.h"
#include "engine.h"

namespace textfont {
const int					FontMap[] = {
// * *_* * * *!* *"* *#* *$* *%* *&* *'* *(* *)* *** *+* *,* *-* *.* */* *0* *1* *2* *3* *4* *5* *6* *7* *8* *9* *:* *;* *<* *=* *>* *?* *@* *A* *B* *C* *D* *E* *F* *G* *H* *I* *J* *K* *L* *M* *N* *O* *P* *Q* *R* *S* *T* *U* *V* *W* *X* *Y* *Z* *[* *\* *]* *^* *_* *`* *a* *b* *c* *d* *e* *f* *g* *h* *i* *j* *k* *l* *m* *n* *o* *p* *q* *r* *s* *t* *u* *v* *w* *x* *y* *z* *{* *|* *}* *~* *o*
	  0,  6,  10, 17, 26, 34, 44, 53, 58, 64, 70, 77, 84, 88, 96,100,110,118,124,132,140,148,156,164,172,180,188,192,196,205,213,222,230,239,249,258,266,275,283,291,300,309,313,322,331,339,348,357,366,375,384,393,402,410,419,429,438,448,458,467,473,483,489,498,507,512,520,528,535,543,551,558,566,574,578,585,593,597,606,614,622,630,638,645,653,661,669,677,686,694,702,710,716,720,726,735,742
};
}

int textfont::TextLength(std::string &str)
{
	int length = str.size();
	int jelenx = 0;
	int i;

	for (i=0 ; i<length; i++)
	{
			if ((str[i]!='\n') && 
		    (str[i]!='á') && 
			(str[i]!='é') &&
			(str[i]!='í') &&
			(str[i]!='ó') &&
			(str[i]!='ú') &&
			(str[i]!='ö') &&
			(str[i]!='õ') &&
			(str[i]!='ü') &&
			(str[i]!='û') &&
			(str[i]!='Á') && 
			(str[i]!='É') &&
			(str[i]!='Í') &&
			(str[i]!='Ó') &&
			(str[i]!='Ú') &&
			(str[i]!='Ö') &&
			(str[i]!='Õ') &&
			(str[i]!='Ü') &&
			(str[i]!='Û') &&
		((str[i]>=' ') && (str[i]<='~'+1)))
			
		{ 
			jelenx += FontMap[str[i]-31]-FontMap[str[i]-32];
		} else
	    if (str[i]=='á')
		{
			jelenx += FontMap['a'-31]-FontMap['a'-32];
		} else
		if (str[i]=='é')
		{
			jelenx += FontMap['e'-31]-FontMap['e'-32];
		} else
		if (str[i]=='í')
		{
			jelenx += FontMap['i'-31]-FontMap['i'-32];
		}else
		if (str[i]=='ó')
		{
			jelenx += FontMap['o'-31]-FontMap['o'-32];
		}else
		if (str[i]=='ú')
		{
			jelenx += FontMap['u'-31]-FontMap['u'-32];
		}else
		if ((str[i]=='ö') || (str[i]=='õ'))
		{
			jelenx += FontMap['o'-31]-FontMap['o'-32];
		}else
		if ((str[i]=='ü') || (str[i]=='û'))
		{
			jelenx += FontMap['u'-31]-FontMap['u'-32];
		} else
		if (str[i]=='Á')
		{
			jelenx += FontMap['A'-31]-FontMap['A'-32];
		} else
		if (str[i]=='É')
		{
			jelenx += FontMap['E'-31]-FontMap['E'-32];
		} else
		if (str[i]=='Í')
		{
			jelenx += FontMap['I'-31]-FontMap['I'-32];
		}else
		if (str[i]=='Ó')
		{
	        jelenx += FontMap['O'-31]-FontMap['O'-32];
		}else
		if (str[i]=='Ú')
		{
	     	jelenx += FontMap['U'-31]-FontMap['U'-32];
		}else
		if ((str[i]=='Ö') || (str[i]=='Ö'))
		{
	     	jelenx += FontMap['O'-31]-FontMap['O'-32];
		}else
		if ((str[i]=='Ü') || (str[i]=='Û'))
		{
	     	jelenx += FontMap['U'-31]-FontMap['U'-32];
		}
	}
	return jelenx;
}

void textfont::DrawChar(
			  int  x,
			  int  y,
			  int  co,
			  char c)
{
	SDL_Rect	rcRect;

	rcRect.x   = FontMap[(c-32)]+1;
	rcRect.y    = (co*24)+1;
	rcRect.w  = FontMap[(c-31)]+1 - rcRect.x;
	rcRect.h = (co*24)+12  - rcRect.y;

	if (rcRect.x>467) 
	{
		rcRect.x-=468;
		rcRect.y+=12;
	}

	BlitTo(g_pDDSBack,rcRect.x,rcRect.y,rcRect.x+rcRect.w,rcRect.y+rcRect.h,x,y,0,g_pDDSOne);
}

void textfont::Write(int x, int y, int jelenx, int co, std::string &str)
{
	int		i;
	int		sor = y+3;
	int		length = str.size();
	int		maxsor = 0;
	int		maxjelenx = 0;

	for (i=0 ; i<length; i++)
	{
		if ((str[i]!='\n') && 
		    (str[i]!='á') && 
			(str[i]!='é') &&
			(str[i]!='í') &&
			(str[i]!='ó') &&
			(str[i]!='ú') &&
			(str[i]!='ö') &&
			(str[i]!='õ') &&
			(str[i]!='ü') &&
			(str[i]!='û') &&
			(str[i]!='Á') && 
			(str[i]!='É') &&
			(str[i]!='Í') &&
			(str[i]!='Ó') &&
			(str[i]!='Ú') &&
			(str[i]!='Ö') &&
			(str[i]!='Õ') &&
			(str[i]!='Ü') &&
			(str[i]!='Û') &&
		((str[i]>=' ') && (str[i]<='~'+1)))
			
		{ 
			DrawChar(jelenx,sor,co,str[i]);
			jelenx += FontMap[str[i]-31]-FontMap[str[i]-32];
		} else
		if (str[i]=='\n')
		{
			sor += 12;
			jelenx=x;
		} else
	    if (str[i]=='á')
		{
	        DrawChar(jelenx+1,sor-1,co,'\'');
			DrawChar(jelenx,sor,co,'a');
			jelenx += FontMap['a'-31]-FontMap['a'-32];
		} else
		if (str[i]=='é')
		{
	        DrawChar(jelenx+1,sor-1,co,'\'');
			DrawChar(jelenx,sor,co,'e');
			jelenx += FontMap['e'-31]-FontMap['e'-32];
		} else
		if (str[i]=='í')
		{
	        DrawChar(jelenx-1,sor-1,co,'\'');
			DrawChar(jelenx,sor,co,'i');
			jelenx += FontMap['i'-31]-FontMap['i'-32];
		}else
		if (str[i]=='ó')
		{
	        DrawChar(jelenx+1,sor-1,co,'\'');
			DrawChar(jelenx,sor,co,'o');
			jelenx += FontMap['o'-31]-FontMap['o'-32];
		}else
		if (str[i]=='ú')
		{
	        DrawChar(jelenx+1,sor-1,co,'\'');
			DrawChar(jelenx,sor,co,'u');
			jelenx += FontMap['u'-31]-FontMap['u'-32];
		}else
		if ((str[i]=='ö') || (str[i]=='õ'))
		{
	        DrawChar(jelenx-1,sor-1,co,'"');
			DrawChar(jelenx,sor,co,'o');
			jelenx += FontMap['o'-31]-FontMap['o'-32];
		}else
		if ((str[i]=='ü') || (str[i]=='û'))
		{
	        DrawChar(jelenx-1,sor-1,co,'"');
			DrawChar(jelenx,sor,co,'u');
			jelenx += FontMap['u'-31]-FontMap['u'-32];
		} else
		if (str[i]=='Á')
		{
	        DrawChar(jelenx+5,sor-3,co,'\'');
			DrawChar(jelenx,sor,co,'A');
			jelenx += FontMap['A'-31]-FontMap['A'-32];
		} else
		if (str[i]=='É')
		{
	        DrawChar(jelenx+2,sor-3,co,'\'');
			DrawChar(jelenx,sor,co,'E');
			jelenx += FontMap['E'-31]-FontMap['E'-32];
		} else
		if (str[i]=='Í')
		{
	        DrawChar(jelenx,sor-3,co,'\'');
			DrawChar(jelenx,sor,co,'I');
			jelenx += FontMap['I'-31]-FontMap['I'-32];
		}else
		if (str[i]=='Ó')
		{
	        DrawChar(jelenx+2,sor-3,co,'\'');
			DrawChar(jelenx,sor,co,'O');
			jelenx += FontMap['O'-31]-FontMap['O'-32];
		}else
		if (str[i]=='Ú')
		{
	        DrawChar(jelenx+2,sor-3,co,'\'');
			DrawChar(jelenx,sor,co,'U');
			jelenx += FontMap['U'-31]-FontMap['U'-32];
		}else
		if ((str[i]=='Ö') || (str[i]=='Õ'))
		{
	        DrawChar(jelenx,sor-3,co,'"');
			DrawChar(jelenx,sor,co,'O');
			jelenx += FontMap['O'-31]-FontMap['O'-32];
		}else
		if ((str[i]=='Ü') || (str[i]=='Û'))
		{
	        DrawChar(jelenx,sor-3,co,'"');
			DrawChar(jelenx,sor,co,'U');
			jelenx += FontMap['U'-31]-FontMap['U'-32];
		}
		if (jelenx>maxjelenx) maxjelenx=jelenx;
		if (sor+13>maxsor) maxsor=sor+13;
	}
}

void textfont::IanOutText(int x, int y, int co, std::string str)
{
	//int		i;
	int		jelenx = x;
	//int		sor = y+3;
	int	length = str.size();
//	int		maxjelenx = 0;
//	int		maxsor = 0;
	/*int hRet;
	HDC     hdc;

	if (co==11)
	{
		
      hRet = g_pDDSBack->GetDC(&hdc);
	  if (!FAILED(hRet))
	  {
		
		SetBkMode(hdc,TRANSPARENT);
        SetTextColor(hdc,RGB(0,0,0));
		TextOut(hdc, x,y,str.c_str(),length);
		g_pDDSBack->ReleaseDC(hdc);
	  }
	  return;
	}*/
	
	Write(x,y,jelenx,co,str);
}

void textfont::IanOutTextC(int x, int y, int co, std::string str)
{
//	int		i;
	int		jelenx = 0;
//	int		sor = y+3;
//	int		length = str.size();
	int     kihossz = 0;
//	int		maxjelenx = 0;
//	int		maxsor = 0;

	kihossz = TextLength(str);
	jelenx = x-kihossz/2;

	Write(x,y,jelenx,co,str);
}

void textfont::IanOutTextR(int x, int y, int co, std::string str)
{
//	int		i;
	int		jelenx = 0;
//	int		sor = y+3;
//	int		length = str.size();
	int     kihossz = 0;
//	int		maxjelenx = 0;
	//int		maxsor = 0;

	kihossz = TextLength(str);
	jelenx = x-kihossz;
	Write(x,y,jelenx,co,str);
}

void textfont::IanOutNum1(int x, int y, int c,std::string &str)
{
	int		i;
	int		length = str.size();
	int     be;

	for (i=0; i<length; i++)
	{
		if ((str[i]>='0') && (str[i]<='9'+2))
		{
			be = (str[i]-'0')*14;
			if (c==7) be+=168;
			BlitTo(g_pDDSBack,be,144,be+14,168,x+i*14,y,0,g_pDDSOne);
		}
	}
}

void textfont::IanOutNum2(int x, int y, int c,std::string &str)
{
	int		i;
	int		length = str.size();
	int     be;

	for (i=0; i<length; i++)
	{
		if ((str[i]>='0') && (str[i]<='9'+2))
		{
			be = (str[i]-'0')*9;
			if (c==9) be+=120;
			if (c==10) be+=240;
			BlitTo(g_pDDSBack,be,169,be+9,186,x+i*9,y,0,g_pDDSOne);
		}
		if ((str[i]=='-'))
		{
			be = 12*9;
			if (c==9) be+=120;
			if (c==10) be+=240;
			BlitTo(g_pDDSBack,be,169,be+9,186,x+i*9,y,0,g_pDDSOne);
		}
	}
}

void textfont::DisplayNum(int x, int y, int c,int i, short count)
{
    char                buff[256];
    int					i2,i3;
	i2=0;
	for (i3=1; i3<=count;i3++)
	{
		i2=i2*10;
		i2=i2+1;
	}
	i2=9*i2;
	
	buff[0]=' ';
    if( i > i2 )
    {
        i = i2;
    }
	if (i<-i2)
	{
		i = -i2;
	}
	if (i<0) { buff[0] = '-'; i =-i;};
	
	switch(count)
	{
	case 1:
		buff[1]=(char)i+'0';
		break;
	case 2:
		buff[1] = (char)((i / 10) + '0');
		buff[2] = (char)((i % 10) + '0');
		break;
	case 3:
		buff[1] = (char)((i / 100) + '0');i-=i/100*100;
		buff[2] = (char)((i / 10) + '0');
		buff[3] = (char)((i % 10) + '0');
		break;
	case 4:
		buff[1] = (char)((i / 1000) + '0');i-=i/1000*1000;
		buff[2] = (char)((i / 100) + '0');i-=i/100*100;
		buff[3] = (char)((i / 10) + '0');
		buff[4] = (char)((i % 10) + '0');
		break;
	case 5:
		buff[1] = (char)((i / 10000) + '0');i-=i/10000*10000;
		buff[2] = (char)((i / 1000) + '0');i-=i/1000*1000;
		buff[3] = (char)((i / 100) + '0');i-=i/100*100;
		buff[4] = (char)((i / 10) + '0');
		buff[5] = (char)((i % 10) + '0');
		break;
	case 6:
		buff[1] = (char)((i / 100000) + '0');i-=i/100000*100000;
		buff[2] = (char)((i / 10000) + '0');i-=i/10000*10000;
		buff[3] = (char)((i / 1000) + '0');i-=i/1000*1000;
		buff[4] = (char)((i / 100) + '0');i-=i/100*100;
		buff[5] = (char)((i / 10) + '0');
		buff[6] = (char)((i % 10) + '0');
		break;
	case 7:
		buff[1] = (char)((i / 1000000) + '0');i-=i/1000000*1000000;
		buff[2] = (char)((i / 100000) + '0');i-=i/100000*100000;
		buff[3] = (char)((i / 10000) + '0');i-=i/10000*10000;
		buff[4] = (char)((i / 1000) + '0');i-=i/1000*1000;
		buff[5] = (char)((i / 100) + '0');i-=i/100*100;
		buff[6] = (char)((i / 10) + '0');
		buff[7] = (char)((i % 10) + '0');
		break;
	case 8:
		buff[1] = (char)((i / 10000000) + '0');i-=i/10000000*10000000;
		buff[2] = (char)((i / 1000000) + '0');i-=i/1000000*1000000;
		buff[3] = (char)((i / 100000) + '0');i-=i/100000*100000;
		buff[4] = (char)((i / 10000) + '0');i-=i/10000*10000;
		buff[5] = (char)((i / 1000) + '0');i-=i/1000*1000;
		buff[6] = (char)((i / 100) + '0');i-=i/100*100;
		buff[7] = (char)((i / 10) + '0');
		buff[8] = (char)((i % 10) + '0');
		break;
	}
	buff[count+1] = '\0';
	std::string xx;
	xx = buff;
	if ((c>=0) && (c<=5)) IanOutText(x,y,c,xx);
	if ((c>=6) && (c<=7)) IanOutNum1(x,y,c,xx);
	if ((c>=8) && (c<=10)) IanOutNum2(x,y,c,xx);
}

