#include "items.h"
#include "io.h"
#include <fcntl.h>
#include "../commonutils/ddutil.h"
#include "../commonutils/discutil.h"
#include "../commonutils/utils.h"
#include "math.h"

void TInventory::AddItem(TInventoryItem Item)
{
 PItem Item2;
 Item2 = Inven->First();
 while (Item2 != NULL) {
	 if ((((PInventoryItem)Item2)->num == Item.num) && 
		 (((PInventoryItem)Item2)->state == Item.state) &&
		 (Item.state==0))
		{
		 ((PInventoryItem)Item2)->count+=Item.count;
		 return;
		}
	 Item2 = Inven->Next(Item2);
 }
 if (Item2 == NULL) {
	 PInventoryItem Item3 = new TInventoryItem();
	 Item3->num = Item.num;
	 Item3->state = Item.state;
	 Item3->count = Item.count;
	 Item3->FRMA = Item.FRMA;
	 Inven->Insert(Item3);
 }
}

void TInventory::AddItem(PInventoryItem Item)
{
 PItem Item2;
 Item2 = Inven->First();
 while (Item2 != NULL) {
	 if ((((PInventoryItem)Item2)->num == Item->num) && 
		 (((PInventoryItem)Item2)->state == Item->state) &&
		 (Item->state==0))
		{
		 ((PInventoryItem)Item2)->count+=Item->count;
		 delete Item;
		 return;
		}
	 Item2 = Inven->Next(Item2);
 }
 if (Item2 == NULL) {
	 Inven->Insert(Item);
 }
}

void TInventory::AddItem(unsigned short num, unsigned int count, unsigned int state, FRMPairCollection &TilesI)
{
 PItem Item2;
 Item2 = Inven->First();
 while (Item2 != NULL) {
	 if ((((PInventoryItem)Item2)->num == num) && 
		 (((PInventoryItem)Item2)->state == state) &&
		 (state==0))
		{
		 ((PInventoryItem)Item2)->count+=count;
		 break;
		}
	 Item2 = Inven->Next(Item2);
 }
 if (Item2 == NULL) {
	 PInventoryItem Item3 = new TInventoryItem();
	 Item3->num = num;
	 Item3->state = state;
	 Item3->count = count;
	 Item3->FRMA = TilesI[num]->FRMA;
	 Inven->Insert(Item3);
 }
}

unsigned int TInventory::CountItem(unsigned short num, unsigned int state)
{
	PItem Item2;
	Item2 = Inven->First();
	while (Item2 != NULL) {
	if ((PInventoryItem(Item2)->num == num) && 
		(PInventoryItem(Item2)->state == state))
		{
			return ((PInventoryItem)Item2)->count;
		}	
	Item2 = Inven->Next(Item2);
	}
	return 0;
}

PInventoryItem TInventory::GetItem(unsigned short num, unsigned int state)
{
	PItem Item2,Item3;
	Item2 = Inven->First();
	while (Item2 != NULL) {
		Item3 = Item2;
		Item2 = Inven->Next(Item2);
		if ((PInventoryItem(Item3)->num == num) && 
			(PInventoryItem(Item3)->state == state))
			{
				Inven->Disconnect(Item3);
				return PInventoryItem(Item3);
			}	
		
	}
	return NULL;
}

PInventoryItem TInventory::GetItem(unsigned short num, unsigned int state, unsigned int count)
{
	PItem Item2,Item3;
	Item2 = Inven->First();
	while (Item2 != NULL) {
		Item3 = Item2;
		Item2 = Inven->Next(Item2);
		if ((PInventoryItem(Item3)->num == num) && 
			(PInventoryItem(Item3)->state == state) &&
			(state == 0)) 
			{
				if (count >= CountItem(num,state)) {
					Inven->Disconnect(Item3);
					return PInventoryItem(Item3);
				} else {
					PInventoryItem(Item3)->count-=count;
					PInventoryItem Item = new TInventoryItem();
					Item->count = count;
					Item->num = num;
					Item->state = state;
					return Item;
				}
			}
		if ((PInventoryItem(Item3)->num == num) && 
			(PInventoryItem(Item3)->state == state))
		{
			Inven->Disconnect(Item3);
			return PInventoryItem(Item3);
		}
	}
	return NULL;
}

PInventoryItem TInventory::GetItem(PInventoryItem Item)
{
	PItem Item2,Item3;
	Item2 = Inven->First();
	while (Item2 != NULL) {
		Item3 = Item2;
		Item2 = Inven->Next(Item2);
		if ((PInventoryItem(Item3) == Item))
			{
				Inven->Disconnect(Item3);
				return PInventoryItem(Item3);
			}	
		
	}
	return NULL;
}