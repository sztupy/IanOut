#include "normit.h"

int movenum;

namespace play {
	void MoveMult(void);
}

void play::MoveMult(void)
{
	if ((strcmp(Temporary[2]->fname,"\\art\\intrface\\movemult.frm")))
	{
		LoadFRMSingle(&Temporary[2],"\\art\\intrface\\movemult.frm",1);
	}
	BlitTo(g_pDDSBack,0,0,Temporary[2]->FRM->x,Temporary[2]->FRM->y,
						addx+190,
						addy+109,
						0,Temporary[2]->FRM->FRM);
}

void play::LootMode(void)
{
	static int page1;
	static int page2;
	int max1,max2;
	int i,i2;
	PInventory IanInventory = Ian->Inven;
	PInventory ItemInventory = IanOs::ActualItem->ItemDesc->Inven;
	if ((strcmp(Temporary[1]->fname,"\\art\\intrface\\loot.frm")))
	{
		LoadFRMSingle(&Temporary[1],"\\art\\intrface\\loot.frm",1);
	}
	BlitTo(g_pDDSBack,0,0,Temporary[1]->FRM->x,Temporary[1]->FRM->y,
						addx+51,
						addy+4,
						0,Temporary[1]->FRM->FRM);

	if (mousetyp==25)
	if ((olddims.buttons[0]) && !(dims.buttons[0])) {
		mousetyp = 9;
		if (mouse::MouseIn(addx+50+180,addy+35+4,addx+280,addy+4+340)) {
			if (InvenT != IanInventory) {
				//if (InvenTItem->count == 1) {
					IanInventory->AddItem(InvenT->GetItem(InvenTItem));
				//}
			}
		}
		if (mouse::MouseIn(addx+50+295,addy+35+4,addx+295+100,addy+4+340)) {
			if (InvenT != ItemInventory) {
				//if (InvenTItem->count == 1) {
					ItemInventory->AddItem(InvenT->GetItem(InvenTItem));
				//}
			}
		}
		FRMMouse = NULL;
		InvenT = NULL;
		InvenTItem = NULL;
	}
	PItem Item;
	i = 0; i2=0;
	Item = IanInventory->Inven->First();
	while (Item!=NULL) {
		if ((i2>=page1) && (i2<page1+5)) {
			PFRM frm = ((PInventoryItem)Item)->FRMA->GetCurFrame();
			if ((frm->x>50) || (frm->y>50)) {
				double d = ((double)frm->y/(double)frm->x*50);
				BlitToStretch(g_pDDSBack,0,0,frm->x,frm->y,addx+230,addy+8+70+i*55-d/2,addx+230+50,addy+8+70+i*55+d/2,0,frm->FRM);
			} else {
				BlitTo(g_pDDSBack,0,0,frm->x,frm->y,addx+230,addy+8+45+i*55,0,frm->FRM);
			}
			int count = ((PInventoryItem)Item)->count;
			if (count!=1) {
				char buf[20];
				sprintf(buf,"x%i",count);
				textfont::IanOutTextR(addx+280,addy+95+i*55,0,buf);
			}
			if (mouse::MInPr(addx+230,addy+8+45+i*55,addx+230+50,addy+8+95+i*55,0)) {
				InvenT = IanInventory;
				InvenTItem = PInventoryItem(Item);
				FRMMouse = frm;
				mousetyp = 25;
			}
			i++;
		}
		Item = IanInventory->Inven->Next(Item);
		i2++;
	}
	max1=i2;
	    
//	PItem Item;
	i = 0;i2 = 0;
	Item = ItemInventory->Inven->First();
	while (Item!=NULL) {
		if ((i2>=page2) && (i2<page2+5)) {
			PFRM frm = ((PInventoryItem)Item)->FRMA->GetCurFrame();
			if ((frm->x>50) || (frm->y>50)) {
				double d = ((double)frm->y/(double)frm->x*50);
				BlitToStretch(g_pDDSBack,0,0,frm->x,frm->y,addx+350,addy+8+70+i*55-d/2,addx+400,addy+8+70+i*55+d/2,0,frm->FRM);
			} else {
				BlitTo(g_pDDSBack,0,0,frm->x,frm->y,addx+350,addy+8+45+i*55,0,frm->FRM);
			}
			int count = ((PInventoryItem)Item)->count;
			if (count!=1) {
				char buf[20];
				sprintf(buf,"x%i",count);
				textfont::IanOutTextR(addx+400,addy+95+i*55,0,buf);
			}
			if (mouse::MInPr(addx+350,addy+8+45+i*55,addx+400,addy+8+95+i*55,0)) {
				FRMMouse = frm;
				InvenT = ItemInventory;
				InvenTItem = PInventoryItem(Item);
				mousetyp = 25;
			}
			i++;
		}
		Item = ItemInventory->Inven->Next(Item);
		i2++;
	}
	max2 = i2;

	int bdown = 0;
	bdown = ButtonList->GetButton(MousX,MousY,mouse::Pressed(6));

	if (bdown == 1) {
		page1 = 0;
		page2 = 0;
		Item = ItemInventory->Inven->First();
		PItem Item2;
		while (Item!=NULL) {
			Item2 = Item;
			Item = ItemInventory->Inven->Next(Item);
			IanInventory->AddItem(
				 ItemInventory->GetItem(PInventoryItem(Item2)));
		}
	}
	if (bdown == 2) {
		page1 = 0;
		page2 = 0;
		InGamePos = 0;mousetyp = 1;
		if (TurnBasedMode)
			buttons::ChangeButtonList(ButtonList,3); else
			buttons::ChangeButtonList(ButtonList,2);
	}
	if (bdown == 3) page1--;
	if (bdown == 4) page1++;
	if (bdown == 5) page2--;
	if (bdown == 6) page2++;

	if ((page1+5)>max1) page1=max1-5;
	if ((page2+5)>max2) page2=max2-5;
	if (page1<0) page1=0;
	if (page2<0) page2=0;
	Ian->Draw(g_pDDSBack,addx+115,addy+115,1002);

	BlitTo(g_pDDSBack, 0,0,640,99,addx,GetMaxY-99,0,KezeloP->FRM->FRM);
	textutil::DrawStuff();
	ButtonList->DrawButtons(g_pDDSBack,MousX,MousY,mouse::Pressed(6));
	DrawIanCucc(mouse::MouseIn(addx+266,GetMaxY-99+27,addx+266+188,GetMaxY-99+27+67) && (mouse::Pressed(6) || mouse::Pressed(7)));
	if ((mousetyp==0) || (mousetyp==9)) BlitTo(g_pDDSBack,0,0,Mouse->FRM->x, Mouse->FRM->y,MousX,MousY,0,Mouse->FRM->FRM);
	if (mousetyp==25) BlitTo(g_pDDSBack,0,0,FRMMouse->x,FRMMouse->y,MousX-FRMMouse->x/2,MousY-FRMMouse->y/2,0,FRMMouse->FRM);
}