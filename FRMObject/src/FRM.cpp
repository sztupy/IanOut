#include "frm.h"

#include "io.h"
#include <fcntl.h>
#include "../commonutils/ddutil.h"
#include "../commonutils/utils.h"
#include "math.h"
#include "freeimage.h"

// --- TFRMAnim ---
//
// ----------------

extern int rsz, gsz, bsz; 	//bitsize of field
extern int rsh, gsh, bsh;	//0’s on left (the shift value)

extern DWORD palcal[256];
extern int BitDepth;

int TFRMAnim::LoadFRM(gzFile stream, LPVOID LoadIn, int x, int y, int p, bool frm_2)
{
   if (frm_2) {
		LPSTR	PointTo,PointTo2,PointTo3;
		WORD*	PointToInt;
		DWORD*	PointToInt32;
		int		i,i2;

		PointTo3 = (LPSTR)malloc(x*3);
	      	
		for (i=0; i<y; i++)
		{
			PointTo2 = PointTo3;
			gzread(stream, PointTo2, x*3);
			PointTo = (LPSTR)LoadIn+(i*p);
			if (BitDepth == 32) {
				PointToInt32 = (DWORD*)PointTo;
			} else {
				PointToInt = (WORD*)PointTo;
			}
			int r,g,b;

			for (i2=0;i2<x;i2++) {
				r = (unsigned char)(*PointTo2);
				g = (unsigned char)(*(PointTo2+1));
				b = (unsigned char)(*(PointTo2+2));

				r >>= (8-rsz);
				g >>= (8-gsz);
				b >>= (8-bsz);

				r <<= rsh;
				g <<= gsh;
				b <<= bsh;

				if (BitDepth == 32) {
					*PointToInt32 = (DWORD)(r | g | b);
					PointToInt32+=1;
				} else {
					*PointToInt = (WORD)(r | g | b);
					PointToInt+=1;
				}
				PointTo2+=3;
			}
		}

		free(PointTo3);
   } else {
		LPSTR	PointTo,PointTo2,PointTo3;
		WORD*	PointToInt;
		DWORD*	PointToInt32;
		int		i,i2;

		PointTo3 = (LPSTR)malloc(x);
	      	
		for (i=0; i<y; i++)
		{
			PointTo2 = PointTo3;
			gzread(stream, PointTo2, x);
			PointTo = (LPSTR)LoadIn+(i*p);
			if (BitDepth == 32) {
				PointToInt32 = (DWORD*)PointTo;
			} else {
				PointToInt = (WORD*)PointTo;
			}
			for (i2=0;i2<x;i2++) {
				if (BitDepth == 32) {
					*PointToInt32 = (DWORD)(palcal[(unsigned char)*PointTo2]);
					PointToInt32+=1;
				} else {
					*PointToInt = (WORD)(palcal[(unsigned char)*PointTo2]);
					PointToInt+=1;
				}
				PointTo2+=1;
			}
		}

		free(PointTo3);
   }
    
   return 0;
}

