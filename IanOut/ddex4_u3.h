HRESULT LoadFRMSingle(PFRMSingle *LoadIn, HWND hWnd, char* filename, int i)
{
	HRESULT hRet;

	*LoadIn = new TFRMSingle();
	if ((hRet = (*LoadIn)->Load(hWnd, g_pDD, filename, i)) != DD_OK)
	   return InitFail(hWnd,hRet,"%s FAILED",filename); else return DD_OK;
}

void AddString(CString duma,int i2)
{
	int i;
	for (i=0; i<49; i++)
	{
	 szoveg[i]=szoveg[i+1];
	}
	char dumaelo;
	if (i2==1) dumaelo = '~'+1;
	szoveg[49]=dumaelo+duma;
}

void DrawStuff()
{
 int i;
 for (i=44; i<50; i++)
 {
	 IanOutText(28+(i-50),462+(i-50)*10,1,szoveg[i]);
 }
}

void DrawTerep(int mousetyp,int x,int y)
{
	int i2,iy,ix,i;
	DDBLTFX                     ddbltfx;
	
    ZeroMemory(&ddbltfx, sizeof(ddbltfx));
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = RGB(21,21,21);
    g_pDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);

	for (ix=0; ix<100; ix++) {
		for (iy=0; iy<100; iy++) {         
			BlitTo(g_pDDSBack,0,0,80,35,ix*48+iy*32+TerX+9,iy*24-ix*12+TerY-5,DDBLTFAST_SRCCOLORKEY,MapInf->Tiles[(unsigned char)MapInf->Map[iy][ix]]->FRM->FRM);
		}
	}

	for (i2=0; i2<145; i2++)
	for (ix=0; ix<165; ix++) 
	{
		  iy = i2;
		  if ((x==ix) && (y==iy))
			  if ((mousetyp==0) && (MouseIn(0,0,640,380))) BlitTo(g_pDDSBack,0,0,32,16,TerX+(x+226)*16,TerY+(y)*12,DDBLTFAST_SRCCOLORKEY,Select->FRM->FRM);
		  i = WallInf->Map[ix][iy];
		  if ((i > 0) && (i<50))
		  {	
			  i--;
			  BlitTo(g_pDDSBack,0,0,WallInf->Tiles[i]->FRM->x,WallInf->Tiles[i]->FRM->y,TerX+(ix+226)*16+WallMap[i][0],TerY+(iy)*12+WallMap[i][1],DDBLTFAST_SRCCOLORKEY,WallInf->Tiles[i]->FRM->FRM);
			  
		  }
		  i = ItemInf->Map[ix][iy];
		  if ((i > 0) && (i<50))
		  {	
			  i--;
			  BlitTo(g_pDDSBack,0,0,ItemInf->Tiles[i]->FRM->x,ItemInf->Tiles[i]->FRM->y,TerX+(ix+226)*16+ItemMap[i][0],TerY+(iy)*12+ItemMap[i][1],DDBLTFAST_SRCCOLORKEY,ItemInf->Tiles[i]->FRM->FRM);
			  
		  }
		  if ((Ian->curth == 1) && (Ian->dir == 5))
		  {
			if ((Ian->ox == ix) && (Ian->oy == iy)) Ian->Draw(g_pDDSBack,TerX,TerY);   
		  } else
		  {
		    if ((Ian->x == ix) && (Ian->y == iy)) Ian->Draw(g_pDDSBack,TerX,TerY);
		  }
		  if ((Aradesh->x == ix) && (Aradesh->y == iy)) Aradesh->Draw(g_pDDSBack,TerX,TerY);
		  for (i=0; i<10;i++)
		  {
			  if ((Paraszt[i]->x == ix) && (Paraszt[i]->y == iy)) Paraszt[i]->Draw(g_pDDSBack,TerX,TerY);
		  }
		  
	  }
}