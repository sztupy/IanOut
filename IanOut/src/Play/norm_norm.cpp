#include "normit.h"

void play::NormalMode(void)
{
	if ((TurnBasedMode) && (ButtonList->actualmode != 3)) buttons::ChangeButtonList(ButtonList,3);
	if ((!TurnBasedMode) && (ButtonList->actualmode != 2)) buttons::ChangeButtonList(ButtonList,2);
	if ((TurnBasedMode) && (Ian->PlayerSkill.secondary[1]<=0) && (Ian->curth == 0)) NextInTurn();
	bool usedItem = false;
	if (mouse::MouseIn(0,0,GetMaxX,GetMaxY-99))
	{

	if (mouse::MouseIn(0,0,GetMaxX,GetMaxY-101) && (mousetyp==21) && (mouse::Pressed(3))) {
		CritterList::iterator iter2;
		iter2 = CritterInf->Critters.begin();
		usedItem = false;
		while (iter2 !=CritterInf->Critters.end()) {
			PFRM frm;
			iter2->second->Idle->SetDir(iter2->second->dir);
			frm = iter2->second->Idle->GetCurFrame();
			int cx = TerX+LocConvertX(iter2->second->x,iter2->second->y)*16+frm->bx;
			int cy = TerY+LocConvertY(iter2->second->x,iter2->second->y)*12+frm->by;
			if (mouse::MouseIn(cx,cy,cx+frm->x,cy+frm->y)) {
				PWeapon Weapon;
				if (Ian->curhand) Weapon = Ian->Hand1; else Weapon = Ian->Hand2;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//std::string xXx = textutil::GetFromProf(Weapon->deflocation,Weapon->GetCurrentMode()->name,"SCRIPTCHR");
				std::string xXx = textutil::GetFromXML(Weapon->deflocation,"/desc/onuse/script:");
				if (xXx!="") {
					if ((TurnBasedMode && (Ian->PlayerSkill.secondary[1]<=Weapon->GetCurrentMode()->apuse-1))) {
						textutil::AddString("Not enough Action Points",1);
					} else {
						if (!usedItem) if (TurnBasedMode) Ian->PlayerSkill.secondary[1]-=Weapon->GetCurrentMode()->apuse;
						usedItem = true;
						Ian->UseCommand(hWnd,g_pDD,iter2->second,true,BlDat,true,xXx);
					}
				}
			}
			iter2++;
		}
		mousetyp=9;
	}
	if (mouse::MouseIn(0,0,GetMaxX,GetMaxY-101) && (mousetyp==22) && (mouse::Pressed(3))) {
		CritterList::iterator iter2;
		iter2 = CritterInf->Critters.begin();
		usedItem = false;
		CountVisible();
		while (iter2 !=CritterInf->Critters.end()) {
			if (iter2->first != 0) {
			PFRM frm;
			iter2->second->Idle->SetDir(iter2->second->dir);
			frm = iter2->second->Idle->GetCurFrame();
			int cx = TerX+LocConvertX(iter2->second->x,iter2->second->y)*16+frm->bx;
			int cy = TerY+LocConvertY(iter2->second->x,iter2->second->y)*12+frm->by;
			if (mouse::MouseIn(cx,cy,cx+frm->x,cy+frm->y)) {
				
				PWeapon Weapon;
				if (Ian->curhand) Weapon = Ian->Hand1; else Weapon = Ian->Hand2;
				if (Weapon->GetCurrentMode()) {
					if (!((Weapon->capacity!=0) && (Weapon->numammo==0))) {
					if (Ian->Distance(iter2->second->x,iter2->second->y)<=Weapon->GetCurrentMode()->maxr) {
					if (iter2->second->visibletype!=60)	{
						if ((TurnBasedMode && (Ian->PlayerSkill.secondary[1]<=Weapon->GetCurrentMode()->apuse-1))) {
							textutil::AddString("Not enough Action Points",1);
						} else {
							Ian->Attack(hWnd,g_pDD,iter2->second,Weapon);
							if (!usedItem) if (TurnBasedMode) Ian->PlayerSkill.secondary[1]-=Weapon->GetCurrentMode()->apuse;
							usedItem = true;
						}
					} else {
						textutil::AddString("Shot is blocked",1);
					}
					} else {
						textutil::AddString("Out of range",1);
					}
					} else {
						textutil::AddString("Out of ammo",1);
					}
				}
			}
			}
			iter2++;
		}
		
		//mousetyp=9;
	}

	if (mouse::MouseIn(0,0,GetMaxX,GetMaxY-101) && (mousetyp==9) && (dims.rgbButtons[0] & 0x80) && (Frame>1)) 
	{
		menet=0;
		mousetyp = 10;
		itersave = StaticInf->Map.end();
		iter2save = CritterInf->Critters.end();
		FRMLocationMap::iterator iter;
		iter = StaticInf->Map.begin();
		while (iter != StaticInf->Map.end()) {
			PFRM frm;
			int iy = DeCompLocY(iter->second->loc);
			int ix = DeCompLocX(iter->second->loc);
			if (iter->second->FRM != NULL) frm = iter->second->FRM->FRM;
			else frm = iter->second->FRMA->GetCurFrame();
			int cx = TerX+LocConvertX(ix,iy)*16+frm->bx;
			int cy = TerY+LocConvertY(ix,iy)*12+frm->by;
			if (mouse::MouseIn(cx,cy,cx+frm->x,cy+frm->y)) {
			switch (iter->second->type) {
				case 1: {
					std::string xxx = "0000001001";
					ability = std::bitset<10>(xxx);
					mousetyp = 13;menet = 35;
					itersave = iter;
			 		break;
					}
				case 2: {
					std::string xxx = "1000001001";
					ability = std::bitset<10>(xxx);
					mousetyp = 19;menet = 95;
					itersave = iter;
					break;
					}
				case 3: {
					std::string xxx = iter->second->ItemDesc->ability;
					if (xxx.length()!=10) xxx="0000001001";
					ability = std::bitset<10>(xxx);
					mousetyp = 19;menet = 95;
					itersave = iter;
					break;
					}
				case 5: {
					std::string xxx = iter->second->ItemDesc->ability;
					if (xxx.length()!=10) xxx="0000001001";
					ability = std::bitset<10>(xxx);
					mousetyp = 19;menet = 95;
					itersave = iter;
					break;
					}
			}
			}
			iter++;
		}

			CritterList::iterator iter2;
			iter2 = CritterInf->Critters.begin();
			while (iter2 !=CritterInf->Critters.end()) {
				PFRM frm;
				iter2->second->Idle->SetDir(iter2->second->dir);
				frm = iter2->second->Idle->GetCurFrame();
				int cx = TerX+LocConvertX(iter2->second->x,iter2->second->y)*16+frm->bx;
				int cy = TerY+LocConvertY(iter2->second->x,iter2->second->y)*12+frm->by;
				if (mouse::MouseIn(cx,cy,cx+frm->x,cy+frm->y)) {
					std::string xxx;
					if (iter2->first == 0) {
						if (menet==0) {
							mousetyp = 15;menet = 55;
							xxx = "0001101101"; 
							ability = std::bitset<10>(xxx);
							iter2save = iter2;
						}
					} else {
						mousetyp = 17;menet = 75;
						if (!TurnBasedMode)
							xxx = "0011011101"; 
						else
							xxx = "0001001101"; 
						ability = std::bitset<10>(xxx);
						iter2save = iter2;
					}
				}
				iter2++;
			}

			if (menet==0) mousetyp = 9;
	}

		if ((mousetyp==9) && (menet == 9)) {
			switch (itersave->second->type) {
				case 2: {
						if ((TurnBasedMode && (Ian->PlayerSkill.secondary[1]<=1))) {
							textutil::AddString("Not enough Action Points",1);
						} else {
							if (TurnBasedMode) Ian->PlayerSkill.secondary[1]-=2;
							Ian->Pickup(hWnd,g_pDD,itersave->second,BlDat);
						}
					break;
				}
				case 3: {
					if (ability[9]) {
						std::string xXx = itersave->second->ItemDesc->desclocation;
						bool type = atoi2(textutil::GetFromXML(xXx,"/desc/onuse.type").c_str()) != 0;
							//GetPrivateProfileInt("ONUSE","TYPE",0,xXx.c_str()) != 0;
						int directionX = atoi2(textutil::GetFromXML(xXx,"/desc/onuse.dirx").c_str());
							//GetPrivateProfileInt("ONUSE","DIRX",0,xXx.c_str());
						int directionY = atoi2(textutil::GetFromXML(xXx,"/desc/onuse.diry").c_str());
							//GetPrivateProfileInt("ONUSE","DIRY",0,xXx.c_str());
						if ((TurnBasedMode && (Ian->PlayerSkill.secondary[1]<=1))) {
							textutil::AddString("Not enough Action Points",1);
						} else {
							if (TurnBasedMode) Ian->PlayerSkill.secondary[1]-=2;
							Ian->UseOn(hWnd,g_pDD,itersave->second,BlDat,type,directionX,directionY);
						}
						break;
					} }
				case 5: {
					if (ability[9]) {
						if ((TurnBasedMode && (Ian->PlayerSkill.secondary[1]<=1))) {
							textutil::AddString("Not enough Action Points",1);
						} else {
							if (TurnBasedMode) Ian->PlayerSkill.secondary[1]-=2;
							Ian->UseOn(hWnd,g_pDD,itersave->second,BlDat,0,0,0);
						}
						break;
					}
					}	
			}
			menet = 0;
		}
		if (((mousetyp==9) && (Frame>=80)) || (mousetyp>=10))
		{
			FRMLocationMap::iterator iter;
			iter = StaticInf->Map.begin();
			while (iter != StaticInf->Map.end()) {
				PFRM frm;
				int iy = DeCompLocY(iter->second->loc);
				int ix = DeCompLocX(iter->second->loc);
				if (iter->second->FRM != NULL) frm = iter->second->FRM->FRM;
				else frm = iter->second->FRMA->GetCurFrame();
				int cx = TerX+LocConvertX(ix,iy)*16+frm->bx;
				int cy = TerY+LocConvertY(ix,iy)*12+frm->by;
				if (mouse::MouseIn(cx,cy,cx+frm->x,cy+frm->y)) {
					switch (iter->second->type) {
						case 2: {
							BlitToRo(g_pDDSBack,0,0,frm->x,frm->y,cx,cy,DDBLTFAST_SRCCOLORKEY,frm->FRM,60);
							break;
						}
					}
				}
				iter++;
			}
		}
		if ((mousetyp==9) && ((Frame==80) || (menet==3)))
		{
		if (menet != 3) {
			FRMLocationMap::iterator iter;
			iter = StaticInf->Map.begin();
			while (iter != StaticInf->Map.end()) {
				PFRM frm;
				int iy = DeCompLocY(iter->second->loc);
				int ix = DeCompLocX(iter->second->loc);
				if (iter->second->FRM != NULL) frm = iter->second->FRM->FRM;
				else frm = iter->second->FRMA->GetCurFrame();
				int cx = TerX+LocConvertX(ix,iy)*16+frm->bx;
				int cy = TerY+LocConvertY(ix,iy)*12+frm->by;
				if (mouse::MouseIn(cx,cy,cx+frm->x,cy+frm->y)) {
				switch (iter->second->type) {
					case 1: {
						char buf[20];
						wsprintf(buf,"SCENERY_%i",iter->second->num);
						std::string xXx;
						xXx = textutil::GetFromProf(GetFile("\\text\\scenery.sve"),"SCENERY",buf);
						if (xXx != "") {
							textutil::AddString("You see: "+xXx,1);
						}
			 			break;
						}
					case 2: {
						char buf[20];
						wsprintf(buf,"ITEMS_%i",iter->second->num);
						std::string xXx;
						xXx = "\\data\\items\\"+textutil::GetFromProf(GetFile("\\proto\\items.pro"),"ITEMS",buf);
						textutil::AddString("You see: "+textutil::GetFromXML(GetFile(xXx),"/text.yousee"),1);
						break;
						}
					case 3: {
						//char buf[20];
						//wsprintf(buf,"OBJECTS_%i",iter->second->num);
						std::string xXx;
						xXx = iter->second->ItemDesc->desclocation;//"\\data\\objects\\"+textutil::GetFromProf(GetFile("\\proto\\objects.pro"),"OBJECTS",buf);
						textutil::AddString("You see: "+textutil::GetFromXML(GetFile(xXx),"/text.yousee"),1);
						break;
						}
					case 5: {
						std::string xXx;
						xXx = iter->second->ItemDesc->desclocation;
						textutil::AddString("You see: "+textutil::GetFromXML(GetFile(xXx),"/text.ondead"),1);
						break;
						}
					}
				}
				iter++;
			}
			CritterList::iterator iter2;
			iter2 = CritterInf->Critters.begin();
			while (iter2 !=CritterInf->Critters.end()) {
				PFRM frm;
				iter2->second->Idle->SetDir(iter2->second->dir);
				frm = iter2->second->Idle->GetCurFrame();
				int cx = TerX+LocConvertX(iter2->second->x,iter2->second->y)*16+frm->bx;
				int cy = TerY+LocConvertY(iter2->second->x,iter2->second->y)*12+frm->by;
				if (mouse::MouseIn(cx,cy,cx+frm->x,cy+frm->y)) {
						std::string xXx;
						xXx = textutil::GetFromXML(iter2->second->Player.protolocation,"/text.yousee");
						textutil::AddString("You see: "+xXx,1);
				}
				iter2++;
			}
		} else {
			if (iter2save!=CritterInf->Critters.end()) {
				std::string xXx;
				xXx = textutil::GetFromXML(iter2save->second->Player.protolocation,"/text.lookat");
				textutil::AddString(xXx,1);
			}
			if (itersave!=StaticInf->Map.end()) {
				switch (itersave->second->type) {
					case 1: {
						char buf[20];
						wsprintf(buf,"SCENERY_%i",itersave->second->num);
						std::string xXx;
						xXx = textutil::GetFromProf(GetFile("\\text\\scenery2.sve"),"SCENERY",buf);
						if (xXx != "") {
							textutil::AddString(xXx,1);
						}
			 			break;
						}
					case 2: {
						char buf[20];
						wsprintf(buf,"ITEMS_%i",itersave->second->num);
						std::string xXx;
						xXx = "\\data\\items\\"+textutil::GetFromProf(GetFile("\\proto\\items.pro"),"ITEMS",buf);
						textutil::AddString(textutil::GetFromXML(GetFile(xXx),"/text.lookat"),1);
						break;
						}
					case 3: {
						std::string xXx = itersave->second->ItemDesc->desclocation;
						textutil::AddString(textutil::GetFromXML(GetFile(xXx),"/text.lookat"),1);
						break;
						}
					case 5: {
						std::string xXx = itersave->second->ItemDesc->desclocation;
						textutil::AddString(textutil::GetFromXML(GetFile(xXx),"/text.ondeadlook"),1);
						break;
						}
				}
			} }
		menet = 0; Frame=81;
		}

		if ((mousetyp == 9) && (menet == 7)) {
			menet = 0;
			CountVisible();
			std::string xXx;
			xXx = textutil::GetFromXML(iter2save->second->Player.protolocation,"/events/ontalk:");
			if (xXx != "") {
				if ((iter2save->second->visibletype==60) || (iter2save->second->Distance(Ian->x,Ian->y)>20)) {
					
					Ian->GoAround(iter2save->second->x,iter2save->second->y,BlDat);
				} else {
					IanOs::Block = &BlDat;
					IanOs::ActualPlayer = iter2save->second;
					IanOs::RunStat(xXx);
				}
			}
			menet = 0;mousetyp=0;
		}

		if ((mousetyp == 9)	&& (menet == 5)) {
			iter2save->second->dir++;
			if (iter2save->second->dir>5) iter2save->second->dir=0;
			menet =	0;
		}
		if ((mousetyp == 9)	&& (menet == 4)) {
			iter2save->second->GoAround(iter2save->second->x,iter2save->second->y,BlDat);
			menet =	0;
		}
		
	}

	if (mousetyp>20) {
	CritterList::iterator iter2;
	iter2 = CritterInf->Critters.begin();
	while (iter2 !=CritterInf->Critters.end()) {
		PFRM frm;
		iter2->second->Idle->SetDir(iter2->second->dir);
		frm = iter2->second->Idle->GetCurFrame();
		int cx = TerX+LocConvertX(iter2->second->x,iter2->second->y)*16+frm->bx;
		int cy = TerY+LocConvertY(iter2->second->x,iter2->second->y)*12+frm->by;
		if (mouse::MouseIn(cx,cy,cx+frm->x,cy+frm->y)) {
			PWeapon Weapon;
			if (Ian->curhand) Weapon = Ian->Hand1; else Weapon = Ian->Hand2;
			PWeaponMode WMode = Weapon->GetCurrentMode();
			char buf[50];
			if (WMode) {
				int enemdist = Ian->Distance(iter2->second->x,iter2->second->y);
				int hitloc = abs(WMode->minr-enemdist);
				int chance = 50-((double)hitloc/(double)WMode->maxr)*50;

				sprintf(buf,"Dist: %i Chance: %i%%", enemdist,chance);
				textfont::IanOutTextR(cx,cy+15,0,buf);
			}
			
			sprintf(buf,"HP: %i/%i",iter2->second->PlayerSkill.HP,iter2->second->PlayerSkill.AlapSk.HP);
			textfont::IanOutTextR(cx,cy,0,buf);
		}
		iter2++;
	}
	}
}