HRESULT TFRMAnim::Load(HWND hWnd, LPDIRECTDRAW7 g_pDD,const char* filename, int direction)
{
    char						buf[100];
	unsigned char				framedat[12];
	unsigned char				framecount[2];
	DDSURFACEDESC2              ddsd;
	HRESULT						hRet;
	PFRM						FRMDat;
	gzFile						stream;
	int							bx,by;
	int							bx1,by1;
	int							bx2,by2;
	unsigned char				bxx[2],byy[2];
	int							x, y;
	int							i;
	bool						frm_2;

    if (direction != -1) {
		sprintf(fname,"%s%i",filename,direction);
		if (!CanOpen2(fname)) sprintf(fname,"%sm",filename); else direction=0;
		direction-=1;
	} else {
		sprintf(fname,"%s",filename);	
	}

	std::string fullname = GetFile(fname);

	if (fullname=="") return InitFail(hWnd,DDERR_NOTLOADED,"File open error: %s",fname);

	FREE_IMAGE_FORMAT filetype = FreeImage_GetFileType(fullname.c_str(),0);

	if (filetype == FIF_UNKNOWN) {
		stream = __IOopen( fname,"rb");
					
		gzread(stream,buf,9);
		if ((buf[0]!=0) ||
			(buf[1]!=0) ||
			(buf[2]!=0) ||
			((buf[3]!=4) && (buf[3]!=5))) InitFail(hWnd,DDERR_NOTLOADED,"Unknown File Format: %s",fname);
		frm_2 = (buf[3]==5);

		gzread(stream,framecount,1);
		gzread(stream,bxx,2);
		gzread(stream,buf,10);
		gzread(stream,byy,2);
		gzread(stream,buf,38);
		bx=bxx[0]*256+bxx[1];if (bx>32767) bx=-(65536-bx);
		by=byy[0]*256+byy[1];if (by>32767) by=-(65536-by);
		bx2=0;by2=0;
		bx1=bx;by1=by;

		framecounter = framecount[0];
		while (direction>=0) {
			for (i=1; i<=framecount[0]; i++) {
				gzread(stream,framedat,8);
				x=framedat[0]*256+framedat[1];
				y=framedat[2]*256+framedat[3];

				gzread(stream,bxx,2);
				gzread(stream,byy,2);

				LPVOID ize;
				ize = malloc(x*y*4);
				LoadFRM(stream,ize,x,y,x,frm_2);
				free(ize);
			}
			direction--;
		}

		for (i=1; i<=framecount[0]; i++)
		{
			gzread(stream,framedat,8);
			x=framedat[0]*256+framedat[1];
			y=framedat[2]*256+framedat[3];

			gzread(stream,bxx,2);
			gzread(stream,byy,2);
			if (i!=1) {
				bx=bxx[0]*256+bxx[1];if (bx>32767) bx=-(65536-bx);
				by=byy[0]*256+byy[1];if (by>32767) by=-(65536-by);
				bx2+=bx;
				by2+=by;
			}

		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PITCH;// | DDSD_PIXELFORMAT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY ;

		ddsd.dwWidth = x;
		ddsd.dwHeight = y;
		if (BitDepth == 32) {
			ddsd.lPitch = x*4;
		} else {
			ddsd.lPitch = x*2;
		}

			FRMList->Insert( new TFRM() );

			FRMDat = (PFRM)(FRMList->FLast);
			hRet = g_pDD->CreateSurface(&ddsd, &FRMDat->FRM, NULL);
			if (hRet != DD_OK) return hRet;

			FRMDat->FRM->Lock(NULL, &ddsd, 0 , NULL);AddToLog("9");
			LoadFRM(stream, ddsd.lpSurface, ddsd.dwWidth, ddsd.dwHeight, ddsd.lPitch,frm_2);
			FRMDat->FRM->Unlock(NULL);AddToLog("10");

			FRMDat->x = x;
			FRMDat->y = y;

			FRMDat->bx = 16-(x/2-(bx1+bx2));
			FRMDat->by = 8-(y-(by1+by2));


			DDSetColorKey(FRMDat->FRM, RGB(0,0,0));
		}

		Counter = FRMList->First();
		
		gzclose(stream);

	} else {
		FIBITMAP* bitmap;
		FIMULTIBITMAP* multibitmap;
		if (filetype==FIF_TIFF) {
			multibitmap = FreeImage_OpenMultiBitmap(FIF_TIFF, fullname.c_str(),true,true,false);
			framecounter = FreeImage_GetPageCount(multibitmap);
			bitmap = FreeImage_LockPage(multibitmap,0);
		} else {
			framecounter = 1;
			bitmap = FreeImage_Load(filetype, fullname.c_str(), 0);
		}
		
		for (int i=0; i<framecounter; i++) {
			x = FreeImage_GetWidth(bitmap);
			y = FreeImage_GetHeight(bitmap);

			ZeroMemory(&ddsd, sizeof(ddsd));
			ddsd.dwSize = sizeof(ddsd);
			ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PITCH;
			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY ;

			ddsd.dwWidth = x;
			ddsd.dwHeight = y;
			if (BitDepth == 32) {
				ddsd.lPitch = x*4;
			} else {
				ddsd.lPitch = x*2;
			}

			FRMList->Insert( new TFRM() );

			FRMDat = (PFRM)(FRMList->FLast);

			hRet = g_pDD->CreateSurface(&ddsd, &FRMDat->FRM, NULL);
			if (hRet != DD_OK) return hRet;
			
			int FIpitch = FreeImage_GetPitch(bitmap);

			FRMDat->FRM->Lock(NULL, &ddsd, 0 , NULL);
			
			if (BitDepth == 32) {
				FreeImage_ConvertToRawBits((BYTE*)ddsd.lpSurface, bitmap, ddsd.lPitch, 32,ddsd.ddpfPixelFormat.dwRBitMask,ddsd.ddpfPixelFormat.dwGBitMask,ddsd.ddpfPixelFormat.dwBBitMask,false);
			} else {
				FreeImage_ConvertToRawBits((BYTE*)ddsd.lpSurface, bitmap, ddsd.lPitch, 16,ddsd.ddpfPixelFormat.dwRBitMask,ddsd.ddpfPixelFormat.dwGBitMask,ddsd.ddpfPixelFormat.dwBBitMask,false);
			}
			FRMDat->FRM->Unlock(NULL);
			
			FRMDat->x = x;
			FRMDat->y = y;

			FRMDat->bx = 16-(x/2);
			FRMDat->by = 8-(y);

			DDSetColorKey(FRMDat->FRM, RGB(0,0,0));

			if (i!=framecounter-1) {
				FreeImage_UnlockPage(multibitmap,bitmap,false);
				bitmap = FreeImage_LockPage(multibitmap,i);
			}
		}

		if (filetype==FIF_TIFF) {
			FreeImage_UnlockPage(multibitmap,bitmap,false);
			FreeImage_CloseMultiBitmap(multibitmap);
		} else {
			FreeImage_Unload(bitmap);
		}
	}

	return DD_OK;
}

