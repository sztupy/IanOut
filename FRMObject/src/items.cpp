#include "items.h"

#include "io.h"
#include <fcntl.h>
#include "../commonutils/ddutil.h"
#include "../commonutils/discutil.h"
#include "../commonutils/utils.h"
#include "math.h"

#include "windows.h"

// --- TIanMap ---
//
// ---------------

PFRMAnim6 LoadAnim(const char* filename);

PFRMSingle ExitGrid[12];

int LoadExitGrid()
{
	int hRet = 0;
	char buf[50];
	for (int i=0; i<12; i++) {
		if (i<4) {
			sprintf(buf,"\\art\\misc\\exitgrd%i.frm",(i+1)*2);
		} else if (i<8) {
			sprintf(buf,"\\art\\misc\\ext2grd%i.frm",(i-3)*2);
		} else {
			sprintf(buf,"\\art\\misc\\ext3grd%i.frm",(i-7)*2);
		}
		ExitGrid[i] = new TFRMSingle();
		hRet = ExitGrid[i]->Load(buf,1);
		if (hRet != 0) return hRet;
	}
	return 0;
}

int DeleteExitGrid()
{
	AddToLog(4,"Done> Freeing exit grid artwork");
	for (int i=0; i<12; i++) if (ExitGrid[i]) { delete ExitGrid[i]; ExitGrid[i] = NULL; }
	return 0;
}

int TIanMap::LoadMap(const char* filename)
{
	AddToLog(3,"Load> Loading tiles layout data");
	int x,y;
	gzFile stream;

	if ((stream = __IOopen(filename,"rb")) == NULL)
		return InitFail(0,"LoadMap FAILED - TilesMAP");

	for (x=0; x<128; x++)
		for (y=0; y<256; y++)
		{
			gzread(stream,&Map[x][y],2);
		}
	for (x=0; x<128; x++)
		for (y=0; y<256; y++)
		{
			gzread(stream,&Map2[x][y],2);
		}
	gzclose(stream);
	return 0;
}

int TIanMap::LoadTiles()
{
	AddToLog(3,"Load> Loading Tiles Artwork (begin)");
	int x,y,z;
	int hRet;
	int numb;
	char buf[150],buf2[150],buf3[150],buf4[150];

	FRMCollection::iterator iter;
	iter = Tiles.begin();
	while (iter!=Tiles.end()) {
		delete iter->second;
		iter++;
	}
	Tiles.clear();
	
	hRet= 0;
	
	for (x=0; x<128; x++)
		for (y=0; y<256; y++)
			for (z=0; z<2; z++)
		{
			numb=0;
			if (z==0) numb = Map[x][y];
			if (z==1) numb = Map2[x][y];
			if (numb!=0)
			if (Tiles.count(numb) ==0)
			{
				sprintf(buf,"TILES_%i",numb);
				sprintf(buf3,"%s",GetFile("\\proto\\tiles.pro").c_str());
				GetPrivateProfileString("TILES",buf,"",buf4,150,buf3);

				sprintf(buf2,"\\art\\tiles\\%s",buf4);
												
				if (Tiles.count(numb) == 0) {
				AddToLog(5,"Load> Name of picture: %s",buf2);
				Tiles[numb] = new TFRMSingle();
				hRet = Tiles[numb]->Load(buf2,1);			
				if (hRet!=0) InitFail(0,buf2);   
				}
			}
		}
	AddToLog(4,"Load> Loading Tiles Artwork (end)");
	return 0;
}

TIanMap::~TIanMap()
{
	FRMCollection::iterator iter;
	iter = Tiles.begin();
	while (iter!=Tiles.end()) {
		delete iter->second;
		iter++;
	}
	Tiles.clear();
}

