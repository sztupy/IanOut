#define SEARCH_TREE_COMPLEXITY 50

#include "player.h"

#include "io.h"
#include <fcntl.h>
#include "../commonutils/ddutil.h"
#include "../commonutils/textutil.h"
#include "../commonutils/utils.h"
#include "../commonutils/engine.h"
#include "../tinyxml/tinyxml.h"
#include "math.h"

// --- TFRMPlayer ---
//
// ------------------

namespace IanOs {
	void RunStat(std::string statement);
	extern PLocationDat ActualItem;
	extern PFRMPlayer ActualPlayer;
	extern BlockType *Block;
}

namespace play {
	extern bool TurnBasedMode;
}

short MoveTable[512][512];

PList AnimList;

TFRMPlayer::~TFRMPlayer()
{
	if (Idle) delete Idle; Idle = NULL;
	if (Moving) delete Moving; Moving = NULL;
	if (Runing) delete Runing; Runing = NULL;
	if (Other[0]) delete Other[0]; Other[0] = NULL;
	if (Other[1]) delete Other[1]; Other[1] = NULL;
	if (Other[2]) delete Other[2]; Other[2] = NULL;

	if (GetWeapon) delete GetWeapon; GetWeapon = NULL;
	if (PutWeapon) delete PutWeapon; PutWeapon = NULL;

	if (Inven) delete Inven; Inven = NULL;
	if (DeadAnimation) delete DeadAnimation; DeadAnimation = NULL;

	if ((Player.Unarmed1) && (Player.Unarmed1 != Hand1)) { delete Player.Unarmed1; Player.Unarmed1 = NULL; }
	if ((Player.Unarmed2) && (Player.Unarmed2 != Hand2)) { delete Player.Unarmed2; Player.Unarmed2 = NULL; }

	if (Hand1) delete Hand1; Hand1 = NULL;
	if (Hand2) delete Hand2; Hand2 = NULL;
}

void GarbageCollect()
{
	PItem Item;
	Item = AnimList->First();
	PItem Item2 = NULL;
	while (Item) {
		if (((PFRMAnim6)Item)->refcount<1) {
			Item2=Item;
		}
		Item = AnimList->Next(Item);
		if (Item2) { AnimList->Disconnect(Item2); delete Item2; Item2=NULL; }
	}
}

PFRMAnim6 LoadAnim(HWND hWnd, LPDIRECTDRAW7 g_pDD, const char* filename)
{
	PItem Item;
	PFRMAnim6 Animation;
	Item = AnimList->First();
	HRESULT hRet;
	while (Item) {
		if (lstrcmpi(((PFRMAnim6)Item)->fname,filename) == 0) {
			//textutil::AddString("reusing",rand()%2);
			return (PFRMAnim6)Item;
		}
		Item = AnimList->Next(Item);
	}
	//textutil::AddString("newanim",rand()%2);
	Animation = new TFRMAnim6();
	hRet = Animation->Load(hWnd,g_pDD,filename);
	if (hRet != DD_OK) return NULL;
	AnimList->Insert(Animation);
	return Animation;
}

void TFRMPlayer::SetStaticState()
{
	curth=0;
	afterth=0;
	Labpos=false;
	fr2=rand()%100;
	fr3=0;
	framenum=0;
	PosIt[0][0]=-1;
	PosIt[0][1]=-1;
	Idle->FirstFrame();
}

void TFRMPlayer::KillPerson(HWND hWnd, LPDIRECTDRAW7 g_pDD, int diemode)
{
	howtodie = diemode;
	char buf[50];
	char buf2[50];
	sprintf(buf2,"DIE_%i",diemode);
	sprintf(buf,"%s%s.fr0",Player.name.c_str(),
		textutil::GetFromProf(GetFile("\\proto\\dead.pro"),"DIE",buf2).c_str());
	if (!CanOpen2(buf)) {
		sprintf(buf,"%s%s.frm",Player.name.c_str(),
		textutil::GetFromProf(GetFile("\\proto\\dead.pro"),"DIE",buf2).c_str());
		if (!CanOpen2(buf)) {
			sprintf(buf,"%s%s.fr",Player.name.c_str(),
			textutil::GetFromProf(GetFile("\\proto\\dead.pro"),"DIE","DIE_0").c_str());
		} else {
			sprintf(buf,"%s%s.fr",Player.name.c_str(),
			textutil::GetFromProf(GetFile("\\proto\\dead.pro"),"DIE",buf2).c_str());
		}
	} else {
		sprintf(buf,"%s%s.fr",Player.name.c_str(),
		textutil::GetFromProf(GetFile("\\proto\\dead.pro"),"DIE",buf2).c_str());
	}
	
	DeadAnimation = new TFRMAnim();
	DeadAnimation->Load(hWnd,g_pDD,buf,dir);
	DeadAnimation->FirstFrame();
	curth = 200;
	framenum=0;
	afterth=0;
	fr3 = 0;
}

void TFRMPlayer::Die(HWND hWnd, LPDIRECTDRAW7 g_pDD, PIanStatic StaticInf)
{
	PLocationDat Loc;
	int y,x;
	char buf[150],buf2[150];
	int direction = dir;
	int deadtype = howtodie;
	unsigned short num = Player.num;
	unsigned int num2 = num;
	num2 = num2+(direction<<20)+(deadtype<<25);

	x = conv;
	
	Loc = new TLocationDat();
	Loc->type = 5;
	Loc->num = num;
	Loc->loc = x;
	Loc->state = 0;
	y = x;
	y=2000000000;

	if (Hand1->number<=65536) {
		Inven->AddItem(Hand1->number,1,Hand1->numammo,StaticInf->TilesI);
	}
	if (Hand2->number<=65536) {
		Inven->AddItem(Hand2->number,1,Hand2->numammo,StaticInf->TilesI);
	}

	Loc->ItemDesc = new TItemDesc();
	Loc->ItemDesc->Inven = new TInventory();
	Loc->ItemDesc->ability = textutil::GetFromXML(Player.protolocation,"/desc.ability");
	Loc->ItemDesc->desclocation = Player.protolocation;
	Loc->ItemDesc->name = Player.name;
	Loc->ItemDesc->Count = 0;
	Loc->ItemDesc->var1 = Player.var1;
	Loc->ItemDesc->var2 = Player.var2;
	Loc->ItemDesc->direction = false;
	Loc->ItemDesc->change = false;

	IanOs::RunStat(textutil::GetFromXML(Loc->ItemDesc->desclocation,"/events/onkilled:"));

	PItem Item;
	Item = Inven->Inven->First();
	PItem Item2;
	while (Item!=NULL) {
		Item2 = Item;
		Item = Inven->Inven->Next(Item);
		Loc->ItemDesc->Inven->AddItem(
			 Inven->GetItem(PInventoryItem(Item2)));
	}

	if (StaticInf->TilesD.count(num2) != 0)
		{
			y=2000000000;
			Loc->FRMA = new TFRMAnimCommunicator(StaticInf->TilesD[num2]->FRMA);
			Loc->FRM = NULL;
		} else {
			y=2000000000;
			PFRMDead Pair;
			Pair = StaticInf->TilesD[num2] = new TFRMDead();
			Pair->FRMA = new TFRMAnim();
			sprintf(buf2,"DEAD_%i",deadtype);
			sprintf(buf,"%s%s.fr",
				Loc->ItemDesc->name.c_str(),
				textutil::GetFromProf(GetFile("\\proto\\dead.pro"),"DEAD",buf2).c_str());

			AddToLog("opcio4 %s",buf);
			Pair->FRMA->Load(hWnd,g_pDD,buf,direction);

			Pair->FRMA->FirstFrame();
			Loc->FRMA = new TFRMAnimCommunicator(Pair->FRMA);
			Loc->FRM = NULL;
		}
	StaticInf->Map.insert( Location_Pair(y,Loc) );
}

