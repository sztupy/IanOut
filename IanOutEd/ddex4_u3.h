HRESULT LoadFRMSingle(PFRMSingle *LoadIn, HWND hWnd, char* filename, int i)
{
	HRESULT hRet;

	*LoadIn = new TFRMSingle();
	if ((hRet = (*LoadIn)->Load(hWnd, g_pDD, filename, i)) != DD_OK)
	   return InitFail(hWnd,hRet,"%s FAILED",filename); else return DD_OK;
}

void DrawTerep(int mousetyp,int x,int y)
{
	int i2,iy,ix,i;//,ix2,iy2;
	DDBLTFX                     ddbltfx;
	
    ZeroMemory(&ddbltfx, sizeof(ddbltfx));
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = RGB(21,21,21);
    g_pDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);

	for (ix=0; ix<100; ix++) {
		for (iy=0; iy<100; iy++) {         
			BlitTo(g_pDDSBack,0,0,80,35,ix*48+iy*32+TerX+9,iy*24-ix*12+TerY-5,DDBLTFAST_SRCCOLORKEY,MapInf->Tiles[(unsigned char)MapInf->Map[iy][ix]]->FRM->FRM);
			//DrawChar(ix*48+iy*32+TerX+9,iy*24-ix*12+TerY-5,0,'X');
			//DisplayNum(ix*48+iy*32+TerX+9,iy*24-ix*12+TerY-5,1,ix,2);
			//DisplayNum(ix*48+iy*32+TerX+9+10,iy*24-ix*12+TerY-5+10,1,iy,2);
		}
	}
	
	for (i2=0; i2<145; i2++)
	for (ix=0; ix<165; ix++) 
	{
		  iy = i2;
		  if ((x==ix) && (y==iy))
			  if ((MousY<460) && (MousY>15) && (mousetyp==0))
			  {
				 BlitTo(g_pDDSBack,0,0,32,16,TerX+(x+226)*16,TerY+(y)*12,DDBLTFAST_SRCCOLORKEY,Select->FRM->FRM);
			  }
		  i = WallInf->Map[ix][iy];
		  if ((i > 0) && (i<50) && (show_wall))
		  {	
			  i--;
			  BlitTo(g_pDDSBack,0,0,WallInf->Tiles[i]->FRM->x,WallInf->Tiles[i]->FRM->y,TerX+(ix+226)*16+WallMap[i][0],TerY+(iy)*12+WallMap[i][1],DDBLTFAST_SRCCOLORKEY,WallInf->Tiles[i]->FRM->FRM);
			  
		  }
		  i = ItemInf->Map[ix][iy];
		  if ((i > 0) && (i<50) && (show_item))
		  {	
			  i--;
			  BlitTo(g_pDDSBack,0,0,ItemInf->Tiles[i]->FRM->x,ItemInf->Tiles[i]->FRM->y,TerX+(ix+226)*16+ItemMap[i][0],TerY+(iy)*12+ItemMap[i][1],DDBLTFAST_SRCCOLORKEY,ItemInf->Tiles[i]->FRM->FRM);
			  
		  }
		
	  }

	if (putmode==1)
	{
	if ((MousY<460) && (MousY>15) && (Selected>=0) && (Selected<50))
	  BlitTo(g_pDDSBack,0,0,WallInf->Tiles[Selected]->FRM->x,WallInf->Tiles[Selected]->FRM->y,TerX+(x+226)*16+WallMap[Selected][0],TerY+(y)*12+WallMap[Selected][1],DDBLTFAST_SRCCOLORKEY,WallInf->Tiles[Selected]->FRM->FRM);
	}

	if (putmode==2)
	{
	if ((MousY<460) && (MousY>15) && (Selected>=0) && (Selected<14))
	  BlitTo(g_pDDSBack,0,0,ItemInf->Tiles[Selected]->FRM->x,ItemInf->Tiles[Selected]->FRM->y,TerX+(x+226)*16+ItemMap[Selected][0],TerY+(y)*12+ItemMap[Selected][1],DDBLTFAST_SRCCOLORKEY,ItemInf->Tiles[Selected]->FRM->FRM);
	}

	if (putmode==4)
	{
	if ((MousY<460) && (MousY>15) && (Selected>=0) && (Selected<6))
	  BlitTo(g_pDDSBack,0,0,80,35,TerX+(x+226)*16+9,TerY+(y)*12-5,DDBLTFAST_SRCCOLORKEY,MapInf->Tiles[Selected]->FRM->FRM);
	}
	

	if (block_plane)
	for (iy=0; iy<145; iy++)
	for (ix=0; ix<165; ix++) 
		if (BlockDat[ix][iy] != 0) DrawChar(TerX+(ix+226)*16+11,TerY+(iy)*12+2,0,'X');

	if (dot_plane)
	for (iy=0; iy<145; iy++)
	for (ix=0; ix<165; ix++)
	{
		if(iy%2==ix%2) DrawChar(TerX+(ix+226)*16+14,TerY+(iy)*12,4,(ix%8==Calculate[iy%8]) ? ((putmode==4) ? ('~'+1) : '.') : '.');
		//if(ix%8==Calculate[iy%8]) DrawChar(TerX+(ix+226)*16-2,TerY+(iy)*12,5,'.');
	}
				
	if (wall_plane)
	for (iy=0; iy<145; iy++)
	for (ix=0; ix<165; ix++) 
		if (WallInf->Map[ix][iy] != 0) DrawChar(TerX+(ix+226)*16+14,TerY+(iy)*12,3,'.');
							
	if (item_plane)
	for (iy=0; iy<145; iy++)
	for (ix=0; ix<165; ix++) 
		if (ItemInf->Map[ix][iy] != 0) DrawChar(TerX+(ix+226)*16+14,TerY+(iy)*12,1,'.');

}