void LoadNewItem(FRMPairCollection &TilesI, int num)
{
	AddToLog(4,"Load> Loading a new item: %i",num);
	int hRet;
	if (TilesI.count(num) == 0) {
		char buf[150],buf3[150],buf4[150];
		sprintf(buf,"ITEMS_%i",num);
		sprintf(buf3,"%s",GetFile("\\proto\\items.pro").c_str());
		GetPrivateProfileString("ITEMS",buf,"",buf4,150,buf3);
		std::string str;
		PFRMPair Pair;
		Pair = TilesI[num] = new TFRMPair();
		Pair->FRM = new TFRMSingle();
		Pair->FRMA = new TFRMAnim();
		sprintf(buf,"\\data\\items\\%s",buf4);
		str = "\\art\\items\\"+textutil::GetFromXML(GetFile(buf),"/graphic.picture")+".frm";
		AddToLog(5,"Load> Name of picture: %s",str.c_str());
		hRet = Pair->FRM->Load(str.c_str(),1);
		if (hRet != 0) InitFail(0,str.c_str());
		str = "\\art\\inven\\"+textutil::GetFromXML(GetFile(buf),"/graphic.inventory")+".frm";
		AddToLog(5,"Load> Name of picture: %s",str.c_str());
		hRet = Pair->FRMA->Load(str.c_str(),-1);
		if (hRet != 0) InitFail(0,str.c_str());
		Pair->FRMA->FirstFrame();
	}
}



