#include "global.h"

//-----------------------------------------------------------------------------
// Local data
//-----------------------------------------------------------------------------
const int					FontMap[] = {
	  0,  6,  10, 17, 26, 34, 44, 53, 58, 64, 70, 77, 84, 88, 96,100,110,118,124,132,140,148,156,164,172,180,188,192,196,205,213,222,230,239,249,258,266,275,283,291,300,309,313,322,331,339,348,357,366,375,384,393,402,410,419,429,438,448,458,467,473,483,489,498,507,512,520,528,535,543,551,558,566,574,578,585,593,597,606,614,622,630,638,645,653,661,669,677,686,694,702,710,716,720,726,735,742
};

const int				Calculate[8] = {0,5,2,7,4,1,6,3};


//-----------------------------------------------------------------------------
// Global data
//-----------------------------------------------------------------------------

int							TerX = 0; 
int							TerY = 0;

PFRMSingle					FullScreen = NULL;
PFRMSingle					Mouse = NULL;
PFRMSingle					MenuUp = NULL;
PFRMSingle					MenuDown = NULL;
PFRMSingle					Select = NULL;
PFRMSingle					Select2 = NULL;
PFRMSingle					MouseScr[8][2];
PFRMSingle					Buttons[2];

PIanMap						MapInf;
PIanStatic					StaticInf;
PIanCritter					CritterInf;

BYTE						paldat[128][256];

int							GamePos = 0;

int							wallpos[6][2];
int							current;
BlockType					BlockDat;

bool						dot_plane = false;
bool						wall_plane = false;
bool						item_plane = false;
bool						block_plane = false;
bool						auto_block = true;

bool						show_wall = true;
bool						show_item = true;

int							putmode	= 1;
int							Selected = -1;

RECT						ScreenRect;

TRect						BoundRect;
bool						BoundType = false;
bool						Bound_plane = false;
bool						BoundTest = false;

int							*ref_to = NULL;
std::string					*ref_str = NULL;

int oldGamePos;
bool middlePressed;


namespace IanOs {
	void RunStat(std::string statement) {};
	PLocationDat ActualItem;
	PFRMPlayer ActualPlayer;
	BlockType *Block;
}

namespace play {
	bool TurnBasedMode = false;
}