TFRMAnim::~TFRMAnim()
{
	if (FRMList)
	{
	 PFRM Lista;
	 Lista = (PFRM)FRMList->First();
	 while (Lista)
	 {
	  Lista->FRM->Release();
	  Lista = (PFRM)FRMList->Next(Lista);
	 }
	 delete FRMList;
	 FRMList = NULL;
	}
	
}

// --- TFRMAnim6 ---
//
// -----------------

TFRMAnim6::~TFRMAnim6()
{
	int i;
	for (i=0; i<6; i++)
	{
	if (FRMList6[i])
	{
	 PFRM Lista;
	 Lista = (PFRM)FRMList6[i]->First();
	 while (Lista)
	 {
	  Lista->FRM->Release();
	  Lista = (PFRM)FRMList6[i]->Next(Lista);
	 }
	 delete FRMList6[i];
	 FRMList6[i] = NULL;
	}
	}
}

TFRMAnim6::TFRMAnim6()
{
	int i;
	for (i=0; i<6; i++)
	{
	  FRMList6[i] = new TList();
	}
	refcount=0;
}

HRESULT TFRMAnim6::Load(HWND hWnd, LPDIRECTDRAW7 g_pDD,const char* filename)
{
    char						buf[100];
	unsigned char				framedat[12];
	unsigned char				framecount[2];
	DDSURFACEDESC2              ddsd;
	HRESULT						hRet;
	PFRM						FRMDat;
	gzFile						stream;
	int							bx,by;
	int							bx1[6],by1[6];
	int							bx2[6],by2[6];
	unsigned char				bxx[6][2],byy[6][2];
	int							x, y;
	int							i,i2;
	bool						frm_2;

	wsprintf(fname,"%s",filename);
	std::string fullname = GetFile(fname);

	if (fullname=="") return InitFail(hWnd,DDERR_NOTLOADED,"File open error: %s",fname);

	FREE_IMAGE_FORMAT filetype = FreeImage_GetFileType(fullname.c_str(),0);

	if (filetype == FIF_UNKNOWN) {
		stream = __IOopen( filename,"rb");
					
		gzread(stream,buf,9);
		if ((buf[0]!=0) ||
			(buf[1]!=0) ||
			(buf[2]!=0) ||
			((buf[3]!=4) && (buf[3]!=5))) InitFail(hWnd,DDERR_NOTLOADED,"Unknown File Format: %s",fname);
		frm_2 = (buf[3]==5);

		gzread(stream,framecount,1);
		for (i=0; i<6; i++){
		gzread(stream,bxx[i],2);
		}
		for (i=0; i<6; i++){
		gzread(stream,byy[i],2);
		}
		gzread(stream,buf,28);
		for (i=0; i<6; i++){
		bx=bxx[i][0]*256+bxx[i][1];if (bx>32767) bx=-(65536-bx);
		by=byy[i][0]*256+byy[i][1];if (by>32767) by=-(65536-by);
		bx2[i]=0;by2[i]=0;
		bx1[i]=bx;by1[i]=by;
		}
		framecounter = framecount[0];
		for (i2=0; i2<6; i2++)
		for (i=1; i<=framecount[0]; i++)
		{
		gzread(stream,framedat,8);
		x=framedat[0]*256+framedat[1];
		y=framedat[2]*256+framedat[3];
		gzread(stream,&bxx[i2],2);
		gzread(stream,&byy[i2],2);

		// if (i!=1) {
 		bx=bxx[i2][0]*256+bxx[i2][1];if (bx>32767) bx=-(65536-bx);
		by=byy[i2][0]*256+byy[i2][1];if (by>32767) by=-(65536-by);
 		bx2[i2]+=bx;
		by2[i2]+=by;
		//}

		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PITCH;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY ;

		ddsd.dwWidth = x;
		ddsd.dwHeight = y;
		if (BitDepth == 32) {
			ddsd.lPitch = x*4;
		} else {
			ddsd.lPitch = x*2;
		}

		FRMList6[i2]->Insert( new TFRM() );

		FRMDat = (PFRM)(FRMList6[i2]->FLast);
		hRet = g_pDD->CreateSurface(&ddsd, &FRMDat->FRM, NULL);
		if (hRet != DD_OK) return hRet;
			
		FRMDat->FRM->Lock(NULL, &ddsd, 0 , NULL);
		LoadFRM(stream, ddsd.lpSurface, ddsd.dwWidth, ddsd.dwHeight, ddsd.lPitch,frm_2);
		FRMDat->FRM->Unlock(NULL);

		FRMDat->x = x;
		FRMDat->y = y;

		FRMDat->bx = 16-(x/2-(bx1[i2]+bx2[i2]));
		FRMDat->by = 8-(y-(by1[i2]+by2[i2]));

		DDSetColorKey(FRMDat->FRM, RGB(0,0,0));

		}

		for (i2=0; i2<6; i2++) Counter6[i2] = FRMList6[i2]->First();
		dir = 0;
		
		gzclose(stream);
	} else {
		FIBITMAP* bitmap;
		FIMULTIBITMAP* multibitmap;
		if (filetype==FIF_TIFF) {
			multibitmap = FreeImage_OpenMultiBitmap(FIF_TIFF, fullname.c_str(),true,true,false);
			framecounter = FreeImage_GetPageCount(multibitmap)/6;
			bitmap = FreeImage_LockPage(multibitmap,0);
		} else {
			framecounter = 1;
			bitmap = FreeImage_Load(filetype, fullname.c_str(), 0);
		}
		
		for (int i2=0; i2<6; i2++) {
			for (int i=0; i<framecounter; i++) {
				x = FreeImage_GetWidth(bitmap);
				y = FreeImage_GetHeight(bitmap);

				ZeroMemory(&ddsd, sizeof(ddsd));
				ddsd.dwSize = sizeof(ddsd);
				ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PITCH;
				ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY ;

				ddsd.dwWidth = x;
				ddsd.dwHeight = y;
				if (BitDepth == 32) {
					ddsd.lPitch = x*4;
				} else {
					ddsd.lPitch = x*2;
				}

				FRMList6[i2]->Insert( new TFRM() );
				FRMDat = (PFRM)(FRMList6[i2]->FLast);

				hRet = g_pDD->CreateSurface(&ddsd, &FRMDat->FRM, NULL);
				if (hRet != DD_OK) return hRet;
				
				int FIpitch = FreeImage_GetPitch(bitmap);

				FRMDat->FRM->Lock(NULL, &ddsd, 0 , NULL);
				
				if (BitDepth == 32) {
					FreeImage_ConvertToRawBits((BYTE*)ddsd.lpSurface, bitmap, ddsd.lPitch, 32,ddsd.ddpfPixelFormat.dwRBitMask,ddsd.ddpfPixelFormat.dwGBitMask,ddsd.ddpfPixelFormat.dwBBitMask,false);
				} else {
					FreeImage_ConvertToRawBits((BYTE*)ddsd.lpSurface, bitmap, ddsd.lPitch, 16,ddsd.ddpfPixelFormat.dwRBitMask,ddsd.ddpfPixelFormat.dwGBitMask,ddsd.ddpfPixelFormat.dwBBitMask,false);
				}
				FRMDat->FRM->Unlock(NULL);
				
				FRMDat->x = x;
				FRMDat->y = y;

				FRMDat->bx = 16-(x/2);
				FRMDat->by = 8-(y);

				DDSetColorKey(FRMDat->FRM, RGB(0,0,0));

				if ((i!=framecounter-1) || (i2!=5)) {
					FreeImage_UnlockPage(multibitmap,bitmap,false);
					bitmap = FreeImage_LockPage(multibitmap,i+(i2*framecounter));
				}
			}
		}

		if (filetype==FIF_TIFF) {
			FreeImage_UnlockPage(multibitmap,bitmap,false);
			FreeImage_CloseMultiBitmap(multibitmap);
		} else {
			FreeImage_Unload(bitmap);
		}
	}
	
	return DD_OK;
}