int TIanStatic::LoadStatic(const char* filename)
{
	AddToLog(3,"Load> Loading static data (begin)");
	unsigned int x,y,proto;
	unsigned short num,opc,type;
	unsigned int num2;
	unsigned char direction,deadtype;
	int xpos,ypos;
	PLocationDat Loc;
	gzFile stream;
	char buf[150],buf2[150],buf3[150],buf4[150];
	int hRet;

	if ((stream = __IOopen(filename,"rb")) == NULL)
		return InitFail(0,"LoadMap FAILED - Static");

	gzseek(stream,(256*256*2)+(512*512), SEEK_SET );
	while (gzread(stream,&x,4) == 4) {
	/* xpos = DeCompLocX(x);
	 ypos = DeCompLocY(x);
	 if ((xpos%4)/2 != 0) ypos+=1;
	 x = CompLoc(xpos,ypos);*/
     gzread(stream,&type,2);
	 gzread(stream,&num,2);
	 if (type==5) gzread(stream,&direction,1);
	 if (type==5) gzread(stream,&deadtype,1);
	 if (type==5) gzread(stream,&proto,4);

	 if (type==5) {
		 num2 = num;
		 num2 = num2+(direction<<20)+(deadtype<<25);
	 }
	 gzread(stream,&opc,2);
	 Loc = new TLocationDat();
     Loc->type = type;
	 Loc->num = num;
	 Loc->loc = x;
	 Loc->state = 0;

	 if (type == 3) {
		sprintf(buf,"OBJECTS_%i",num);
		sprintf(buf3,"%s",GetFile("\\proto\\objects.pro").c_str());
		GetPrivateProfileString("OBJECTS",buf,"",buf4,150,buf3);
		sprintf(buf,"\\data\\objects\\%s",buf4);

		Loc->ItemDesc = new TItemDesc();
		Loc->ItemDesc->Inven = new TInventory();
		Loc->ItemDesc->ability = textutil::GetFromXML(GetFile(buf),"/desc.ability");
		Loc->ItemDesc->desclocation = GetFile(buf);
		Loc->ItemDesc->Count = 0;
		Loc->ItemDesc->var1 = 0;
		Loc->ItemDesc->var2 = 0;
		Loc->ItemDesc->direction = false;
		Loc->ItemDesc->change = false;
	}
 	 if (type == 4) {
		 
		 Loc->num = abs(Loc->num%12);
		Loc->ItemDesc = new TItemDesc();
		Loc->ItemDesc->Inven = NULL;
		Loc->ItemDesc->ability = "0000000000";
		Loc->ItemDesc->desclocation = "";
		Loc->ItemDesc->Count = 0;
		Loc->ItemDesc->var1 = 0;
		Loc->ItemDesc->var2 = 0;
		Loc->ItemDesc->direction = false;
		Loc->ItemDesc->change = false;
	}
    if (type == 5) {
		sprintf(buf2,"CRITTERS_%i",num);
		sprintf(buf3,"%s",GetFile("\\proto\\critters.pro").c_str());
		GetPrivateProfileString("CRITTERS",buf2,"",buf4,150,buf3);
		sprintf(buf,"\\data\\critters\\%s",buf4);

		sprintf(buf3,"%s",GetFile(buf).c_str());
		sprintf(buf4,"%s",textutil::GetFromXML(buf3,".graphics").c_str());

		sprintf(buf3,"\\proto\\critters\\%08i.pro",proto);
		//AddToLog(buf3);
		Loc->ItemDesc = new TItemDesc();
		Loc->ItemDesc->Inven = new TInventory();
		Loc->ItemDesc->ability = textutil::GetFromXML(GetFile(buf3),"/desc.ability");
		Loc->ItemDesc->desclocation = GetFile(buf3);
		Loc->ItemDesc->name = "\\art\\critters\\";
		Loc->ItemDesc->name+= buf4;
		Loc->ItemDesc->Count = 0;
		Loc->ItemDesc->var1 = direction;
		Loc->ItemDesc->var2 = deadtype;
		Loc->ItemDesc->direction = false;
		Loc->ItemDesc->change = false;
	}
	y = x;
	while (opc != 0xff)
	{
			if (opc == 0xffff) {
				gzread(stream,&y,4);
			}
			if (opc == 0xee) {
				gzread(stream,&Loc->state,4);
			}
			if (opc == 0x11) {
				gzread(stream,&Loc->ItemDesc->Count,4);
			}
			if (opc == 0x22) {
				gzread(stream,&Loc->ItemDesc->var1,4);
			}
			if (opc == 0x33) {
				gzread(stream,&Loc->ItemDesc->var2,4);
			}
			if (opc == 0x25) {
				int tempor;
				gzread(stream,&tempor,4);
				Loc->ItemDesc->direction = tempor!=0;
			}
			if (opc == 0xAA) {
				int num;
				num=1;
				while (num != 0) {
					unsigned short type;
					gzread(stream,&type,2);
					gzread(stream,&num,4);
					if (num!=0) LoadNewItem(TilesI,type);
					if (num!=0) Loc->ItemDesc->Inven->AddItem(type,num,0,TilesI);
				}
			}
		  gzread(stream,&opc,2);
	  }
	
	 if (type == 0)
	  if (TilesW.count(num) != 0)
	  {
 		Loc->FRM = TilesW[num];
	  } else
	  {
		sprintf(buf,"WALLS_%i",num);
		sprintf(buf3,"%s",GetFile("\\proto\\walls.pro").c_str());
		GetPrivateProfileString("WALLS",buf,"",buf4,150,buf3);
		sprintf(buf2,"\\art\\walls\\%s",buf4);
		if (TilesW.count(num) == 0) {
		TilesW[num] = new TFRMSingle();
		AddToLog(5,"Load> Loading data: %s",buf2);
		hRet = TilesW[num]->Load(buf2,1);			
		if (hRet!=0) InitFail(0,buf2);
		}
		Loc->FRM = TilesW[num];
	  }
	 if (type == 1) 
	  if (TilesS.count(num) != 0)
	  {
 		Loc->FRM = TilesS[num];
	  } else
	  {
		sprintf(buf,"SCENERY_%i",num);
		sprintf(buf3,"%s",GetFile("\\proto\\scenery.pro").c_str());
		GetPrivateProfileString("SCENERY",buf,"",buf4,150,buf3);
		sprintf(buf2,"\\art\\scenery\\%s",buf4);
		if (TilesS.count(num) == 0) {
		TilesS[num] = new TFRMSingle();
		AddToLog(5,"Load> Loading data: %s",buf2);
		hRet = TilesS[num]->Load(buf2,1);
		if (hRet!=0) InitFail(0,buf2);
		}
		Loc->FRM = TilesS[num];
	}
	if (type == 2)
		if (TilesI.count(num) != 0)
		{
			//y+=(1<<10);
			y = 2000000000;
			Loc->FRM = TilesI[num]->FRM;
			Loc->FRMA = new TFRMAnimCommunicator(TilesI[num]->FRMA);
		} else
		{
			y = 2000000000;
			LoadNewItem(TilesI,num);
			Loc->FRM = TilesI[num]->FRM;
			Loc->FRMA = new TFRMAnimCommunicator(TilesI[num]->FRMA);
		}
	if (type == 3)
		if (TilesO.count(num) != 0)
		{
			Loc->FRMA = new TFRMAnimCommunicator(TilesO[num]->FRMA);
			Loc->FRM = NULL;
		} else
		{
			sprintf(buf,"OBJECTS_%i",num);
			sprintf(buf3,"%s",GetFile("\\proto\\objects.pro").c_str());
			GetPrivateProfileString("OBJECTS",buf,"",buf4,150,buf3);
			std::string str;
			PFRMPair Pair;
			Pair = TilesO[num] = new TFRMPair();
			Pair->FRM = NULL;
			Pair->FRMA = new TFRMAnim();
			sprintf(buf,"\\data\\objects\\%s",buf4);
			str = "\\art\\"+textutil::GetFromXML(GetFile(buf),"/graphic.picture")+".frm";
			AddToLog(5,"Load> Loading data: %s",str.c_str());
			hRet = Pair->FRMA->Load(str.c_str(),-1);
			if (hRet != 0) InitFail(0,str.c_str());
			Loc->FRM = NULL;
			Pair->FRMA->FirstFrame();
			Loc->FRMA = new TFRMAnimCommunicator(Pair->FRMA);	
		}
	if (type == 4) {
			y = 2000000000;
			//AddToLog("X");
			Loc->FRM = ExitGrid[num];
			//AddToLog("X");
			Loc->FRMA = NULL;
			//AddToLog("X");
		}
	if (type == 5)
		if (TilesD.count(num2) != 0)
		{
			y = 2000000000;
			Loc->FRMA = new TFRMAnimCommunicator(TilesD[num2]->FRMA);
			Loc->FRM = NULL;
		} else {
			y = 2000000000;
			PFRMDead Pair;
			Pair = TilesD[num2] = new TFRMDead();
			Pair->FRMA = new TFRMAnim();

			sprintf(buf2,"DEAD_%i",deadtype);
			sprintf(buf,"%s%s.fr",
				Loc->ItemDesc->name.c_str(),
				textutil::GetFromProf(GetFile("\\proto\\dead.pro"),"DEAD",buf2).c_str());

			AddToLog(5,"Load> Loading data: %s",buf);
			Pair->FRMA->Load(buf,direction);

			Pair->FRMA->FirstFrame();
			Loc->FRMA = new TFRMAnimCommunicator(Pair->FRMA);
			Loc->FRM = NULL;
		}
    Map.insert( Location_Pair(y,Loc) );
	}
	gzclose(stream);
	return 0;
	AddToLog(4,"Load> Loading static data (end)");
}

