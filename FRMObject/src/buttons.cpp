#include "buttons.h"
#include "../commonutils/ddutil.h"
#include "../commonutils/discutil.h"
#include "../commonutils/engine.h"

PFRMSingle					MenuUp = NULL;
PFRMSingle					MenuDown = NULL;
PFRMSingle					Buttons[25][2];

void buttons::TButton::Draw(SDL_Surface* Back,bool Press)
{
	PFRM frm = Press ? on->FRM : off->FRM;
	//AddToLog("x");
	BlitTo(Back,0,0,frm->x,frm->y,x,y,0,frm->FRM);
}

void buttons::TButtonList::DrawButtons(SDL_Surface* Back,int Mx, int My, bool Press)
{
	PItem Item;
	Item = Buttons->First();
	while (Item != NULL) {
		PButton(Item)->Draw(Back,Press && PButton(Item)->Over(Mx,My));
		Item = Buttons->Next(Item);
	}
}

int buttons::TButtonList::GetButton(int Mx, int My, bool Press)
{
	PItem Item;
	Item = Buttons->First();
	while (Item != NULL) {
		if (PButton(Item)->Pressed(Mx,My,Press)) return PButton(Item)->retval;
		Item = Buttons->Next(Item);
	}
	return 0;
}


void buttons::ChangeButtonList(PButtonList &List, int mode)
{
	AddToLog(5,"Buttons> ButtonList changed to number %i",mode);
	int addx = (GetMaxX-640)/2;
	int addy = (GetMaxY-480)/2;

	if (List) delete List;
	List = new TButtonList();

	List->actualmode = mode;

	switch (mode) {
		case 1:
			List->AddButton(addx+30,addy+20,MenuUp,MenuDown,1);
			List->AddButton(addx+30,addy+61,MenuUp,MenuDown,2);
			List->AddButton(addx+30,addy+102,MenuUp,MenuDown,3);
			List->AddButton(addx+30,addy+143,MenuUp,MenuDown,4);
			List->AddButton(addx+30,addy+184,MenuUp,MenuDown,5);
			List->AddButton(addx+30,addy+225,MenuUp,MenuDown,6);
		break;
		case 3:
			List->AddButton(addx+590,GetMaxY-99+38+5,Buttons[9][0],Buttons[9][1],51);
			List->AddButton(addx+590,GetMaxY-99+38+27,Buttons[8][0],Buttons[8][1],50);
		case 2:
			List->AddButton(addx+211,GetMaxY-99+40,Buttons[5][0],Buttons[5][1],1);
			List->AddButton(addx+266,GetMaxY-99+27,Buttons[6][0],Buttons[6][1],100);
			List->AddButton(addx+218,GetMaxY-99+6,Buttons[7][0],Buttons[7][1],2);
		break;
		case 10:
			List->AddButton(addx+432+52,addy+206,Buttons[1][0],Buttons[1][1],1);
			List->AddButton(addx+50+475,addy+330,MenuUp,MenuDown,2);
			List->AddButton(addx+50+125,addy+4+35,Buttons[4][0],Buttons[4][1],3);
			List->AddButton(addx+50+125,addy+4+35+25,Buttons[3][0],Buttons[3][1],4);
			List->AddButton(addx+50+380,addy+4+35,Buttons[4][0],Buttons[4][1],5);
			List->AddButton(addx+50+380,addy+4+35+25,Buttons[3][0],Buttons[3][1],6);
		break;
		case 11:
			List->AddButton(addx+70+430,addy+8+325,MenuUp,MenuDown,2);
			List->AddButton(addx+70+130,addy+4+40,Buttons[4][0],Buttons[4][1],3);
			List->AddButton(addx+70+130,addy+4+40+25,Buttons[3][0],Buttons[3][1],4);
		break;
	}
	switch (mode) {
		case 10: case 11:
			List->AddButton(addx+266,GetMaxY-99+27,Buttons[6][0],Buttons[6][1],100);
		break;
	}
}