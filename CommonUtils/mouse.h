#ifndef __MOUSE_H
#define __MOUSE_H

#include "engine.h"

namespace mouse {
	HRESULT InitDirectInput( HWND hDlg,HINSTANCE g_hInst );
	bool MouseIn(int x1,int y1,int x2,int y2);
	bool Pressed(int i);
	bool MInPr(int x1,int y1, int x2, int y2,int i);
	HRESULT SetAcquire( );
	HRESULT UpdateInputState( );
	HRESULT FreeDirectInput( );
}

#endif //__MOUSE_H