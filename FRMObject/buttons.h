#ifndef __BUTTONS_H
#define __BUTTONS_H

#include "FRM.h"

extern PFRMSingle					MenuUp;
extern PFRMSingle					MenuDown;
extern PFRMSingle					Buttons[25][2];

namespace buttons {
	class TButton;
	class TButtonList;
	
	typedef TButton *PButton;
	typedef TButtonList *PButtonList;

	class TButton : public TItem {
	public:
		TButton(int Ix,int Iy, PFRMSingle Ioff, PFRMSingle Ion, int Iretval)
			{
				x = Ix;
				y = Iy;
				on = Ion;
				off = Ioff;
				retval = Iretval;
				state = false;
			};
		~TButton() {};

		bool Over(int Mx, int My) {
			return ((Mx>=x) && (Mx<=x+on->FRM->x) && (My>=y) && (My<=y+on->FRM->y));
		}
		bool Pressed(int Mx,int My,bool Press) {
			if (Over(Mx,My) && (state) && (!Press)) { state = false; return true; }
			if (Over(Mx,My) && (!state) && (Press)) state = true;
			if (!Press) state = false;
			return false;
		}
		
		void Draw(LPDIRECTDRAWSURFACE7 Back,bool Press);

		PFRMSingle on, off;
		int x,y;
		int retval;
		bool state;
	};

	class TButtonList : public TItem {
	public:
		TButtonList() { Buttons = new TList(); actualmode = 0;};
		~TButtonList() { if (Buttons) delete Buttons; };

		void AddButton(int Ix,int Iy, PFRMSingle Ioff, PFRMSingle Ion, int Iretval)
		{
			Buttons->Insert(new TButton(Ix,Iy,Ioff,Ion,Iretval));
		}
		void DrawButtons(LPDIRECTDRAWSURFACE7 Back,int Mx, int My, bool Press);
		int GetButton(int Mx, int My, bool Press);

		int actualmode;

		PList Buttons;
	};

	void ChangeButtonList(PButtonList &List, int mode);
}

#endif