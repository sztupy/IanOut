#include "play.h"
#include "load.h"
#include "../commonutils/mouse.h"
#include "terep.h"
#include "../commonutils/textfont.h"
#include "../commonutils/textutil.h"
#include "done.h"

void play::ChooseTerep(void)
{
	HRESULT								hRet;
	static int							Frame = 0;
	static int							Frame2 = 0;
	static int							mode = 0;
	int									cpos;
	static DWORD						ThisTick;
	static DWORD						LastTick = 0;
	static int							mousetyp = 0;
	DDBLTFX								ddbltfx;
	int									i;
	char								buf[150];
	char								buf2[150];
	char								buf3[150];
	FRMCollection						*point = NULL;
	FRMCollection::iterator				iter;

	FRMPairCollection					*pointP = NULL;
	FRMPairCollection::iterator			iterP;

	if ((mousetyp>0) && (mousetyp<9)) mousetyp=0;
	ThisTick = GetTickCount();

	if ((ThisTick - LastTick) > (DWORD)gameSpeed)
        {
            LastTick = ThisTick;
        }

	olddims=dims;
	mouse::UpdateInputState();
															
	MousX += dims.lX;									
	MousY += dims.lY;									
														
	if (MousX>=GetMaxX) { MousX=GetMaxX; }
	if (MousX<=0)		{ MousX=0;   }							
	if (MousY>=GetMaxY) { MousY=GetMaxY; }							
	if (MousY<=0)		{ MousY=0;   }						

	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = 0;
	g_pDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
	
	textfont::IanOutText(100,0,1,"Main Menu");
	textfont::IanOutText(30,10,1,"Current elements:");
	textfont::IanOutText(30,400,1,"UP");
	textfont::IanOutText(60,400,1,"DOWN");
	textfont::IanOutText(30,415,2,"Load");
	textfont::IanOutText(30,430,2,"Add");
	textfont::IanOutText(30,445,2,"Save");
	textfont::IanOutText(30,460,1,"Delete All");
	ClearRect(g_pDDSBack,30,30,200,400,64);

	textfont::IanOutText(230,10,1,"Actual Item");
	textfont::IanOutText(230,400,(mode==0) ? 0 : 1,"Wall");
	textfont::IanOutText(230,415,(mode==1) ? 0 : 1,"Tile");
	textfont::IanOutText(230,430,(mode==2) ? 0 : 1,"Scenery");
	textfont::IanOutText(230,445,(mode==3) ? 0 : 1,"Item");
	textfont::IanOutText(230,460,(mode==4) ? 0 : 1,"Object");
	//textfont::IanOutText(230,430,(mode==2) ? 0 : 1,"Scenery");
	ClearRect(g_pDDSBack,230,30,400,400,64);
	textfont::DisplayNum(230,20,1,Selected,5);

	textfont::IanOutText(430,10,1,"Selectable Items");
	textfont::IanOutText(430,400,1,"+30");
	textfont::IanOutText(470,400,1,"-30");
	textfont::IanOutText(430,415,1,"+300");
	textfont::IanOutText(470,415,1,"-300");
	textfont::IanOutText(520,415,1,"+3000");
	textfont::IanOutText(560,415,1,"-3000");
	textfont::IanOutText(430,430,1,"Add");
	ClearRect(g_pDDSBack,430,30,600,400,64);

	if (Frame2<1) Frame2=1;
	if (Selected<0) Selected=0;
	Frame=Selected/30;
	
	if (mode==0) point=&StaticInf->TilesW;
	if (mode==1) point=&MapInf->Tiles;
	if (mode==2) point=&StaticInf->TilesS;
	if (mode==3) pointP=&StaticInf->TilesI;
	if (mode==4) pointP=&StaticInf->TilesO;

	if (point) {
	iter = point->begin();
	cpos=0;
	while (iter != point->end())
	{
		cpos++;
		if ((cpos>=Frame2) && (cpos<=Frame2+89))
		{
			textfont::DisplayNum(30+60*((cpos-Frame2)%3),30+12*((cpos-Frame2)/3),1,iter->first,5);
		}
		iter++;
	}
	}

	if (pointP) {
	iterP = pointP->begin();
	cpos=0;
	while (iterP != pointP->end())
	{
		cpos++;
		if ((cpos>=Frame2) && (cpos<=Frame2+89))
		{
			textfont::DisplayNum(30+60*((cpos-Frame2)%3),30+12*((cpos-Frame2)/3),1,iterP->first,5);
		}
		iterP++;
	}
	}
	std::string xXxloc;
	switch (mode) {
		case 0:xXxloc = GetFile("\\proto\\walls.pro");break;
		case 1:xXxloc = GetFile("\\proto\\tiles.pro");break;
		case 2:xXxloc = GetFile("\\proto\\scenery.pro");break;
	    case 3:xXxloc = GetFile("\\proto\\items.pro");break;
		case 4:xXxloc = GetFile("\\proto\\objects.pro");break;
	}
	for (i=0; i<30; i++)
	{
		if (mode==0) 
		{
			wsprintf(buf,"WALLS_%i",Frame*30+i);
			wsprintf(buf3,"%s",xXxloc.c_str());
			textutil::WriteFromProf(490,30+i*12,(Selected==Frame*30+i) ? 0 : 1,1,buf3,"WALLS",buf);
		}
		if (mode==1) 
		{
			wsprintf(buf,"TILES_%i",Frame*30+i);
			wsprintf(buf3,"%s",xXxloc.c_str());
			textutil::WriteFromProf(490,30+i*12,(Selected==Frame*30+i) ? 0 : 1,1,buf3,"TILES",buf);
		}
		if (mode==2) 
		{
			wsprintf(buf,"SCENERY_%i",Frame*30+i);
			wsprintf(buf3,"%s",xXxloc.c_str());
			textutil::WriteFromProf(490,30+i*12,(Selected==Frame*30+i) ? 0 : 1,1,buf3,"SCENERY",buf);
		}
		if (mode==3) 
		{
			wsprintf(buf,"ITEMS_%i",Frame*30+i);
			wsprintf(buf3,"%s",xXxloc.c_str());
			textutil::WriteFromProf(490,30+i*12,(Selected==Frame*30+i) ? 0 : 1,1,buf3,"ITEMS",buf);
		}
		if (mode==4) 
		{
			wsprintf(buf,"OBJECTS_%i",Frame*30+i);
			wsprintf(buf3,"%s",xXxloc.c_str());
			textutil::WriteFromProf(490,30+i*12,(Selected==Frame*30+i) ? 0 : 1,1,buf3,"OBJECTS",buf);
		}
		textfont::DisplayNum(430,30+i*12,(Selected==Frame*30+i) ? 0 : 1,Frame*30+i,5);
	}

	if (mouse::MInPr(230,400,300,414,0)) {
	  mode=0;Frame=0;Frame2=0;Selected=0;
	}
	if (mouse::MInPr(230,415,300,429,0)) {
	  mode=1;Frame=0;Frame2=0;Selected=0;
	}
	if (mouse::MInPr(230,430,300,444,0)) {
	  mode=2;Frame=0;Frame2=0;Selected=0;
	}
	if (mouse::MInPr(230,445,300,459,0)) {
	  mode=3;Frame=0;Frame2=0;Selected=0;
	}
	if (mouse::MInPr(230,460,300,474,0)) {
	  mode=4;Frame=0;Frame2=0;Selected=0;
	}


	if (mouse::MInPr(30,400,59,412,0)) {
	  Frame2-=15;
	}
	if (mouse::MInPr(60,400,89,412,0)) {
	  Frame2+=15;
	}

	if (mouse::MInPr(430,400,469,412,0)) {
	  Frame++;Selected+=30;
	}
	if (mouse::MInPr(470,400,489,412,0)) {
	  Frame--;Selected-=30;
	}

	if (mouse::MInPr(400,415,469,427,0)) {
	  Frame+=10;Selected+=300;
	}
	if (mouse::MInPr(470,415,519,428,0)) {
	  Frame-=10;Selected-=300;
	}
	if (mouse::MInPr(520,415,559,427,0)) {
	  Frame+=100;Selected+=3000;
	}
	if (mouse::MInPr(560,415,599,428,0)) {
	  Frame-=100;Selected-=3000;
	}

	if (mouse::MInPr(430,30,600,400,0)) {
	  Selected = (MousY-30)/12 + Frame*30;
	}
	
	if (mode==0){
		wsprintf(buf,"WALLS_%i",Selected);
		wsprintf(buf3,"%s",GetFile("\\proto\\walls.pro").c_str());
		wsprintf(buf2,"\\art\\walls\\%s",textutil::GetFromProf(buf3,"WALLS",buf).c_str());
	}
	if (mode==1){
		wsprintf(buf,"TILES_%i",Selected);
		wsprintf(buf3,"%s",GetFile("\\proto\\tiles.pro").c_str());
		wsprintf(buf2,"\\art\\tiles\\%s",textutil::GetFromProf(buf3,"TILES",buf).c_str());
	}
	if (mode==2){
		wsprintf(buf,"SCENERY_%i",Selected);
		wsprintf(buf3,"%s",GetFile("\\proto\\scenery.pro").c_str());
		wsprintf(buf2,"\\art\\scenery\\%s",textutil::GetFromProf(buf3,"SCENERY",buf).c_str());
	}
	if (mode==3){
		wsprintf(buf,"ITEMS_%i",Selected);
		wsprintf(buf3,"%s",GetFile("\\proto\\items.pro").c_str());
		wsprintf(buf2,"\\data\\items\\%s",textutil::GetFromProf(buf3,"ITEMS",buf).c_str());
		wsprintf(buf,"%s",GetFile(buf2).c_str());
		//wsprintf(buf2,"\\art\\inven\\%s.frm",textutil::GetFromXML(buf,"/graphic.inventory").c_str());
		wsprintf(buf2,"");
	}
	if (mode==4){
		wsprintf(buf,"OBJECTS_%i",Selected);
		wsprintf(buf3,"%s",GetFile("\\proto\\objects.pro").c_str());
		wsprintf(buf2,"\\data\\objects\\%s",textutil::GetFromProf(buf3,"OBJECTS",buf).c_str());
		wsprintf(buf,"%s",GetFile(buf2).c_str());
		//wsprintf(buf2,"\\art\\%s.frm",textutil::GetFromXML(buf,"/graphic.picture").c_str());
		wsprintf(buf2,"");
	}
	//textfont::IanOutText(240,40,0,buf2);
	if (!CanOpen2(buf2)) {
		wsprintf(buf2,"\\art\\intrface\\Scr8x.frm");
	}	

	if (mouse::MInPr(430,430,500,444,0)) {
		if (point) {
			if (point->count(Selected) != 0) delete (*point)[Selected];
			(*point)[Selected] = new TFRMSingle();
			hRet = (*point)[Selected]->Load(hWnd,g_pDD,buf2,1);			
			if (hRet!=DD_OK) InitFail(hWnd,hRet,"ERROR");
		}
		if (pointP) {
			if (mode==3) {
				LoadNewItem(hWnd,g_pDD,StaticInf->TilesI,Selected);
			}
			if (mode==4) {
				char buf4[150];
				wsprintf(buf,"OBJECTS_%i",Selected);
				wsprintf(buf3,"%s",GetFile("\\proto\\objects.pro").c_str());
				GetPrivateProfileString("OBJECTS",buf,"",buf4,150,buf3);
				std::string str;
				PFRMPair Pair;
				Pair = StaticInf->TilesO[Selected] = new TFRMPair();
				Pair->FRM = NULL;
				Pair->FRMA = new TFRMAnim();
				wsprintf(buf,"\\data\\objects\\%s",buf4);
				str = "\\art\\"+textutil::GetFromXML(GetFile(buf),"/graphic.picture")+".frm";
				hRet = Pair->FRMA->Load(hWnd,g_pDD,str.c_str(),-1);
				if (hRet != DD_OK) InitFail(hWnd,hRet,str.c_str());
				Pair->FRMA->FirstFrame();
			}
		}
	}

	if ((lstrcmpi(FullScreen->fname,buf2)))
	{
   		LoadFRMSingle(&FullScreen,hWnd,buf2,1);
	}

	if (mouse::MInPr(100,0,200,12,0)) {
		GamePos = 0;
	}

	BlitTo(g_pDDSBack,0,0,FullScreen->FRM->x,FullScreen->FRM->y,240,40,DDBLTFAST_SRCCOLORKEY,FullScreen->FRM->FRM);

    BlitTo(g_pDDSBack,0,0,Mouse->FRM->x, Mouse->FRM->y,MousX,MousY,DDBLTFAST_SRCCOLORKEY,Mouse->FRM->FRM);

	textutil::DisplayFrameRate();

	// Flip the surfaces
    while (TRUE)
    {
        hRet = g_pDDSPrimary->Flip(NULL, 0);
        if  (hRet == DD_OK)
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
