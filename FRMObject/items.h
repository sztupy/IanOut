#ifndef __ITEMS_H
#define __ITEMS_H

#include "../commonutils/ItemObj.h"
#include "../commonutils/textutil.h"
#include <map>
#include <stdio.h>
#include "FRM.h"
#include <string>

class TIanMap;
class TIanStatic;
class TItemDesc;
class TLocationDat;
class TFRMPair;
class TFRMDead;
class TInventory;
class TInventoryItem;

typedef TInventory *PInventory;
typedef	TInventoryItem *PInventoryItem;
typedef TIanMap *PIanMap;
typedef TIanStatic *PIanStatic;
typedef TLocationDat *PLocationDat;
typedef TItemDesc *PItemDesc;
typedef TFRMPair *PFRMPair;
typedef TFRMDead *PFRMDead;

typedef unsigned short ItemInfo[128][256];
typedef unsigned char BlockType[512][512];

typedef std::map<unsigned int,PFRMSingle> FRMCollection;
typedef std::map<unsigned int,PFRMPair> FRMPairCollection;
typedef std::map<unsigned int,PFRMDead> FRMDeadCollection;

typedef std::multimap<unsigned int,PLocationDat> FRMLocationMap;
typedef std::pair<unsigned int, PLocationDat> Location_Pair;

typedef std::pair<unsigned int, PFRMDead> Dead_Pair;

void LoadNewItem(FRMPairCollection &TilesI, int num);
int LoadExitGrid();
int DeleteExitGrid();

inline int DeCompLocY(int loc) {return (loc>>10) ^ 511;};
inline int DeCompLocX(int loc) {return loc & 1023;};
inline int CompLoc(int lx, int ly) {return ((ly^511) << 10)+lx;};

inline int LocConvertX(int lx, int ly) { return lx+ly-(lx+2)/4; };
inline int LocConvertY(int lx, int ly) { return ly-(lx+2)/4; };

/*inline int DeCompLocY(int loc) {return (loc & 1023) ^ 511;};
inline int DeCompLocX(int loc) {return loc >> 10;};
inline int CompLoc(int lx, int ly) {return ((ly^511))+(lx<<10);};*/

class TInventory : public TItem
{
public:
	TInventory() { Inven = new TList(); };
	~TInventory() { delete Inven; };
	void AddItem(TInventoryItem Item);
	void AddItem(PInventoryItem Item);
	void AddItem(unsigned short num, unsigned int count, unsigned int state, FRMPairCollection &TilesI);
	unsigned int CountItem(unsigned short num, unsigned int state);
	PInventoryItem GetItem(unsigned short num, unsigned int state);
	PInventoryItem GetItem(unsigned short num, unsigned int state, unsigned int count);
	PInventoryItem GetItem(PInventoryItem Item);
	PList Inven;
};

class TInventoryItem : public TItem
{
public:
	TInventoryItem() {};
	~TInventoryItem() {};

	unsigned int count;
	unsigned short num;
	unsigned int state;
	PFRMAnim FRMA;
};

class TItemDesc
{
public:
	TItemDesc() { Inven = NULL; };
	~TItemDesc() { if (Inven) delete Inven; };

	std::string ability;
	std::string desclocation;
	std::string name;
	PInventory Inven;
	int Count;
	int var1,var2;
	bool direction;
	bool change;
};

class TLocationDat : public TItem
{
public:
	TLocationDat() { ItemDesc = NULL; FRMA = NULL; FRM = NULL;};
	~TLocationDat() {if (ItemDesc) delete ItemDesc; if (FRMA) delete FRMA;};

	unsigned short num;
	unsigned short type;
	unsigned int loc;
	unsigned int state;
	PItemDesc ItemDesc;
	PFRMSingle FRM;
	PFRMAnimCommunicator FRMA;
};

class TFRMPair : public TItem
{
public:
	TFRMPair() {};
	~TFRMPair() { if (FRM) delete FRM; if (FRMA) delete FRMA;};

	PFRMSingle FRM;
	PFRMAnim FRMA;
};

class TFRMDead : public TItem
{
public:
	TFRMDead() { FRMA = NULL; };
	~TFRMDead() { if (FRMA) delete FRMA; };

	PFRMAnim FRMA;
};

class TIanMap : public TItem
{
public:
	TIanMap() {};
	~TIanMap();

	int LoadMap(const char* filename);
	int LoadTiles();

    ItemInfo Map;
	ItemInfo Map2;
	FRMCollection Tiles;
};

class TIanStatic : public TItem
{
public:
	TIanStatic() {};
	~TIanStatic();

	int LoadStatic(const char* filename);
	int DeleteButTiles();

    FRMLocationMap Map;
	FRMCollection TilesW;
	FRMCollection TilesS;
	FRMPairCollection TilesI;
	FRMPairCollection TilesO;
	FRMDeadCollection TilesD;
};

#endif __ITEMS_H