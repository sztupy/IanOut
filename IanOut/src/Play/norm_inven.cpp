#include "normit.h"

void play::InventoryMode(void)
{
	static int page1;
	static int source;
	static unsigned int num;
	static unsigned int state;

	int i,i2;
	int max1;
	if ((lstrcmpi(Temporary[1]->fname,"\\art\\intrface\\invbox.frm")))
	{
		LoadFRMSingle(&Temporary[1],hWnd,"\\art\\intrface\\invbox.frm",1);
	}
	BlitTo(g_pDDSBack,0,0,Temporary[1]->FRM->x,Temporary[1]->FRM->y,
						addx+70,
						addy+8,
						DDBLTFAST_SRCCOLORKEY,Temporary[1]->FRM->FRM);


    if (mouse::MInPr(addx+70+152,addy+8+283,addx+70+242,addy+8+348,0))
	{
		if (Ian->Hand1->HasGraphic) {
			source = 2;
			mousetyp=25;
			FRMMouse = Ian->Hand1->Picture->GetCurFrame();
		}
	}
	if (mouse::MInPr(addx+70+246,addy+8+283,addx+70+337,addy+8+348,0))
	{
		if (Ian->Hand2->HasGraphic) {
			source = 3;
			mousetyp=25;
			FRMMouse = Ian->Hand2->Picture->GetCurFrame();
		}
	}

	if (mousetyp==25)
	if ((olddims.rgbButtons[0] & 0x80) && !(dims.rgbButtons[0] & 0x80)) {
		if (mouse::MouseIn(addx+70+152,addy+8+283,addx+70+242,addy+8+348)) {
			if (source == 1) {
				if (Ian->Hand1->ammotype==num) {
					int numleft = Ian->Inven->CountItem(Ian->Hand1->ammotype,0);
					int need = Ian->Hand1->capacity-Ian->Hand1->numammo;
					if ((need!=0) && (numleft>0)) {
						if (numleft>=need) {
							Ian->Hand1->numammo = Ian->Hand1->capacity;
							delete Ian->Inven->GetItem(Ian->Hand1->ammotype,0,need);
						} else {
							Ian->Hand1->numammo = Ian->Hand1->numammo+numleft;
							delete Ian->Inven->GetItem(Ian->Hand1->ammotype,0,numleft);
						}
					}
				} else {
					if (Ian->Hand1->number<=65536) {
						Ian->Inven->AddItem(Ian->Hand1->number,1,Ian->Hand1->numammo,StaticInf->TilesI);
					}
					Ian->ChangeWeapon(hWnd,g_pDD,true,num,state,StaticInf->TilesI);
					delete Ian->Inven->GetItem(num,state,1);
				}
			}
			if (source == 3) {
				if (Ian->Hand1->number<=65536) {
					Ian->Inven->AddItem(Ian->Hand1->number,1,Ian->Hand1->numammo,StaticInf->TilesI);
				}
				Ian->ChangeWeapon(hWnd,g_pDD,true,Ian->Hand2->number,Ian->Hand2->numammo,StaticInf->TilesI);
				Ian->ChangeWeapon(hWnd,g_pDD,false,20000000,0,StaticInf->TilesI);
			}
		}
		if (mouse::MouseIn(addx+70+246,addy+8+283,addx+70+337,addy+8+348)) {
			if (source == 1) {
				if (Ian->Hand2->ammotype==num) {
					int numleft = Ian->Inven->CountItem(Ian->Hand2->ammotype,0);
					int need = Ian->Hand2->capacity-Ian->Hand2->numammo;
					if ((need!=0) && (numleft>0)) {
						if (numleft>=need) {
							Ian->Hand2->numammo = Ian->Hand2->capacity;
							delete Ian->Inven->GetItem(Ian->Hand2->ammotype,0,need);
						} else {
							Ian->Hand2->numammo = Ian->Hand2->numammo+numleft;
							delete Ian->Inven->GetItem(Ian->Hand2->ammotype,0,numleft);
						}
					}
				} else {
					if (Ian->Hand2->number<=65536) {
						Ian->Inven->AddItem(Ian->Hand2->number,1,Ian->Hand2->numammo,StaticInf->TilesI);
					}
					Ian->ChangeWeapon(hWnd,g_pDD,false,num,state,StaticInf->TilesI);
					delete Ian->Inven->GetItem(num,state,1);
				}
			}
			if (source == 2) {
				if (Ian->Hand2->number<=65536) {
					Ian->Inven->AddItem(Ian->Hand2->number,1,Ian->Hand2->numammo,StaticInf->TilesI);
				}
				Ian->ChangeWeapon(hWnd,g_pDD,false,Ian->Hand1->number,Ian->Hand1->numammo,StaticInf->TilesI);
				Ian->ChangeWeapon(hWnd,g_pDD,true,20000000,0,StaticInf->TilesI);
			}
		}
		if (mouse::MouseIn(addx+70+46,addy+8+38,addx+70+110,addy+8+336)) {
			if (source == 2) {
				if (Ian->Hand1->number<=65536) {
					Ian->Inven->AddItem(Ian->Hand1->number,1,Ian->Hand1->numammo,StaticInf->TilesI);
				}
				Ian->ChangeWeapon(hWnd,g_pDD,true,20000000,0,StaticInf->TilesI);
			}
			if (source == 3) {
				if (Ian->Hand2->number<=65536) {
					Ian->Inven->AddItem(Ian->Hand2->number,1,Ian->Hand2->numammo,StaticInf->TilesI);
				}
				Ian->ChangeWeapon(hWnd,g_pDD,false,20000000,0,StaticInf->TilesI);
			}
		
		}
		mousetyp = 9;
		FRMMouse = NULL;
		source = 0;
	}

	PItem Item;
	i = 0; i2=0;
	Item = Ian->Inven->Inven->First();

	PItem ItemDelSave = NULL;

	while (Item!=NULL) {
		if ((i2>=page1) && (i2<page1+5)) {
			PFRM frm = ((PInventoryItem)Item)->FRMA->GetCurFrame();
			if ((frm->x>50) || (frm->y>50)) {
				double d = ((double)frm->y/(double)frm->x*50);
				BlitToStretch(g_pDDSBack,0,0,frm->x,frm->y,addx+70+50,addy+8+70+i*55-d/2,addx+70+50+50,addy+8+70+i*55+d/2,DDBLTFAST_SRCCOLORKEY,frm->FRM);
			} else {
				BlitTo(g_pDDSBack,0,0,frm->x,frm->y,addx+70+50,addy+8+45+i*55,DDBLTFAST_SRCCOLORKEY,frm->FRM);
			}
			int count = ((PInventoryItem)Item)->count;
			if (count>1) {
				char buf[20];
				wsprintf(buf,"x%i",count);
				textfont::IanOutTextR(addx+170,addy+95+i*55,0,buf);
			}
			if (mouse::MInPr(addx+70+50,addy+8+45+i*55,addx+170,addy+8+95+i*55,0)) {
				FRMMouse = frm;
				num = PInventoryItem(Item)->num;
				state = PInventoryItem(Item)->state;
				mousetyp = 25;
				source = 1;
			}

			if (mouse::MInPr(addx+70+50,addy+8+45+i*55,addx+170,addy+8+95+i*55,1)) {
				if (mousetyp==9)
					{
						PWeapon Weapon = new TWeapon();
						Weapon->Load(PInventoryItem(Item)->num,StaticInf->TilesI);
						std::string xxx = "0000001011";
						ability = std::bitset<10>(xxx);
						if ((Weapon->WeaponType==1) && (PInventoryItem(Item)->state !=0)) ability[8] = true;
						if ((Weapon->WeaponType==2) && (Weapon->GetCurrentMode()) && (Weapon->GetCurrentMode()->mode==1)) ability[9] = true;

						delete Weapon;
						mousetyp=19;
						menet=95;
					}
			} else if (mouse::MInPr(addx+70+50,addy+8+45+i*55,addx+170,addy+8+95+i*55,4)) {
				if (mousetyp!=9) {
					switch (mousetyp) {
						case 11: {
							int x = Ian->x;
							int y = Ian->y;
							if (PInventoryItem(Item)->state != 0) {
								PWeapon Weapon = new TWeapon();
								Weapon->Load(PInventoryItem(Item)->num,StaticInf->TilesI);
																
								PLocationDat Loc = new TLocationDat();
								Loc->num=Weapon->ammotype;
								Loc->type=2;
								Loc->loc=CompLoc(x,y);
								Loc->state = PInventoryItem(Item)->state-1;
								Loc->FRM = StaticInf->TilesI[Loc->num]->FRM;
								Loc->FRMA = new TFRMAnimCommunicator(StaticInf->TilesI[Loc->num]->FRMA);
								StaticInf->Map.insert( Location_Pair (2000000000,Loc) );
					 
								delete Weapon;
							}
							PLocationDat Loc = new TLocationDat();
							Loc->num=PInventoryItem(Item)->num;
							Loc->type=2;
							Loc->loc=CompLoc(x,y);
							Loc->state = PInventoryItem(Item)->count-1;
							Loc->FRM = StaticInf->TilesI[Loc->num]->FRM;
							Loc->FRMA = new TFRMAnimCommunicator(StaticInf->TilesI[Loc->num]->FRMA);
							StaticInf->Map.insert( Location_Pair (2000000000,Loc) );
							ItemDelSave = Item;
							break;
						}
						case 13: {
							char buf[20];
							wsprintf(buf,"ITEMS_%i",PInventoryItem(Item)->num);
							std::string xXx;
							xXx = "\\data\\items\\"+textutil::GetFromProf(GetFile("\\proto\\items.pro"),"ITEMS",buf);
							textutil::AddString(textutil::GetFromXML(GetFile(xXx),"/text.lookat"),1);
							break;
						}
						case 18: {
							if (PInventoryItem(Item)->state != 0) {
								PWeapon Weapon = new TWeapon();
								Weapon->Load(PInventoryItem(Item)->num,StaticInf->TilesI);
								Ian->Inven->AddItem(Weapon->ammotype,PInventoryItem(Item)->state,0,StaticInf->TilesI);
								delete Weapon;
							}
							break;
						}
						case 19: {
							PWeapon Weapon = new TWeapon();
							Weapon->Load(PInventoryItem(Item)->num,StaticInf->TilesI);
							////////////////////////////////////////////////////////////////////////////////////////////////////////
							//std::string xXx = textutil::GetFromXML(Weapon->deflocation,"/"+Weapon->GetCurrentMode()->name,"SCRIPTCHR");
							std::string xXx = textutil::GetFromXML(Weapon->deflocation,"/desc/onuse/script:");
							if (xXx!="") {
								Ian->UseCommand(hWnd,g_pDD,Ian,true,BlDat,true,xXx);
							}
							delete Weapon;
							break;
						}
					}
				}
				mousetyp = 9;
			}
			i++;
		}
		Item = Ian->Inven->Inven->Next(Item);
		i2++;
	}
	if (ItemDelSave) delete Ian->Inven->GetItem(PInventoryItem(ItemDelSave));
    max1 = i2;
	int bdown = 0;
	bdown = ButtonList->GetButton(MousX,MousY,mouse::Pressed(6));

	Ian->Draw(g_pDDSBack,addx+260,addy+115,1002);

	BlitTo(g_pDDSBack, 0,0,640,99,addx,GetMaxY-99,0,KezeloP->FRM->FRM);
	Ian->Hand1->DrawOutSmall(g_pDDSBack,addx+75+152,addy+8+288,80);
	Ian->Hand2->DrawOutSmall(g_pDDSBack,addx+75+245,addy+8+288,80);
	textutil::DrawStuff();
	ButtonList->DrawButtons(g_pDDSBack,MousX,MousY,mouse::Pressed(6));
	DrawIanCucc(mouse::MouseIn(addx+266,GetMaxY-99+27,addx+266+188,GetMaxY-99+27+67) && (mouse::Pressed(6) || mouse::Pressed(7)));
	if ((mousetyp==0) || (mousetyp==9)) BlitTo(g_pDDSBack,0,0,Mouse->FRM->x, Mouse->FRM->y,MousX,MousY,DDBLTFAST_SRCCOLORKEY,Mouse->FRM->FRM);

	if (mousetyp==25) BlitTo(g_pDDSBack,0,0,FRMMouse->x,FRMMouse->y,MousX-FRMMouse->x/2,MousY-FRMMouse->y/2,DDBLTFAST_SRCCOLORKEY,FRMMouse->FRM);

	if (bdown == 2)
		{
			InGamePos = 0;
			mousetyp = 1;
			if (TurnBasedMode)
				buttons::ChangeButtonList(ButtonList,3); else
				buttons::ChangeButtonList(ButtonList,2);
		}
	if (bdown == 3) page1--;
	if (bdown == 4) page1++;

	if (page1<0) page1=0;
	if ((page1+5)>max1) page1=max1-5;
	
}

