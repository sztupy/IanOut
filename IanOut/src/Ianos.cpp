// Ianos.cpp : Defines the entry point for the console application.
//
#pragma warning (disable : 4244)

#define LUABIND_MAX_ARITY 10

#include "IanOs.h"
#include "../frmobject/player.h"
#include "stdlib.h"
#include <string>
#include <map>
#include "../commonutils/engine.h"
#include "../commonutils/discutil.h"
#include "../commonutils/textutil.h"
#include "../frmobject/specob.h"
#include "../commonutils/date.h"
#include "../frmobject/buttons.h"
#include "../frmobject/wloc.h"
#include "../commonutils/palette.h"


extern "C"
{
	#include "../lualib/lua.h"
	#include "../lualib/lstate.h"
	#include "../lualib/ldo.h"
	#include "../lualib/lauxlib.h"
	#include "../lualib/lualib.h"
}

#include <../luabind/luabind/luabind.hpp>
#include <../luabind/luabind/function.hpp>
#include <../luabind/luabind/class.hpp>
#include <../luabind/luabind/object.hpp>
#include <../luabind/luabind/adopt_policy.hpp>

#undef ERROR
#undef TRUE
#undef FALSE
#undef max
#undef NAME

extern int						GamePos;
extern int						InGamePos;
extern unsigned char			TravelMap[28][30];
extern PLocationList			MapLocations;

namespace play {
	extern bool TurnBasedMode;
	extern int mousetyp;
	extern PFRMPlayer CurrentPlayer;
	extern bool AllowUserInteract;
}
namespace ianskill {
	void CountSk(Skills *ISkill);
}

extern buttons::PButtonList		ButtonList;
extern PList					TimerList;
extern PList					TauntList;
extern PIanMap					MapInf;
extern PIanStatic				StaticInf;
extern PIanCritter				CritterInf;

extern int TerX;
extern int TerY;
void RunGame(bool type);

namespace IanOs {

	namespace GVar {
		std::map<std::string,int> GlobalVars;
		int Get(std::string i);
		void Set(std::string i,int i2);
	}

	namespace Text {
		std::map<int,std::string> TextValue;
		std::map<int,int> ToDoValue;
		std::map<int,int> ColorValue;
		std::string TextFile;
		std::string MainValue;
		int TextCount;
		int TextPosition;
	}

	BlockType		*Block;
	BlockType		*BigBlock;
	PFRMPlayer		ActualPlayer;
	PLocationDat	ActualItem;

	void Init_Lua(lua_State* L);

	using namespace std;
	using namespace luabind;


class c_TInventory {
public:
	c_TInventory() { HolderItem = NULL; }
	c_TInventory(PInventory IHItem ) { HolderItem = IHItem; }

	c_TInventory(const c_TInventory& InvC) { HolderItem = InvC.HolderItem; };
	c_TInventory& operator=(const c_TInventory& InvC) { HolderItem = InvC.HolderItem; return *this; };

	void f_AddItem(int type, int count) {
		LoadNewItem(hWnd,g_pDD,StaticInf->TilesI,type);
		HolderItem->AddItem(type,count,0,StaticInf->TilesI);
	}
	int f_CountItem(int type) {
		return HolderItem->CountItem(type,0);
	}

    PInventory HolderItem;
};

class c_TItem {
public:
	c_TItem() { HolderItem = NULL; }
	c_TItem(PLocationDat IHItem ) { HolderItem = IHItem; }

	c_TItem(const c_TItem& InvC) { HolderItem = InvC.HolderItem; };
	c_TItem& operator=(const c_TItem& InvC) { HolderItem = InvC.HolderItem; return *this; };

	void f_ChangeState(int newstate) { 
		HolderItem->ItemDesc->direction = (newstate!=0);
		HolderItem->ItemDesc->change = true;
	}
	int f_GetState() { 
		return (HolderItem->ItemDesc->direction?1:0);
	}

	void SetVar1(const int iv1) { HolderItem->ItemDesc->var1 = iv1; }
	int GetVar1() const { return HolderItem->ItemDesc->var1; }

	void SetVar2(const int iv1) { HolderItem->ItemDesc->var2 = iv1; }
	int GetVar2() const { return HolderItem->ItemDesc->var2; }

	void SetID(const int iv1) { HolderItem->ItemDesc->Count = iv1; }
	int GetID() const { return HolderItem->ItemDesc->Count; }