void TFRMPlayer::LookAt(int atx, int aty)
{
	int fromx = LocConvertX(x,y);
	int fromy = LocConvertY(x,y);
	
	int tox = LocConvertX(atx,aty);
	int toy = LocConvertY(atx,aty);

	if (fromx<tox) {
		if (fromy>toy) {
			dir = 0;
		} else {
			dir = 2;
		}
	} else {
		if (fromy<toy) {
			dir = 3;
		} else {
			dir = 5;
		}
	}
	if ((dir==0) || (dir==2)) {
		if ((tox-fromx)>=abs(toy-fromy)*4) dir=1;
	}
	if ((dir==3) || (dir==5)) {
		if ((fromx-tox)>=abs(toy-fromy)*4) dir=4;
	}
}

std::string TFRMPlayer::GetGraphicNameWeapon(char* descriptor, PWeapon Weapon)
{
	std::string xXx = "";
	if (Weapon) {
		if (Weapon->GetCurrentMode()) {
			std::string realname;
			realname = GetFile(Weapon->deflocation);

			TiXmlDocument XMLD(realname.c_str());
			XMLD.LoadFile();
			TiXmlNode* parent2;
			const TiXmlElement* parent = textutil::XMLSearchNode(XMLD.RootElement(),"/desc.");
			TiXmlNode* child = 0;
			while( child = parent->IterateChildren("mode",child ) )
			{
				if ((child->ToElement()->Attribute("name")) && (Weapon->GetCurrentMode()->name == child->ToElement()->Attribute("name"))) {
					parent2 = child->FirstChild("critter");
					if (parent2) {
						if (parent2->ToElement()->Attribute(descriptor)) {
							xXx=Player.name+parent2->ToElement()->Attribute(descriptor)+".frm";
						}
					}
				}
			}
		}
		if (FileExist.find(xXx) == FileExist.end()) {
			if (!CanOpen2(xXx)) {
				FileExist[xXx] = false;
				xXx = Player.name+textutil::GetFromXML(Weapon->deflocation,std::string("/critter.")+descriptor)+".frm";
			} else {
				FileExist[xXx] = true;
			}
		} else {
			if (!FileExist[xXx]) xXx = Player.name+textutil::GetFromXML(Weapon->deflocation,std::string("/critter.")+descriptor)+".frm";
		}
	}
	if (FileExist.find(xXx) == FileExist.end()) {
			if (!CanOpen2(xXx)) {
				FileExist[xXx] = false;
				xXx = xXx = Player.name+textutil::GetFromXML(Player.deflocation,std::string("/moving.")+descriptor)+".frm";
			} else {
				FileExist[xXx] = true;
			}
		} else {
			if (!FileExist[xXx]) xXx = Player.name+textutil::GetFromXML(Player.deflocation,std::string("/moving.")+descriptor)+".frm";
		}
	return xXx;
}

std::string TFRMPlayer::GetGraphicName(char* descriptor)
{
	PWeapon Weapon;
	if (curhand) Weapon = Hand1; else Weapon = Hand2;
	return GetGraphicNameWeapon(descriptor,Weapon);
}

void TFRMPlayer::LoadPlayer(HWND hWnd, LPDIRECTDRAW7 g_pDD, bool isLoaded)
{
	PFRMAnim6 Animation;

	if (!isLoaded) {
		Hand1 = Player.Unarmed1;
		Hand2 = Player.Unarmed2;
		ianskill::CountSk(&PlayerSkill);
		PlayerSkill.HP = PlayerSkill.AlapSk.HP;
		Player.CharName = textutil::GetFromXML(Player.protolocation,"/text.yousee");
	}
	
	if (Player.bIdle)
	{
	 if (Idle) delete Idle; Idle = NULL;
	 Animation = LoadAnim(hWnd,g_pDD,(GetGraphicName("idle")).c_str());
	 Idle = new TFRMCommunicator(Animation);
	}
	if (Player.bMove)
	{
	 if (Moving) delete Moving; Moving = NULL;
	 Animation = LoadAnim(hWnd,g_pDD,(GetGraphicName("move")).c_str());
	 Moving = new TFRMCommunicator(Animation);
	}
	if (Player.bRun)
	{
	 if (Runing) delete Runing; Runing = NULL;
	 Animation = LoadAnim(hWnd,g_pDD,(GetGraphicName("run")).c_str());
	 Runing = new TFRMCommunicator(Animation);
	}
}

inline int TFRMPlayer::Distance(int atx,int aty)
{
	int ydist = abs(y-aty);
	int xdist = abs((x+y)-(atx+aty));
	return (xdist+ydist)/2+(ydist>xdist ? (ydist-xdist)/2 : 0);
}

void TFRMPlayer::GoAround(int atx, int aty, BlockType &Block)
{
	int dist[6];bool dprb[6];
	int i,i2;
	for (i=0;i<6;i++) dprb[i] = false;
	int p=(atx%4)/2;
	dist[0] = RealDistance(atx+2,aty+1-p,Block);
	dist[1] = RealDistance(atx+2,aty-p,Block);
	dist[2] = RealDistance(atx  ,aty+1,Block);
	dist[3] = RealDistance(atx-2,aty+1-p,Block);
	dist[4] = RealDistance(atx-2,aty-p,Block);
	dist[5] = RealDistance(atx  ,aty-1,Block);

	SetStaticState();
	for (i2=0;i2<6;i2++) {
		int mind=0;
		int min = 1000000; 
		for (i=0;i<6;i++) {
			if ((dist[i]>=0) && (min>dist[i]) && (!dprb[i])) {
				min = dist[i];
				mind = i;
			}
		}
		dprb[mind] = true;
		if (curth==0) {
			//AddToLog("Runing");
			int p=((atx+2)%4)/2;
			switch (mind) {
				case 0: GenerateTree(atx+2,aty+1-p,Block); break;
				case 1: GenerateTree(atx+2,aty-p,Block); break;
				case 2: GenerateTree(atx  ,aty+1,Block); break;
				case 3: GenerateTree(atx-2,aty+1-p,Block); break;
				case 4: GenerateTree(atx-2,aty-p,Block); break;
				case 5: GenerateTree(atx  ,aty-1,Block); break;
			}
		}
	}
}

