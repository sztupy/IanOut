#include "play.h"

#include "../commonutils/textutil.h"
#include "../commonutils/textfont.h"
#include "load.h"
#include "done.h"
#include "../commonutils/mouse.h"
#include "../commonutils/palette.h"
#include "../FRMObject/IanSkill.h"

void play::CreateChar(void)
{
    HRESULT                     hRet;
		
	static int					Frame = -25;
	static DWORD				ThisTick;
	static DWORD				LastTick = 0;
	char						buf[80];
	char						buf2[80];
	int							i,i2;
	static int					Points = 5;
	PFRMPlayer Ian = CritterInf->Critters.find(0)->second;

	ianskill::CountSk(&Ian->PlayerSkill);

    int addx = (GetMaxX - 640) /2;
	int addy = (GetMaxY - 480) /2;

	ThisTick = GetTickCount();

	if ((lstrcmpi(FullScreen->fname,"\\art\\intrface\\edtrcrte.frm")))
	{
    	LoadFRMSingle(&FullScreen,hWnd,"\\art\\intrface\\edtrcrte.frm",1);
	}

	if ((ThisTick - LastTick) > 0)
        {
            LastTick = ThisTick;
	
            Frame+=3;
			
        }

		std::string charfname;
		std::string chardfname;
	charfname = GetFile("\\text\\dialog\\char.sve");
	chardfname = GetFile("\\text\\dialog\\chard.sve");
	olddims = dims;
	mouse::UpdateInputState();

	MousX += dims.lX;
	MousY += dims.lY;

	if (MousX>=GetMaxX) { MousX=GetMaxX;}
	if (MousX<=0)   { MousX=0;  }
	if (MousY>=GetMaxY) { MousY=GetMaxY;}
	if (MousY<=0)   { MousY=0;  }

	ClearSurface(g_pDDSBack,0,0,0);
	BlitTo(g_pDDSBack,0,0,640,480,addx+0,addy+0,0,FullScreen->FRM->FRM);

	for (i=0; i<7; i++)
	{
	  if ((Ian->PlayerSkill.primary[i]>10) || (Ian->PlayerSkill.primary[i]<1))
	   textfont::DisplayNum(addx+44,addy+37+i*33,7,Ian->PlayerSkill.primary[i],2); else
	   textfont::DisplayNum(addx+44,addy+37+i*33,6,Ian->PlayerSkill.primary[i],2);
		  
	  wsprintf(buf,"typ%i",Ian->PlayerSkill.primary[i]);
	  if (Selected==i+1) textutil::WriteFromProf(addx+100,addy+37+i*33+3,0,1,charfname,"Primaryskill",buf);
					else textutil::WriteFromProf(addx+100,addy+37+i*33+3,1,1,charfname,"Primaryskill",buf);
	  if ((mouse::MInPr(addx+152,addy+37+i*33,addx+165,addy+45+i*33,0)) && (Points>=1) && (Ian->PlayerSkill.primary[i]<10))
	  { Ian->PlayerSkill.AlapSk.primary[i]++; Points--; }
	  if ((mouse::MInPr(addx+152,addy+46+i*33,addx+165,addy+57+i*33,0)) && (Ian->PlayerSkill.primary[i]>1))
	  { Ian->PlayerSkill.AlapSk.primary[i]--; Points++; }
	  if (mouse::MInPr(addx+20,addy+38+i*33,addx+166,addy+60+i*33,0)) Selected = i+1;
	 
	}

	textfont::DisplayNum(addx+111,addy+282,6,Points,2);
	textfont::DisplayNum(addx+508,addy+228,6,3,2);

	textutil::WriteFromProf(addx+86,addy+8,2,2,charfname,"Global","def1");
	textutil::WriteFromProf(addx+195,addy+8,2,2,charfname,"Global","def2");
	textutil::WriteFromProf(addx+274,addy+8,2,2,charfname,"Global","def3");
		
	textutil::WriteFromProf(addx+355,addy+8,2,1,charfname,"Global","def4");
	textutil::WriteFromProf(addx+355,addy+233,2,1,charfname,"Global","def5");

	textutil::WriteFromProf(addx+22,addy+286,2,1,charfname,"Global","def6");
	textutil::WriteFromProf(addx+25,addy+330,2,1,charfname,"Global","def7");

	if (mouse::MInPr(addx+190,addy+43,addx+320,addy+55,0)) Selected=8;
	if (Selected==8)
	textutil::WriteFromProf(addx+190,addy+43,0,1,charfname,"Health","hea1"); else
	textutil::WriteFromProf(addx+190,addy+43,1,1,charfname,"Health","hea1");

	Ian->PlayerSkill.HP = Ian->PlayerSkill.AlapSk.HP;
	wsprintf(buf,"%i/%i",Ian->PlayerSkill.HP,Ian->PlayerSkill.AlapSk.HP);
	textfont::IanOutText(addx+190+64,addy+43,1,buf);

	for (i=1; i<8; i++)
	{
		wsprintf(buf,"hea%i",i+1);
		if (mouse::MInPr(addx+190,addy+43+i*13,addx+320,addy+55+i*13,0)) Selected=8+i;
		if (Selected==8+i)
		textutil::WriteFromProf(addx+190,addy+43+i*13,0,1,charfname,"Health",buf); else
		textutil::WriteFromProf(addx+190,addy+43+i*13,2,1,charfname,"Health",buf); 
	}
	
	for (i=0; i<10; i++)
	{
		wsprintf(buf,"sec%i",i+1);
		if (mouse::MInPr(addx+190,addy+177+i*13,addx+320,addy+177+12+i*13,0)) Selected=16+i;
		if (Selected==16+i)
		textutil::WriteFromProf(addx+190,addy+177+i*13,0,1,charfname,"Secondary",buf); else
		textutil::WriteFromProf(addx+190,addy+177+i*13,1,1,charfname,"Secondary",buf);
		if ((i==4) || (i==5) || (i==9))
		wsprintf(buf,"%i%%",Ian->PlayerSkill.secondary[i]); else
		wsprintf(buf,"%i",Ian->PlayerSkill.secondary[i]); 
		textfont::IanOutText(addx+285,addy+177+i*13,1,buf);
	}

	for (i=0; i<18; i++)
	{
		wsprintf(buf,"ski%i",i+1);
		if (mouse::MInPr(addx+346,addy+24+11*i,addx+600,addy+24+11*i+10,0)) Selected=42+i;
		if (mouse::MInPr(addx+346,addy+26+11*i,addx+346+17,addy+26+11*i+10,0)) Ian->PlayerSkill.tagSk[i]=!Ian->PlayerSkill.tagSk[i];
		if  ((Ian->PlayerSkill.tagSk[i]) && !(Selected==42+i))
		textutil::WriteFromProf(addx+375,addy+24+11*i,3,1,charfname,"Skills",buf); else
		if ((Selected==42+i) && !(Ian->PlayerSkill.tagSk[i]))
		textutil::WriteFromProf(addx+375,addy+24+11*i,0,1,charfname,"Skills",buf); else
		if (Selected==42+i)
		textutil::WriteFromProf(addx+375,addy+24+11*i,2,1,charfname,"Skills",buf); else
		textutil::WriteFromProf(addx+375,addy+24+11*i,1,1,charfname,"Skills",buf);
		if ((mouse::MouseIn(addx+346,addy+26+11*i,addx+346+17,addy+26+11*i+10)) && (dims.rgbButtons[0] & 0x80))
		{
			i2=1;
		} else
		{
			i2=0;
		}
		BlitTo(g_pDDSBack,0,0,Buttons[0][i2]->FRM->x,Buttons[0][i2]->FRM->y,addx+346,addy+26+11*i,0,Buttons[0][i2]->FRM->FRM);
	} 
	for (i=0; i<18; i++)
	{
	 wsprintf(buf,"%i%%",Ian->PlayerSkill.skills[i]);
	 textfont::IanOutText(addx+575,addy+24+11*i,1,buf);
	}

	for (i=0; i<8; i++)
	{
		wsprintf(buf,"tra%i",i+1);

		if (mouse::MInPr(addx+25,addy+350+13*i,addx+168,addy+350+13*i+12,0)) Selected=26+i;
		if (mouse::MInPr(addx+25,addy+350+13*i,addx+25+17,addy+350+13*i+12,0)) Ian->PlayerSkill.perktra[i]=!Ian->PlayerSkill.perktra[i];
		if ((Selected==26+i) || (Ian->PlayerSkill.perktra[i]))
		textutil::WriteFromProf(addx+47,addy+350+13*i,0,1,charfname,"Traits",buf); else
		textutil::WriteFromProf(addx+47,addy+350+13*i,1,1,charfname,"Traits",buf);

		if (mouse::MInPr(addx+168,addy+350+13*i,addx+317,addy+350+13*i+12,0)) Selected=34+i;
		if (mouse::MInPr(addx+300,addy+350+13*i,addx+317,addy+350+13*i+12,0)) Ian->PlayerSkill.perktra[i+8]=!Ian->PlayerSkill.perktra[i+8];
		wsprintf(buf,"tra%i",i+9);
		if ((Selected==34+i) || (Ian->PlayerSkill.perktra[i+8]))
		textutil::WriteFromProf(addx+290,addy+350+13*i,0,3,charfname,"Traits",buf); else
		textutil::WriteFromProf(addx+290,addy+350+13*i,1,3,charfname,"Traits",buf);
		
		if ((mouse::MouseIn(addx+25,addy+353+13*i,addx+25+17,addy+353+13*i+10)) && (dims.rgbButtons[0] & 0x80))
		{ i2=1;	} else	{ i2=0;	}
		BlitTo(g_pDDSBack,0,0,Buttons[0][i2]->FRM->x,Buttons[0][i2]->FRM->y,addx+25,addy+353+13*i,0,Buttons[0][i2]->FRM->FRM);
		if ((mouse::MouseIn(addx+300,addy+353+13*i,addx+317,addy+353+13*i+10)) && (dims.rgbButtons[0] & 0x80))
		{ i2=1;	} else	{ i2=0;	}
		BlitTo(g_pDDSBack,0,0,Buttons[0][i2]->FRM->x,Buttons[0][i2]->FRM->y,addx+300,addy+353+13*i,0,Buttons[0][i2]->FRM->FRM);
	}

	wsprintf(buf2,"generic.frm");
	switch (Selected)
	{
	 case 1:wsprintf(buf2,"strength.frm");break;
	 case 2:wsprintf(buf2,"perceptn.frm");break;
	 case 3:wsprintf(buf2,"endur.frm");break;
	 case 4:wsprintf(buf2,"charisma.frm");break;
	 case 5:wsprintf(buf2,"intel.frm");break;
	 case 6:wsprintf(buf2,"agility.frm");break;
	 case 7:wsprintf(buf2,"luck.frm");break;

	 case 8:wsprintf(buf2,"hitpoint.frm");break;
	 case 9:case 10:case 11:
	 case 12:case 13:case 14:
	 case 15:wsprintf(buf2,"poisoned.frm");break;
	 case 16:wsprintf(buf2,"armorcls.frm");break;
	 case 17:wsprintf(buf2,"actionpt.frm");break;
	 case 18:wsprintf(buf2,"carryamt.frm");break;
	 case 19:wsprintf(buf2,"meleedam.frm");break;
	 case 20:wsprintf(buf2,"damresis.frm");break;
	 case 21:wsprintf(buf2,"poisnres.frm");break;
	 case 22:wsprintf(buf2,"here&now.frm");break;
	 case 23:wsprintf(buf2,"sequence.frm");break;
	 case 24:wsprintf(buf2,"healrate.frm");break;
	 case 25:wsprintf(buf2,"critchnc.frm");break;

	 case 26:wsprintf(buf2,"fastmeta.frm");break;
	 case 27:wsprintf(buf2,"bruiser.frm");break;
	 case 28:wsprintf(buf2,"smlframe.frm");break;
	 case 29:wsprintf(buf2,"onehand.frm");break;
	 case 30:wsprintf(buf2,"stranger.frm");break;
	 case 31:wsprintf(buf2,"kamikaze.frm");break;
	 case 32:wsprintf(buf2,"heavyhnd.frm");break;
	 case 33:wsprintf(buf2,"fastshot.frm");break;

	 case 34:wsprintf(buf2,"bldmess.frm");break;
	 case 35:wsprintf(buf2,"jinxed.frm");break;
	 case 36:wsprintf(buf2,"goodnatr.frm");break;
	 case 37:wsprintf(buf2,"addict.frm");break;
	 case 38:wsprintf(buf2,"drugrest.frm");break;
	 case 39:wsprintf(buf2,"empathy.frm");break;
	 case 40:wsprintf(buf2,"skilled.frm");break;
	 case 41:wsprintf(buf2,"gifted.frm");break;

	 case 42:wsprintf(buf2,"gunsml.frm");break;
	 case 43:wsprintf(buf2,"gunbig.frm");break;
	 case 44:wsprintf(buf2,"energywp.frm");break;
	 case 45:wsprintf(buf2,"unarmed.frm");break;
	 case 46:wsprintf(buf2,"melee.frm");break;
	 case 47:wsprintf(buf2,"throwing.frm");break;
	 case 48:wsprintf(buf2,"firstaid.frm");break;
	 case 49:wsprintf(buf2,"doctor.frm");break;
     case 50:wsprintf(buf2,"sneak.frm");break;
	 case 51:wsprintf(buf2,"lockpick.frm");break;
	 case 52:wsprintf(buf2,"steal.frm");break;
	 case 53:wsprintf(buf2,"traps.frm");break;
	 case 54:wsprintf(buf2,"science.frm");break;
	 case 55:wsprintf(buf2,"repair.frm");break;
	 case 56:wsprintf(buf2,"speech.frm");break;
	 case 57:wsprintf(buf2,"barter.frm");break;
	 case 58:wsprintf(buf2,"gambling.frm");break;
	 case 59:wsprintf(buf2,"outdoors.frm");break;
	}
	wsprintf(buf,"\\art\\skilldex\\%s",buf2);
	if ((lstrcmpi(SkillDex->fname,buf)))
	{
      LoadFRMSingle(&SkillDex,hWnd,buf,1);
	}
	BlitTo(g_pDDSBack,0,0,140,117,addx+480,addy+310,0,SkillDex->FRM->FRM);

	for (i=0; i<2; i++)
	{
		wsprintf(buf,"de%i_%i",Selected,i);
		textutil::WriteFromProf(addx+350,addy+280+i*14,11,1,chardfname,"Dialog",buf);
	}
	for (i=2; i<10; i++)
	{
		wsprintf(buf,"de%i_2",Selected,i);
		textfont::IanOutText(addx+350,addy+280+i*14,11,textutil::CutString(textutil::GetFromProf(chardfname,"Dialog",buf),165,i-1));
	}

	if ((dims.rgbButtons[0] & 0x80) && mouse::MouseIn(addx+457,addy+450,addx+457+26,addy+450+26))
	{
	  BlitTo(g_pDDSBack,0,0,MenuDown->FRM->x,MenuDown->FRM->y,addx+457,addy+450,DDBLTFAST_SRCCOLORKEY,MenuDown->FRM->FRM);
	} else
	{
	  BlitTo(g_pDDSBack,0,0,MenuUp->FRM->x,MenuUp->FRM->y,addx+457,addy+450,DDBLTFAST_SRCCOLORKEY,MenuUp->FRM->FRM);
	}
	textfont::IanOutText(addx+457+30,addy+457,2,"Vége");

	textutil::DisplayFrameRate();
	BlitTo(g_pDDSBack,0,0,Mouse->FRM->x, Mouse->FRM->y,MousX,MousY,DDBLTFAST_SRCCOLORKEY,Mouse->FRM->FRM);
	
    // Flip the surfaces
    while (TRUE)
    {
        hRet = g_pDDSPrimary->Flip(NULL, 0);
        if (hRet == DD_OK)
            break;
        if (hRet == DDERR_SURFACELOST)
        {
            hRet = RestoreAll();
            if (hRet != DD_OK)
                break;
        }
        if (hRet != DDERR_WASSTILLDRAWING)
            break;
    }
	if (mouse::MInPr(addx+457,addy+450,addx+457+26,addy+450+26,0))
	 {
		 palette::FadeOutWh();
		 ClearSurface(g_pDDSBack,0,0,0);
		 ClearSurface(g_pDDSPrimary,0,0,0);
		 GamePos = 1;
		 buttons::ChangeButtonList(ButtonList,2);

		 InitializeMap();

		 //play::NormalPlay();
		 palette::FadeInWh();
		 //palette::SetPaletteBl(65535);
	 }
}