void TFRMAnim6::FirstFrame()
{
	int i2;
	for (i2=0; i2<6; i2++) { Counter6[i2] = FRMList6[i2]->First(); }
}

void TFRMAnim6::NextFrame()
{
	Counter6[dir] = FRMList6[dir]->RNext(Counter6[dir]);
}

PFRM TFRMAnim6::GetCurFrame()
{
	PFRM Lista;
	Lista = (PFRM)Counter6[dir];
	return Lista;
}

void TFRMAnim6::SetDir(int i_dir)
{
	dir = i_dir;
}

// --- TFRMSingle ---
//
// ------------------

HRESULT TFRMSingle::Load(HWND hWnd, LPDIRECTDRAW7 g_pDD, const char* filename, int framenum)
{
    char						buf[100];
	char						buf2[100];						
	unsigned char				framedat[12];
	DDSURFACEDESC2              ddsd;
	HRESULT						hRet;
	gzFile						stream;
	int							x, y;
	int							bx,by;
	int							bx1,by1;
	int							bx2,by2;
	unsigned char				bxx[2],byy[2];
	int							i;
	bool						frm_2;
	LPVOID						ize;

	wsprintf(buf2,"%s",filename);
	wsprintf(fname,"%s",filename);
 
	std::string fullname = GetFile(buf2);

	if (fullname=="") return InitFail(hWnd,DDERR_NOTLOADED,"File open error: %s",buf2);

	FREE_IMAGE_FORMAT filetype = FreeImage_GetFileType(fullname.c_str(),0);

	if (filetype == FIF_UNKNOWN) {

		stream = __IOopen( fname,"rb");

        gzread(stream,buf,10);
		if ((buf[0]!=0) ||
			(buf[1]!=0) ||
			(buf[2]!=0) ||
			((buf[3]!=4) && (buf[3]!=5))) InitFail(hWnd,DDERR_NOTLOADED,"Unknown File Format: %s",buf2);
		frm_2 = (buf[3]==5);
		gzread(stream,bxx,2);
		gzread(stream,buf,10);
		gzread(stream,byy,2);
		gzread(stream,buf,38);
		bx=bxx[0]*256+bxx[1];if (bx>32767) bx=-(65536-bx);
		by=byy[0]*256+byy[1];if (by>32767) by=-(65536-by);
		bx2=0;by2=0;
		bx1=bx;by1=by;
		for (i=1; i<=framenum; i++)
		{
		gzread(stream,framedat,8);
		x=framedat[0]*256+framedat[1];
		y=framedat[2]*256+framedat[3];

		gzread(stream,bxx,2);
		gzread(stream,byy,2);
		//if (i!=1) {
		bx=bxx[0]*256+bxx[1];if (bx>32767) bx=-(65536-bx);
		by=byy[0]*256+byy[1];if (by>32767) by=-(65536-by);
 		bx2+=bx;
		by2+=by;
		//}

		if (i!=framenum) {
		ize = malloc(x*y);
		LoadFRM(stream,ize,x,y,x,frm_2);
		free(ize);
		}
		}

		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PITCH;// | DDSD_PIXELFORMAT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY ;

		ddsd.dwWidth = x;
		ddsd.dwHeight = y;
		if (BitDepth == 32) {
			ddsd.lPitch = x*4;
		} else {
			ddsd.lPitch = x*2;
		}

		if (FRM) {FRM->FRM->Release();delete FRM;};
	      
		FRM = new TFRM();

		hRet = g_pDD->CreateSurface(&ddsd, &FRM->FRM, NULL);
		if (hRet != DD_OK) return hRet;
		
		FRM->FRM->Lock(NULL, &ddsd, 0 , NULL);
		LoadFRM(stream, ddsd.lpSurface, ddsd.dwWidth, ddsd.dwHeight, ddsd.lPitch,frm_2);
		FRM->FRM->Unlock(NULL);

		FRM->x = x;
		FRM->y = y;

		FRM->bx = 16-(x/2-(bx1+bx2));
		FRM->by = 8-(y-(by1+by2));

		DDSetColorKey(FRM->FRM, RGB(0,0,0));
		//FRM->FRM->SetPalette(g_pDDPal);
		gzclose(stream);
	} else {
		FIBITMAP* bitmap;
		FIMULTIBITMAP* multibitmap;
		if (filetype==FIF_TIFF) {
			multibitmap = FreeImage_OpenMultiBitmap(FIF_TIFF, fullname.c_str(),false,true,false);
			bitmap = FreeImage_LockPage(multibitmap,framenum-1);
		} else {
			bitmap = FreeImage_Load(filetype, fullname.c_str(), 0);
		}
		//FreeImage_ConvertTo32Bits(bitmap);
		
		x = FreeImage_GetWidth(bitmap);
		y = FreeImage_GetHeight(bitmap);

		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PITCH;// | DDSD_PIXELFORMAT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY ;

		ddsd.dwWidth = x;
		ddsd.dwHeight = y;
		if (BitDepth == 32) {
			ddsd.lPitch = x*4;
		} else {
			ddsd.lPitch = x*2;
		}

		if (FRM) {FRM->FRM->Release();delete FRM;};
     
		FRM = new TFRM();
		hRet = g_pDD->CreateSurface(&ddsd, &FRM->FRM, NULL);
		if (hRet != DD_OK) return hRet;
		
		int FIpitch = FreeImage_GetPitch(bitmap);

		FRM->FRM->Lock(NULL, &ddsd, 0 , NULL);
		if (BitDepth == 32) {
			FreeImage_ConvertToRawBits((BYTE*)ddsd.lpSurface, bitmap, ddsd.lPitch, 32,ddsd.ddpfPixelFormat.dwRBitMask,ddsd.ddpfPixelFormat.dwGBitMask,ddsd.ddpfPixelFormat.dwBBitMask,false);
		} else {
			FreeImage_ConvertToRawBits((BYTE*)ddsd.lpSurface, bitmap, ddsd.lPitch, 16,ddsd.ddpfPixelFormat.dwRBitMask,ddsd.ddpfPixelFormat.dwGBitMask,ddsd.ddpfPixelFormat.dwBBitMask,false);
		}
		FRM->FRM->Unlock(NULL);
		
		FRM->x = x;
		FRM->y = y;

		FRM->bx = 16-(x/2);
		FRM->by = 8-(y);

		DDSetColorKey(FRM->FRM, RGB(0,0,0));

		if (filetype==FIF_TIFF) {
			FreeImage_UnlockPage(multibitmap,bitmap,false);
			FreeImage_CloseMultiBitmap(multibitmap);
		} else {
			FreeImage_Unload(bitmap);
		}

		
	}

	return DD_OK;
}


