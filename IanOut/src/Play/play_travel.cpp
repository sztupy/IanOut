#include "play.h"
#include "load.h"
#include "../commonutils/mouse.h"
#include "../commonutils/textutil.h"
#include "../commonutils/textfont.h"
#include "done.h"
#include "math.h"
#include "../commonutils/date.h"
#include "../commonutils/palette.h"

int destX = -1;
int destY = -1;

double sourX,sourY;

void play::Travel(void)
{
	DDBLTFX ddbltfx;
	HRESULT hRet;

	static DWORD						ThisTick;
	static DWORD						LastTick = 0;
	static int							Frame = 0;

	static int							mousetyp = 0;
	BOOL								kintvan;
	BOOL								oldal[4];
	int									i,x,y;
	char								buf[150];
	
    int addx = (GetMaxX - 640) /2;
	int addy = (GetMaxY - 480) /2;

	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = RGB(0,0,0);
    g_pDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);

	if ((lstrcmpi(FullScreen->fname,"\\art\\intrface\\wmapbox.frm")))
	{
		LoadFRMSingle(&FullScreen,hWnd,"\\art\\intrface\\wmapbox.frm",1);
	}

	if ((lstrcmpi(TempAnim[0]->fname,"\\art\\intrface\\wmdial.frm")))
	{
    	LoadFRMAnim(&TempAnim[0],hWnd,"\\art\\intrface\\wmdial.frm");
		int hour;
		hour = date::Hour-12;
		if (hour<0) hour+=24;
		TempAnim[0]->FirstFrame();
		for (x=0; x<hour; x++) TempAnim[0]->NextFrame();
	}

	
	if ((lstrcmpi(Temporary[9]->fname,"\\art\\intrface\\wmpatrg2.frm")))
		{
    		LoadFRMSingle(&Temporary[9],hWnd,"\\art\\intrface\\wmaptrg2.frm",1);
		}

	bool MPressed = false;
	if (destX!=-1) {
		if ((lstrcmpi(Temporary[10]->fname,"\\art\\intrface\\wmaploc.frm")))
			{
    			LoadFRMSingle(&Temporary[10],hWnd,"\\art\\intrface\\wmaploc.frm",1);
				//AddToLog("Change");
			}
	} else { if (mouse::MInPr(TerX+TravelMapX-12+22,TerY+TravelMapY-7+22,TerX+TravelMapX+13+22,TerY+TravelMapY+8+22,6)) {
		if ((lstrcmpi(Temporary[10]->fname,"\\art\\intrface\\hotspot2.frm")))
			{
    			//AddToLog("Change");
				LoadFRMSingle(&Temporary[10],hWnd,"\\art\\intrface\\hotspot2.frm",1);
			}
			MPressed = true;
	} else {
		if ((lstrcmpi(Temporary[10]->fname,"\\art\\intrface\\hotspot1.frm")))
			{
				//AddToLog("Change");
    			LoadFRMSingle(&Temporary[10],hWnd,"\\art\\intrface\\hotspot1.frm",1);
			}
		}
    }
	//AddToLog("%i %i %i %i",TerX+TravelMapX-12,TerY+TravelMapY-7,TerX+TravelMapX+13,TerY+TravelMapY+8);
	if ((lstrcmpi(Temporary[16]->fname,"\\art\\intrface\\wmtabs.frm")))
		{
    		LoadFRMSingle(&Temporary[16],hWnd,"\\art\\intrface\\wmtabs.frm",1);
		}
	if ((lstrcmpi(Temporary[15]->fname,"\\art\\intrface\\months.frm")))
		{
    		LoadFRMSingle(&Temporary[15],hWnd,"\\art\\intrface\\months.frm",1);
		}

	SetClipRect(addx,addy,addx+640,addy+480);

	olddims = dims;
	mouse::UpdateInputState();

	MousX += dims.lX;
	MousY += dims.lY;
						
	mousetyp = 0;

	if ((MousX>=640) && (MousY<=0)) {mousetyp = 2;} else    		//
	if ((MousX>=640) && (MousY>=480)) {mousetyp = 4;} else  		//
	if ((MousX<=0)   && (MousY<=0)) {mousetyp = 8;} else    		//
	if ((MousX<=0)   && (MousY>=480)) {mousetyp = 6;} else  		//
	if (MousX>=640) {mousetyp = 3;} else							//
	if (MousX<=0) {mousetyp = 7;} else								//
	if (MousY>=480) {mousetyp = 5;} else							//
	if (MousY<=0) {mousetyp = 1;}									//
																	//
	kintvan=FALSE;													//
	oldal[0]=FALSE;oldal[2]=FALSE;									//
	oldal[1]=FALSE;oldal[3]=FALSE;									//
																	//
	if (MousX>=640) { MousX=640; TerX-=3; }						//	
	if (MousX<=0)   { MousX=0;   TerX+=3; }						//	
	if (MousY>=480) { MousY=480; TerY-=3; }						//	
	if (MousY<=0)   { MousY=0;   TerY+=3; }						//	

	if (TerX>0) TerX=0;
	if (TerY>0) TerY=0;
	if (TerX<-951) TerX=-951;
	if (TerY<-1060) TerY=-1060;

	for (x=0; x<3; x++)
	for (y=0; y<3; y++)
	{
		int ix,iy;
		ix = abs(TerX/350)+x;
		iy = abs(TerY/300)+y;
		i = iy*4+ix;
		if (i<10) wsprintf(buf,"\\art\\intrface\\wrldmp0%i.frm",i);
		if ((i>=10) && (i<20)) wsprintf(buf,"\\art\\intrface\\wrldmp%i.frm",i);
		if ((lstrcmpi(Temporary[x*3+y]->fname,buf)))
		{
    		LoadFRMSingle(&Temporary[x*3+y],hWnd,buf,1);
		}
	}

	ThisTick = GetTickCount();

	if ((ThisTick - LastTick) > (DWORD)gameSpeed)
        {
            LastTick = ThisTick;
			Frame++;
            if (destX != -1) {
				int hour;
				hour = date::Hour-12;
				if (hour<0) hour+=24;
				TempAnim[0]->FirstFrame();
				for (x=0; x<hour; x++) TempAnim[0]->NextFrame();
				date::AddMinute(30);
				double xdest = destX-TravelMapX;
				double ydest = destY-TravelMapY;
				double destination = sqrt(xdest*xdest+ydest*ydest);
				sourX+=xdest/destination*2;
				sourY+=ydest/destination*2;

				TravelMapX = sourX;
				TravelMapY = sourY;
			}
        }

	int intx = abs((TerX)/350);
	int inty = abs((TerY)/300);

