#include "IanOs.h"
#include "play.h"
#include "terep.h"
#include "load.h"
#include "../commonutils/mouse.h"
#include "../commonutils/textutil.h"
#include "../commonutils/textfont.h"
#include "done.h"
#include "../commonutils/date.h"

void play::Talk(void)
{
	HRESULT hRet;
	static std::string  VLista[15];
	//static int			GoToList[15];
	static int  DoList[15];
	static int			NumList[15];

	int addx = (GetMaxX - 640) /2;
	int addy = (GetMaxY - 480) /2;

	char buf1[80];
//	char buf2[80];

	//static char filename[80] = "\\text\\talk\\duma.sve";

	//static int curpos = 1;
	static int page = 1;

	static int i,i2,i3,ib,ie;

	DDBLTFX ddbltfx;

	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = RGB(255,255,255);
    g_pDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);

	ClearSurface(g_pDDSBack,0,0,0);

	CritterList::iterator iter2;
	iter2 = CritterInf->Critters.begin();
	while (iter2 != CritterInf->Critters.end()) {
		iter2->second->AlreadyDrawn = false;
		iter2->second->conv = CompLoc(iter2->second->x,iter2->second->y);		
		iter2++;
	}

	terep::DrawTerep(0,0,0);
	ClearRect(g_pDDSBack,0,0,GetMaxX,addy,0);
	ClearRect(g_pDDSBack,0,GetMaxY-addy,GetMaxX,GetMaxY,0);
	ClearRect(g_pDDSBack,0,0,addx,GetMaxY,0);
	ClearRect(g_pDDSBack,GetMaxX-addx,0,GetMaxX,GetMaxY,0);

	if ((lstrcmpi(FullScreen->fname,"\\art\\intrface\\alltlk.frm")))
	{
    	LoadFRMSingle(&FullScreen,hWnd,"\\art\\intrface\\alltlk.frm",1);
	}
	if ((lstrcmpi(Temporary[0]->fname,"\\art\\intrface\\di_talk.frm")))
	{
    	LoadFRMSingle(&Temporary[0],hWnd,"\\art\\intrface\\di_talk.frm",1);
	}

	olddims = dims;
	mouse::UpdateInputState();

	MousX += dims.lX;
	MousY += dims.lY;

	if (MousX>=GetMaxX)	MousX=GetMaxX;
	if (MousX<=0)		MousX=0;
	if (MousY>=GetMaxY)	MousY=GetMaxY;
	if (MousY<=0)		MousY=0;

	BlitTo(g_pDDSBack,0,0,640,480,addx,addy,DDBLTFAST_SRCCOLORKEY,FullScreen->FRM->FRM);
	BlitTo(g_pDDSBack,0,0,640,190,addx,GetMaxY-190-addy,DDBLTFAST_SRCCOLORKEY,Temporary[0]->FRM->FRM);

	int numitems = 0;

		i2=1;
		i3=1;
		int buf22;
		for (i=0; i<15; i++)
		{
		buf22=i2-1;
		if (i2>IanOs::Text::TextCount) buf22=-1;
		if (textutil::CutString(IanOs::Text::TextValue[buf22],390,i3) == "")
		{
			NumList[i2]=i3-1;
			i3=1;
			i2++;
			buf22=i2-1;
			//wsprintf(buf2,"CHO%i",i2);
			if (i2>IanOs::Text::TextCount) buf22=-1;
		}
		VLista[i]=(textutil::CutString(IanOs::Text::TextValue[buf22],390,i3));
		if ((i3==1) && (VLista[i]!=""))
		{
			VLista[i]=char(' ')+VLista[i];
			VLista[i]=char('~'+1)+VLista[i];
			numitems++;
		} else
		{
			if (VLista[i]!="") numitems++;
			VLista[i]=char(' ')+VLista[i];
			VLista[i]=char(' ')+VLista[i];
		}
		//wsprintf(buf2,"CHO%iB",i2);
		DoList[i]=IanOs::Text::ToDoValue[i2-1];
		i3++;
		}

	for (i=1; i<=4; i++)
	{
	  //wsprintf(buf2,"text%i",(page-1)*4+i);
		textfont::IanOutText(addx+140,addy+230+(i-1)*10,1,textutil::CutString(IanOs::Text::MainValue,375,(page-1)*4+i));
//		textutil::WriteFromProf(addx+140,addy+230+(i-1)*10,1,1,/*GetFile(*/filename/*)*/,buf1,buf2);
	}

	i2=0;
	i3=0;ib=-1;ie=-1;
	for (i=0; i<15; i++)
	{
	  i3++;
	  if (mouse::MouseIn(addx+130,addy+330+i2*12,addx+515,addy+329+(i2+NumList[i3])*12))
	  {
	    ib=i2;ie=i2+NumList[i3]-1;
	  }
	  i2+=NumList[i3];
	  //textfont::DisplayNum(0,i*12,1,NumList[i],3);
	  //textfont::DisplayNum(40,i*12,1,i,3);
	}

	textfont::DisplayNum(100,0,0,numitems,3);

	for (i=0; i<15; i++)
	{
		if ((i>=ib) && (i<=ie))
		{
			textfont::IanOutText(addx+120+i,addy+330+i*12,0,VLista[i]);
			if ((mouse::Pressed(3)) /*&& !(DoList[i]=="")*/ && !(VLista[i]=="") && (i<numitems))
			{
				//date::AddSecond(page*4+5);
				page = 1;
				IanOs::Text::SetPosition(DoList[i]);
				IanOs::RunStat(std::string("System.RunFile([[")+IanOs::Text::TextFile+"]])");
				mouse::UpdateInputState();
				return;
			}
		} else
		{
			textfont::IanOutText(addx+120+i,addy+330+i*12,1,VLista[i]);
		}
	}

	wsprintf(buf1,"$%i",CritterInf->Critters.find(0)->second->Inven->CountItem(65535,0));
	textfont::IanOutTextC(addx+40,GetMaxY-addy-160,1,buf1);

	if (mouse::MouseIn(addx+140,addy+270,addx+520,addy+280))
	{
		BlitTo(g_pDDSBack,0,0,Arrows[1]->FRM->x, Arrows[1]->FRM->y,MousX,MousY,DDBLTFAST_SRCCOLORKEY,Arrows[1]->FRM->FRM);
		if ((mouse::Pressed(3)) && (textutil::CutString(IanOs::Text::MainValue,375,(page)*4+1) != "")) page++;
	} else
	if (mouse::MouseIn(addx+140,addy+230,addx+520,addy+240))
	{
		BlitTo(g_pDDSBack,0,0,Arrows[0]->FRM->x, Arrows[0]->FRM->y,MousX,MousY,DDBLTFAST_SRCCOLORKEY,Arrows[0]->FRM->FRM);
		if (mouse::Pressed(3)) page--;
	} else
	{
		BlitTo(g_pDDSBack,0,0,Mouse->FRM->x, Mouse->FRM->y,MousX,MousY,DDBLTFAST_SRCCOLORKEY,Mouse->FRM->FRM);	 
	} 


	textutil::DisplayFrameRate();

	if (page<1) page=1;
	
	while (true)
    {
        hRet = g_pDDSPrimary->Flip(NULL, 0);
        if (hRet == DD_OK)
            break;
        if (hRet == DDERR_SURFACELOST)
        {
            hRet = RestoreAll();
            if (hRet != DD_OK)
                break;
        }
        if (hRet != DDERR_WASSTILLDRAWING)
            break;
    }
	
}
