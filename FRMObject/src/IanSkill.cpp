#include "IanSkill.h"

void ianskill::Nullaz(Skills *_Sk)
{
	int i;
	for (i=0; i<7; i++) _Sk->AlapSk.primary[i]=5;
	for (i=0; i<10; i++) _Sk->secondary[i]=0;
	for (i=0; i<18; i++) { _Sk->skills[i]=0; _Sk->tagSk[i]=false; }
	for (i=0; i<50; i++) _Sk->perktra[i]=false;
	for (i=0; i<7; i++) _Sk->beteg[i]=false;
	_Sk->HP = 1;
	_Sk->AlapSk.HP = 1;
}

void ianskill::CountSk(Skills *ISkill)
{
int i;

for (i=0; i<7; i++) ISkill->primary[i] = ISkill->AlapSk.primary[i];

#define I_ST (ISkill->primary[0])
#define I_PE (ISkill->primary[1])
#define I_EN (ISkill->primary[2])
#define I_CH (ISkill->primary[3])
#define I_IN (ISkill->primary[4])
#define I_AG (ISkill->primary[5])
#define I_LK (ISkill->primary[6])
#define SKI(i,i2) (ISkill->skills[(i)]+=(i2))
#define SEKI(i,i2) (ISkill->secondary[(i)]+=(i2))

if (ISkill->perktra[1]) {
 I_ST++;
 I_AG--;
}
if (ISkill->perktra[2]) {
 I_AG++;
}
if (ISkill->perktra[15]) {
 I_ST++;I_PE++;I_EN++;I_CH++;I_IN++;I_AG++;I_LK++;
}
		
ISkill->AlapSk.skills[0] = (5+(I_AG)*4);
ISkill->AlapSk.skills[1] = (0+(I_AG)*2);
ISkill->AlapSk.skills[2] = (0+(I_AG)*2);
ISkill->AlapSk.skills[3] = (30+(I_AG+I_ST)*2);
ISkill->AlapSk.skills[4] = (20+(I_AG+I_ST)*2);
ISkill->AlapSk.skills[5] = (0+(I_AG)*4);
ISkill->AlapSk.skills[6] = (0+(I_PE+I_IN)*2);
ISkill->AlapSk.skills[7] = (5+(I_PE+I_IN));
ISkill->AlapSk.skills[8] = (5+(I_AG)*3);
ISkill->AlapSk.skills[9] = (10+(I_PE+I_AG));
ISkill->AlapSk.skills[10] = (0+(I_AG)*3);
ISkill->AlapSk.skills[11] = (10+(I_PE+I_AG));
ISkill->AlapSk.skills[12] = (0+(I_IN)*4);
ISkill->AlapSk.skills[13] = (0+(I_IN)*3);
ISkill->AlapSk.skills[14] = (0+(I_CH)*5);
ISkill->AlapSk.skills[15] = (0+(I_CH)*4);
ISkill->AlapSk.skills[16] = (0+(I_LK)*5);
ISkill->AlapSk.skills[17] = (0+(I_EN+I_IN)*2);

ISkill->AlapSk.secondary[0] = 5+I_AG/2+I_AG%2;
ISkill->AlapSk.secondary[1] = 5+I_AG/2;
ISkill->AlapSk.secondary[2] = 100+(I_ST-4)*25;
ISkill->AlapSk.secondary[3] = I_ST/2;
ISkill->AlapSk.secondary[4] = (I_ST+I_PE+I_EN+I_AG);
ISkill->AlapSk.secondary[5] = (I_EN+I_AG)*2;
ISkill->AlapSk.secondary[6] = 5+I_IN*3;
ISkill->AlapSk.secondary[7] = I_AG/2;
ISkill->AlapSk.secondary[8] = (I_ST+I_EN)/3;
ISkill->AlapSk.secondary[9] = I_ST+I_LK;

ISkill->AlapSk.HP = 15+(I_ST*I_EN)/4;

for (i=0; i<18; i++) ISkill->skills[i] = ISkill->AlapSk.skills[i];
for (i=0; i<10; i++) ISkill->secondary[i] = ISkill->AlapSk.secondary[i];

if (ISkill->perktra[0]) { SEKI(5,-10); SEKI(8,3); }
if (ISkill->perktra[2]) { SEKI(2,-25); }
if (ISkill->perktra[3]) { SKI(0,25); SKI(1,-15); }
if (ISkill->perktra[5]) { SEKI(0,-10); SEKI(7,5); }
if (ISkill->perktra[6]) { SEKI(9,-10); SKI(3,25); }
if (ISkill->perktra[7]) { SKI(0,-15); SKI(1,-30); SKI(2,-30); }
if (ISkill->perktra[10]) { SKI(0,-10); SKI(1,-20);SKI(2,-20); SKI(6,10); SKI(7,10); SKI(14,10); SKI(15,10); }
if (ISkill->perktra[11]) { SEKI(5,-10); SEKI(8,3); }
if (ISkill->perktra[12]) { SEKI(5,10); SEKI(8,-3); }
if (ISkill->perktra[14]) { SEKI(6,5); for (i=0; i<18; i++) { SKI(i,10); } }
if (ISkill->perktra[15]) { for (i=0; i<18; i++) { SKI(i,-10); } SEKI(6,-5); }

for (i=0; i<18; i++) if (ISkill->tagSk[i]) ISkill->skills[i]=(int)((double)(ISkill->skills[i])*2);
for (i=0; i<18; i++) if (ISkill->skills[i]<=0) ISkill->skills[i]=1;
for (i=0; i<10; i++) if (ISkill->secondary[i]<=0) ISkill->secondary[i]=1;

#undef I_ST
#undef I_PE
#undef I_EN
#undef I_CH
#undef I_IN
#undef I_AG
#undef I_LK
#undef SKI
#undef SEKI

}