//	for (x=0; x<10; x++) TravelMap[x][x] = 1;
//	for (x=0; x<10; x++) TravelMap[x][x+1] = 2;
	int xx = abs((TravelMapX)/50);
	int yy = abs((TravelMapY)/50);
	if ((xx<28) && (yy<30)) {

		for (x=xx-1; x<xx+2; x++)
		if ((x>=0) && (x<28))
		for (y=yy-1; y<yy+2; y++)
		if ((y>=0) && (y<30))
			if (TravelMap[x][y]==0) TravelMap[x][y] = 1;
		
		TravelMap[xx][yy] = 2;
	}
		

	for (x=0; x<3; x++)
	for (y=0; y<3; y++)
	{
		for (int xx=0; xx<7; xx++)
		for (int yy=0; yy<6; yy++) {
			if (((inty+y)*6+yy<30) && ((intx+x)*7+xx<28)) {
				if (TravelMap[(intx+x)*7+xx][(inty+y)*6+yy] == 1)
        			BlitToAlt(g_pDDSBack,xx*50,yy*50,xx*50+50,yy*50+50,TerX+(intx+x)*350+22+addx+xx*50,TerY+(inty+y)*300+22+addy+yy*50,DDBLTFAST_SRCCOLORKEY,Temporary[x*3+y]->FRM->FRM,3);
				if (TravelMap[(intx+x)*7+xx][(inty+y)*6+yy] >= 2)
        			BlitTo(g_pDDSBack,xx*50,yy*50,xx*50+50,yy*50+50,TerX+(intx+x)*350+22+addx+xx*50,TerY+(inty+y)*300+22+addy+yy*50,DDBLTFAST_SRCCOLORKEY,Temporary[x*3+y]->FRM->FRM);
			}
		}
	}

	//BlitTo(g_pDDSBack,0,186,50,186+50,addx+TerX+150+22,addy+TerY+100+22,DDBLTFAST_SRCCOLORKEY,g_pDDSOne);
	//BlitTo(g_pDDSBack,50,186,75,186+25,addx+TerX+250+22,addy+TerY+100+22,DDBLTFAST_SRCCOLORKEY,g_pDDSOne);
	//BlitTo(g_pDDSBack,75,186,88,186+13,addx+TerX+300+22,addy+TerY+100+22,DDBLTFAST_SRCCOLORKEY,g_pDDSOne);

	//BlitTo(g_pDDSBack,0,186,50,186+50,addx+TerX+1200+22,addy+TerY+1300+22,DDBLTFAST_SRCCOLORKEY,g_pDDSOne);

	PLocation Item;
	Item = PLocation(MapLocations->Locations->First());
	while (Item != NULL) {
		if (Item->visible)
		if (TravelMap[(Item->x/50)][Item->y/50]>0)
		switch (Item->size) {
			case 1:
				BlitTo(g_pDDSBack,75,186,88,186+13,addx+TerX+Item->x+15,addy+TerY+Item->y+15,DDBLTFAST_SRCCOLORKEY,g_pDDSOne);
				textfont::IanOutTextC(addx+TerX+Item->x+22,addy+TerY+Item->y+30,1,Item->name);
				break;
			case 2:
				BlitTo(g_pDDSBack,50,186,75,186+25,addx+TerX+Item->x+10,addy+TerY+Item->y+10,DDBLTFAST_SRCCOLORKEY,g_pDDSOne);
				textfont::IanOutTextC(addx+TerX+Item->x+22,addy+TerY+Item->y+37,1,Item->name);
				break;
			default:
				BlitTo(g_pDDSBack,0,186,50,186+50,addx+TerX+Item->x-3,addy+TerY+Item->y-3,DDBLTFAST_SRCCOLORKEY,g_pDDSOne);
				textfont::IanOutTextC(addx+TerX+Item->x+22,addy+TerY+Item->y+50,1,Item->name);
				break;
		}
		Item = PLocation(MapLocations->Locations->Next(Item));
	}

	BlitFRMTo(g_pDDSBack,Temporary[10]->FRM,TerX+TravelMapX+addx+22,TerY+TravelMapY+addy+22,DDBLTFAST_SRCCOLORKEY,0,0);
	if (destX != -1)
		BlitFRMTo(g_pDDSBack,Temporary[9]->FRM,TerX+destX+addx+22,TerY+destY+addy+22,DDBLTFAST_SRCCOLORKEY,0,0);
	
	BlitTo(g_pDDSBack,0,0,145,480,GetMaxX-addx-145,addy+0,DDBLTFAST_SRCCOLORKEY,Temporary[16]->FRM->FRM);
	//PLocation Item;
	Item = PLocation(MapLocations->Locations->First());
	x = 0;
	while (Item != NULL) {
		if (Item->visible)
		if (TravelMap[(Item->x/50)][Item->y/50]>0) {
			textfont::IanOutText(GetMaxX-addx-145+40,addy+140+x*25,3,Item->shortname);
			if (mouse::MInPr(GetMaxX-addx-145+15,addy+138+x*25,GetMaxX-addx-145+35,addy+138+x*25+20,0)) {
				destX = Item->x;
				destY = Item->y;
				sourX = TravelMapX;
				sourY = TravelMapY;
			}
			x++;
		}
		Item = PLocation(MapLocations->Locations->Next(Item));
	}

	if ((!MPressed) && (mouse::MInPr(addx+22,addy+22,470+addx,460+addy,0)))
	{
		destX = -TerX+MousX-22;
		destY = -TerY+MousY-22;
		sourX = TravelMapX;
		sourY = TravelMapY;
	}
	
	if ((abs(destX-TravelMapX)<3) && (abs(destY-TravelMapY))<3)
	{
		TravelMapX = destX;
		TravelMapY = destY;
		destX = -1;
		destY = -1;
	}

	BlitTo(g_pDDSBack,0,0,640,480,addx+0,addy+0,DDBLTFAST_SRCCOLORKEY,FullScreen->FRM->FRM);
	BlitTo(g_pDDSBack,0,0,TempAnim[0]->GetCurFrame()->x,TempAnim[0]->GetCurFrame()->y,addx+532,addy+45,DDBLTFAST_SRCCOLORKEY,TempAnim[0]->GetCurFrame()->FRM);

	textfont::DisplayNum(addx+486-8,addy+12,8,date::Day,2);
	textfont::DisplayNum(addx+547-8,addy+12,8,date::Year,4);
	BlitTo(g_pDDSBack,0,date::Month*15,29,date::Month*15+14,addx+513,addy+13,DDBLTFAST_SRCCOLORKEY,Temporary[15]->FRM->FRM);
	textfont::DisplayNum(addx+592-7,addy+12,8,date::Hour,2);
	textfont::DisplayNum(addx+611-8,addy+12,8,date::Minute,2);

	textfont::DisplayNum(50,0,1,TravelMapX,5);
	textfont::DisplayNum(100,0,1,TravelMapY,5);
	textfont::DisplayNum(200,00,1,destX,5);
	textfont::DisplayNum(250,00,1,destY,5);

	if ((destX==-1) && (mouse::MInPr(TerX+TravelMapX-12+22,TerY+TravelMapY-7+22,TerX+TravelMapX+13+22,TerY+TravelMapY+8+22,3))) {
		Item = PLocation(MapLocations->Locations->First());

		while (Item != NULL) {
			if (Item->visible)
			if (TravelMap[(Item->x/50)][Item->y/50]>0) {
				int temp = 25;
				if (Item->size == 1) temp = 7;
				if (Item->size == 2) temp = 13;
				if ((TravelMapX-Item->x)*(TravelMapX-Item->x)+
					(TravelMapY-Item->y)*(TravelMapY-Item->y)<=temp*temp) {
						palette::FadeOut();
						ClearSurface(g_pDDSBack,0,0,0);
						ClearSurface(g_pDDSPrimary,0,0,0);
						char buf[150],buf2[150];
						int i,i2;
						sprintf(buf,"/area-id=%i/start.loc",Item->num);
						i2 = atoi2(textutil::GetFromXML(GetFile("\\data\\city.dat"),buf).c_str());
						sprintf(buf,"/location-id=%i/entrdat.defaultentr",i2);
						i = atoi2(textutil::GetFromXML(GetFile("\\data\\maps.dat"),buf).c_str());
						buttons::ChangeButtonList(ButtonList,2);
						GamePos = 1;
						//AddToLog("loc %i %i",i2,i);
						LoadLocation(i2,i);
						play::NormalPlay();
						palette::FadeIn();
					}
			}
			Item = PLocation(MapLocations->Locations->Next(Item));
		}
	}

	if ((mousetyp>0) && (mousetyp<9))
	{
		if (kintvan) i=1; else i=0;
		x = MouseScr[mousetyp-1][i]->FRM->x; 
		y = MouseScr[mousetyp-1][i]->FRM->y;
		if ((mousetyp==1) || (mousetyp>6))
		BlitTo(g_pDDSBack,0,0,x,y,addx+MousX,addy+MousY,DDBLTFAST_SRCCOLORKEY,
							  MouseScr[mousetyp-1][i]->FRM->FRM);
		if (mousetyp==2)
		BlitTo(g_pDDSBack,0,0,x,y,addx+MousX-x,addy+MousY,DDBLTFAST_SRCCOLORKEY,
							  MouseScr[mousetyp-1][i]->FRM->FRM);
		if (mousetyp==3)
		BlitTo(g_pDDSBack,0,0,x,y,addx+MousX-x,addy+MousY,DDBLTFAST_SRCCOLORKEY,
							  MouseScr[mousetyp-1][i]->FRM->FRM);
		if (mousetyp==4)
		BlitTo(g_pDDSBack,0,0,x,y,addx+MousX-x,addy+MousY-y,DDBLTFAST_SRCCOLORKEY,
							  MouseScr[mousetyp-1][i]->FRM->FRM);
		if (mousetyp==5)
		BlitTo(g_pDDSBack,0,0,x,y,addx+MousX,addy+MousY-y,DDBLTFAST_SRCCOLORKEY,
							  MouseScr[mousetyp-1][i]->FRM->FRM);
		if (mousetyp==6)
		BlitTo(g_pDDSBack,0,0,x,y,addx+MousX,addy+MousY-y,DDBLTFAST_SRCCOLORKEY,
							  MouseScr[mousetyp-1][i]->FRM->FRM);

		
	}

	if (mousetyp==0) BlitTo(g_pDDSBack,0,0,Mouse->FRM->x, Mouse->FRM->y,addx+MousX,addy+MousY,DDBLTFAST_SRCCOLORKEY,Mouse->FRM->FRM);	 

	SetClipRect(0,0,GetMaxX,GetMaxY);

	textutil::DisplayFrameRate();
	while (TRUE)
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
