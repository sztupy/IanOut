#ifndef __GLOBAL_H
#define __GLOBAL_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include <string>
#include <ddraw.h>
#include <dinput.h>
#include <stdio.h>
#include <stdarg.h>
#include <io.h> 
#include <fcntl.h>      
#include "../commonutils/itemobj.h"
#include "resource.h"
#include "../commonutils/ddutil.h"
#include "../frmobject/FRM.h"
#include "../commonutils/discutil.h"
#include "../frmobject/Player.h"
#include "../frmobject/items.h"
#include "../commonutils/utils.h"
#include <time.h>


//-----------------------------------------------------------------------------
// Local definitions
//-----------------------------------------------------------------------------
#define NAME                "IanEd"
#define TITLE               "IanOut Editor"

#define VERSIONNUM	        "0.8 - Public Beta 3"

//-----------------------------------------------------------------------------
// Local data
//-----------------------------------------------------------------------------
extern const int					FontMap[];

extern const int				Calculate[8];

//-----------------------------------------------------------------------------
// Global data
//-----------------------------------------------------------------------------

extern int							TerX; 
extern int							TerY;

extern PFRMSingle					FullScreen;
extern PFRMSingle					Mouse;
extern PFRMSingle					MenuUp;
extern PFRMSingle					MenuDown;
extern PFRMSingle					Select;
extern PFRMSingle					Select2;
extern PFRMSingle					MouseScr[8][2];
extern PFRMSingle					Buttons[2];

extern PIanMap						MapInf;
extern PIanStatic					StaticInf;
extern PIanCritter					CritterInf;

extern BYTE							paldat[128][256];

extern int							GamePos;

extern int							wallpos[6][2];
extern int							current;
extern BlockType					BlockDat;

extern bool						dot_plane;
extern bool						wall_plane;
extern bool						item_plane;
extern bool						block_plane;
extern bool						auto_block;

extern bool						show_wall;
extern bool						show_item;

extern int						putmode;
extern int						Selected;

extern RECT						ScreenRect;

struct TRect {
	int top,bottom,left,right;
};

extern TRect					BoundRect;
extern bool						BoundType;
extern bool						Bound_plane;
extern bool						BoundTest;

extern int						*ref_to;
extern std::string				*ref_str;

#endif //__GLOBAL_H