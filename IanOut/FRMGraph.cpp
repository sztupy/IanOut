#include "FRMGraph.h"
#include "io.h"
#include <fcntl.h>
#include "ddutil.h"
#include "utils.h"
#include "math.h"

#include "FRMGr_1.h"
#include "FRMGr_2.h"
#include "FRMGr_3.h"

// --- TFRMPlayer ---
//
// ------------------

void TFRMPlayer::LoadPlayer(PlayerDefAnim PDefAni)
{
	if (Player.bIdle)
	{
	 if (Idle) delete Idle;
	 Idle = new TFRMCommunicator(PDefAni.AIdle);
	}
	if (Player.bMove)
	{
	 if (Moving) delete Moving;
	 Moving = new TFRMCommunicator(PDefAni.AMoving);
	}
}

void TFRMPlayer::GenerateTree(int tx, int ty, BlockType Block)
{
	int ix,iy,i,i2;
	int ix2,iy2,tx2,ty2;
	int Long[6][3];
	BOOL Mehet;
	BOOL Ok;

	for (ix=0; ix<170; ix++)
		for (iy=0; iy<150; iy++)
		{
		  Table[ix][iy] = 111;
		}
	Table[x][y] = 1;
	i = 1;
	Mehet = TRUE;
	while (Mehet)
	{
	  for (ix=2; ix<163; ix++)
		for (iy=1; iy<143; iy++)
		{
			if (Table[ix][iy] == i)
			{
				if ((Table[ix-1][iy-1] > i+1) && (Block[ix-1][iy-1]!=1)) Table[ix-1][iy-1] = i+1; 
				if ((Table[ix+1][iy-1] > i+1) && (Block[ix+1][iy-1]!=1)) Table[ix+1][iy-1] = i+1;
				if ((Table[ix-1][iy+1] > i+1) && (Block[ix-1][iy+1]!=1)) Table[ix-1][iy+1] = i+1;
				if ((Table[ix+1][iy+1] > i+1) && (Block[ix+1][iy+1]!=1)) Table[ix+1][iy+1] = i+1;

				if ((Table[ix-2][iy  ] > i+1) && (Block[ix-1][iy  ]!=1) && (Block[ix-2][iy  ]!=1)) Table[ix-2][iy  ] = i+1;
				if ((Table[ix+2][iy  ] > i+1) && (Block[ix+1][iy  ]!=1) && (Block[ix+2][iy  ]!=1)) Table[ix+2][iy  ] = i+1;
			}
		}
	  i++;
	  if (i>100) Mehet=FALSE;
	  if (Table[tx][ty]!=111) Mehet=FALSE;
	}

	if ((tx==x) && (ty==y)) {i=101;}; 

	if (i<100)
	{
		PosIt[0][0]=i;
		PosIt[0][1]=i;
		PosIt[i][0]=tx;
		PosIt[i][1]=ty;

		Mehet = TRUE;
		while (Mehet)
		{
			i--;
			for (i2=0; i2<6; i2++)
			{
				Long[i2][0] = -1;
				Long[i2][1] = -1;
				Long[i2][2] = -1;
			}
			i2=0;
			if (i<1) {Mehet=FALSE;i=1;}
			for (ix=2; ix<163; ix++)
				for (iy=1; iy<143; iy++)
				{
				if (Table[ix][iy] == i)
				{
					Ok = FALSE;
					if ((ix+2 == PosIt[i+1][0]) &&
						(iy   == PosIt[i+1][1])) { Ok = TRUE; }
					if ((ix-2 == PosIt[i+1][0]) &&
						(iy   == PosIt[i+1][1])) { Ok = TRUE; }
					if ((ix+1 == PosIt[i+1][0]) &&
						(iy+1 == PosIt[i+1][1])) { Ok = TRUE; }
					if ((ix+1 == PosIt[i+1][0]) &&
						(iy-1 == PosIt[i+1][1])) { Ok = TRUE; }
					if ((ix-1 == PosIt[i+1][0]) &&
						(iy+1 == PosIt[i+1][1])) { Ok = TRUE; }
					if ((ix-1 == PosIt[i+1][0]) &&
						(iy-1 == PosIt[i+1][1])) { Ok = TRUE; }

					if (Ok)
					{
						Long[i2][0]=ix;
						Long[i2][1]=iy;
						ix2 = ix*14;
						iy2 = iy*12;
						tx2 = tx*14;
						ty2 = ty*12;
						Long[i2][2]=(int)sqrt(abs(ix2-tx2)*abs(ix2-tx2) + abs(iy2-ty2)*abs(iy2-ty2));
						i2++;
					}
				}
				}
			iy2=9999;
			tx2=0;
			for (ix2=0; ix2<i2; ix2++)
			{
				if (Long[ix2][2]<iy2)
				{
					iy2 = Long[ix2][2];
					tx2=ix2;
				}
			}
			PosIt[i][0]=Long[tx2][0];
			PosIt[i][1]=Long[tx2][1];
			}
		    
	} else
	{
		PosIt[0][0]=-1;
		PosIt[0][1]=-1;
		curth=0;
	}

	i2 = PosIt[0][0];
	if (i2>1)
	{
		tx2 = PosIt[1][0];
		ty2 = PosIt[1][1];
		ix2 = PosIt[2][0];
		iy2 = PosIt[2][1];
		if ((tx2+1==ix2) && (ty2+1==iy2)) { dir =2; }
		if ((tx2+1==ix2) && (ty2-1==iy2)) { dir =0; }
		if ((tx2+2==ix2) && (ty2  ==iy2)) { dir =1; }
		if ((tx2-2==ix2) && (ty2  ==iy2)) { dir =4; }
		if ((tx2-1==ix2) && (ty2+1==iy2)) { dir =3; }
		if ((tx2-1==ix2) && (ty2-1==iy2)) { dir =5; }
		curth=1;
	} else curth=0;

	ox = x;
	oy = y;
	if (i2>1)
	{
	PosIt[0][1]--;
	x = PosIt[PosIt[0][0]+1-PosIt[0][1]][0];
	y = PosIt[PosIt[0][0]+1-PosIt[0][1]][1];
	}

	framenum=0;
	fr2 = rand()%100;
	fr3=0;
	Moving->FirstFrame();
	Idle->FirstFrame();
	Labpos = FALSE;

}