void TFRMPlayer::Pickup(HWND hWnd, LPDIRECTDRAW7 g_pDD,PLocationDat LocDat,BlockType &Block)
{
	afterth = 0;
	PFRMAnim6 Animation;
	if (Other[0]) delete Other[0]; Other[0] = NULL;
	Animation = LoadAnim(hWnd,g_pDD,(GetGraphicName("pickup")).c_str());
	Other[0] = new TFRMCommunicator(Animation);

	int iy = DeCompLocY(LocDat->loc);
	int ix = DeCompLocX(LocDat->loc);
	GoAround(ix,iy,Block);
	
	afterth = 4;
	if ((x == ix) && (y == iy)) curth = 4;
	framenum = 0;
	fr3=0;
	LocationSave = LocDat;
	PlayerSave = NULL;
}

void TFRMPlayer::UseOn(HWND hWnd, LPDIRECTDRAW7 g_pDD,PLocationDat LocDat,BlockType &Block,bool type, signed char dirx, signed char diry)
{
	afterth = 0;
	PFRMAnim6 Animation;
	if (Other[1]) delete Other[1]; Other[1] = NULL;
	if (type) {
		Animation = LoadAnim(hWnd,g_pDD,(GetGraphicName("use")).c_str());
	} else {
		Animation = LoadAnim(hWnd,g_pDD,(GetGraphicName("pickup")).c_str());
	}
	Other[1] = new TFRMCommunicator(Animation);

	int iy = DeCompLocY(LocDat->loc);
	int ix = DeCompLocX(LocDat->loc);
	if ((dirx==0) && (diry==0)) GoAround(ix,iy,Block);
		else GenerateTree(ix+dirx,iy+diry,Block);
	afterth = 5;
	if ((x == ix+dirx) && (y == iy+diry)) curth = 5;
	framenum = 0;
	fr3=0;
	LocationSave = LocDat;
	PlayerSave = NULL;
}

void TFRMPlayer::UseCommand(HWND hWnd, LPDIRECTDRAW7 g_pDD,PItem Item,bool ChrType,BlockType &Block,bool type,std::string command)
{ 
	afterth = 0;
	PFRMAnim6 Animation;
	if (Other[1]) delete Other[1]; Other[1] = NULL;
	if (type) {
		Animation = LoadAnim(hWnd,g_pDD,(GetGraphicName("use")).c_str());
	} else {
		Animation = LoadAnim(hWnd,g_pDD,(GetGraphicName("pickup")).c_str());
	}
	Other[1] = new TFRMCommunicator(Animation);

	int ix;
	int iy;
	if (ChrType) {
		ix = PFRMPlayer(Item)->x;
		iy = PFRMPlayer(Item)->y;
		PlayerSave = PFRMPlayer(Item);
		LocationSave = NULL;
	} else {
		iy = DeCompLocY(PLocationDat(Item)->loc);
		ix = DeCompLocX(PLocationDat(Item)->loc);
		LocationSave = PLocationDat(Item);
		PlayerSave = NULL;
	}

	if (!((x==ix) && (y==iy)))
		GoAround(ix,iy,Block);
	afterth = 6;
	if ((x == ix) && (y == iy)) curth = 6;
	framenum = 0;
	fr3=0;

	UseType = ChrType;
	execcomm = command;
}

void TFRMPlayer::HitAndMiss(HWND hWnd, LPDIRECTDRAW7 g_pDD,int type)
{
	PFRMAnim6 Animation;
	if (Other[2]) delete Other[2]; Other[2] = NULL;
	if (type == 0) {
		Animation = LoadAnim(hWnd,g_pDD,(GetGraphicName("hit")).c_str());
	} else if (type == 1) {
		Animation = LoadAnim(hWnd,g_pDD,(GetGraphicName("hitback")).c_str());
	} else {
		Animation = LoadAnim(hWnd,g_pDD,(GetGraphicName("miss")).c_str());
	}
	Other[2] = new TFRMCommunicator(Animation);
	framenum = 0;
	fr3=0;
	afterth = 50;
	curth = 50;
}

void TFRMPlayer::Attack(HWND hWnd, LPDIRECTDRAW7 g_pDD,PFRMPlayer Item,PWeapon AttackWh)
{ 
	afterth = 0;
	PFRMAnim6 Animation;
	if (Other[0]) delete Other[0];Other[0]=NULL;
	std::string xXx;
	xXx = GetGraphicName("before");
	//AddToLog(xXx.c_str());
	if (CanOpen2(xXx)) {
		Animation = LoadAnim(hWnd,g_pDD,(xXx).c_str());
		Other[0] = new TFRMCommunicator(Animation);
	}
	if (Other[1]) delete Other[1];Other[1]=NULL;
	xXx = GetGraphicName("shoot");
	//AddToLog(xXx.c_str());
	if (CanOpen2(xXx)) {
		Animation = LoadAnim(hWnd,g_pDD,(xXx).c_str());
		Other[1] = new TFRMCommunicator(Animation);
	}
	if (Other[2]) delete Other[2];Other[2] = NULL;
	xXx = GetGraphicName("after");
	//AddToLog(xXx.c_str());
	if (CanOpen2(xXx)) {
		Animation = LoadAnim(hWnd,g_pDD,(xXx).c_str());
		Other[2] = new TFRMCommunicator(Animation);
	}

	int ix;
	int iy;
	ix = PFRMPlayer(Item)->x;
	iy = PFRMPlayer(Item)->y;
	PlayerSave = PFRMPlayer(Item);
	LocationSave = NULL;

	WeaponSave = AttackWh;

	LookAt(ix,iy);
	curth = 20;
	framenum = 0;
	fr3=0;
	//AddToLog("TeljesenOK");
}