	int GetX() const { return DeCompLocX(HolderItem->loc); }
	int GetY() const { return DeCompLocY(HolderItem->loc); }

	c_TInventory* GetInventory() const { return new c_TInventory(HolderItem->ItemDesc->Inven); }

    PLocationDat HolderItem;
};

class c_TCharacter {
public:
	c_TCharacter() { HolderItem = NULL; }
	c_TCharacter(PFRMPlayer IHItem ) { HolderItem = IHItem; }

	c_TCharacter(const c_TCharacter& InvC) { HolderItem = InvC.HolderItem; };
	c_TCharacter& operator=(const c_TCharacter& InvC) { HolderItem = InvC.HolderItem; return *this; };

	int f_DistanceFromIan() {
		PFRMPlayer Ian = CritterInf->Critters.find(0)->second;
		return HolderItem->Distance(Ian->x,Ian->y);
	}

	int f_DistanceFromCoor(int x, int y) {
		return HolderItem->Distance(x,y);
	}

	void f_Kill(int type) {
		HolderItem->KillPerson(hWnd,g_pDD,type);
	}

	void f_Erase() {
		HolderItem->curth = 260;
	}

	int f_IncHp(int amount) {
		if (HolderItem->PlayerSkill.HP+amount<=HolderItem->PlayerSkill.AlapSk.HP)
		{
			HolderItem->PlayerSkill.HP+=amount;
			return (int)amount;
		} else {
			int i = HolderItem->PlayerSkill.AlapSk.HP-HolderItem->PlayerSkill.HP;
			HolderItem->PlayerSkill.HP=HolderItem->PlayerSkill.AlapSk.HP;
			return i;
		}
	}

	void f_Taunt(int color, const object& text)
	{
		PFRM frm = HolderItem->Idle->GetCurFrame();
		int x = HolderItem->x;
		int y = HolderItem->y;
		if (text.type() == LUA_TNUMBER) {
			char buf[20];
			sprintf(buf,"%d",object_cast<int>(text));
			TauntList->Insert(new TTauntObj(LocConvertX(x,y)*16+frm->bx+frm->x/2,LocConvertY(x,y)*12+frm->by-10-rand()%24,color,buf));
		}
		if (text.type() == LUA_TSTRING) {
			TauntList->Insert(new TTauntObj(LocConvertX(x,y)*16+frm->bx+frm->x/2,LocConvertY(x,y)*12+frm->by-10-rand()%24,color,object_cast<std::string>(text).c_str()));
		}
	}

	void f_MoveRel(int mx, int my) {
		int x=HolderItem->x;
		int y=HolderItem->y;
		HolderItem->GenerateTree(x+mx,y+my,*Block);
	}

	void f_MoveAbs(int mx, int my) {
		HolderItem->GenerateTree(mx,my,*Block);
	}

	void f_MoveRandom(int radius) {
		PFRMPlayer Ply = HolderItem;
		int x=Ply->x;
		int y=Ply->y;
		int xXx = radius;
		int ix=rand()%xXx+1;
		int iy=rand()%xXx+1;
		switch (rand()%6)
		{
			case 0:Ply->GenerateTree(x     ,y-iy,*Block);break;
			case 1:Ply->GenerateTree(x-ix*2,y+iy,*Block);break;
			case 2:Ply->GenerateTree(x-ix*2,y   ,*Block);break;
			case 3:Ply->GenerateTree(x+ix*2,y   ,*Block);break;
			case 4:Ply->GenerateTree(x+ix*2,y-iy,*Block);break;
			case 5:Ply->GenerateTree(x     ,y+iy,*Block);break;
		}
	}
	
	void f_CenterScreen() {
		PFRMPlayer Ply = HolderItem;
		if (Ply) {
			TerX = -LocConvertX(Ply->x,Ply->y)*16+GetMaxX/2;
			TerY = -LocConvertY(Ply->x,Ply->y)*12+100+(GetMaxY/2);
		}
	}

	void SetVar1(const int iv1) { HolderItem->Player.var1 = iv1; }
	int GetVar1() const { return HolderItem->Player.var1; }

	void SetVar2(const int iv1) { HolderItem->Player.var2 = iv1; }
	int GetVar2() const { return HolderItem->Player.var2; }

	void SetID(const int iv1) { HolderItem->Player.Count = iv1; }
	int GetID() const { return HolderItem->Player.Count; }

