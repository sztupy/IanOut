#ifndef __PLAY_H
#define __PLAY_H

#include "global.h"

namespace play {
   void EditTerep(void);
   void MainMenu(void);
   void ChooseTerep(void);
   void FileLoadSave(void);
   void EditObjects(void);
   void EditInventory(void);
   void EditCritters(void);
   extern PInventory InventorySave;
}

#endif __PLAY_H