#include "SYS\STAT.H"
#include "play.h"
#include "load.h"
#include "../commonutils/mouse.h"
#include "terep.h"
#include "../commonutils/textfont.h"
#include "../commonutils/textutil.h"
#include "done.h"

void play::FileLoadSave(void)
{
	int								hRet;
	static int							Frame = 0;
	static Uint32						ThisTick;
	static Uint32						LastTick = 0;
	PIanMap								OldMapInf;
	int									i,i2;
	std::string							FN;
	FRMCollection::iterator				iter;
	static std::string FilName = "untitled.map";
		
	ThisTick = SDL_GetTicks();

	if ((ThisTick - LastTick) > (Uint32)gameSpeed)
        {
            LastTick = ThisTick;
        }
														
	if (MousX>=GetMaxX) { MousX=GetMaxX; }
	if (MousX<=0)   { MousX=0;   }							
	if (MousY>=GetMaxY) { MousY=GetMaxY; }							
	if (MousY<=0)   { MousY=0;   }						

	ClearSurface(g_pDDSBack,0,0,0);
	
	textfont::IanOutText(30,50,1,"MAIN MENU");
	textfont::IanOutText(30,80,1,"New Map, Filled");
	textfont::IanOutText(30,100,1,"New Map (Load Blank.map)");
	textfont::IanOutText(30,120,1,"Load");
	textfont::IanOutText(30,140,1,"Save");

	textfont::IanOutText(30,200,1,"Filename :");
	putmode=4;
	ref_str = NULL;
	bool InSide = false;
	if (mouse::MouseIn(200,200,GetMaxX,215)) { ref_str = &FilName; InSide = true;} else InSide = false;
	textfont::IanOutText(200,200,InSide ? 0 : 1,"\"mod\\IanEd\\maps\\"+FilName+"\"");
	textutil::DisplayFrameRate();

	if (mouse::MInPr(30,50,200,80,0)) {
		dims.buttons[0]=0;
		dims.buttons[1]=0;
		dims.buttons[2]=0;
		GamePos=0;
	}
	if (mouse::MInPr(30,80,200,99,0)) {
        OldMapInf = MapInf;
		MapInf = NULL;
		if (StaticInf) delete StaticInf;
		MapInf = new TIanMap();
		StaticInf = new TIanStatic();
		int x,y;
		if (!OldMapInf->Tiles.empty()) iter=OldMapInf->Tiles.begin();
		for (x=0; x<128; x++)
		for (y=0; y<256; y++)
		{
			MapInf->Map[x][y] = 0;
			if ((OldMapInf->Tiles.empty()) == false) {
				i2 = rand()%15;
                for (i=0; i<i2; i++)
				{
					if (iter==OldMapInf->Tiles.end()) iter=OldMapInf->Tiles.begin(); else iter++;
				}
				if (iter==OldMapInf->Tiles.end()) iter=OldMapInf->Tiles.begin();
			    MapInf->Map[x][y]=iter->first;
			}
			MapInf->Map2[x][y] = 0;
		}
		for (x=0; x<512; x++)
		for (y=0; y<512; y++)
		   BlockDat[x][y] = 0;

		if (OldMapInf) delete OldMapInf;
		MapInf->LoadTiles();
		BoundRect.top = 0;
		BoundRect.left = 0;
		BoundRect.bottom = 511;
		BoundRect.right = 511;
		BoundType = false;BoundTest = false;
		dims.buttons[0]=0;
		dims.buttons[1]=0;
		dims.buttons[2]=0;
		GamePos=0;
	}
	if (mouse::MInPr(30,100,200,119,0)) {
		FN = "mod\\IanEd\\maps\\blank.map";
		LoadLevel(FN.c_str());
		BoundRect.top = 0;
		BoundRect.left = 0;
		BoundRect.bottom = 511;
		BoundRect.right = 511;
		BoundType = false;BoundTest = false;
		dims.buttons[0]=0;
		dims.buttons[1]=0;
		dims.buttons[2]=0;
		GamePos=0;
	}
	if (mouse::MInPr(30,120,200,139,0)) {
		FN = "mod\\IanEd\\maps\\"+FilName;
		LoadLevel(FN.c_str());
		BoundRect.top = 0;
		BoundRect.left = 0;
		BoundRect.bottom = 511;
		BoundRect.right = 511;
		BoundType = false;BoundTest = false;
		dims.buttons[0]=0;
		dims.buttons[1]=0;
		dims.buttons[2]=0;
		GamePos=0;
	}
	if (mouse::MInPr(30,140,200,169,0)) {
		int x,y;
		int stream;
		FN = "mod\\IanEd\\maps\\"+FilName;
		
		stream = _open(FN.c_str(),_O_BINARY | _O_RDWR | _O_TRUNC | _O_CREAT, _S_IREAD | _S_IWRITE  );
		AddToLog(2,"Save> %s",FN.c_str());
		if (stream != -1) {

		for (x=0; x<128; x++)
		for (y=0; y<256; y++)
		{
			_write(stream,&MapInf->Map[x][y],2);
		}
		for (x=0; x<128; x++)
		for (y=0; y<256; y++)
		{
			_write(stream,&MapInf->Map2[x][y],2);
		}
		for (x=0; x<512; x++)
		for (y=0; y<512; y++)
		{
			_write(stream,&BlockDat[x][y],1);
		}

		FRMLocationMap::iterator iter;
		if (!StaticInf->Map.empty()) {
			iter = StaticInf->Map.begin();
			while (iter != StaticInf->Map.end()) {
				//int newloc = (DeCompLocX(iter->second->loc)<<10) + (DeCompLocY(iter->second->loc) ^ 511);
				_write(stream,&iter->second->loc,4);
				_write(stream,&iter->second->type,2);
				_write(stream,&iter->second->num,2);
                unsigned int opc; 
				if (iter->second->state!=0) {
					opc = 0xee; _write(stream,&opc,2);
					_write(stream,&iter->second->state,4);
				}
				if ((iter->first != 2000000000)&&(iter->first != iter->second->loc)) {
					opc = 0xffff; _write(stream,&opc,2);
					_write(stream,&iter->first,4);
				}
				if (iter->second->ItemDesc) {
					if (iter->second->ItemDesc->Count !=0) {
						opc = 0x11; _write(stream,&opc,2);
						_write(stream,&iter->second->ItemDesc->Count,4);
					}
					if (iter->second->ItemDesc->var1 !=0) {
						opc = 0x22; _write(stream,&opc,2);
						_write(stream,&iter->second->ItemDesc->var1,4);
					}
					if (iter->second->ItemDesc->var2 !=0) {
						opc = 0x33; _write(stream,&opc,2);
						_write(stream,&iter->second->ItemDesc->var2,4);
					}
					if (iter->second->ItemDesc->direction == true) {
						opc = 0x25; _write(stream,&opc,2);
						int tempor = 1;
						_write(stream,&tempor,4);
					}
					if (iter->second->ItemDesc->Inven)
					if (!iter->second->ItemDesc->Inven->Inven->Empty()) {
						opc = 0xAA; _write(stream,&opc,2);
						PInventoryItem Item;
						Item = PInventoryItem(iter->second->ItemDesc->Inven->Inven->First());
						while (Item != NULL) {
							_write(stream,&Item->num,2);
							_write(stream,&Item->count,4);
							Item = PInventoryItem(iter->second->ItemDesc->Inven->Inven->Next(Item));
						}
						int tempnum = 0;
						unsigned short temptype = 0;
						_write(stream,&temptype,2);
						_write(stream,&tempnum,4);
					}
				}
				opc=0xff; _write(stream,&opc,2);
				iter++;
			}
		}
		_close(stream);
		}
		dims.buttons[0]=0;
		dims.buttons[1]=0;
		dims.buttons[2]=0;
		GamePos=0;
	}

	BlitTo(g_pDDSBack,0,0,Mouse->FRM->x, Mouse->FRM->y,MousX,MousY,0,Mouse->FRM->FRM);

	// Flip the surfaces
    SDL_Flip(g_pDDSBack);
	
}