void TFRMPlayer::GenerateTree(int tx, int ty, BlockType &Block)
{
	if (curth>=3) return;
	if (play::TurnBasedMode && (PlayerSkill.secondary[1]<=0)) {SetStaticState(); return; }
	if (Block[tx][ty]!=0) {SetStaticState();return;}
	StopMove = false;
	cx = tx;
	cy = ty;
	int ix,iy,i,i2;
	int ix2,iy2,tx2,ty2;
	int Long[6][3];
	bool Mehet;
	bool Ok;

	for (ix=0; ix<512; ix++)
		for (iy=0; iy<512; iy++)
		{
		  MoveTable[ix][iy] = SEARCH_TREE_COMPLEXITY+100;
		}
	MoveTable[x][y] = 1;
	i = 1;
	Mehet = true;
	while (Mehet)
	{
	  for (ix=2; ix<509; ix++)
		for (iy=1; iy<509; iy++)
		{
			if (MoveTable[ix][iy] == i)
			{
				int p = ((ix+2)%4)/2;
				if ((MoveTable[ix  ][iy-1  ] > i+1) && (Block[ix  ][iy-1  ]==0)) MoveTable[ix  ][iy-1  ] = i+1; 
				if ((MoveTable[ix  ][iy+1  ] > i+1) && (Block[ix  ][iy+1  ]==0)) MoveTable[ix  ][iy+1  ] = i+1;

				if ((MoveTable[ix+2][iy-p  ] > i+1) && (Block[ix+2][iy-p  ]==0)) MoveTable[ix+2][iy-p  ] = i+1;
				if ((MoveTable[ix-2][iy-p  ] > i+1) && (Block[ix-2][iy-p  ]==0)) MoveTable[ix-2][iy-p  ] = i+1;

				if ((MoveTable[ix-2][iy+1-p] > i+1) && (Block[ix-2][iy+1-p]==0)) MoveTable[ix-2][iy+1-p] = i+1;
				if ((MoveTable[ix+2][iy+1-p] > i+1) && (Block[ix+2][iy+1-p]==0)) MoveTable[ix+2][iy+1-p] = i+1;
			}
		}
	  i++;
	  if (i>SEARCH_TREE_COMPLEXITY ) Mehet=false;
	  if (MoveTable[tx][ty]!=(SEARCH_TREE_COMPLEXITY+100)) Mehet=false;
	}

	if ((tx==x) && (ty==y)) {i=(SEARCH_TREE_COMPLEXITY + 1);}; 

	if (i<SEARCH_TREE_COMPLEXITY)
	{
		PosIt[0][0]=i;
		PosIt[0][1]=i;
		PosIt[i][0]=tx;
		PosIt[i][1]=ty;

		Mehet = true;
		while (Mehet)
		{
			i--;
			for (i2=0; i2<6; i2++)
			{
				Long[i2][0] = -1;
				Long[i2][1] = -1;
				Long[i2][2] = -1;
			}
			i2=0;
			if (i<1) {Mehet=false;i=1;}
			for (ix=2; ix<509; ix++)
				for (iy=1; iy<509; iy++)
				{
				if (MoveTable[ix][iy] == i)
				{
					Ok = false;
					int p = ((ix+2)%4)/2;
					if ((ix+2 == PosIt[i+1][0]) &&
						(iy-p == PosIt[i+1][1])) { Ok = true; }

					if ((ix-2 == PosIt[i+1][0]) &&
						(iy-p == PosIt[i+1][1])) { Ok = true; }

					if ((ix   == PosIt[i+1][0]) &&
						(iy+1 == PosIt[i+1][1])) { Ok = true; }

					if ((ix+2 == PosIt[i+1][0]) &&
						(iy+1-p == PosIt[i+1][1])) { Ok = true; }

					if ((ix-2 == PosIt[i+1][0]) &&
						(iy+1-p == PosIt[i+1][1])) { Ok = true; }

					if ((ix   == PosIt[i+1][0]) &&
						(iy-1 == PosIt[i+1][1])) { Ok = true; }

					if (Ok)
					{
						Long[i2][0]=ix;
						Long[i2][1]=iy;

						iy2 = LocConvertY(ix,iy)*12;
						ix2 = LocConvertX(ix,iy)*16;
						tx2 = LocConvertX(tx,ty)*16;
						ty2 = LocConvertY(x,y)*12;
						Long[i2][2]=(int)sqrt(double(abs(ix2-tx2)*abs(ix2-tx2) + abs(iy2-ty2)*abs(iy2-ty2)));
						i2++;
					}
				}
				}
			iy2=9999;
			tx2=0;
			for (ix2=0; ix2<i2; ix2++)
			{
				if (Long[ix2][2]<iy2)
				{
					iy2 = Long[ix2][2];
					tx2=ix2;
				}
			}
			PosIt[i][0]=Long[tx2][0];
			PosIt[i][1]=Long[tx2][1];
			}
		    
	} else
	{
		SetStaticState();
	}

	i2 = PosIt[0][0];
	if (i2>1)
	{
		tx2 = PosIt[1][0];
		ty2 = PosIt[1][1];
		ix2 = PosIt[2][0];
		iy2 = PosIt[2][1];
		int p =((tx2+2)%4)/2;
		if ((tx2+2==ix2) && (ty2-p  ==iy2)) { dir =0; }
		if ((tx2+2==ix2) && (ty2+1-p==iy2)) { dir =1; }
		if ((tx2  ==ix2) && (ty2+1  ==iy2)) { dir =2; }
		if ((tx2-2==ix2) && (ty2+1-p==iy2)) { dir =3; }
		if ((tx2-2==ix2) && (ty2-p  ==iy2)) { dir =4; }
		if ((tx2  ==ix2) && (ty2-1  ==iy2)) { dir =5; }
		curth=1;
	} else {
		SetStaticState();
	}

	ox = x;
	oy = y;
	if (i2>1)
	{
	PosIt[0][1]--;
	x = PosIt[PosIt[0][0]+1-PosIt[0][1]][0];
	y = PosIt[PosIt[0][0]+1-PosIt[0][1]][1];
	}
	framenum=0;
	afterth=0;
	fr2 = rand()%100;
	fr3=0;
	Moving->FirstFrame();
	Runing->FirstFrame();
	Idle->FirstFrame();
	//Labpos = false;
	RunPos = 0;
	if (curth==1)
		if (play::TurnBasedMode) PlayerSkill.secondary[1]--;
}

int TFRMPlayer::RealDistance(int tx, int ty, BlockType &Block)
{
	if (Block[tx][ty]!=0) {return -1;}
	cx = tx;
	cy = ty;
	int ix,iy,i;
	bool Mehet;
	for (ix=0; ix<512; ix++)
		for (iy=0; iy<512; iy++)
		{
		  MoveTable[ix][iy] = SEARCH_TREE_COMPLEXITY + 20;
		}
	MoveTable[x][y] = 1;
	i = 1;
	Mehet = true;
	while (Mehet)
	{
	  for (ix=2; ix<509; ix++)
		for (iy=1; iy<509; iy++)
		{
			if (MoveTable[ix][iy] == i)
			{
				int p = ((ix+2)%4)/2;
				if ((MoveTable[ix  ][iy-1  ] > i+1) && (Block[ix  ][iy-1  ]==0)) MoveTable[ix  ][iy-1  ] = i+1; 
				if ((MoveTable[ix  ][iy+1  ] > i+1) && (Block[ix  ][iy+1  ]==0)) MoveTable[ix  ][iy+1  ] = i+1;

				if ((MoveTable[ix+2][iy-p  ] > i+1) && (Block[ix+2][iy-p  ]==0)) MoveTable[ix+2][iy-p  ] = i+1;
				if ((MoveTable[ix-2][iy-p  ] > i+1) && (Block[ix-2][iy-p  ]==0)) MoveTable[ix-2][iy-p  ] = i+1;

				if ((MoveTable[ix-2][iy+1-p] > i+1) && (Block[ix-2][iy+1-p]==0)) MoveTable[ix-2][iy+1-p] = i+1;
				if ((MoveTable[ix+2][iy+1-p] > i+1) && (Block[ix+2][iy+1-p]==0)) MoveTable[ix+2][iy+1-p] = i+1;
			}
		}
	  i++;
	  if (i>SEARCH_TREE_COMPLEXITY ) Mehet=false;
	  if (MoveTable[tx][ty]!=(SEARCH_TREE_COMPLEXITY +20)) Mehet=false;
	}
	if (i>SEARCH_TREE_COMPLEXITY) return -1;
	return i;
}