int TFRMSingle::LoadFRM(gzFile stream, LPVOID LoadIn, int x, int y, int p, bool frm_2)
{
	if (frm_2) {
		LPSTR	PointTo,PointTo2,PointTo3;
		WORD*	PointToInt;
		DWORD*	PointToInt32;
		int		i,i2;

		PointTo3 = (LPSTR)malloc(x*3);
	      	
		for (i=0; i<y; i++)
		{
			PointTo2 = PointTo3;
			gzread(stream, PointTo2, x*3);
			PointTo = (LPSTR)LoadIn+(i*p);
			if (BitDepth == 32) {
				PointToInt32 = (DWORD*)PointTo;
			} else {
				PointToInt = (WORD*)PointTo;
			}
			int r,g,b;

			for (i2=0;i2<x;i2++) {
				r = (unsigned char)(*PointTo2);
				g = (unsigned char)(*(PointTo2+1));
				b = (unsigned char)(*(PointTo2+2));

				r >>= (8-rsz);
				g >>= (8-gsz);
				b >>= (8-bsz);

				r <<= rsh;
				g <<= gsh;
				b <<= bsh;

				if (BitDepth == 32) {
					*PointToInt32 = (DWORD)(r | g | b);
					PointToInt32+=1;
				} else {
					*PointToInt = (WORD)(r | g | b);
					PointToInt+=1;
				}
				PointTo2+=3;
			}
		}

		free(PointTo3);
   } else {
		LPSTR	PointTo,PointTo2,PointTo3;
		WORD*	PointToInt;
		DWORD*	PointToInt32;
		int		i,i2;

		PointTo3 = (LPSTR)malloc(x);
	      	
		for (i=0; i<y; i++)
		{
			PointTo2 = PointTo3;
			gzread(stream, PointTo2, x);
			PointTo = (LPSTR)LoadIn+(i*p);
			if (BitDepth == 32) {
				PointToInt32 = (DWORD*)PointTo;
			} else {
				PointToInt = (WORD*)PointTo;
			}
			for (i2=0;i2<x;i2++) {
				if (BitDepth == 32) {
					*PointToInt32 = (DWORD)(palcal[(unsigned char)*PointTo2]);
					PointToInt32+=1;
				} else {
					*PointToInt = (WORD)(palcal[(unsigned char)*PointTo2]);
					PointToInt+=1;
				}
				PointTo2+=1;
			}
		}

		free(PointTo3);
   }
    
   return 0;
}


/* - TFRMCommunicator - */

TFRMCommunicator::TFRMCommunicator(PFRMAnim6 FRMAssign)
{
   Animation = FRMAssign;
   dir = 0;
   FirstFrame();
   Animation->IncRefCount();
}

void TFRMCommunicator::FirstFrame()
{
	int i2;
	for (i2=0; i2<6; i2++) { Count6[i2] = Animation->FRMList6[i2]->First(); }
}

PFRM TFRMCommunicator::GetCurFrame()
{
	PFRM Lista;
	Lista = (PFRM)Count6[dir];
	return Lista;
}

TFRMAnimCommunicator::TFRMAnimCommunicator(PFRMAnim FRMAssign)
{
   Animation = FRMAssign;
   FirstFrame();
   Animation->IncRefCount();
}