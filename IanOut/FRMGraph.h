#include "ItemObj.h"
#include <afx.h>
#include <stdio.h>
#include <ddraw.h>

#ifndef __FRMGRAPH_H
#define __FRMGRAPH_H

class TFRM;
class TFRMAnim;
class TFRMAnim6;
class TFRMSingle;

class TFRMCommunicator;
class TFRMPlayer;

class TIanMap;
class TIanWall;
class TIanItems;

typedef TFRM *PFRM;
typedef TFRMAnim *PFRMAnim;
typedef TFRMAnim6 *PFRMAnim6;
typedef TFRMSingle *PFRMSingle;

typedef TFRMCommunicator *PFRMCommunicator;
typedef TFRMPlayer *PFRMPlayer;

typedef TIanWall *PIanWall;
typedef TIanMap *PIanMap;
typedef TIanItems *PIanItems;

typedef unsigned char MapInfo[100][100];
typedef unsigned char WallInfo[171][151];
typedef unsigned char BlockType[171][151];

typedef struct __PDF
{
	BOOL bIdle;
	BOOL bMove;
	BOOL bComputer;
	char name[10];
	int maxtaunt;
	char taunt[10][30];
} PlayerDef;


typedef struct __PDFAnim
{
    PFRMAnim6 AIdle;
	PFRMAnim6 AMoving;
} PlayerDefAnim;

class TFRM : public TItem
{
public:
  TFRM(void) {};
  ~TFRM(void) {};
//protected:
  int x,y;
  int bx,by;
  LPDIRECTDRAWSURFACE7 FRM;

// friend class TFRMAnim;
};

class TFRMAnim : public TItem
{
public:
	TFRMAnim() { FRMList = new TList(); };
	~TFRMAnim();
	
	virtual HRESULT Load(HWND hWnd, LPDIRECTDRAW7 g_pDD, char* filename);

	virtual void NextFrame();
	virtual PFRM GetCurFrame();
	
	int framecounter;
protected:
	virtual int LoadFRM(int stream, LPVOID LoadIn, int x, int y, int p);
	PItem Counter;

private:
  PList FRMList;

};

class TFRMAnim6 : public TFRMAnim
{
public:
	TFRMAnim6();
	~TFRMAnim6();
	virtual HRESULT Load(HWND hWnd, LPDIRECTDRAW7 g_pDD, char* filename);

	virtual void FirstFrame();
	virtual void NextFrame();
	virtual PFRM GetCurFrame();
	virtual void SetDir(int i_dir);
	
	int dir;
	PItem Counter6[6];
    PList FRMList6[6];
};

class TFRMSingle : public TItem
{
public:
	PFRM FRM;
	TFRMSingle() {FRM = NULL;};
	~TFRMSingle() { if (FRM) {FRM->FRM->Release();delete FRM;}};

	HRESULT Load(HWND hWnd, LPDIRECTDRAW7 g_pDD, char* filename, int framenum);
	
	char fname[80];
protected:
	int LoadFRM(int stream, LPVOID LoadIn, int x, int y, int p);
};

class TIanMap : public TItem
{
public:
	TIanMap() {};
	~TIanMap();

	HRESULT LoadMap(HWND hWnd,char* filename);
	HRESULT LoadTiles(HWND hWnd, LPDIRECTDRAW7 g_pDD);

    MapInfo Map;
	PFRMSingle Tiles[10];
};

class TIanWall : public TItem
{
public:
	TIanWall() {};
	~TIanWall();

	HRESULT LoadMap(HWND hWnd,char* filename);
	HRESULT LoadWalls(HWND hWnd, LPDIRECTDRAW7 g_pDD);

    WallInfo Map;
	PFRMSingle Tiles[50];
};

class TIanItems : public TItem
{
public:
	TIanItems() {};
	~TIanItems();

	HRESULT LoadMap(HWND hWnd,char* filename);
	HRESULT LoadItems(HWND hWnd, LPDIRECTDRAW7 g_pDD);

    WallInfo Map;
	PFRMSingle Tiles[50];
};

class TFRMCommunicator : public TItem
{
public:
	TFRMCommunicator(PFRMAnim6 FRMAssign);
	~TFRMCommunicator() {};
    
	virtual void FirstFrame();
	virtual void NextFrame() { Count6[dir] = Animation->FRMList6[dir]->RNext(Count6[dir]);};
	virtual PFRM GetCurFrame();
	virtual void SetDir(int i_dir) {dir = i_dir;};

	int dir;
	PItem Count6[6];

	PFRMAnim6 Animation;
};

class TFRMPlayer : public TItem
{
public:
	TFRMPlayer(PlayerDef PDef) 
		{
		  curth = 0;
		  dir = 0;
		  framenum = 0;
		  Player = PDef;
		  fr2 = 0;
		  fr3 = 0;
		};
	~TFRMPlayer() { if (Idle) delete Idle; if (Moving) delete Moving;};

	void LoadPlayer(PlayerDefAnim PDefAni);
	
	void Draw(LPDIRECTDRAWSURFACE7 g_pDDSBack,int TerX, int TerY);
	void GenerateTree(int tx, int ty, BlockType Block);
	void MoveTo(int t_x, int t_y) {x = t_x; y=t_y; };
	void Next(BlockType Block);
	void Move(BlockType Block);
	int GetDir();

	int x,y;
	int ox,oy;
	int curth;
	int framenum;
	int fr2,fr3;
	int dir;
	BOOL Labpos;
	PlayerDef Player;

	short Table[170][150];
	short PosIt[101][2];
	PFRMCommunicator Idle;    /* 0 */
	PFRMCommunicator Moving;  /* 1 */
};

#endif // __FRMGRAPH_H 