void TFRMPlayer::Move(BlockType &Block)
{
	int i;

	if (play::TurnBasedMode && (PlayerSkill.secondary[1]<=0)) 
	{
		SetStaticState();
		//textutil::AddString("Not enough Action Points",1);
	} else
	if ((PosIt[0][0] != -1))
	{
	if (PosIt[0][1] == 1)
	{
		int afterthsave = afterth;
		SetStaticState();
		curth = afterthsave;
	} else
	{
		if (play::TurnBasedMode) PlayerSkill.secondary[1]--;
		PosIt[0][1]--;
		ox = x;
		oy = y;
		x = PosIt[PosIt[0][0]+1-PosIt[0][1]][0];
		y = PosIt[PosIt[0][0]+1-PosIt[0][1]][1];
		int p=((ox+2)%4)/2;
		if ((ox==x  ) && (oy==y+1  )) { dir =5; }
		if ((ox==x+2) && (oy==y-1+p)) { dir =3; }
		if ((ox==x+2) && (oy==y+p  )) { dir =4; }
		if ((ox==x-2) && (oy==y-1+p)) { dir =1; }
		if ((ox==x-2) && (oy==y+p  )) { dir =0; }
		if ((ox==x  ) && (oy==y-1  )) { dir =2; }
		if (curth==1) {
		Labpos = !Labpos;
		Moving->SetDir(dir);
		Moving->FirstFrame();
		if (Labpos == true)
		{
			for (i=0; i<4; i++)
			{
				Moving->NextFrame();
			}
		}
		}
		if (curth==2) {
		RunPos++;
		if (RunPos>2) RunPos = 0;
		Runing->SetDir(dir);
		Runing->FirstFrame();
		for (i=0; i<RunPos*3; i++)
		{
			Runing->NextFrame();
		}
		}
	}
	} else
	{
		int afterthsave = afterth;
		SetStaticState();
		curth = afterthsave;
	}
}

int TFRMPlayer::GetDir()
{
	int ox,oy,nx,ny;
	ox = x;
	oy = y;
	nx = PosIt[PosIt[0][0]+1-PosIt[0][1]-1][0];
	ny = PosIt[PosIt[0][0]+1-PosIt[0][1]-1][1];
	int p=((ox+2)%4)/2;
	if ((ox==nx  ) && (oy==ny+1  )) { dir =5; }
	if ((ox==nx+2) && (oy==ny-1+p)) { dir =3; }
	if ((ox==nx+2) && (oy==ny+p  )) { dir =4; }
	if ((ox==nx-2) && (oy==ny-1+p)) { dir =1; }
	if ((ox==nx-2) && (oy==ny+p  )) { dir =0; }
	if ((ox==nx  ) && (oy==ny-1  )) { dir =2; }
	return 0;
}

//FRMLocationMap::iterator FindItem(FRMLocationMap &FRMMap, PLocationDat 
template<class T> typename T::iterator FindItem(T &FRMMap, typename T::mapped_type LocationSave)
{
	typename T::iterator iter = FRMMap.begin();
	typename T::iterator useon = FRMMap.end();
	while (iter != FRMMap.end()) {
		if (iter->second == LocationSave) {
			useon = iter;
			break;
		}
		iter++;
	}
	return useon;
}

