#ifndef __WLOC_H
#define __WLOC_H

#include <string>
#include "player.h"

class TLocationList;
class TLocation;
class TMapLoader;

typedef TLocationList *PLocationList;
typedef TLocation *PLocation;
typedef TMapLoader *PMapLoader;

class TLocation : public TItem
{
public:
	TLocation(
		int areanum,
		int ix, 
		int iy,
		int isize,
		bool ivisible,
		std::string ishortname,
		std::string iname)
		{
			num = areanum;
			x = ix;
			y = iy;
			size = isize;
			name = iname;
			visible = ivisible;
			shortname = ishortname;
		};
	~TLocation() {};
	int x,y,size,num;
	bool visible;
	std::string name,shortname;
};

class TLocationList : public TItem
{
public:
	TLocationList() { Locations = new TList(); };
	~TLocationList() { if (Locations) delete Locations; };
	PList Locations;
};

class TMapLoader : public TItem
{
public:
	TMapLoader(PIanMap *IMapInf, PIanStatic *IStaticInf, PIanCritter *ICritterInf, BlockType *IBlockDat )
		{
			MapInf = IMapInf;
			StaticInf = IStaticInf;
			CritterInf = ICritterInf;
			BlockDat = IBlockDat;
		};
	~TMapLoader();

	void SaveLocations(HWND hWnd, LPDIRECTDRAW7 g_pDD);
	HRESULT InitMainChar(HWND hWnd, LPDIRECTDRAW7 g_pDD);
	HRESULT LoadMap(HWND hWnd, LPDIRECTDRAW7 g_pDD, char* filename, char* actorfile);

	std::string fname;
	std::string actorname;
	PIanMap *MapInf;
	PIanStatic *StaticInf;
	PIanCritter *CritterInf;
	BlockType *BlockDat;
};

#endif //__WLOC_H