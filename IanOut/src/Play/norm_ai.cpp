#include "normit.h"

void play::NextInTurn(void)
{
	CritterList::iterator iter = CritterInf->Critters.begin();
	CritterList::iterator iter2 = CritterInf->Critters.end();
	while (iter != CritterInf->Critters.end()) {
		if (iter->second == CurrentPlayer) {
			iter2 = iter;
			iter2++;
		}
		iter++;
	}
	if (iter2!=CritterInf->Critters.end()) {
		CurrentPlayer = iter2->second;
		ianskill::CountSk(&CurrentPlayer->PlayerSkill);
		if ((CurrentPlayer->Hand1->capacity==0) || (CurrentPlayer->Hand1->numammo==0)) {
		PItem Item = NULL,Item2 = NULL;
		Item = CurrentPlayer->Inven->Inven->First();
		while (Item!=NULL) {
			PWeapon Weapon = new TWeapon();
            Weapon->Load(PInventoryItem(Item)->num,StaticInf->TilesI);	
			if (Weapon->WeaponType==1) {
				if (CanOpen2(CurrentPlayer->GetGraphicNameWeapon("shoot",Weapon)) && (CurrentPlayer->Inven->CountItem(Weapon->ammotype,0)>0)) Item2=Item;
			}
			delete Weapon;
			Item = CurrentPlayer->Inven->Inven->Next(Item);
		}
		if (Item2!=NULL) {
			CurrentPlayer->ChangeWeapon(true,PInventoryItem(Item2)->num,PInventoryItem(Item2)->state,StaticInf->TilesI);
			delete CurrentPlayer->Inven->GetItem(PInventoryItem(Item2));
		} else {
			if (CurrentPlayer->Hand1->number<=65536) {
					CurrentPlayer->Inven->AddItem(CurrentPlayer->Hand1->number,1,CurrentPlayer->Hand1->numammo,StaticInf->TilesI);
				}
			CurrentPlayer->ChangeWeapon(true,20000000,0,StaticInf->TilesI);
		}
		}
	} else {
		CurrentPlayer = Ian;
		ianskill::CountSk(&CurrentPlayer->PlayerSkill);
		date::AddSecond(10);
	}

	if ((TerX+LocConvertX(CurrentPlayer->x,CurrentPlayer->y)*16<0) ||
	   (TerX+LocConvertX(CurrentPlayer->x,CurrentPlayer->y)*16>GetMaxX) ||
	   (TerY+LocConvertY(CurrentPlayer->x,CurrentPlayer->y)*12<0) ||
	   (TerY+LocConvertY(CurrentPlayer->x,CurrentPlayer->y)*12>GetMaxY)) {
			TerX = -LocConvertX(CurrentPlayer->x,CurrentPlayer->y)*16+GetMaxX/2;
			TerY = -LocConvertY(CurrentPlayer->x,CurrentPlayer->y)*12+(GetMaxY/2);
	   }
}

void play::AiMove(void)
{
	CritterList::iterator iter = CritterInf->Critters.begin();
	CritterList::iterator iter2 = CritterInf->Critters.end();
	while (iter != CritterInf->Critters.end()) {
		if (iter->second == CurrentPlayer) {
			iter2 = iter;
			break;
		}
		iter++;
	}
	if (iter2==CritterInf->Critters.end()) {
		NextInTurn();
	} else {
		if (CurrentPlayer->curth==0) {
			if (CurrentPlayer->PlayerSkill.secondary[1]<=0)
			{
				NextInTurn();
			} else {
				bool refill = false;
				CountVisible();
				PWeapon Weapon;
				if (CurrentPlayer->curhand) Weapon = CurrentPlayer->Hand1; else Weapon = CurrentPlayer->Hand2;
				if ((CurrentPlayer->visibletype==60) || (CurrentPlayer->Distance(Ian->x,Ian->y)> ((Weapon->GetCurrentMode()) ? Weapon->GetCurrentMode()->maxr : 1)))
				{
					CurrentPlayer->GoAround(Ian->x,Ian->y,BlDat);
				} else {
					if (Weapon->GetCurrentMode())
					if (!((Weapon->capacity!=0) && (Weapon->numammo==0))) {
						if (CurrentPlayer->PlayerSkill.secondary[1]<=Weapon->GetCurrentMode()->apuse-1) {
							} else {
								CurrentPlayer->Attack(Ian,Weapon);
								CurrentPlayer->PlayerSkill.secondary[1]-=Weapon->GetCurrentMode()->apuse;
							}
					} else {
						PWeaponMode PModeFirst = Weapon->GetCurrentMode();
						Weapon->NextMode();
						while (Weapon->GetCurrentMode() != PModeFirst) {
							if (Weapon->GetCurrentMode()->mode == 10) {
								int numleft = CurrentPlayer->Inven->CountItem(Weapon->ammotype,0);
								if (numleft==0) textutil::AddString(CurrentPlayer->Player.CharName+" run out of ammo",1);
								int need = Weapon->capacity-Weapon->numammo;
								if ((need!=0) && (numleft>0))
								if (CurrentPlayer->PlayerSkill.secondary[1]<=Weapon->GetCurrentMode()->apuse-1) {
								} else {
									CurrentPlayer->PlayerSkill.secondary[1]-=Weapon->GetCurrentMode()->apuse;
									if (numleft>=need) {
										Weapon->numammo = Weapon->capacity;
										refill = true;
										textutil::AddString(CurrentPlayer->Player.CharName+" reloaded",1);
										delete CurrentPlayer->Inven->GetItem(Weapon->ammotype,0,need);
									} else {
										refill = true;
										Weapon->numammo = Weapon->numammo+numleft;
										textutil::AddString(CurrentPlayer->Player.CharName+" reloaded",1);
										delete CurrentPlayer->Inven->GetItem(Weapon->ammotype,0,numleft);
									}
								}
							}
							Weapon->NextMode();
						}
					}
				}
				if (CurrentPlayer->curth==0)
				{
					if (!refill) NextInTurn();
				}
			}
		} else if (CurrentPlayer->curth<=2) {
			CountVisible();
			PWeapon Weapon;
			if (CurrentPlayer->curhand) Weapon = CurrentPlayer->Hand1; else Weapon = CurrentPlayer->Hand2;
				
			if ((CurrentPlayer->visibletype==60) || (CurrentPlayer->Distance(Ian->x,Ian->y)> ((Weapon->GetCurrentMode()) ? Weapon->GetCurrentMode()->maxr : 1)))
				{ } else { CurrentPlayer->StopMove = true; }
		}
	}
}