void TFRMPlayer::Next(HWND hWnd, LPDIRECTDRAW7 g_pDD,BlockType &Block,FRMLocationMap &FRMMap,CritterList &Critters)
{
	if (curth == 20)
	{
		if (Other[0]) {
		fr3++;
		if (fr3>1) {
			fr3=0;
			framenum++;
			Other[0]->NextFrame();
		}
		if (framenum>=Other[0]->Animation->framecounter) { curth=21;framenum=0;fr3=0; }
		} else curth=21;				
	}
	if (curth == 21)
	{
		if (Other[1]) {
		fr3++;
		if (fr3>1) {
			fr3=0;
			framenum++;
			Other[1]->NextFrame();
		}
		if ((framenum>=1) && (WeaponSave))
			{
				CritterList::iterator useonchr = FindItem(Critters,PlayerSave);
				if (useonchr != Critters.end()) {
					int befdir = PlayerSave->dir;
					PlayerSave->LookAt(x,y);
					int SHtype = 2;
					switch (PlayerSave->dir) {
						case 0:if ((befdir==3) || (befdir==4)) SHtype = 1;break;
						case 1:if ((befdir==4) || (befdir==5)) SHtype = 1;break;
						case 2:if ((befdir==5) || (befdir==0)) SHtype = 1;break;
						case 3:if ((befdir==0) || (befdir==1)) SHtype = 1;break;
						case 4:if ((befdir==1) || (befdir==2)) SHtype = 1;break;
						case 5:if ((befdir==2) || (befdir==3)) SHtype = 1;break;
					}
					PlayerSave->dir = befdir;

					if (WeaponSave->capacity!=0) WeaponSave->numammo--;

					PWeaponMode WMode = WeaponSave->GetCurrentMode();
					if (WMode) {
						int enemdist = PlayerSave->Distance(x,y);
						int hitloc = abs(WMode->minr-enemdist);
						int chance = 50-((double)hitloc/(double)WMode->maxr)*50;
						bool isHit = rand()%100<=chance;
						if (isHit && (SHtype != 1)) SHtype = 0;
						int hitamount = rand()%(WMode->maxdam-WMode->mindam)+WMode->mindam;
						char buf[100];
						if (isHit) {
							PlayerSave->PlayerSkill.HP-=hitamount;
						}
						if (PlayerSave->PlayerSkill.HP<=0) {
							int diemode;
							diemode = (SHtype == 1) ? 1 : 0;
							if (rand()%100<=WMode->normalcriticalchance) diemode = WMode->criticaldeath;
							PlayerSave->KillPerson(hWnd,g_pDD,diemode);
							sprintf(buf,"%s was hit for %i hit points, and was killed",PlayerSave->Player.CharName.c_str(),hitamount);
							textutil::AddString(buf,1);
						} else {
							if (((!isHit) && (rand()%10<=7)) || (isHit))
								PlayerSave->HitAndMiss(hWnd,g_pDD,SHtype);

							if ((SHtype==0) && (isHit))
								sprintf(buf,"%s was hit for %i hit points",PlayerSave->Player.CharName.c_str(),hitamount);
							if ((SHtype==1) && (isHit))
								sprintf(buf,"%s was hit in back for %i hit points",PlayerSave->Player.CharName.c_str(),hitamount);
							if ((SHtype==2) || (!isHit))
								sprintf(buf,"%s missed",Player.CharName.c_str());
							textutil::AddString(buf,1);
						}
					}
					
				} else PlayerSave = NULL;
				WeaponSave = NULL;
				}
		if (framenum>=Other[1]->Animation->framecounter) { curth=22;framenum=0;fr3=0; }
		} else curth=22;				
	}
	if (curth == 22)
	{
		if (Other[2]) {
		fr3++;
		if (fr3>1) {
			fr3=0;
			framenum++;
			Other[2]->NextFrame();
		}
		if (framenum>=Other[2]->Animation->framecounter) { SetStaticState();}
		} else SetStaticState();				
	}
	if (curth == 50)
	{
		if (Other[2]) {
		fr3++;
		if (fr3>1) {
			fr3=0;
			framenum++;
			Other[2]->NextFrame();
		}
		if (framenum>=Other[2]->Animation->framecounter) { SetStaticState();}
		} else SetStaticState();				
	}
	if (curth == 1)
	{
		fr3++;
		if (fr3>0) {
			fr3=0;
			framenum++;
			Moving->NextFrame();
		}
		if (framenum>Moving->Animation->framecounter/2-1) { framenum=0;if (StopMove) { SetStaticState(); }Move(Block); }
				
	}
	if (curth == 2)
	{
		framenum++;
		Runing->NextFrame();
		int xXx = Runing->Animation->framecounter%3;
		if (RunPos!=2)
		{
		if (framenum>Runing->Animation->framecounter/3-1) { framenum=0;if (StopMove) { SetStaticState();} else Move(Block); }
		} else
		{
		if (framenum>Runing->Animation->framecounter/3-1-xXx) { framenum=0;if (StopMove) { SetStaticState(); }Move(Block); }
		}
		
	}
	if (curth == 4)
	{
		if (LocationSave != NULL) {
			FRMLocationMap::iterator useon = FindItem(FRMMap,LocationSave);
			if (useon!=FRMMap.end()) {
				int iy = DeCompLocY(useon->second->loc);
				int ix = DeCompLocX(useon->second->loc);
				LookAt(ix,iy);
			} else  {
				LocationSave = NULL;
			}
		}
		fr3++;
		if (fr3>1) { framenum++;Other[0]->NextFrame();fr3=0; }
		if (framenum>=Other[0]->Animation->framecounter) { SetStaticState(); }
		if ((framenum==Other[0]->Animation->framecounter/2) && (LocationSave != NULL)) 
			{
				FRMLocationMap::iterator useon = FindItem(FRMMap,LocationSave);
				if (useon!=FRMMap.end()) {
					TInventoryItem Item;
					Item.count = useon->second->state+1;
					Item.state = 0;
					Item.num = useon->second->num;
					Item.FRMA = useon->second->FRMA->Animation;
					Inven->AddItem(Item);
					FRMMap.erase(useon);
				}
				LocationSave = NULL;
			}
	}
	if (curth == 5)
	{
		if (LocationSave != NULL) {
			FRMLocationMap::iterator useon = FindItem(FRMMap,LocationSave);
			if (useon!=FRMMap.end()) {
				int iy = DeCompLocY(useon->second->loc);
				int ix = DeCompLocX(useon->second->loc);
				LookAt(ix,iy);
			} else  {
				LocationSave = NULL;
			}
		}
		fr3++;
		if (fr3>1) { framenum++;Other[1]->NextFrame();fr3=0;Labpos=false;}
		if (framenum==Other[1]->Animation->framecounter/2) {
			if ((!Labpos) && (LocationSave != NULL)) {
				FRMLocationMap::iterator useon = FindItem(FRMMap,LocationSave);
				if (useon!=FRMMap.end()) {
					IanOs::Block = &Block;
					IanOs::ActualItem = useon->second;
					IanOs::RunStat(textutil::GetFromXML(useon->second->ItemDesc->desclocation,"/desc/onuse/script:"));
					Labpos = true;
				} else LocationSave = NULL;
			}
		}
		if (framenum>=Other[1]->Animation->framecounter)
		 {
			SetStaticState();
			if (LocationSave != NULL) {
				FRMLocationMap::iterator useon = FindItem(FRMMap,LocationSave);
				if (useon!=FRMMap.end()) {
					IanOs::Block = &Block;
					IanOs::ActualItem = useon->second;
					IanOs::RunStat(textutil::GetFromXML(useon->second->ItemDesc->desclocation,"/desc/onuse/script2:"));
				}
				LocationSave = NULL;
			}
		 }
	}
	if (curth == 6)
	{
		if (LocationSave != NULL) {
			FRMLocationMap::iterator useon = FindItem(FRMMap,LocationSave);
			if (useon != FRMMap.end()) {
				int iy = DeCompLocY(useon->second->loc);
				int ix = DeCompLocX(useon->second->loc);
				LookAt(ix,iy);
			} else LocationSave = NULL;
		}
		if (PlayerSave != NULL) {
			CritterList::iterator useonchr = FindItem(Critters,PlayerSave);
			if (useonchr != Critters.end()) {
				int iy = (useonchr->second->y);
				int ix = (useonchr->second->x);
				LookAt(ix,iy);
			} else PlayerSave = NULL;
		}
		fr3++;
		if (fr3>1) { framenum++;Other[1]->NextFrame();fr3=0; }
		if (framenum==Other[1]->Animation->framecounter/2) {
			if ((LocationSave)) {
				FRMLocationMap::iterator useon = FindItem(FRMMap,LocationSave);
				if (useon!=FRMMap.end()) {
					IanOs::Block = &Block;
					IanOs::ActualItem = useon->second;
					IanOs::RunStat(execcomm);
				}
				LocationSave = NULL;
			}
			if ((PlayerSave)) {
				CritterList::iterator useonchr = FindItem(Critters,PlayerSave);
				if (useonchr!=Critters.end()) {
					if (Distance(PlayerSave->x,PlayerSave->y)<=1) {
						IanOs::Block = &Block;
						IanOs::ActualPlayer = useonchr->second;
						IanOs::RunStat(execcomm);
					}
				}
				PlayerSave = NULL;
			}
			
		
		}
		if (framenum>=Other[1]->Animation->framecounter)
		 {
			 SetStaticState();
		 }
	}
	if (curth == 200)
	{
		if (DeadAnimation->GetLastFrame()) {
			framenum=0;
			curth=255;
			afterth=0;
		}
		fr3++;
		if (fr3>1) { framenum++;DeadAnimation->NextFrame();fr3=0; }

		
	}
	if (curth == 0)
	{
     framenum++;
	 fr3++;
	 if ((framenum>fr2) && (fr3%2==0)) {Idle->NextFrame();}
	 if (framenum>fr2+Idle->Animation->framecounter*2) {framenum=0;Idle->FirstFrame();fr2 = rand()%100;}
	}
}

