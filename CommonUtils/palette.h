#ifndef __PALETTE_H
#define __PALETTE_H

//#include "global.h"

namespace palette {
	void Calculate();
	void Calculate16bit();
	void SetPaletteBl(int Frame);
	void FadeOut();
	void FadeIn();
	void FadeInWh();
	void SetPaletteNo();
	void SetPaletteWh(int Frame);
	void FadeOutWh();
	void SetGamma128();
}

#endif //__PALETTE_H