#ifndef __FRM_H
#define __FRM_H

#include "../commonutils/ItemObj.h"
#include "../commonutils/discutil.h"
#include <stdio.h>
#include <ddraw.h>

class TFRM;
class TFRMAnim;
class TFRMAnim6;
class TFRMSingle;
class TFRMCommunicator;
class TFRMAnimCommunicator;

typedef TFRM *PFRM;
typedef TFRMAnim *PFRMAnim;
typedef TFRMAnim6 *PFRMAnim6;
typedef TFRMSingle *PFRMSingle;
typedef TFRMCommunicator *PFRMCommunicator;
typedef TFRMAnimCommunicator *PFRMAnimCommunicator;

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
	TFRMAnim() { FRMList = new TList(); refcount = 0;};
	~TFRMAnim();
	
	virtual HRESULT Load(HWND hWnd, LPDIRECTDRAW7 g_pDD, const char* filename, int direction);

	virtual void FirstFrame() { Counter = FRMList->First(); };
	virtual void LastFrame() { Counter = FRMList->Last(); };
	virtual void NextFrame() { Counter = FRMList->RNext(Counter);}; 
	virtual void PrevFrame() { Counter = FRMList->RPrev(Counter);};
	virtual bool GetLastFrame() { return Counter == FRMList->Last();};
	virtual bool GetFirstFrame() { return Counter == FRMList->First();};
	virtual PFRM GetCurFrame() { PFRM Lista;Lista = (PFRM)Counter;return Lista; };

	void IncRefCount() {refcount++;};
	void DecRefCount() {refcount--;};

	int framecounter;
	char fname[100];
	int refcount;

	virtual int LoadFRM(gzFile stream, LPVOID LoadIn, int x, int y, int p, bool frm_2);
	PItem Counter;
	
	PList FRMList;

};

class TFRMAnim6 : public TFRMAnim
{
public:
	TFRMAnim6();
	~TFRMAnim6();
	virtual HRESULT Load(HWND hWnd, LPDIRECTDRAW7 g_pDD, const char* filename);

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

	HRESULT Load(HWND hWnd, LPDIRECTDRAW7 g_pDD, const char* filename, int framenum);
	
	char fname[100];
protected:
	int LoadFRM(gzFile stream, LPVOID LoadIn, int x, int y, int p, bool frm_2);
};

class TFRMCommunicator// : public TFRMAnim
{
public:
	TFRMCommunicator(PFRMAnim6 FRMAssign);
	~TFRMCommunicator() {Animation->DecRefCount();};
    
	virtual void FirstFrame();
	virtual void NextFrame() { Count6[dir] = Animation->FRMList6[dir]->RNext(Count6[dir]);};
	virtual PFRM GetCurFrame();
	virtual void SetDir(int i_dir) {dir = i_dir;};

	int dir;
	PItem Count6[6];

	PFRMAnim6 Animation;
};

class TFRMAnimCommunicator
{
public:
	TFRMAnimCommunicator(PFRMAnim FRMAssign);
	~TFRMAnimCommunicator() {Animation->DecRefCount();};
    
	virtual void FirstFrame() { Count = Animation->FRMList->First(); };
	virtual void NextFrame() { Count = Animation->FRMList->RNext(Count);};
	virtual PFRM GetCurFrame() { return (PFRM)Count; }

	virtual void LastFrame() { Count = Animation->FRMList->Last(); };
	virtual void PrevFrame() { Count = Animation->FRMList->RPrev(Count);};
	virtual bool GetLastFrame() { return Count == Animation->FRMList->Last();};
	virtual bool GetFirstFrame() { return Count == Animation->FRMList->First();};

	int dir;
	PItem Count;

	PFRMAnim Animation;
};


#endif //__FRM_H