#include "global.h"
#include "../commonutils/mouse.h"

#include "sys\stat.h"
#include "done.h"
#include "load.h"
#include "../commonutils/palette.h"
#include "play.h"

#include "../frmobject/freeimage.h"

extern int oldGamePos;
extern bool middlePressed;

long FAR PASCAL
WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 switch (message)
    {
        case WM_ACTIVATE:
            // Pause if minimized
            g_bActive = !((BOOL)HIWORD(wParam));
			mouse::SetAcquire();
            return 0L;

        case WM_DESTROY:
            // Clean up and close the app
            ReleaseAllObjects();
            PostQuitMessage(0);
            return 0L;

		case WM_CHAR:
			if (ref_str)
			{
				if (wParam!=VK_BACK) *ref_str+=wParam;
				if ((wParam==VK_BACK) && ((*ref_str).length()!=0))(*ref_str).erase((*ref_str).length()-1,1);
			} 
			if (ref_to) {
			switch (wParam) {
				case '1': if (ref_to) { *ref_to*=10; *ref_to+=1; };break;
				case '2': if (ref_to) { *ref_to*=10; *ref_to+=2; };break;
				case '3': if (ref_to) { *ref_to*=10; *ref_to+=3; };break;
				case '4': if (ref_to) { *ref_to*=10; *ref_to+=4; };break;
				case '5': if (ref_to) { *ref_to*=10; *ref_to+=5; };break;
				case '6': if (ref_to) { *ref_to*=10; *ref_to+=6; };break;
				case '7': if (ref_to) { *ref_to*=10; *ref_to+=7; };break;
				case '8': if (ref_to) { *ref_to*=10; *ref_to+=8; };break;
				case '9': if (ref_to) { *ref_to*=10; *ref_to+=9; };break;
				case '0': if (ref_to) { *ref_to*=10; };break;
				case '-': if (ref_to) { *ref_to=-*ref_to; };break;
			}
			}
			break;


        case WM_KEYDOWN:
            // Handle any non-accelerated key commands
			
            switch (wParam)
            {
				case VK_PAUSE:
                    PostMessage(hWnd, WM_CLOSE, 0, 0);
                    return 0L;
				case VK_ADD:
					if (gamma>=1) gamma+=0.1;
						    else gamma+=0.05;	
					if (gamma>4) gamma = 4;
					palette::SetPaletteBl(65535);
					break;
				case VK_SUBTRACT:
					if (gamma>=1) gamma-=0.1;
						    else gamma-=0.05;	
					if (gamma<0.2) gamma = 0.2;
					palette::SetPaletteBl(65535);
					break;
				case VK_MULTIPLY:
					gameSpeed-=5;
					if (gameSpeed<0) gameSpeed=0;
					break;
				case VK_DIVIDE:
					gameSpeed+=5;
					if (gameSpeed>100) gameSpeed=100;
					break;
				case 'D': if (GamePos==1) dot_plane = !dot_plane;break;
				case 'W': if (GamePos==1) wall_plane = !wall_plane;break;
				case 'I': if (GamePos==1) item_plane = !item_plane; else if (play::InventorySave) { oldGamePos=GamePos; GamePos=5; } break;
				case 'B': if (GamePos==1) block_plane = !block_plane;break;
				case VK_DELETE: if (ref_str){ (*ref_str) = ""; }; if (ref_to) { *ref_to = 0; };break;
				case VK_BACK: if (ref_to) { *ref_to/=10; };break;
				case VK_SPACE: (putmode<9) ? putmode++ : putmode=1; break;
				case VK_HOME: Selected=0; break;
				case VK_UP: if (Selected>0) Selected--; break;
				case VK_DOWN: Selected++; break;
				case VK_PRIOR: Selected-=30; if (Selected<0) Selected=0;break;
				case VK_NEXT: Selected+=30;break;
				case VK_ESCAPE: middlePressed=true;break;

case VK_F12: {
			DDSURFACEDESC2			 ddsd;
			HRESULT hRet;
			int x,y;
			int stream;
			char buf[80];
			
			x=0;
			wsprintf(buf,"shot%i.bmp",x);
			while (CanOpen(buf)) {
				x++;
				wsprintf(buf,"shot%i.bmp",x);
			}
			stream = _open(buf,_O_BINARY | _O_RDWR | _O_TRUNC | _O_CREAT, _S_IREAD | _S_IWRITE);
			ZeroMemory(&ddsd, sizeof(ddsd));
			ddsd.dwSize = sizeof(ddsd);
			hRet = g_pDDSPrimary->Lock(NULL,&ddsd,0,NULL);
			FIBITMAP* bitmap;
			if (BitDepth == 32) {
				bitmap = FreeImage_ConvertFromRawBits((BYTE*)ddsd.lpSurface,GetMaxX,GetMaxY,ddsd.lPitch,32,ddsd.ddpfPixelFormat.dwRBitMask,ddsd.ddpfPixelFormat.dwGBitMask,ddsd.ddpfPixelFormat.dwBBitMask,false);
			} else {
				bitmap = FreeImage_ConvertFromRawBits((BYTE*)ddsd.lpSurface,GetMaxX,GetMaxY,ddsd.lPitch,16,ddsd.ddpfPixelFormat.dwRBitMask,ddsd.ddpfPixelFormat.dwGBitMask,ddsd.ddpfPixelFormat.dwBBitMask,false);
			}
			FreeImage_Save(FIF_BMP,bitmap,buf,0);
			FreeImage_Unload(bitmap);
			g_pDDSPrimary->Unlock(NULL);
			mouse::SetAcquire();
			break;
		}



				case 'Z': case 'X':
	
	FRMCollection::iterator iter;
	FRMCollection *point = NULL;

	FRMPairCollection::iterator iterP;
    FRMPairCollection *pointP = NULL;

	FRMDeadCollection::iterator iterD;
    FRMDeadCollection *pointD = NULL;
	
	switch (putmode)
	{
		case 1:point=&StaticInf->TilesW;break;
		case 2:point=&StaticInf->TilesS;break;
		case 4:point=&MapInf->Tiles;break;
		case 5:pointP=&StaticInf->TilesI;break;
		case 6:pointP=&StaticInf->TilesO;break;
		case 8:pointD=&StaticInf->TilesD;break;
	}
	if (point) 
	{
 		if (wParam=='Z')
		{
		if (point->count(Selected) != 0)
		{
			iter = point->find(Selected);
			if (iter != point->begin()) {
				iter--;
				Selected = iter->first;
			}
		} else {
			if (point->lower_bound(Selected) != point->end()) {
			iter = point->lower_bound(Selected);
			if (iter != point->begin()) iter--;
			Selected = point->lower_bound(Selected)->first;
			} else
			{
			if (!point->empty()) {
			Selected = point->rbegin()->first;
			}
			}
		}
		}
		if (wParam=='X')
		{
			if (point->count(Selected) != 0)
		{
			iter = point->find(Selected);
			iter++;
			if (iter != point->end()) {
				Selected = iter->first;
			}
		} else {
			if (point->upper_bound(Selected) != point->end()) {
			Selected = point->upper_bound(Selected)->first;
			}
		}

		}
	}

	if (pointP) 
	{
 		if (wParam=='Z')
		{
		if (pointP->count(Selected) != 0)
		{
			iterP = pointP->find(Selected);
			if (iterP != pointP->begin()) {
				iterP--;
				Selected = iterP->first;
			}
		} else {
			if (pointP->lower_bound(Selected) != pointP->end()) {
			iterP = pointP->lower_bound(Selected);
			if (iterP != pointP->begin()) iterP--;
			Selected = pointP->lower_bound(Selected)->first;
			} else
			{
			if (!pointP->empty()) {
			Selected = pointP->rbegin()->first;
			}
			}
		}
		}
		if (wParam=='X')
		{
			if (pointP->count(Selected) != 0)
		{
			iterP = pointP->find(Selected);
			iterP++;
			if (iterP != pointP->end()) {
				Selected = iterP->first;
			}
		} else {
			if (pointP->upper_bound(Selected) != pointP->end()) {
			Selected = pointP->upper_bound(Selected)->first;
			}
		}

		}
	}


	if (pointD) 
	{
 		if (wParam=='Z')
		{
		if (pointD->count(Selected) != 0)
		{
			iterD = pointD->find(Selected);
			if (iterD != pointD->begin()) {
				iterD--;
				Selected = iterD->first;
			}
		} else {
			if (pointD->lower_bound(Selected) != pointD->end()) {
			iterD = pointD->lower_bound(Selected);
			if (iterD != pointD->begin()) iterD--;
			Selected = pointD->lower_bound(Selected)->first;
			} else
			{
			if (!pointD->empty()) {
			Selected = pointD->rbegin()->first;
			}
			}
		}
		}
		if (wParam=='X')
		{
			if (pointD->count(Selected) != 0)
		{
			iterD = pointD->find(Selected);
			iterD++;
			if (iterD != pointD->end()) {
				Selected = iterD->first;
			}
		} else {
			if (pointD->upper_bound(Selected) != pointD->end()) {
			Selected = pointD->upper_bound(Selected)->first;
			}
		}

		}
	}

	break;
			}
            break;

        case WM_SETCURSOR:
            // Turn off the cursor since this is a full-screen app
            SetCursor(NULL);
            return TRUE;
    }
	
    return DefWindowProc(hWnd, message, wParam, lParam);
}

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Initialization, message loop
//-----------------------------------------------------------------------------
int PASCAL
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow)
{
    MSG                         msg;
    
    if (InitApp(hInstance, nCmdShow) != DD_OK)
        return FALSE;

    while (TRUE)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (!GetMessage(&msg, NULL, 0, 0))
                return msg.wParam;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else if (g_bActive)
        {
            switch (GamePos)
			{
			case 0:
				play::MainMenu();
				break;
			case 1:
				play::EditTerep();
				break;
			case 2:
				play::ChooseTerep();
				break;
			case 3:
				play::FileLoadSave();
				break;
			case 4:
				play::EditObjects();
				break;
			case 5:
				play::EditInventory();
				break;
			case 6:
				play::EditCritters();
				break;
			}
        }
        else
        {
            // Make sure we go to sleep if we have nothing else to do
            WaitMessage();
        }
    }
}