void TFRMPlayer::Move(BlockType Block)
{
   int i;
   if (PosIt[0][0] != -1)
   {
	 if (PosIt[0][1] == 1)
	 {
		 PosIt[0][0] = -1;
		 PosIt[0][1] = -1;
		 curth = 0;
		 framenum =0;
		 fr2 = rand()%100;
		 fr3=0;
		 Idle->FirstFrame();
	 } else
	 {
		 PosIt[0][1]--;
		 ox = x;
		 oy = y;
		 x = PosIt[PosIt[0][0]+1-PosIt[0][1]][0];
		 y = PosIt[PosIt[0][0]+1-PosIt[0][1]][1];
		
		 if ((ox==x+1) && (oy==y+1)) { dir =5; }
		 if ((ox==x+1) && (oy==y-1)) { dir =3; }
		 if ((ox==x+2) && (oy==y  )) { dir =4; }
		 if ((ox==x-2) && (oy==y  )) { dir =1; }
		 if ((ox==x-1) && (oy==y+1)) { dir =0; }
		 if ((ox==x-1) && (oy==y-1)) { dir =2; }
		 Labpos = !Labpos;
		 Moving->SetDir(dir);
		 Moving->FirstFrame();
		 if (Labpos == TRUE)
		 {
			 for (i=0; i<4; i++)
			 {
				 Moving->NextFrame();
			 }
		 }
	 }
   } else
   {
    curth = 0;
	framenum =0;
	fr2 = rand()%100;
	fr3=0;
	Idle->FirstFrame();
   }
}

int TFRMPlayer::GetDir()
{
	int ox,oy,nx,ny;
	ox = x;
	oy = y;
	nx = PosIt[PosIt[0][0]+1-PosIt[0][1]-1][0];
	ny = PosIt[PosIt[0][0]+1-PosIt[0][1]-1][1];
	if ((ox+1==nx) && (oy+1==ny)) { return 5; }
	if ((ox+1==nx) && (oy-1==ny)) { return 3; }
	if ((ox+2==nx) && (oy  ==ny)) { return 4; }
	if ((ox-2==nx) && (oy  ==ny)) { return 1; }
	if ((ox-1==nx) && (oy+1==ny)) { return 0; }
	if ((ox-1==nx) && (oy-1==ny)) { return 2; }
	return 0;
}

void TFRMPlayer::Next(BlockType Block)
{
	int ix,iy;
	if (curth == 1)
	{
	//dir = GetDir();
	framenum++;
	Moving->NextFrame();
	if (framenum>Moving->Animation->framecounter/2-1) { framenum=0;Move(Block); }
	}
	if (curth == 0)
	{
     framenum++;
	 fr3++;
	 if ((framenum>fr2) && (fr3%2==0)) {Idle->NextFrame();}
	 if (framenum>fr2+Idle->Animation->framecounter*2) {framenum=0;Idle->FirstFrame();fr2 = rand()%100;}
	 if ((rand()%100==0) && (Player.bComputer))
	 {
		 ix=rand()%3+1;
		 iy=rand()%3+1;
		 switch (rand()%6)
		 {
		 case 0:GenerateTree(x-ix,y-iy,Block);break;
		 case 1:GenerateTree(x-ix,y+iy,Block);break;
		 case 2:GenerateTree(x-ix*2,y,Block);break;
		 case 3:GenerateTree(x+ix*2,y,Block);break;
		 case 4:GenerateTree(x+ix,y-iy,Block);break;
		 case 5:GenerateTree(x+ix,y+iy,Block);break;
		 }
	 }
	}
}

void TFRMPlayer::Draw(LPDIRECTDRAWSURFACE7 g_pDDSBack, int TerX, int TerY)
{
 PFRM Lista;
 switch (curth)
 {
 case 0:
	 Idle->SetDir(dir); 
	 Lista = (PFRM)Idle->Count6[dir];
	 BlitTo(g_pDDSBack,0,0,Lista->x,Lista->y,TerX+(x+226)*16+Lista->bx,TerY+(y)*12+Lista->by,DDBLTFAST_SRCCOLORKEY,Lista->FRM);
	 break;
 case 1:
	 Moving->SetDir(dir); 
	 Lista = (PFRM)Moving->Count6[dir];
	 BlitTo(g_pDDSBack,0,0,Lista->x,Lista->y,TerX+(ox+226)*16+Lista->bx,TerY+(oy)*12+Lista->by,DDBLTFAST_SRCCOLORKEY,Lista->FRM);
	 break;
 }
}