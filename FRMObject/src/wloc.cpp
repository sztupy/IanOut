#include "sys\stat.h"
#include "wloc.h"
#include "../commonutils/utils.h"
#include "fcntl.h"
#include "io.h"

extern std::string TempDir;

void TMapLoader::SaveLocations()
{
	int x,y;
	int stream;
	std::string FN;
	FN = TempDir+fname;
		
	stream = _open(FN.c_str(),_O_BINARY | _O_RDWR | _O_TRUNC | _O_CREAT, _S_IREAD | _S_IWRITE  );
	AddToLog(4,"Save> Saving Location %s",FN.c_str());
	if (stream != -1) {

	for (x=0; x<128; x++)
	for (y=0; y<256; y++)
	{
		_write(stream,&(*MapInf)->Map[x][y],2);
	}
	for (x=0; x<128; x++)
	for (y=0; y<256; y++)
	{
		_write(stream,&(*MapInf)->Map2[x][y],2);
	}
	for (x=0; x<512; x++)
	for (y=0; y<512; y++)
	{
		_write(stream,&(*BlockDat)[x][y],1);
	}
	FRMLocationMap::iterator iter;
	if (!(*StaticInf)->Map.empty()) {
		iter = (*StaticInf)->Map.begin();
		while (iter != (*StaticInf)->Map.end()) {
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

	FN = TempDir+actorname;
	stream = _open(FN.c_str(),_O_BINARY | _O_RDWR | _O_TRUNC | _O_CREAT, _S_IREAD | _S_IWRITE  );
	AddToLog(4,"Save> Saving Location %s",FN.c_str());
	if (stream != -1) {

	CritterList::iterator iter;
	if (!(*CritterInf)->Critters.empty()) {
		iter = (*CritterInf)->Critters.begin();
		while (iter != (*CritterInf)->Critters.end()) {
			if (iter->first!=0) {
			PFRMPlayer Ian = iter->second;

			if (Ian->Hand1->number<=65536) {
				Ian->Inven->AddItem(Ian->Hand1->number,1,Ian->Hand1->numammo,(*StaticInf)->TilesI);
				Ian->ChangeWeapon(true,20000000,0,(*StaticInf)->TilesI);
			}
			if (Ian->Hand2->number<=65536) {
				Ian->Inven->AddItem(Ian->Hand2->number,1,Ian->Hand2->numammo,(*StaticInf)->TilesI);
				Ian->ChangeWeapon(false,20000000,0,(*StaticInf)->TilesI);
			}

			unsigned int convert = CompLoc(Ian->x,Ian->y);//(Ian->x)+(((Ian->y) ^ 511) << 10);

			_write(stream,&convert,4);
			_write(stream,&iter->first,2);
            _write(stream,&Ian->Player.proto,4);
			
            unsigned int opc; 
			if (Ian->Player.Count !=0) {
				opc = 0x11; _write(stream,&opc,2);
				_write(stream,&Ian->Player.Count,4);
			}
			if (Ian->Player.var1 !=0) {
				opc = 0x22; _write(stream,&opc,2);
				_write(stream,&Ian->Player.var1,4);
			}
			if (Ian->Player.var2 !=0) {
				opc = 0x33; _write(stream,&opc,2);
				_write(stream,&Ian->Player.var2,4);
			}
			if (Ian->Player.group !=0) {
				opc = 0x44; _write(stream,&opc,2);
				_write(stream,&Ian->Player.group,4);
			}
			if (Ian->Player.karma !=0) {
				opc = 0x55; _write(stream,&opc,2);
				_write(stream,&Ian->Player.karma,4);
			}
			if (Ian->Inven)
			if (!Ian->Inven->Inven->Empty()) {
				opc = 0xAA; _write(stream,&opc,2);
				PInventoryItem Item;
				Item = PInventoryItem(Ian->Inven->Inven->First());
				while (Item != NULL) {
					_write(stream,&Item->num,2);
					_write(stream,&Item->count,4);
					Item = PInventoryItem(Ian->Inven->Inven->Next(Item));
				}
				int tempnum = 0;
				unsigned short temptype = 0;
				_write(stream,&temptype,2);
				_write(stream,&tempnum,4);
			}
		opc=0xff; _write(stream,&opc,2);
		}
		iter++;
		}
	}
	_close(stream);
	}

}

TMapLoader::~TMapLoader()
{
	AddToLog(3,"Done> Freeing MapLoader data");
	if (*StaticInf) delete *StaticInf; *StaticInf = NULL;
	if (*CritterInf) delete *CritterInf; *CritterInf = NULL;
	if (*MapInf) delete *MapInf; *MapInf = NULL;
}

int TMapLoader::InitMainChar()
{
	AddToLog(2,"Load> Initialising Main Character (begin)");
	
	int hRet = 0;
	if (*CritterInf) delete *CritterInf;
	(*CritterInf) = new TIanCritter();

	if (*StaticInf) delete *StaticInf;
	(*StaticInf) = new TIanStatic();

	if ((*CritterInf) == NULL) return InitFail(0,"Error while initialising-1");
	if ((*StaticInf) == NULL) return InitFail(0,"Error while initialising-2");

	hRet = (*CritterInf)->LoadMainCharacter((*StaticInf)->TilesI);
	if (hRet != 0) return InitFail(0,"Error while initialising-3");
	AddToLog(2,"Load> Initialising Main Character (end)");
	return hRet;
}

int TMapLoader::LoadMap(char* filename, char* actorfile)
{
	SaveLocations();
	int hRet = 0;
	AddToLog(2,"Load> Loading a Map (begin) file: %s",filename);
	if (fname != filename) {
		fname = filename;

		if (*MapInf) delete MapInf;
		if (*StaticInf == NULL) return InitFail(0,"Error while accessing static data");
		(*StaticInf)->DeleteButTiles();

		AddToLog(3,"Load> Initialising map");
		(*MapInf) = new TIanMap();
		if ((*MapInf) == NULL) return InitFail(0,"Error while accessing map data");
		(*MapInf)->LoadMap(filename);
		(*MapInf)->LoadTiles();

		AddToLog(3,"Load> Loading Block data");
		int x,y;
		gzFile stream;
		if ((stream = __IOopen(filename,"rb")) == NULL)
			return InitFail(0,"LoadBlock FAILED");
		gzseek( stream, 256*256*2, SEEK_SET );
		for (x=0; x<512; x++)
			for (y=0; y<512; y++)
			{
				gzread(stream,&(*BlockDat)[x][y],1);
			}
		gzclose(stream);
		(*StaticInf) = new TIanStatic();
		if ((*StaticInf) == NULL) return InitFail(0,"LoadStaticMap Item Baj");
		(*StaticInf)->LoadStatic(filename);

	}
	if (actorfile != actorname) {
		actorname = actorfile;
		if (*CritterInf == NULL) return InitFail(0,"Error while accessing critter data");
		(*CritterInf)->ClearButMain();

		hRet = (*CritterInf)->LoadCritters(actorfile,(*StaticInf)->TilesI);
		if (hRet != 0) return InitFail(0,"Error while loding critters");

		PItem Item;
		PInventory Inven = (*CritterInf)->Critters.find(0)->second->Inven;
		Item = Inven->Inven->First();
		while (Item != NULL) {
			LoadNewItem((*StaticInf)->TilesI,PInventoryItem(Item)->num);
			Item = Inven->Inven->Next(Item);
		}
	}
	AddToLog(2,"Load> Loading a Map (end)");
	return 0;
}