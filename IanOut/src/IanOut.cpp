#include "global.h"

#include "../commonutils/mouse.h"
#include "done.h"
#include "load.h"
#include "../commonutils/palette.h"
#include "play.h"

#include "../commonutils/date.h"
#include "../commonutils/textutil.h"
#include "../frmobject/freeimage.h"

#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>

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
		
		
	case WM_KEYDOWN:
		// Handle any non-accelerated key commands
		switch (wParam)
		{
		case VK_PAUSE:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			return 0L;
		case VK_UP:
			if ((Selected>1) && (Selected<=7)) Selected--;
			if ((Selected>8) && (Selected<=25)) Selected--;
			if ((Selected>26) && (Selected<=41)) Selected--;
			if ((Selected>42) && (Selected<=59)) Selected--;
			break;
		case VK_DOWN:
			if ((Selected>=1) && (Selected<7)) Selected++;
			if ((Selected>=8) && (Selected<25)) Selected++;
			if ((Selected>=26) && (Selected<41)) Selected++;
			if ((Selected>=42) && (Selected<59)) Selected++;
			break;
		case 'Q': date::AddMinute(15);break;
		case 'A': date::AddMinute(-15);break;
		case 'N':
			if (GamePos==1) { CritterInf->Critters.find(0)->second->dir++; if (CritterInf->Critters.find(0)->second->dir>5) CritterInf->Critters.find(0)->second->dir=0; } 
			break;
		case 'M':
			if (GamePos==1) { CritterInf->Critters.find(0)->second->dir--; if (CritterInf->Critters.find(0)->second->dir<0) CritterInf->Critters.find(0)->second->dir=5; } 
			break;
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
			std::string xXx;
			xXx = "Screenshot taken: ";
			xXx += buf;
			textutil::AddString(xXx,1);
			
			break;
		}
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
		
		}
		break;
		
        case WM_SETCURSOR:
            // Turn off the cursor since this is a full-screen app
            SetCursor(NULL);
            return TRUE;
    }
	
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void RunGame(bool type)
{
	switch (GamePos)
	{
	case 0:
		if (type) play::MainMenu();
		break;
	case 1:
		play::NormalPlay();
		break;
	case 2:
		if (type) play::Credits();
		break;
	case 3:
		if (type) play::Intro();
		break;
	case 4:
		if (type) play::CreateChar();
		break;
	case 5:
		play::Talk();
		break;
	case 6:
		play::Travel();
		break;
	}
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
			RunGame(true);
        }
        else
        {
            // Make sure we go to sleep if we have nothing else to do
            WaitMessage();
        }
    }
}

