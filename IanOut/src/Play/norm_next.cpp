#include "normit.h"


void play::OnNextFr(void)
{
    CritterList::iterator iter2;
	iter2 = CritterInf->Critters.begin();
	while (iter2 != CritterInf->Critters.end()) {
		iter2->second->Next(hWnd,g_pDD,BlDat,StaticInf->Map,CritterInf->Critters);

		if (TurnBasedMode || (!AllowUserInteract)) {
		} else {
		if (rand()%400 == 0) {
			std::string xXx;
			xXx = textutil::GetFromXML(iter2->second->Player.protolocation,"/events/onrare:");
			if (xXx != "") {
				IanOs::Block = &BlDat;
				IanOs::ActualPlayer = iter2->second;
				IanOs::RunStat(xXx);
			}
		}
		if (rand()%200 == 0) {
			std::string xXx;
			xXx = textutil::GetFromXML(iter2->second->Player.protolocation,"/events/onuncommon:");
			if (xXx != "") {
				IanOs::Block = &BlDat;
				IanOs::ActualPlayer = iter2->second;
				IanOs::RunStat(xXx);
			}
		}
		if (rand()%100 == 0) {
			std::string xXx;
			xXx = textutil::GetFromXML(iter2->second->Player.protolocation,"/events/oncommon:");
			if (xXx != "") {
				IanOs::Block = &BlDat;
				IanOs::ActualPlayer = iter2->second;
				IanOs::RunStat(xXx);
			}
		}
		if (rand()%50 == 0) {
			std::string xXx;
			xXx = textutil::GetFromXML(iter2->second->Player.protolocation,"/events/onfrequent:");
			if (xXx != "") {
				IanOs::Block = &BlDat;
				IanOs::ActualPlayer = iter2->second;
				IanOs::RunStat(xXx);
			}
		}
		if (rand()%5 == 0) {
			std::string xXx;
			xXx = textutil::GetFromXML(iter2->second->Player.protolocation,"/events/onforced:");
			if (xXx != "") {
				IanOs::Block = &BlDat;
				IanOs::ActualPlayer = iter2->second;
				IanOs::RunStat(xXx);
			}
		}
		}
		
		CritterList::iterator iter3;
		iter3 = iter2;
		iter2++;
		if (iter3->second->curth >= 255) {
			if (iter3->second->curth == 255) iter3->second->Die(hWnd,g_pDD,StaticInf);
			delete iter3->second;
			CritterInf->Critters.erase(iter3);
		}
	}
	if (!TurnBasedMode) {
		secondinc++;
		if (secondinc>25) { date::AddSecond(1);secondinc=0; }
	}

	FRMLocationMap::iterator iter;
	if (!StaticInf->Map.empty()) 
	{ iter = StaticInf->Map.begin();
	while (iter != StaticInf->Map.end()) {
		if ((iter->second->type == 3) && (iter->second->ItemDesc->change)) {
			if (iter->second->ItemDesc->direction) {
				iter->second->FRMA->NextFrame();
				if (iter->second->FRMA->GetLastFrame()) iter->second->ItemDesc->change = false;
			} else {
				iter->second->FRMA->PrevFrame();
				if (iter->second->FRMA->GetFirstFrame()) iter->second->ItemDesc->change = false;
			}
		}
		iter++;
	}}

	PItem TheItem;
	TheItem = TauntList->First();
	while (TheItem != NULL) {
		((PTauntObj)TheItem)->TimeOut--;
		PItem ItemSave = TheItem;
		TheItem = TauntList->Next(TheItem);
		if (((PTauntObj)ItemSave)->TimeOut<0) TauntList->Delete(ItemSave);
	}
	TheItem = TimerList->First();
	while (TheItem != NULL) {
		PTimerObj ItemSave = (PTimerObj)TheItem;
		TheItem = TimerList->Next(TheItem);
		if (ItemSave->Elapsed()) {
			IanOs::RunStat(ItemSave->ToDo);
			TimerList->Delete(ItemSave);
		};
	}
}