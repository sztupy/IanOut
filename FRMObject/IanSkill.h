#ifndef __IANSKILL_H
#define __IANSKILL_H

//#include "windows.h"

struct Alap
{
  int primary[7];
  int secondary[10];
  int HP;
  int skills[18];
};

struct Skills
{
   int primary[7];
   int secondary[10];
   int HP;
   int skills[18];
   bool beteg[7];
   bool tagSk[18];
   bool perktra[50];
   Alap AlapSk;
};


namespace ianskill {
	void Nullaz(Skills *_Sk);
	void CountSk(Skills *ISkill);
}

#endif //__IANSKILL_H