void TFRMPlayer::Draw(LPDIRECTDRAWSURFACE7 g_pDDSBack, int TerX, int TerY, int colorinf)
{
	int bbx;
	int bby;
	int usex = x;
	int usey = y;
	PFRM Lista;
	if (colorinf>1000) {
		Lista = (PFRM)Idle->Animation->FRMList6[colorinf-1000]->First();
		bbx = Lista->bx;
		bby = Lista->by;
		BlitTo(g_pDDSBack,0,0,Lista->x,Lista->y,TerX+bbx,TerY+bby,DDBLTFAST_SRCCOLORKEY,Lista->FRM);
		return;
	}
	if (colorinf<59) colorinf += 5;
	switch (curth)
	{
		case 0:
			Idle->SetDir(dir); 
			Lista = (PFRM)Idle->Count6[dir];
			bbx = Lista->bx;
			bby = Lista->by;
			break;
		case 1:
			Moving->SetDir(dir); 
			Lista = (PFRM)Moving->Count6[dir];
			bbx = Lista->bx;
			bby = Lista->by;
			usex = ox;
			usey = oy;
			if (Labpos == true)
			{
				if (dir==0) {bbx-=16;bby+=12;};
				if (dir==1) {bbx-=32;        };
				if (dir==2) {bbx-=16;bby-=12;};
				if (dir==3) {bbx+=16;bby-=12;};
				if (dir==4) {bbx+=32;        };
				if (dir==5) {bbx+=16;bby+=12;};
			}
			break;
		case 2:
			Runing->SetDir(dir); 
			Lista = (PFRM)Runing->Count6[dir];
			bbx = Lista->bx;
			bby = Lista->by;
			usex = ox;
			usey = oy;
			if (dir==0) {bbx-=RunPos*16;bby+=RunPos*12;};
			if (dir==1) {bbx-=RunPos*32;               };
			if (dir==2) {bbx-=RunPos*16;bby-=RunPos*12;};
			if (dir==3) {bbx+=RunPos*16;bby-=RunPos*12;};
			if (dir==4) {bbx+=RunPos*32;               };
			if (dir==5) {bbx+=RunPos*16;bby+=RunPos*12;};
			break;
		case 4:
			Other[0]->SetDir(dir); 
			Lista = (PFRM)Other[0]->Count6[dir];
			bbx = Lista->bx;
			bby = Lista->by;
			break;
		case 5: case 6:
			Other[1]->SetDir(dir); 
			Lista = (PFRM)Other[1]->Count6[dir];
			bbx = Lista->bx;
			bby = Lista->by;
			break;
		case 50:
			Other[2]->SetDir(dir); 
			Lista = (PFRM)Other[2]->Count6[dir];
			bbx = Lista->bx;
			bby = Lista->by;
			break;
		case 200:
			Lista = DeadAnimation->GetCurFrame();
 			bbx = Lista->bx;
			bby = Lista->by;
			break;
	}
	if (curth == 20) {
		if (Other[0]) {
			Other[0]->SetDir(dir); 
			Lista = (PFRM)Other[0]->Count6[dir];
			bbx = Lista->bx;
			bby = Lista->by;
		} else curth=21;
	}
	if (curth == 21) {
		if (Other[1]) {
			Other[1]->SetDir(dir); 
			Lista = (PFRM)Other[1]->Count6[dir];
			bbx = Lista->bx;
			bby = Lista->by;
		} else curth=22;
	}
	if (curth == 22) {
		if (Other[2]) {
			Other[2]->SetDir(dir); 
			Lista = (PFRM)Other[2]->Count6[dir];
			bbx = Lista->bx;
			bby = Lista->by;
		} else { 
			SetStaticState();
			Idle->SetDir(dir); 
			Lista = (PFRM)Idle->Count6[dir];
			bbx = Lista->bx;
			bby = Lista->by;
		}
	}

	if (colorinf>8) {
		if (colorinf>50) {
			BlitToRo(g_pDDSBack,0,0,Lista->x,Lista->y,TerX+LocConvertX(usex,usey)*16+bbx,TerY+LocConvertY(usex,usey)*12+bby,DDBLTFAST_SRCCOLORKEY,Lista->FRM,palcal[(colorinf==1000) ? visibletype : colorinf]);
			//textutil::AddString("BlitRo",1);
		} else {
			BlitTo(g_pDDSBack,0,0,Lista->x,Lista->y,TerX+LocConvertX(usex,usey)*16+bbx,TerY+LocConvertY(usex,usey)*12+bby,DDBLTFAST_SRCCOLORKEY,Lista->FRM);
		}
	}
	else
		BlitToAlt(g_pDDSBack,0,0,Lista->x,Lista->y,TerX+LocConvertX(usex,usey)*16+bbx,TerY+LocConvertY(usex,usey)*12+bby,DDBLTFAST_SRCCOLORKEY,Lista->FRM,colorinf);
	
}

void TFRMPlayer::ChangeHand(HWND hWnd, LPDIRECTDRAW7 g_pDD)
{
	curhand = !curhand;
}

void TFRMPlayer::ChangeWeapon(HWND hWnd, LPDIRECTDRAW7 g_pDD, bool whichhand, unsigned int num, unsigned int ammonum, FRMPairCollection &TilesI)
{
	if (num==20000000) {
		if (whichhand) {
			if (Hand1)
			if (Hand1 != Player.Unarmed1) delete Hand1;
			Hand1 = Player.Unarmed1;
		} else {
			if (Hand2)
			if (Hand2 != Player.Unarmed2) delete Hand2;
			Hand2 = Player.Unarmed2;
		}
	} else {
		PWeapon Weapon;
		if (whichhand) {
			Hand1 = new TWeapon();
			Weapon = Hand1;
		} else {
			Hand2 = new TWeapon();
			Weapon = Hand2;
		}
		Weapon->Load(num,TilesI);
		Weapon->numammo = ammonum;
	}
	LoadPlayer(hWnd,g_pDD,true);
}

