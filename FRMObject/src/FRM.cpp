#include "frm.h"

#include "io.h"
#include "../commonutils/ddutil.h"
#include "../commonutils/utils.h"
#include "math.h"
#include "sdl_image.h"
#include "../commonutils/sdl_zlib.h"

extern SDL_PixelFormat*				g_pDDPixelFormat;

extern Uint32 palcal[256];
extern int BitDepth;

int TFRMAnim::LoadFRM(gzFile stream, void* LoadIn, int x, int y, int p, bool frm_2)
{
   if (frm_2) {
		char*	PointTo,*PointTo2,*PointTo3;
		Uint16*	PointToInt;
		Uint32*	PointToInt32;
		int		i,i2;

		PointTo3 = (char*)malloc(x*3);
	      	
		for (i=0; i<y; i++)
		{
			PointTo2 = PointTo3;
			gzread(stream, PointTo2, x*3);
			PointTo = (char*)LoadIn+(i*p);
			if (BitDepth == 32) {
				PointToInt32 = (Uint32*)PointTo;
			} else {
				PointToInt = (Uint16*)PointTo;
			}
			int r,g,b;

			for (i2=0;i2<x;i2++) {
				r = (unsigned char)(*PointTo2);
				g = (unsigned char)(*(PointTo2+1));
				b = (unsigned char)(*(PointTo2+2));

				if (BitDepth == 32) {
					*PointToInt32 = (Uint32)SDL_MapRGB(g_pDDPixelFormat,r,g,b);
					PointToInt32+=1;
				} else {
					*PointToInt = (Uint16)SDL_MapRGB(g_pDDPixelFormat,r,g,b);
					PointToInt+=1;
				}
				PointTo2+=3;
			}
		}

		free(PointTo3);
   } else {
		char*	PointTo,*PointTo2,*PointTo3;
		Uint16*	PointToInt;
		Uint32*	PointToInt32;
		int		i,i2;

		PointTo3 = (char*)malloc(x);
	      	
		for (i=0; i<y; i++)
		{
			PointTo2 = PointTo3;
			gzread(stream, PointTo2, x);
			PointTo = (char*)LoadIn+(i*p);
			if (BitDepth == 32) {
				PointToInt32 = (Uint32*)PointTo;
			} else {
				PointToInt = (Uint16*)PointTo;
			}
			for (i2=0;i2<x;i2++) {
				if (BitDepth == 32) {
					*PointToInt32 = (Uint32)(palcal[(unsigned char)*PointTo2]);
					PointToInt32+=1;
				} else {
					*PointToInt = (Uint16)(palcal[(unsigned char)*PointTo2]);
					PointToInt+=1;
				}
				PointTo2+=1;
			}
		}

		free(PointTo3);
   }
    
   return 0;
}