TIanStatic::~TIanStatic()
{
	AddToLog(3,"Done> Freeing static data (begin)");
	FRMLocationMap::iterator it;
	it = Map.begin();
	while (it!=Map.end()) {
		delete it->second;
		it++;
	}
	Map.clear();

	FRMCollection::iterator iter;
	iter = TilesW.begin();
	while (iter!=TilesW.end()) {
		delete iter->second;
		iter++;
	}
	TilesW.clear();

	iter = TilesS.begin();
	while (iter!=TilesS.end()) {
		delete iter->second;
		iter++;
	}
	TilesS.clear();

	FRMPairCollection::iterator iter2;
	iter2 = TilesI.begin();
	while (iter2!=TilesI.end()) {
		delete iter2->second;
		iter2++;
	}
	TilesI.clear();

	iter2 = TilesO.begin();
	while (iter2!=TilesO.end()) {
		delete iter2->second;
		iter2++;
	}
	TilesO.clear();

	FRMDeadCollection::iterator iter3;
	iter3 = TilesD.begin();
	while (iter3!=TilesD.end()) {
		delete iter3->second;
		iter3++;
	}
	TilesD.clear();
	AddToLog(4,"Done> Freeing static data (end)");
}

int TIanStatic::DeleteButTiles()
{
	AddToLog(3,"Done> Freeing static data (all but tiles) (begin)");
	FRMLocationMap::iterator it;
	it = Map.begin();
	while (it!=Map.end()) {
		delete it->second;
		it++;
	}
	Map.clear();

	FRMCollection::iterator iter;
	iter = TilesW.begin();
	while (iter!=TilesW.end()) {
		delete iter->second;
		iter++;
	}
	TilesW.clear();

	iter = TilesS.begin();
	while (iter!=TilesS.end()) {
		delete iter->second;
		iter++;
	}
	TilesS.clear();

	FRMPairCollection::iterator iter2;
	iter2 = TilesO.begin();
	while (iter2!=TilesO.end()) {
		delete iter2->second;
		iter2++;
	}
	TilesO.clear();

	FRMDeadCollection::iterator iter3;
	iter3 = TilesD.begin();
	while (iter3!=TilesD.end()) {
		delete iter3->second;
		iter3++;
	}
	TilesD.clear();
	AddToLog(4,"Done> Freeing static data (all but tiles) (end)");
	return 0;
}