	void SetType(const int iv1) { HolderItem->Player.num = iv1; }
	int GetType() const { return HolderItem->Player.num; }

	void SetGroup(const int iv1) { HolderItem->Player.group = iv1; }
	int GetGroup() const { return HolderItem->Player.group; }

	void SetKarma(const int iv1) { HolderItem->Player.karma = iv1; }
	int GetKarma() const { return HolderItem->Player.karma; }	

	void SetX(const int iv1) { HolderItem->x = iv1; }
	int GetX() const { return HolderItem->x; }

	void SetY(const int iv1) { HolderItem->y = iv1; }
	int GetY() const { return HolderItem->y; }

	c_TInventory* GetInventory() const { return new c_TInventory(HolderItem->Inven); }

	PFRMPlayer HolderItem;
};
	
void f_AddLog(const object& text)
{
	if (text.type() == LUA_TNUMBER) {
		char buf[20];
		sprintf(buf,"%d",object_cast<int>(text));
		AddToLog(buf);
	}
	if (text.type() == LUA_TSTRING) {
		AddToLog(object_cast<std::string>(text).c_str());
	}
}

void f_WriteLn(const object& text)
{
	if (text.type() == LUA_TNUMBER) {
		char buf[20];
		sprintf(buf,"%d",object_cast<int>(text));
		textutil::AddString(buf,1);
	}
	if (text.type() == LUA_TSTRING) {
		textutil::AddString(object_cast<std::string>(text),1);
	}
}

c_TItem* f_SetActualItem(int number)
{
	FRMLocationMap::iterator iter;
	FRMLocationMap::iterator iter2;
	iter2 = StaticInf->Map.end();
	iter = StaticInf->Map.begin();
	while (iter != StaticInf->Map.end()) {
		if (iter->second->ItemDesc->Count == number) iter2=iter;
		iter++;
	}
	if (iter2 == StaticInf->Map.end()) {
		return NULL;
	}
	ActualItem = iter2->second;
	return new c_TItem(ActualItem);
}

c_TItem* f_GetItem(int number)
{
	FRMLocationMap::iterator iter;
	FRMLocationMap::iterator iter2;
	iter2 = StaticInf->Map.end();
	iter = StaticInf->Map.begin();
	while (iter != StaticInf->Map.end()) {
		if (iter->second->ItemDesc->Count == number) iter2=iter;
		iter++;
	}
	if (iter2 == StaticInf->Map.end()) {
		return NULL;
	}
	return new c_TItem(iter2->second);
}

c_TCharacter* f_SetActualCharacter(int number)
{
	CritterList::iterator iter;
	CritterList::iterator iter2;
	iter2 = CritterInf->Critters.end();
	iter = CritterInf->Critters.begin();
	while (iter != CritterInf->Critters.end()) {
		if (iter->second->Player.Count == number) iter2=iter;
		iter++;
	}
	if (iter2 == CritterInf->Critters.end()) {
		return NULL;
	} else {
		ActualPlayer = iter2->second;
		return new c_TCharacter(ActualPlayer);
	}
}

c_TCharacter* f_GetCharacter(int number)
{
	CritterList::iterator iter;
	CritterList::iterator iter2;
	iter2 = CritterInf->Critters.end();
	iter = CritterInf->Critters.begin();
	while (iter != CritterInf->Critters.end()) {
		if (iter->second->Player.Count == number) iter2=iter;
		iter++;
	}
	if (iter2 == CritterInf->Critters.end()) {
		return NULL;
	} else {
		return new c_TCharacter(iter2->second);
	}
}
	
c_TCharacter* f_GetIan()
{
	return new c_TCharacter(CritterInf->Critters.find(0)->second);
}

c_TCharacter* f_GetCurrentChar()
{
	return new c_TCharacter(ActualPlayer);
}

c_TItem* f_GetCurrentItem()
{
	return new c_TItem(ActualItem);
}

c_TCharacter* f_CreateCharacter(int pl1, int pl2, int pl3, int pl4, int pl5)
{
	unsigned int x,proto;
	unsigned short num;
	char buf[150],buf2[150],buf3[150],buf4[150];

	PlayerDef PLoad;
	PFRMPlayer Ian;
	PInventory Inven;

	Inven = new TInventory();
	x = CompLoc(pl1,pl2);
	num = pl3;
	proto = pl4;

	PLoad.Count = pl5;
	PLoad.var1 = 0;
	PLoad.var2 = 0;
	PLoad.group = 0;
	PLoad.karma = 0;
	PLoad.proto = pl4;
				
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
	if (unarmedloc<65536) LoadNewItem(hWnd,g_pDD,StaticInf->TilesI,unarmedloc);
	PLoad.Unarmed1->Load(unarmedloc,StaticInf->TilesI);
	
	PLoad.Unarmed2 = new TWeapon();
	unarmedloc = atoi2(textutil::GetFromXML(PLoad.protolocation.c_str(),"/unarmed.hand2").c_str());
	if (unarmedloc<65536) LoadNewItem(hWnd,g_pDD,StaticInf->TilesI,unarmedloc);
	PLoad.Unarmed2->Load(unarmedloc,StaticInf->TilesI);

	Ian = new TFRMPlayer(PLoad,Inven);
	Ian->LoadPlayer(hWnd,g_pDD,false);
	Ian->MoveTo(DeCompLocX(x),DeCompLocY(x));
	CritterInf->Critters.insert( Critter_Pair(num,Ian));
	
	return new c_TCharacter(Ian);
}

void f_AddTimer(int pl0,int pl1,int pl2,int pl3,int pl4,int pl5,std::string pl6)
{
	TimerList->Insert(new TTimerObj(pl0,pl1,pl2,pl3,pl4,pl5,pl6));
}

void f_AddTime(int pl0,int pl1,int pl2,int pl3,int pl4,int pl5)
{
	date::AddYear(pl0);
	date::AddMonth(pl1);
	date::AddDay(pl2);
	date::AddHour(pl3);
	date::AddMinute(pl4);
	date::AddSecond(pl5);
}

std::string f_GetText(std::string filename, int from, int to) {
	char buf[50];
	int a = from;
	int b = to;
	if ((to == from)) {
		sprintf(buf,"T%i",a);
	} else {
		sprintf(buf,"T%i",a+rand()%(b-a+1));
	}
	return textutil::GetFromProf(GetFile("\\text\\"+filename),"TEXT",buf);
}

void f_WorldMapShow(int x, int y) {
	if ((x>=0) && (x<28) &&
	   (y>=0) && (y<30))
		TravelMap[x][y] = 1;
}

void f_ShowCity(int citynum) {
	PItem Item;
	Item = MapLocations->Locations->First();
	while (Item!= NULL) {
		if (((PLocation)Item)->num==citynum) ((PLocation)Item)->visible=true;
		Item = MapLocations->Locations->Next(Item);
	}
}

void f_HideCity(int citynum) {
	PItem Item;
	Item = MapLocations->Locations->First();
	while (Item!= NULL) {
		if (((PLocation)Item)->num==citynum) ((PLocation)Item)->visible=false;
		Item = MapLocations->Locations->Next(Item);
	}
}

void f_StopUserInteract() {
	play::AllowUserInteract = false;
}

void f_AllowUserInteract() {
	play::AllowUserInteract = true;
}

void f_Fade(int type) {
	if (type == 0) palette::FadeOut(); else
	if (type == 1) palette::FadeIn(); else
	if (type == 2) palette::FadeOutWh(); else
	if (type == 3) palette::FadeInWh();
}

void f_CenterScreen(int nx, int ny) {
	TerX = -LocConvertX(nx,ny)*16+GetMaxX/2;
	TerY = -LocConvertY(nx,ny)*12+100+(GetMaxY/2);
}

void f_SetBlock(int wx, int wy, int na) {
	if ((wx>=0) && (wx<512) && (wy>=0) && (wy<512)) {
		(*Block)[wx][wy] = na;
		(*BigBlock)[wx][wy] = na;
	}
}

void f_NormalMode() {
	InGamePos = 0;
	GamePos = 1;
	play::TurnBasedMode = false;
	play::mousetyp = 0;
	buttons::ChangeButtonList(ButtonList,2);
}

void f_TurnBasedMode() {
	InGamePos = 0;
	GamePos = 1;
	play::TurnBasedMode = true;
	play::mousetyp = 0;
	play::CurrentPlayer = ActualPlayer;
	ianskill::CountSk(&ActualPlayer->PlayerSkill);
	buttons::ChangeButtonList(ButtonList,3);
}

void f_LootBox() {
	InGamePos = 2;
	GamePos = 1;
	buttons::ChangeButtonList(ButtonList,10);
}

void f_TalkMode() {
	PFRMPlayer Ply = ((PFRMPlayer)ActualPlayer);
	if (Ply) {
		TerX = -LocConvertX(Ply->x,Ply->y)*16+GetMaxX/2;
		TerY = -LocConvertY(Ply->x,Ply->y)*12+100+(GetMaxY-480);
	}
	InGamePos = 0;
	GamePos = 5;
	f_RunFile(Text::TextFile);
}

int GVar::Get(std::string i)
{
	return GlobalVars[i];
}

void GVar::Set(std::string i, int i2)
{
	GlobalVars[i] = i2;
}

int Text::GetPosition()
{
	return TextPosition;
}

void Text::SetPosition(int newpos)
{
	TextPosition = newpos;
}

void Text::ClearTexts()
{
	MainValue = "";
	ColorValue.clear();
	TextValue.clear();
	ToDoValue.clear();
	TextCount = 0;
}

void Text::SetFile(std::string newmain)
{
	TextFile = newmain;
}

void Text::SetMain(std::string newmain)
{
	MainValue = newmain;
}

void Text::AddLine(std::string newtext,int newtodo, int newcolor)
{
	ColorValue[TextCount] = newcolor;
	ToDoValue[TextCount] = newtodo;
	TextValue[TextCount] = newtext;
	TextCount++;
}

std::string f_GetFile(std::string filename)
{
	AddToLog(GetFile("\\scripts\\"+filename).c_str());
	return GetFile("\\scripts\\"+filename);
}

lua_State* L;

void f_RunFile(std::string filename)
{
	//lua_State* L;
	//L = lua_open();
	//Init_Lua(L);
	try {
		lua_dofile(L,GetFile("\\scripts\\"+filename).c_str()); 
	//	lua_close(L); 
	}
	catch(luabind::error& e)
	{
		lua_State* Ll = e.state();
		lua_Debug ar;
		lua_getstack (Ll, 0,&ar);
		lua_getinfo (Ll, "nSlu",&ar);
		AddToLog("--------------------------");
		AddToLog("LuaScript Error in file: %s.",filename.c_str());
		AddToLog("Long Source:",ar.source);
		AddToLog("Short Source:",ar.short_src);
		AddToLog("Linedefined:",ar.linedefined);
		AddToLog("What:",ar.what);
		AddToLog("Currentline:",ar.currentline);
		AddToLog("Name:",ar.name);
		AddToLog("NameWhat:",ar.namewhat);
		AddToLog("Nups:",ar.nups);
		AddToLog("--------------------------");
		textutil::AddString("LuaScript Error in file: "+filename+". See Log File for details.",1);
	}
}

void RunStat(std::string statement)
{
	L = lua_open();
	Init_Lua(L);
	try {
		lua_dostring(L,statement.c_str()); 
		lua_close(L); 
	}
	catch(luabind::error& e)
	{
		lua_State* Ll = e.state();
		lua_Debug ar;
		lua_getstack (Ll, 0,&ar);
		lua_getinfo (Ll, "nSlu",&ar);
		AddToLog("--------------------------");
		AddToLog("LuaScript Error in statement: %s.",statement.c_str());
		AddToLog("Long Source:",ar.source);
		AddToLog("Short Source:",ar.short_src);
		AddToLog("Linedefined:",ar.linedefined);
		AddToLog("What:",ar.what);
		AddToLog("Currentline:",ar.currentline);
		AddToLog("Name:",ar.name);
		AddToLog("NameWhat:",ar.namewhat);
		AddToLog("Nups:",ar.nups);
		AddToLog("--------------------------");
		textutil::AddString("LuaScript Error in statement. See Log File for details.",1);
	}
	L = NULL;
}

void Init_Lua(lua_State* L)
{

	luaopen_base(L);
	luaopen_string(L);
	luaopen_table(L);
	luaopen_math(L);
	luaopen_debug(L);
	luaopen_io(L);
	luaopen_loadlib(L);

	luabind::open(L);

	luabind::module(L)
	[
		namespace_("System")
		[
			def("RunFile",&f_RunFile),
			def("GetFile",&f_GetFile),
			def("AddToLog",&f_AddLog),
			def("WriteLn",&f_WriteLn),
			def("Fade",&f_Fade),
			def("AddTimer",&f_AddTimer),
			def("AddTime",&f_AddTime),
			def("GetText",&f_GetText),
			def("CenterScreen",&f_CenterScreen)
		],
		namespace_("Map")
		[
			def("WorldMapShow",&f_WorldMapShow),
			def("ShowCity",&f_ShowCity),
			def("HideCity",&f_HideCity),
			def("SetBlock",&f_SetBlock)
		],
		namespace_("Mode")
		[
			def("StopUserInteract",&f_StopUserInteract),
			def("AllowUserInteract",&f_AllowUserInteract),
			def("NormalMode",&f_NormalMode),
			def("TurnBasedMode",&f_TurnBasedMode),
			def("LootBox",&f_LootBox),
			def("TalkMode",&f_TalkMode)
		],		
		namespace_("Text")
		[
			def("ClearTexts",&Text::ClearTexts),
			def("SetFile",&Text::SetFile),
			def("SetMain",&Text::SetMain),
			def("AddLine",&Text::AddLine),
			def("GetPosition",&Text::GetPosition),
			def("SetPosition",&Text::SetPosition)
		],
		namespace_("GVar")
		[
			def("Get",&GVar::Get),
			def("Set",&GVar::Set)
		],
		namespace_("Classes")
		[
			def("SetActualItem",&f_SetActualItem, adopt(return_value)),
			def("GetItem",&f_GetItem, adopt(return_value)),
			def("SetActualCharacter",&f_SetActualCharacter, adopt(return_value)),
			def("GetCharacter",&f_GetCharacter, adopt(return_value)),
			def("GetIan",&f_GetIan, adopt(return_value)),
			def("GetCurrentChar",&f_GetCurrentChar, adopt(return_value)),
			def("GetCurrentItem",&f_GetCurrentItem, adopt(return_value)),
			def("CreateCharacter",&f_CreateCharacter, adopt(return_value)),
			class_<c_TItem>("TItem")
				.def(constructor<>())
				.def(constructor<PLocationDat>())
				.def("ChangeState",&c_TItem::f_ChangeState)
				.def("GetState",&c_TItem::f_GetState)
				.property("var1",&c_TItem::GetVar1,&c_TItem::SetVar1)
				.property("var2",&c_TItem::GetVar2,&c_TItem::SetVar2)
				.property("id",&c_TItem::GetID,&c_TItem::SetID)
				.property("x",&c_TItem::GetX)
				.property("y",&c_TItem::GetY)
				.property("inventory",&c_TItem::GetInventory),
			class_<c_TInventory>("TInventory")
				.def(constructor<>())
				.def(constructor<PInventory>())
				.def("AddItem",&c_TInventory::f_AddItem)
				.def("CountItem",&c_TInventory::f_CountItem),
			class_<c_TCharacter>("TCharacter")
				.def(constructor<>())
				.def(constructor<PFRMPlayer>())
				.def("DistanceFromIan",&c_TCharacter::f_DistanceFromIan)
				.def("DistanceFromCoor",&c_TCharacter::f_DistanceFromCoor)
				.def("Kill",&c_TCharacter::f_Kill)
				.def("Erase",&c_TCharacter::f_Erase)
				.def("IncHp",&c_TCharacter::f_IncHp)
				.def("Taunt",&c_TCharacter::f_Taunt)
				.def("MoveRel",&c_TCharacter::f_MoveRel)
				.def("MoveAbs",&c_TCharacter::f_MoveAbs)
				.def("MoveRandom",&c_TCharacter::f_MoveRandom)
				.def("CenterScreen",&c_TCharacter::f_CenterScreen)
				.property("var1",&c_TCharacter::GetVar1,&c_TCharacter::SetVar1)
				.property("var2",&c_TCharacter::GetVar2,&c_TCharacter::SetVar2)
				.property("id",&c_TCharacter::GetID,&c_TCharacter::SetID)
				.property("group",&c_TCharacter::GetGroup,&c_TCharacter::SetGroup)
				.property("type",&c_TCharacter::GetType,&c_TCharacter::SetType)
				.property("karma",&c_TCharacter::GetKarma,&c_TCharacter::SetKarma)
				.property("x",&c_TCharacter::GetX,&c_TCharacter::SetX)
				.property("y",&c_TCharacter::GetY,&c_TCharacter::SetY)
				.property("inventory",&c_TCharacter::GetInventory)
		]
	];		
}

}