int TFRMAnim::Load(const char* filename, int direction)
{
    char						buf[100];
	unsigned char				framedat[12];
	unsigned char				framecount[2];
	int							hRet;
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

	if (fullname=="") return InitFail(0,"File open error: %s",fname);

	int ffiletype = 0;

	gzFile file = __IOopen(fname,"rb");
	SDL_RWops *rwop = SDL_RWFromGzip(file);
	if (IMG_isBMP(rwop)) ffiletype = 1;
	gzseek(file, 0, SEEK_SET);
	if (IMG_isJPG(rwop)) ffiletype = 1;
	gzseek(file, 0, SEEK_SET);
	if (IMG_isTIF(rwop)) ffiletype = 1;
	gzclose(file);

	if (ffiletype == 0) {
		stream = __IOopen( fname,"rb");
					
		gzread(stream,buf,9);
		if ((buf[0]!=0) ||
			(buf[1]!=0) ||
			(buf[2]!=0) ||
			((buf[3]!=4) && (buf[3]!=5))) return InitFail(0,"Unknown File Format: %s",fname);
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

				void* ize;
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

			FRMList->Insert( new TFRM() );

			FRMDat = (PFRM)(FRMList->FLast);

			FRMDat->FRM = SDL_CreateRGBSurface(SDL_SWSURFACE, x,y, BitDepth, g_pDDPixelFormat->Rmask, g_pDDPixelFormat->Gmask,g_pDDPixelFormat->Bmask,g_pDDPixelFormat->Amask);
		
			SDL_LockSurface(FRMDat->FRM);
			LoadFRM(stream, FRMDat->FRM->pixels, FRMDat->FRM->w, FRMDat->FRM->h, FRMDat->FRM->pitch,frm_2);
			SDL_UnlockSurface(FRMDat->FRM);

			FRMDat->x = x;
			FRMDat->y = y;

			FRMDat->bx = 16-(x/2-(bx1+bx2));
			FRMDat->by = 8-(y-(by1+by2));
	
			SDL_SetColorKey(FRMDat->FRM, SDL_SRCCOLORKEY, 0);

			SDL_UpdateRect(FRMDat->FRM,0,0,0,0);
		}

		Counter = FRMList->First();
		
		gzclose(stream);

	} else {
		FRMList->Insert( new TFRM() );
		FRMDat = (PFRM)(FRMList->FLast);
		
		gzFile file = __IOopen(fname,"rb");
		SDL_RWops *rwop = SDL_RWFromGzip(file);
		SDL_Surface* temp = IMG_Load_RW(rwop,1);

		FRMDat->FRM = SDL_ConvertSurface(temp, g_pDDPixelFormat, SDL_SWSURFACE);
		SDL_FreeSurface(temp);

		x = FRMDat->FRM->w;
		y = FRMDat->FRM->h;

		FRMDat->x = x;
		FRMDat->y = y;

		FRMDat->bx = 16-(x/2);
		FRMDat->by = 8-(y);
	
		SDL_UpdateRect(FRMDat->FRM,0,0,0,0);

		SDL_SetColorKey(FRMDat->FRM, SDL_SRCCOLORKEY, 0);
	}

	return 0;
}

