#include "windows.h"

#include "play.h"
#include "load.h"
#include "../commonutils/mouse.h"
#include "terep.h"
#include "../commonutils/textfont.h"
#include "../commonutils/textutil.h"
#include "done.h"

void play::ChooseTerep(void)
{
	int								hRet;
	static int							Frame = 0;
	static int							Frame2 = 0;
	static int							mode = 0;
	int									cpos;
	static Uint32						ThisTick;
	static Uint32						LastTick = 0;
	static int							mousetyp = 0;
	int									i;
	char								buf[150];
	char								buf2[150];
	char								buf3[150];
	FRMCollection						*point = NULL;
	FRMCollection::iterator				iter;

	FRMPairCollection					*pointP = NULL;
	FRMPairCollection::iterator			iterP;

	if ((mousetyp>0) && (mousetyp<9)) mousetyp=0;
	ThisTick = SDL_GetTicks();

	if ((ThisTick - LastTick) > (Uint32)gameSpeed)
        {
            LastTick = ThisTick;
        }

				
	if (MousX>=GetMaxX) { MousX=GetMaxX; }
	if (MousX<=0)		{ MousX=0;   }							
	if (MousY>=GetMaxY) { MousY=GetMaxY; }							
	if (MousY<=0)		{ MousY=0;   }						

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
			sprintf(buf,"WALLS_%i",Frame*30+i);
			sprintf(buf3,"%s",xXxloc.c_str());
			textutil::WriteFromProf(490,30+i*12,(Selected==Frame*30+i) ? 0 : 1,1,buf3,"WALLS",buf);
		}
		if (mode==1) 
		{
			sprintf(buf,"TILES_%i",Frame*30+i);
			sprintf(buf3,"%s",xXxloc.c_str());
			textutil::WriteFromProf(490,30+i*12,(Selected==Frame*30+i) ? 0 : 1,1,buf3,"TILES",buf);
		}
		if (mode==2) 
		{
			sprintf(buf,"SCENERY_%i",Frame*30+i);
			sprintf(buf3,"%s",xXxloc.c_str());
			textutil::WriteFromProf(490,30+i*12,(Selected==Frame*30+i) ? 0 : 1,1,buf3,"SCENERY",buf);
		}
		if (mode==3) 
		{
			sprintf(buf,"ITEMS_%i",Frame*30+i);
			sprintf(buf3,"%s",xXxloc.c_str());
			textutil::WriteFromProf(490,30+i*12,(Selected==Frame*30+i) ? 0 : 1,1,buf3,"ITEMS",buf);
		}
		if (mode==4) 
		{
			sprintf(buf,"OBJECTS_%i",Frame*30+i);
			sprintf(buf3,"%s",xXxloc.c_str());
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
		sprintf(buf,"WALLS_%i",Selected);
		sprintf(buf3,"%s",GetFile("\\proto\\walls.pro").c_str());
		sprintf(buf2,"\\art\\walls\\%s",textutil::GetFromProf(buf3,"WALLS",buf).c_str());
	}
	if (mode==1){
		sprintf(buf,"TILES_%i",Selected);
		sprintf(buf3,"%s",GetFile("\\proto\\tiles.pro").c_str());
		sprintf(buf2,"\\art\\tiles\\%s",textutil::GetFromProf(buf3,"TILES",buf).c_str());
	}
	if (mode==2){
		sprintf(buf,"SCENERY_%i",Selected);
		sprintf(buf3,"%s",GetFile("\\proto\\scenery.pro").c_str());
		sprintf(buf2,"\\art\\scenery\\%s",textutil::GetFromProf(buf3,"SCENERY",buf).c_str());
	}
	if (mode==3){
		sprintf(buf,"ITEMS_%i",Selected);
		sprintf(buf3,"%s",GetFile("\\proto\\items.pro").c_str());
		sprintf(buf2,"\\data\\items\\%s",textutil::GetFromProf(buf3,"ITEMS",buf).c_str());
		sprintf(buf,"%s",GetFile(buf2).c_str());
		if (CanOpen(buf))
			sprintf(buf2,"\\art\\inven\\%s.frm",textutil::GetFromXML(buf,"/graphic.inventory").c_str());
		else
			sprintf(buf2,"");
	}
	if (mode==4){
		sprintf(buf,"OBJECTS_%i",Selected);
		sprintf(buf3,"%s",GetFile("\\proto\\objects.pro").c_str());
		sprintf(buf2,"\\data\\objects\\%s",textutil::GetFromProf(buf3,"OBJECTS",buf).c_str());
		sprintf(buf,"%s",GetFile(buf2).c_str());
		if (CanOpen(buf))
			sprintf(buf2,"\\art\\%s.frm",textutil::GetFromXML(buf,"/graphic.picture").c_str());
		else
			sprintf(buf2,"");
	}
	//textfont::IanOutText(240,40,0,buf2);
	if (!CanOpen2(buf2)) {
		sprintf(buf2,"\\art\\intrface\\Scr8x.frm");
	}	

	if (mouse::MInPr(430,430,500,444,0)) {
		if (point) {
			if (point->count(Selected) != 0) delete (*point)[Selected];
			(*point)[Selected] = new TFRMSingle();
			hRet = (*point)[Selected]->Load(buf2,1);			
			if (hRet!=0) InitFail(hRet,"ERROR");
		}
		if (pointP) {
			if (mode==3) {
				LoadNewItem(StaticInf->TilesI,Selected);
			}
			if (mode==4) {
				char buf4[150];
				sprintf(buf,"OBJECTS_%i",Selected);
				sprintf(buf3,"%s",GetFile("\\proto\\objects.pro").c_str());
				GetPrivateProfileString("OBJECTS",buf,"",buf4,150,buf3);
				std::string str;
				PFRMPair Pair;
				Pair = StaticInf->TilesO[Selected] = new TFRMPair();
				Pair->FRM = NULL;
				Pair->FRMA = new TFRMAnim();
				sprintf(buf,"\\data\\objects\\%s",buf4);
				str = "\\art\\"+textutil::GetFromXML(GetFile(buf),"/graphic.picture")+".frm";
				hRet = Pair->FRMA->Load(str.c_str(),-1);
				if (hRet != 0) InitFail(hRet,str.c_str());
				Pair->FRMA->FirstFrame();
			}
		}
	}

	if ((strcmp(FullScreen->fname,buf2)))
	{
   		LoadFRMSingle(&FullScreen,buf2,1);
	}

	if (mouse::MInPr(100,0,200,12,0)) {
		GamePos = 0;
	}

	BlitTo(g_pDDSBack,0,0,FullScreen->FRM->x,FullScreen->FRM->y,240,40,0,FullScreen->FRM->FRM);

    BlitTo(g_pDDSBack,0,0,Mouse->FRM->x, Mouse->FRM->y,MousX,MousY,0,Mouse->FRM->FRM);

	textutil::DisplayFrameRate();

	// Flip the surfaces
    SDL_Flip(g_pDDSBack);
	
}
