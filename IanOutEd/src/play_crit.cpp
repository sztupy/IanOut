#include "windows.h"

#include "sys\stat.h"
#include "play.h"
#include "../commonutils/mouse.h"
#include "terep.h"
#include "../commonutils/textfont.h"
#include "../commonutils/textutil.h"
#include "done.h"

int tcid = 0;
int tcvar1 = 0;
int tcvar2 = 0;
int tcgroup = 0;
int tckarma = 0;
int tcx = 0;
int tcy = 0;
int tcproto = 0;
int tctype = 0;

void play::EditCritters(void)
{
    int									x,y;
	int								hRet;
	static int							Frame = 0;
	static Uint32						ThisTick;
	static Uint32						LastTick = 0;
	static int							mousetyp = 0;
	int									i;
	bool								kintvan;
	bool								oldal[4];
	BlockType							BlDat;

	static std::string FName = "untitled.act";
	std::string FN;

	static int color;

	for (x=0; x<512; x++)
		for (y=0; y<512; y++)
		{
		  BlDat[x][y] = BlockDat[x][y];
		}

	if ((mousetyp>0) && (mousetyp<9)) mousetyp=0;
	ThisTick = SDL_GetTicks();

	if ((ThisTick - LastTick) > (Uint32)gameSpeed)
        {
            LastTick = ThisTick;
        }

// -------------------------------------------------------------------
	if ((dims.lX == olddims.lX) && (dims.lY == olddims.lY)) Frame++; else Frame=0;

	if ((MousX>=GetMaxX) && (MousY<=0)) {mousetyp = 2;} else    	//
	if ((MousX>=GetMaxX) && (MousY>=GetMaxY)) {mousetyp = 4;} else  //
	if ((MousX<=0)   && (MousY<=0)) {mousetyp = 8;} else    		//
	if ((MousX<=0)   && (MousY>=GetMaxY)) {mousetyp = 6;} else  	//
	if (MousX>=GetMaxX) {mousetyp = 3;} else						//
	if (MousX<=0) {mousetyp = 7;} else								//
	if (MousY>=GetMaxY) {mousetyp = 5;} else						//
	if (MousY<=0) {mousetyp = 1;}									//
																	//
	kintvan=false;													//
	oldal[0]=false;oldal[2]=false;									//
	oldal[1]=false;oldal[3]=false;									//
																	//
	if (MousX>=GetMaxX) { MousX=GetMaxX; TerX-=16; }				//	
	if (MousX<=0)   { MousX=0;   TerX+=16; }						//	
	if (MousY>=GetMaxY) { MousY=GetMaxY; TerY-=12; }				//
	if (MousY<=0)   { MousY=0;   TerY+=12; }						//	
																	//
	if ((mousetyp == 2) && (oldal[0] != oldal[3])) kintvan=false;	//
	if ((mousetyp == 4) && (oldal[1] != oldal[3])) kintvan=false;	//
	if ((mousetyp == 6) && (oldal[2] != oldal[1])) kintvan=false;	//
	if ((mousetyp == 8) && (oldal[2] != oldal[0])) kintvan=false;	//
																	//
// -------------------------------------------------------------------
	
	y = (MousX-TerX+4*(8+MousY-TerY))/64;
	x = (MousX-TerX+8-16*y)/12;
	
	terep::DrawTerep(mousetyp,x,y);

	if (mouse::MInPr(611,GetMaxY-20,640,GetMaxY,0)) dot_plane=!dot_plane;
	if (mouse::MInPr(571,GetMaxY-20,610,GetMaxY,0)) wall_plane=!wall_plane;
	if (mouse::MInPr(541,GetMaxY-20,570,GetMaxY,0)) item_plane=!item_plane;
	if (mouse::MInPr(500,GetMaxY-20,540,GetMaxY,0)) block_plane=!block_plane;

	if (mouse::MInPr(GetMaxX-100,0,GetMaxX,15,0)) {
		GamePos = 0;
	}

	if (mouse::MInPr(380,GetMaxY-20,390,GetMaxY,0)) Selected++;
	if (mouse::MInPr(450,GetMaxY-20,GetMaxY-20,GetMaxY,0)) Selected--;
	
	if (mouse::Pressed(1)) Selected++;
	bool isPressed = false;
	if (mouse::Pressed(0)) isPressed = true;

	CritterList::iterator iter,itersave;
	itersave = CritterInf->Critters.end();
	iter = CritterInf->Critters.begin();
	int Counter = 0;
	bool drawn = false;
	char buf2[150];
	sprintf(buf2,"");
	int ActualType = -1;
	color++;
	InventorySave = NULL;
	if (color>256) color=0;
	if (color<0) color=0;

	if (mouse::MouseIn(0,15,GetMaxX-100,GetMaxY-20))
	while (iter != CritterInf->Critters.end()) {
		PFRM frm;
		int iy = (iter->second->y);
		int ix = (iter->second->x);
		frm = iter->second->Idle->GetCurFrame();
		int cx = TerX+LocConvertX(ix,iy)*16+frm->bx;
		int cy = TerY+LocConvertY(ix,iy)*12+frm->by;

		

		if (mouse::MouseIn(cx,cy,cx+frm->x,cy+frm->y)) {
			if (Counter == Selected) {
				BlitToRo(g_pDDSBack,0,0,frm->x,frm->y,cx,cy,0,frm->FRM,palcal[color]);
				char buf[100];
				sprintf(buf,"%08i",iter->second->Player.proto);
				textfont::IanOutTextR(cx,cy,1,buf);

				textfont::IanOutTextR(cx,cy+15,1,iter->second->Player.name);
				sprintf(buf,"Id: %i",iter->second->Player.Count);
				textfont::IanOutText(cx+frm->x,cy,1,buf);
				sprintf(buf,"Var_1: %i",iter->second->Player.var1);
				textfont::IanOutText(cx+frm->x,cy+15,1,buf);
				sprintf(buf,"Var_2: %i",iter->second->Player.var2);
				textfont::IanOutText(cx+frm->x,cy+30,1,buf);
				sprintf(buf,"Group: %i",iter->second->Player.group);
				textfont::IanOutText(cx+frm->x,cy+45,1,buf);
				sprintf(buf,"Karma: %i",iter->second->Player.karma);
				textfont::IanOutText(cx+frm->x,cy+60,1,buf);

				if (mouse::Pressed(2))
					{
						tcid = iter->second->Player.Count;
						tcvar1 = iter->second->Player.var1;
						tcvar2 = iter->second->Player.var2;
						tcgroup = iter->second->Player.group;
						tckarma = iter->second->Player.karma;
						tcx = iter->second->x;
						tcy = iter->second->y;
						tcproto = iter->second->Player.proto;
						tctype = iter->first;
					};
				if (mouse::Pressed(0))
					{
						iter->second->Player.Count = tcid;
						iter->second->Player.var1 = tcvar1;
						iter->second->Player.var2 = tcvar2;
						iter->second->Player.group = tcgroup;
						iter->second->Player.karma = tckarma;
						iter->second->x = tcx;
						iter->second->y = tcy;
						//iter->second->Player.proto = tcproto;
						//iter->first = tctype;
					};
				if (mouse::Pressed(6) && mouse::Pressed(7)) itersave = iter;

				InventorySave = iter->second->Inven;
				if (!iter->second->Inven->Inven->Empty()) 
					textfont::IanOutText(cx+frm->x,cy+75,1,"Not Empty");

				sprintf(buf2,"Location: %i (x: %i y: %i) Type: %i",iter->second->conv,
					(iter->second->x),
					(iter->second->y),
					iter->first);
				drawn = true;
			}
			Counter++;
		}
		iter++;
	}

	if (itersave != CritterInf->Critters.end()) {
		delete itersave->second;
		CritterInf->Critters.erase(itersave);
	}

	if (!drawn) Selected = 0;

	ClearRect(g_pDDSBack,0,GetMaxY-20,GetMaxX,GetMaxY,0);
	ClearRect(g_pDDSBack,0,0,GetMaxX,15,0);
	ClearRect(g_pDDSBack,GetMaxX-100,15,GetMaxX,GetMaxY-20,0);

	textfont::IanOutText(0,GetMaxY-20,0,buf2);

	ref_to = NULL;
	bool InSide = false;
	char buf[150];
	
	textfont::IanOutText(380,GetMaxY-20,mouse::MouseIn(380,GetMaxY-20,390,GetMaxY) ? 0 : 1,"+");
	textfont::IanOutText(450,GetMaxY-20,mouse::MouseIn(450,GetMaxY-20,460,GetMaxY) ? 0 : 1,"-");
	textfont::DisplayNum(390,GetMaxY-20,1,Selected,5);

	textfont::IanOutTextR(GetMaxX,0,mouse::MouseIn(GetMaxX-100,0,GetMaxX,15) ? 0 : 1,"MAIN MENU");

	textfont::IanOutTextR(640,GetMaxY-20,dot_plane ? 0 : 1,"DOT");
	textfont::IanOutTextR(610,GetMaxY-20,wall_plane ? 0 : 1,"WALL");
	textfont::IanOutTextR(570,GetMaxY-20,item_plane ? 0 : 1,"ITEM");
	textfont::IanOutTextR(540,GetMaxY-20,block_plane ? 0 : 1,"BLCK");

	ref_str = NULL;
	InSide = false;
	if (mouse::MouseIn(0,0,GetMaxX-100,15)) { ref_str = &FName; InSide = true;} else InSide = false;
	textfont::IanOutTextC(GetMaxX/2,0,InSide ? 0 : 1,"\"mod\\IanEd\\maps\\"+FName+"\"");

	FN = "mod\\IanEd\\maps\\";

	if (mouse::MouseIn(GetMaxX-100,15,GetMaxX,29)) InSide = true; else InSide = false;
	textfont::IanOutTextC(GetMaxX-50,15,InSide ? 0 : 1,"LOAD");
	if (InSide && mouse::Pressed(3)) {
		if (CritterInf) delete CritterInf;
		CritterInf = new TIanCritter();
		FN+=FName;
		CritterInf->LoadCritters(FN.c_str(),StaticInf->TilesI);
	}
	if (mouse::MouseIn(GetMaxX-100,30,GetMaxX,44)) InSide = true; else InSide = false;
	textfont::IanOutTextC(GetMaxX-50,30,InSide ? 0 : 1,"SAVE");
	if (InSide && mouse::Pressed(3)) {
		int stream;
		FN+=FName;
		stream = _open(FN.c_str(),_O_BINARY | _O_RDWR | _O_TRUNC | _O_CREAT, _S_IREAD | _S_IWRITE  );
		AddToLog(2,"Save> %s",FN.c_str());
		if (stream != -1) {

		CritterList::iterator iter;
		if (!CritterInf->Critters.empty()) {
			iter = CritterInf->Critters.begin();
			while (iter != CritterInf->Critters.end()) {
				PFRMPlayer Ian = iter->second;
				//unsigned int convert = (Ian->x<<10) + (Ian->y ^ 511);
				unsigned int convert = CompLoc(Ian->x,Ian->y);

				_write(stream,&convert,4);
				_write(stream,&iter->first,2);
                _write(stream,&Ian->Player.proto,4);
				
                unsigned int opc; 
				if (Ian->Player.Count !=0) {
					opc = 0x11; _write(stream,&opc,2);
					_write(stream,&Ian->Player.Count,4);
				}
				if (Ian->Player.var1 !=0) {
					opc = 0x22; _write(stream,&opc,2);
					_write(stream,&Ian->Player.var1,4);
				}
				if (Ian->Player.var2 !=0) {
					opc = 0x33; _write(stream,&opc,2);
					_write(stream,&Ian->Player.var2,4);
				}
				if (Ian->Player.group !=0) {
					opc = 0x44; _write(stream,&opc,2);
					_write(stream,&Ian->Player.group,4);
				}
				if (Ian->Player.karma !=0) {
					opc = 0x55; _write(stream,&opc,2);
					_write(stream,&Ian->Player.karma,4);
				}
				if (Ian->Inven)
				if (!Ian->Inven->Inven->Empty()) {
					opc = 0xAA; _write(stream,&opc,2);
					PInventoryItem Item;
					Item = PInventoryItem(Ian->Inven->Inven->First());
					while (Item != NULL) {
						_write(stream,&Item->num,2);
						_write(stream,&Item->count,4);
						Item = PInventoryItem(Ian->Inven->Inven->Next(Item));
					}
					int tempnum = 0;
					unsigned short temptype = 0;
					_write(stream,&temptype,2);
					_write(stream,&tempnum,4);
				}
			opc=0xff; _write(stream,&opc,2);
			iter++;
			}
		}
		_close(stream);
		}
	}


	if (mouse::MouseIn(GetMaxX-100,45,GetMaxX,59)) InSide = true; else InSide = false;
	textfont::IanOutTextC(GetMaxX-50,45,InSide ? 0 : 1,"CLEAR");
	if (InSide && mouse::Pressed(3)) {
		if (CritterInf) delete CritterInf;
		CritterInf = new TIanCritter();
		FN+="blank.act";
		CritterInf->LoadCritters(FN.c_str(),StaticInf->TilesI);
	}


	textfont::IanOutTextR(GetMaxX,90,1,"Modify");
	textfont::IanOutTextR(GetMaxX,105,1,"------------");
		if (mouse::MouseIn(GetMaxX-100,120,GetMaxX,134))
		{ InSide=true; ref_to = &tcid; } else { InSide=false; if (tcid<0) tcid=0; };
	sprintf(buf,"Id: %i",tcid);
	textfont::IanOutText(GetMaxX-100,120,InSide ? 0 : 1,buf);

	if (mouse::MouseIn(GetMaxX-100,135,GetMaxX,149))
		{ InSide=true; ref_to = &tcvar1; } else { InSide=false; };
	sprintf(buf,"Var_1: %i",tcvar1); 
	textfont::IanOutText(GetMaxX-100,135,InSide ? 0 : 1,buf);

	if (mouse::MouseIn(GetMaxX-100,150,GetMaxX,164))
		{ InSide=true; ref_to = &tcvar2; }  else { InSide=false; };
	sprintf(buf,"Var_2: %i",tcvar2);
	textfont::IanOutText(GetMaxX-100,150,InSide ? 0 : 1,buf);

	if (mouse::MouseIn(GetMaxX-100,175,GetMaxX,189))
		{ InSide=true; ref_to = &tcgroup; }  else { InSide=false; };
	sprintf(buf,"Group: %i",tcgroup);
	textfont::IanOutText(GetMaxX-100,175,InSide ? 0 : 1,buf);

	if (mouse::MouseIn(GetMaxX-100,190,GetMaxX,209))
		{ InSide=true; ref_to = &tckarma; }  else { InSide=false; };
	sprintf(buf,"Karma: %i",tckarma);
	textfont::IanOutText(GetMaxX-100,190,InSide ? 0 : 1,buf);

	if (mouse::MouseIn(GetMaxX-100,215,GetMaxX,229))
		{ InSide=true; ref_to = &tcx; }  else { InSide=false; };
	sprintf(buf,"X: %i",tcx);
	textfont::IanOutText(GetMaxX-100,215,InSide ? 0 : 1,buf);

	if (mouse::MouseIn(GetMaxX-100,230,GetMaxX,244))
		{ InSide=true; ref_to = &tcy; }  else { InSide=false; };
	sprintf(buf,"Y: %i",tcy);
	textfont::IanOutText(GetMaxX-100,230,InSide ? 0 : 1,buf);

	textfont::IanOutTextR(GetMaxX,260,1,"Create");
	textfont::IanOutTextR(GetMaxX,275,1,"------------");

	if (mouse::MouseIn(GetMaxX-100,290,GetMaxX,304))
		{ InSide=true; ref_to = &tcproto; }  else { InSide=false; };
	sprintf(buf,"%08i",tcproto);
	textfont::IanOutText(GetMaxX-100,290,InSide ? 0 : 1,buf);

	if (mouse::MouseIn(GetMaxX-100,305,GetMaxX,320))
		{ InSide=true; ref_to = &tctype; }  else { InSide=false; };
	sprintf(buf,"Type: %i",tctype);
	textfont::IanOutText(GetMaxX-100,305,InSide ? 0 : 1,buf);

	if (mouse::MouseIn(GetMaxX-100,340,GetMaxX,355))
		{ InSide=true; }  else { InSide=false; };
	sprintf(buf,"CREATE");
	textfont::IanOutText(GetMaxX-100,340,InSide ? 0 : 1,buf);
	if (InSide && mouse::Pressed(3)) {
			unsigned int x,proto;
			unsigned short num;
			char buf[150],buf2[150],buf3[150],buf4[150];

			PlayerDef PLoad;
			PFRMPlayer Ian;
			PInventory Inven;

			Inven = new TInventory();
			x = CompLoc(tcx,tcy);
			num = tctype;
			proto = tcproto;

			PLoad.Count = tcid;
			PLoad.var1 = tcvar1;
			PLoad.var2 = tcvar2;
			PLoad.group = tcgroup;
			PLoad.karma = tckarma;
			PLoad.proto = tcproto;
						
			sprintf(buf2,"CRITTERS_%i",num);
			sprintf(buf3,"%s",GetFile("\\proto\\critters.pro").c_str());
			GetPrivateProfileString("CRITTERS",buf2,"",buf4,150,buf3);
			sprintf(buf,"\\data\\critters\\%s",buf4);

			sprintf(buf3,"%s",GetFile(buf).c_str());
			sprintf(buf4,"%s",textutil::GetFromXML(buf3,".graphics").c_str());

			PLoad.deflocation = GetFile(buf);
			
			sprintf(buf3,"\\proto\\critters\\%08i.pro",proto);
			PLoad.protolocation = GetFile(buf3);
			PLoad.num = num;
			
			PLoad.name = "\\art\\critters\\";
			PLoad.name += buf4;

			PLoad.bComputer = true;
			PLoad.bIdle = true;
			PLoad.bMove = true;
			PLoad.bRun = true;
			
			int unarmedloc;
			PLoad.Unarmed1 = new TWeapon();
			unarmedloc = atoi2(textutil::GetFromXML(PLoad.protolocation.c_str(),"/unarmed.hand1").c_str());
			if (unarmedloc<65536) LoadNewItem(StaticInf->TilesI,unarmedloc);
			PLoad.Unarmed1->Load(unarmedloc,StaticInf->TilesI);
			
			PLoad.Unarmed2 = new TWeapon();
			unarmedloc = atoi2(textutil::GetFromXML(PLoad.protolocation.c_str(),"/unarmed.hand2").c_str());
			if (unarmedloc<65536) LoadNewItem(StaticInf->TilesI,unarmedloc);
			PLoad.Unarmed2->Load(unarmedloc,StaticInf->TilesI);

			Ian = new TFRMPlayer(PLoad,Inven);
			Ian->LoadPlayer(false);
			Ian->MoveTo(DeCompLocX(x),DeCompLocY(x));
			CritterInf->Critters.insert( Critter_Pair(num,Ian));
			Ian = NULL;
	}


    if ((mousetyp<1) || (mousetyp>8)) BlitTo(g_pDDSBack,0,0,Mouse->FRM->x, Mouse->FRM->y,MousX,MousY,0,Mouse->FRM->FRM);
	
	textfont::DisplayNum(55,0,1,x,3);
	textfont::DisplayNum(85,0,1,y,3);
	textfont::DisplayNum(115,0,1,CompLoc(x,y),8);
	 
	  if ((mousetyp>0) && (mousetyp<9))
	  {
		if (kintvan) i=1; else i=0;
		x = MouseScr[mousetyp-1][i]->FRM->x;
		y = MouseScr[mousetyp-1][i]->FRM->y;
		if ((mousetyp==1) || (mousetyp>6))
		BlitTo(g_pDDSBack,0,0,x,y,MousX,MousY,0,
							  MouseScr[mousetyp-1][i]->FRM->FRM);
		if (mousetyp==2)
		BlitTo(g_pDDSBack,0,0,x,y,MousX-x,MousY,0,
							  MouseScr[mousetyp-1][i]->FRM->FRM);
		if (mousetyp==3)
		BlitTo(g_pDDSBack,0,0,x,y,MousX-x,MousY,0,
							  MouseScr[mousetyp-1][i]->FRM->FRM);
		if (mousetyp==4)
		BlitTo(g_pDDSBack,0,0,x,y,MousX-x,MousY-y,0,
							  MouseScr[mousetyp-1][i]->FRM->FRM);
		if (mousetyp==5)
		BlitTo(g_pDDSBack,0,0,x,y,MousX,MousY-y,0,
							  MouseScr[mousetyp-1][i]->FRM->FRM);
		if (mousetyp==6)
		BlitTo(g_pDDSBack,0,0,x,y,MousX,MousY-y,0,
							  MouseScr[mousetyp-1][i]->FRM->FRM);

	}



	//DisplayNum(130,0,1,gameSpeed,2);
	textutil::DisplayFrameRate();

	// Flip the surfaces
    SDL_Flip(g_pDDSBack);
	
}