TFRMAnim::~TFRMAnim()
{
	if (FRMList)
	{
	 PFRM Lista;
	 Lista = (PFRM)FRMList->First();
	 while (Lista)
	 {
	  SDL_FreeSurface(Lista->FRM);
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
	  SDL_FreeSurface(Lista->FRM);
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

int TFRMAnim6::Load(const char* filename)
{
    char						buf[100];
	unsigned char				framedat[12];
	unsigned char				framecount[2];
	int							hRet;
	PFRM						FRMDat;
	gzFile						stream;
	int							bx,by;
	int							bx1[6],by1[6];
	int							bx2[6],by2[6];
	unsigned char				bxx[6][2],byy[6][2];
	int							x, y;
	int							i,i2;
	bool						frm_2;

	sprintf(fname,"%s",filename);
	std::string fullname = GetFile(fname);

	if (fullname=="") return InitFail(0,"File open error: %s",fname);

	int ffiletype = 0;

	gzFile file = __IOopen(fname,"rb");
	SDL_RWops *rwop = SDL_RWFromGzip(file);
	if (IMG_isBMP(rwop)) ffiletype = 1;
	gzseek(file, 0, SEEK_SET);
	if (IMG_isJPG(rwop)) ffiletype = 1;
	gzseek(file, 0, SEEK_SET);
	if (IMG_isTIF(rwop)) ffiletype = 1;
	gzclose(file);

	if (ffiletype == 0) {
		stream = __IOopen( filename,"rb");
					
		gzread(stream,buf,9);
		if ((buf[0]!=0) ||
			(buf[1]!=0) ||
			(buf[2]!=0) ||
			((buf[3]!=4) && (buf[3]!=5))) return InitFail(0,"Unknown File Format: %s",fname);
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

		FRMList6[i2]->Insert( new TFRM() );

		FRMDat = (PFRM)(FRMList6[i2]->FLast);
		FRMDat->FRM = SDL_CreateRGBSurface(SDL_SWSURFACE, x,y, BitDepth, g_pDDPixelFormat->Rmask, g_pDDPixelFormat->Gmask,g_pDDPixelFormat->Bmask,g_pDDPixelFormat->Amask);
		
		SDL_LockSurface(FRMDat->FRM);
		LoadFRM(stream, FRMDat->FRM->pixels, FRMDat->FRM->w, FRMDat->FRM->h, FRMDat->FRM->pitch,frm_2);
		SDL_UnlockSurface(FRMDat->FRM);

		FRMDat->x = x;
		FRMDat->y = y;

		FRMDat->bx = 16-(x/2-(bx1[i2]+bx2[i2]));
		FRMDat->by = 8-(y-(by1[i2]+by2[i2]));
	
		SDL_UpdateRect(FRMDat->FRM,0,0,0,0);

		SDL_SetColorKey(FRMDat->FRM, SDL_SRCCOLORKEY, 0);
		}

		for (i2=0; i2<6; i2++) Counter6[i2] = FRMList6[i2]->First();
		dir = 0;
		
		gzclose(stream);
	} else {
		for (int i2=0; i2<6; i2++) {
			FRMList6[i2]->Insert( new TFRM() );
			FRMDat = (PFRM)(FRMList6[i2]->FLast);
			
			gzFile file = __IOopen(fname,"rb");
			SDL_RWops *rwop = SDL_RWFromGzip(file);
			SDL_Surface* temp = IMG_Load_RW(rwop,1);

			FRMDat->FRM = SDL_ConvertSurface(temp, g_pDDPixelFormat, SDL_SWSURFACE);
			SDL_FreeSurface(temp);

			x = FRMDat->FRM->w;
			y = FRMDat->FRM->h;

			FRMDat->x = x;
			FRMDat->y = y;

			FRMDat->bx = 16-(x/2);
			FRMDat->by = 8-(y);
		
			SDL_UpdateRect(FRMDat->FRM,0,0,0,0);

			SDL_SetColorKey(FRMDat->FRM, SDL_SRCCOLORKEY, 0);
		}
	}
	
	return 0;
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

int TFRMSingle::Load(const char* filename, int framenum)
{

    char						buf[100];
	char						buf2[100];						
	unsigned char				framedat[12];
	int							hRet;
	gzFile						stream;
	int							x, y;
	int							bx,by;
	int							bx1,by1;
	int							bx2,by2;
	unsigned char				bxx[2],byy[2];
	int							i;
	bool						frm_2;
	void*						ize;

	sprintf(buf2,"%s",filename);
	sprintf(fname,"%s",filename);

 	std::string fullname = GetFile(buf2);

	if (fullname=="") return InitFail(0,"File open error: %s",buf2);

	int ffiletype = 0;

	gzFile file = __IOopen(fname,"rb");
	SDL_RWops *rwop = SDL_RWFromGzip(file);
	if (IMG_isBMP(rwop)) ffiletype = 1;
	gzseek(file, 0, SEEK_SET);
	if (IMG_isJPG(rwop)) ffiletype = 1;
	gzseek(file, 0, SEEK_SET);
	if (IMG_isTIF(rwop)) ffiletype = 1;
	gzclose(file);
	
	if (ffiletype == 0) {

		stream = __IOopen( fname,"rb");

        gzread(stream,buf,10);
		if ((buf[0]!=0) ||
			(buf[1]!=0) ||
			(buf[2]!=0) ||
			((buf[3]!=4) && (buf[3]!=5))) return InitFail(0,"Unknown File Format: %s",buf2);
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
		
		if (FRM) {SDL_FreeSurface(FRM->FRM);delete FRM;};
	      
		FRM = new TFRM();
		FRM->FRM = SDL_CreateRGBSurface(SDL_SWSURFACE, x,y, BitDepth, g_pDDPixelFormat->Rmask, g_pDDPixelFormat->Gmask,g_pDDPixelFormat->Bmask,g_pDDPixelFormat->Amask);
		SDL_LockSurface(FRM->FRM);
		LoadFRM(stream, FRM->FRM->pixels, FRM->FRM->w, FRM->FRM->h, FRM->FRM->pitch, frm_2);
		SDL_UnlockSurface(FRM->FRM);

		FRM->x = x;
		FRM->y = y;

		FRM->bx = 16-(x/2-(bx1+bx2));
		FRM->by = 8-(y-(by1+by2));
	
		SDL_SetColorKey(FRM->FRM, SDL_SRCCOLORKEY, 0);

		SDL_UpdateRect(FRM->FRM,0,0,0,0);
		SDL_SetClipRect(FRM->FRM, NULL);

		gzclose(stream);
	} else {
		
		if (FRM) {SDL_FreeSurface(FRM->FRM);delete FRM;};
		FRM = new TFRM();

		gzFile file = __IOopen(fname,"rb");
		SDL_RWops *rwop = SDL_RWFromGzip(file);
		SDL_Surface* temp = IMG_Load_RW(rwop,1);
		FRM->FRM = SDL_ConvertSurface(temp, g_pDDPixelFormat, SDL_SWSURFACE);
		SDL_FreeSurface(temp);

		x = FRM->FRM->w;
		y = FRM->FRM->h;

		FRM->x = x;
		FRM->y = y;

		FRM->bx = 16-(x/2);
		FRM->by = 8-(y);
	
		SDL_UpdateRect(FRM->FRM,0,0,0,0);

		SDL_SetColorKey(FRM->FRM, SDL_SRCCOLORKEY, 0);

		//gzclose(file);
	}

	return 0;
}


int TFRMSingle::LoadFRM(gzFile stream, void* LoadIn, int x, int y, int p, bool frm_2)
{
	if (frm_2) {
		char*	PointTo,*PointTo2,*PointTo3;
		Uint16*	PointToInt;
		Uint32*	PointToInt32;
		int		i,i2;

		PointTo3 = (char*)malloc(x*3);
	      	
		for (i=0; i<y; i++)
		{
			PointTo2 = PointTo3;
			gzread(stream, PointTo2, x*3);
			PointTo = (char*)LoadIn+(i*p);
			if (BitDepth == 32) {
				PointToInt32 = (Uint32*)PointTo;
			} else {
				PointToInt = (Uint16*)PointTo;
			}
			int r,g,b;

			for (i2=0;i2<x;i2++) {
				r = (unsigned char)(*PointTo2);
				g = (unsigned char)(*(PointTo2+1));
				b = (unsigned char)(*(PointTo2+2));

				if (BitDepth == 32) {
					*PointToInt32 = (Uint32)SDL_MapRGB(g_pDDPixelFormat,r,g,b);
					PointToInt32+=1;
				} else {
					*PointToInt = (Uint16)SDL_MapRGB(g_pDDPixelFormat,r,g,b);
					PointToInt+=1;
				}
				PointTo2+=3;
			}
		}

		free(PointTo3);
   } else {
		char*	PointTo,*PointTo2,*PointTo3;
		Uint16*	PointToInt;
		Uint32*	PointToInt32;
		int		i,i2;

		PointTo3 = (char*)malloc(x);
	      	
		for (i=0; i<y; i++)
		{
			PointTo2 = PointTo3;
			gzread(stream, PointTo2, x);
			PointTo = (char*)LoadIn+(i*p);
			if (BitDepth == 32) {
				PointToInt32 = (Uint32*)PointTo;
			} else {
				PointToInt = (Uint16*)PointTo;
			}
			for (i2=0;i2<x;i2++) {
				if (BitDepth == 32) {
					*PointToInt32 = (Uint32)(palcal[(unsigned char)*PointTo2]);
					PointToInt32+=1;
				} else {
					*PointToInt = (Uint16)(palcal[(unsigned char)*PointTo2]);
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