void TFRMPlayer::GetDraw(int& XX, int& YY)
{
 PFRM Lista;
 switch (curth)
 {
 case 0:
	 Idle->SetDir(dir); 
	 Lista = (PFRM)Idle->Count6[dir];
	 XX = Lista->bx;
	 YY = Lista->by;
	 break;
 case 1:
	 Moving->SetDir(dir); 
	 Lista = (PFRM)Idle->Count6[dir];
	 XX = Lista->bx;
	 YY = Lista->by;	 
	 break;
 case 2:
	 Runing->SetDir(dir); 
	 Lista = (PFRM)Idle->Count6[dir];
	 XX = Lista->bx;
	 YY = Lista->by;	 
	 break;
 }
}

// ---- TIanCritter ----
//
// -------------------
HRESULT TIanCritter::LoadMainCharacter(HWND hWnd, LPDIRECTDRAW7 g_pDD, FRMPairCollection &TilesI)
{
	PlayerDef PLoad;
	PFRMPlayer Ian;
	char buf[100], buf3[100], buf4[100];
	
	wsprintf(buf3,"%s",GetFile("\\proto\\critters.pro").c_str());
	GetPrivateProfileString("CRITTERS","CRITTERS_0","",buf4,150,buf3);
	wsprintf(buf,"\\data\\critters\\%s",buf4);

	wsprintf(buf3,"%s",GetFile(buf).c_str());
	sprintf(buf4,"%s",textutil::GetFromXML(buf3,".graphics").c_str());
	
	PLoad.bIdle = true;
	PLoad.bMove = true;
	PLoad.bRun = true;

	sprintf(buf3,"\\proto\\critters\\%08i.pro",0);
	PLoad.protolocation = GetFile(buf3);
	PLoad.deflocation = GetFile(buf);
	PLoad.num = 0;
	PLoad.Count = 0;
	PLoad.proto = 0;
	
	PLoad.bComputer = false;
	PLoad.name = "\\art\\critters\\";
	PLoad.name += buf4;

	PInventory Inven = new TInventory();

	int unarmedloc;
	PLoad.Unarmed1 = new TWeapon();
	unarmedloc = atoi2(textutil::GetFromXML(PLoad.protolocation.c_str(),"/unarmed.hand1").c_str());
	if (unarmedloc<65536) LoadNewItem(hWnd,g_pDD,TilesI,unarmedloc);
	PLoad.Unarmed1->Load(unarmedloc,TilesI);
	
	PLoad.Unarmed2 = new TWeapon();
	unarmedloc = atoi2(textutil::GetFromXML(PLoad.protolocation.c_str(),"/unarmed.hand2").c_str());
	if (unarmedloc<65536) LoadNewItem(hWnd,g_pDD,TilesI,unarmedloc);
	PLoad.Unarmed2->Load(unarmedloc,TilesI);

	Ian = new TFRMPlayer(PLoad,Inven);
	Ian->LoadPlayer(hWnd,g_pDD,false);
	Ian->MoveTo(10,10);
	Critters.insert( Critter_Pair(0,Ian));
	return DD_OK;
}

HRESULT TIanCritter::LoadCritters(HWND hWnd, LPDIRECTDRAW7 g_pDD, const char* filename, FRMPairCollection &TilesI)
{
	unsigned int x,proto;
	unsigned short num,opc;
	gzFile stream;
	char buf[150],buf2[150],buf3[150],buf4[150];

	PlayerDef PLoad;
	PFRMPlayer Ian;
	PInventory Inven;

	if ((stream = __IOopen(filename,"rb")) == NULL)
		return InitFail(hWnd,DDERR_NOTLOADED,"LoadMap FAILED - Critters");

	while (gzread(stream,&x,4) == 4) {
		Inven = new TInventory();
		
		gzread(stream,&num,2);
		gzread(stream,&proto,4);
		gzread(stream,&opc,2);
		PLoad.Count = num;
		PLoad.var1 = 0;
		PLoad.var2 = 0;
		PLoad.group = 0;
		PLoad.karma = 0;
		PLoad.proto = proto;
		while (opc != 0xff)
		{
			if (opc == 0x11) {
				gzread(stream,&PLoad.Count,4);
			}
			if (opc == 0x22) {
				gzread(stream,&PLoad.var1,4);
			}
			if (opc == 0x33) {
				gzread(stream,&PLoad.var2,4);
			}
			if (opc == 0x44) {
				gzread(stream,&PLoad.group,4);
			}
			if (opc == 0x55) {
				gzread(stream,&PLoad.karma,4);
			}
			if (opc == 0xAA) {
				int num;
				num=1;
				while (num != 0) {
					unsigned short type;
					gzread(stream,&type,2);
					gzread(stream,&num,4);
					if (num!=0) LoadNewItem(hWnd,g_pDD,TilesI,type);
					if (num!=0) Inven->AddItem(type,num,0,TilesI);
				}
			}
			gzread(stream,&opc,2);
		}
	
		wsprintf(buf2,"CRITTERS_%i",num);
		wsprintf(buf3,"%s",GetFile("\\proto\\critters.pro").c_str());
		GetPrivateProfileString("CRITTERS",buf2,"",buf4,150,buf3);
		wsprintf(buf,"\\data\\critters\\%s",buf4);

		wsprintf(buf3,"%s",GetFile(buf).c_str());
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
		if (unarmedloc<65536) LoadNewItem(hWnd,g_pDD,TilesI,unarmedloc);
		PLoad.Unarmed1->Load(unarmedloc,TilesI);
		PLoad.Unarmed2 = new TWeapon();
		unarmedloc = atoi2(textutil::GetFromXML(PLoad.protolocation.c_str(),"/unarmed.hand2").c_str());
		if (unarmedloc<65536) LoadNewItem(hWnd,g_pDD,TilesI,unarmedloc);
		PLoad.Unarmed2->Load(unarmedloc,TilesI);
		
		Ian = new TFRMPlayer(PLoad,Inven);
		Ian->LoadPlayer(hWnd,g_pDD,false);
		Ian->MoveTo(DeCompLocX(x),DeCompLocY(x));
		Critters.insert( Critter_Pair(num,Ian));
		Ian = NULL;
	}
	gzclose(stream);
	
	return DD_OK;
}

TIanCritter::~TIanCritter()
{
	CritterList::iterator iter;
	iter = Critters.begin();
	while (iter!=Critters.end()) {
		//AddToLog("%i",iter->first);
		delete iter->second;
		iter++;
	}
	Critters.clear();
}

HRESULT TIanCritter::ClearButMain()
{
	PFRMPlayer IanSave = NULL;
	CritterList::iterator iter;
	iter = Critters.begin();
	while (iter!=Critters.end()) {
		if ((iter->first)!=0)
		{ 
			delete iter->second;
		} else {
			if (IanSave)
				delete iter->second; else
				IanSave = iter->second;
		}
		iter++;
	}
	Critters.clear();
	Critters.insert( Critter_Pair(0,IanSave));
	return DD_OK;
}