#include "armwep.h"
#include "../commonutils/textutil.h"
#include "../commonutils/ddutil.h"
#include "../tinyXML/tinyXML.h"

void TWeapon::Load(unsigned int itemnum,FRMPairCollection &TilesI)
{
	number = itemnum;
	std::string filename;
	std::string typestuff;
	char buf[20];

	sprintf(buf,"ITEMS_%i",itemnum);
	filename = textutil::GetFromProf(GetFile("\\proto\\items.pro"),"ITEMS",buf);

	deflocation = GetFile("\\data\\items\\"+filename);

	typestuff = textutil::GetFromXML(deflocation,".type");
	for (unsigned int i2=0;i2<typestuff.length();i2++) typestuff[i2]=toupper(typestuff[i2]);
	if (typestuff=="WEAPON") {
		ammotype = atoi2(textutil::GetFromXML(deflocation,"/desc/ammo.type").c_str());
		capacity = atoi2(textutil::GetFromXML(deflocation,"/desc/ammo.capacity").c_str());
		numammo = 0;
		WeaponType = 1;
	} else
	if (typestuff=="DRUG") {
		ammotype = 0;
		capacity = 0;
		numammo = 0;
		WeaponType = 2;
	} else {
		WeaponType = 0;
		ammotype = 0;
		capacity = 0;
		numammo = 0;
	}
	HasGraphic = (textutil::GetFromXML(deflocation,"/graphic.picture") != "");
	if (HasGraphic) {
		Picture = TilesI[itemnum]->FRMA;
	}
	
	TiXmlDocument XMLD(deflocation.c_str());
	XMLD.LoadFile();
	const TiXmlElement* parent = textutil::XMLSearchNode(XMLD.RootElement(),"/desc.");
            
	TiXmlNode* child = NULL;
	TiXmlNode* parent2;
	while( child = parent->IterateChildren("mode",child ) )
	{
		PWeaponMode CMode = new TWeaponMode();
		CMode->name = child->ToElement()->Attribute("name");
		CMode->apuse = atoi2(child->ToElement()->Attribute("apuse"));
		CMode->mode = atoi2(child->ToElement()->Attribute("type"));
		//AddToLog("%s %i %i",CMode->name.c_str(),CMode->apuse,CMode->mode);
			//textutil::GetFromProf(deflocation,"GAME",buf);
		//for (unsigned int i2=0;i2<CMode->name.length();i2++) CMode->name[i2]=toupper(CMode->name[i2]);

		CMode->minr = 1;
		CMode->maxr = 1;
		CMode->maxdam = 0;
		CMode->mindam = 0;
		CMode->before = "";
		CMode->shoot = "";
		CMode->after = "";
		CMode->criticaldeath = 0;
		CMode->normalcriticalchance = 0;
		

		if (parent2 = child->FirstChild("range"))
		{
			CMode->minr = atoi2(parent2->ToElement()->Attribute("min"));
			CMode->maxr = atoi2(parent2->ToElement()->Attribute("max"));
		}
		if (parent2 = child->FirstChild("dammage"))
		{
			CMode->mindam = atoi2(parent2->ToElement()->Attribute("min"));
			CMode->maxdam = atoi2(parent2->ToElement()->Attribute("max"));
		}
		if (parent2 = child->FirstChild("critical"))
		{
			CMode->criticaldeath = atoi2(parent2->ToElement()->Attribute("deathtype"));
			CMode->normalcriticalchance = atoi2(parent2->ToElement()->Attribute("chance"));
		}
		if (parent2 = child->FirstChild("critter"))
		{
			if (parent2->ToElement()->Attribute("before")) CMode->before = parent2->ToElement()->Attribute("before");
			if (parent2->ToElement()->Attribute("shoot")) CMode->shoot = parent2->ToElement()->Attribute("shoot");
			if (parent2->ToElement()->Attribute("after")) CMode->after = parent2->ToElement()->Attribute("after");
			
		}
		WepMode->Insert(CMode);
		
	}
	FirstMode();
}

namespace textfont {
	void IanOutTextR(int x, int y, int co, std::string str);
	void IanOutText(int x, int y, int co, std::string str);
}

#pragma warning (disable : 4244)

HRESULT TWeapon::DrawOut(LPDIRECTDRAWSURFACE7 Back,int x, int y)
{
	if ((HasGraphic) && (Picture)) {
		y+=4;
		int maxx = 188;
		PFRM frm = Picture->GetCurFrame();
		//BlitTo(Back,0,0,frm->x,frm->y,x+99-frm->x/2,y+33-frm->y/2,DDBLTFAST_SRCCOLORKEY,frm->FRM);
		if ((frm->x>maxx)) {
			double d = ((double)frm->y/(double)frm->x*maxx);
			BlitToStretch(Back,0,0,frm->x,frm->y,x,y+30-d/2,x+maxx,y+30+d/2,DDBLTFAST_SRCCOLORKEY,frm->FRM);
		} else {
		if (frm->y>60) {
			double d = ((double)frm->x/(double)frm->y*60);
			BlitToStretch(Back,0,0,frm->x,frm->y,x+maxx/2-d/2,y,x+maxx/2+d/2,y+60,DDBLTFAST_SRCCOLORKEY,frm->FRM);
		} else
			BlitTo(Back,0,0,frm->x,frm->y,x+maxx/2-frm->x/2,y+30-frm->y/2,DDBLTFAST_SRCCOLORKEY,frm->FRM);
		}
		y-=4;

	}
	PWeaponMode Item = GetCurrentMode();
	if (Item) {
		textfont::IanOutTextR(x+170,y+10,2,Item->name);
		if (Item->apuse>0) {
			char buf[10];
			wsprintf(buf,"%i AP",Item->apuse);
			textfont::IanOutText(x+10,y+45,2,buf);
		}
	}
	return DD_OK;
}

HRESULT TWeapon::DrawOutSmall(LPDIRECTDRAWSURFACE7 Back,int x, int y,int maxx)
{
	if ((HasGraphic) && (Picture)) {
		PFRM frm = Picture->GetCurFrame();
		if ((frm->x>maxx)) {
			double d = ((double)frm->y/(double)frm->x*maxx);
			BlitToStretch(Back,0,0,frm->x,frm->y,x,y+30-d/2,x+maxx,y+30+d/2,DDBLTFAST_SRCCOLORKEY,frm->FRM);
		} else {
		if (frm->y>60) {
			double d = ((double)frm->x/(double)frm->y*60);
			BlitToStretch(Back,0,0,frm->x,frm->y,x+maxx/2-d/2,y,x+maxx/2+d/2,y+60,DDBLTFAST_SRCCOLORKEY,frm->FRM);
		} else
			BlitTo(Back,0,0,frm->x,frm->y,x+maxx/2-frm->x/2,y+30-frm->y/2,DDBLTFAST_SRCCOLORKEY,frm->FRM);
		}
	}
	return DD_OK;
}