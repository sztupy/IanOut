//-----------------------------------------------------------------------------
// Name: ReleaseAllObjects()
// Desc: Finished with all objects we use; release them
//-----------------------------------------------------------------------------

void Nullaz(Skills *_Sk)
{
	int i;
	for (i=0; i<7; i++) _Sk->AlapSk.primary[i]=5;
	for (i=0; i<10; i++) _Sk->secondary[i]=0;
	for (i=0; i<18; i++) { _Sk->skills[i]=0; _Sk->tagSk[i]=FALSE; }
	for (i=0; i<50; i++) _Sk->perktra[i]=FALSE;
	for (i=0; i<7; i++) _Sk->beteg[i]=FALSE;
	_Sk->HP = 0;
	_Sk->AlapSk.HP = 10;
}

void CountSk(Skills *ISkill)
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
for (i=0; i<18; i++) if (ISkill->secondary[i]<=0) ISkill->secondary[i]=1;

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

HRESULT
InitFail(HWND hWnd, HRESULT hRet, LPCTSTR szError,...);

HRESULT InitDirectInput( HWND hDlg,HINSTANCE g_hInst )
{
    HRESULT hr;

    hr = DirectInputCreate( g_hInst, DIRECTINPUT_VERSION, &g_pDI, NULL );
    if ( FAILED(hr) ) 
        return InitFail(hDlg,hr,"DirectInputCreate Failed");
    hr = g_pDI->CreateDevice( GUID_SysMouse, &g_pMouse, NULL );
    if ( FAILED(hr) ) 
        return InitFail(hDlg,hr,"CreateDevice (mouse) failed");
    hr = g_pMouse->SetDataFormat( &c_dfDIMouse );
    if ( FAILED(hr) ) 
        return InitFail(hDlg,hr,"SetDataFormat (mouse) failed");
    hr = g_pMouse->SetCooperativeLevel( hDlg, 
                                        DISCL_EXCLUSIVE | DISCL_FOREGROUND);
    if ( FAILED(hr) ) 
        return InitFail(hDlg,hr,"SetCooperativeLevel (mouse) failed");

    return S_OK;
}

BOOL MouseIn(int x1,int y1,int x2,int y2)
{
	return ((MousX>=x1) && (MousX<=x2) && (MousY>=y1) && (MousY<=y2));
}

BOOL Pressed(int i)
{
	if ((i>=0) && (i<=2))
	return ((dims.rgbButtons[i] & 0x80) && !(olddims.rgbButtons[i] & 0x80)); else
	return ((olddims.rgbButtons[i-3] & 0x80) && !(dims.rgbButtons[i-3] & 0x80));
}

BOOL MInPr(int x1,int y1, int x2, int y2,int i)
{
	return (MouseIn(x1,y1,x2,y2) && (Pressed(i)));
}

//-----------------------------------------------------------------------------
// Function: SetAcquire
//
// Description: 
//      Acquire or unacquire the mouse, depending on if the app is active
//       Input device must be acquired before the GetDeviceState is called
//
//-----------------------------------------------------------------------------
HRESULT SetAcquire( )
{

    if (NULL == g_pMouse)
        return S_FALSE;

    if (g_bActive) 
    {
        g_pMouse->Acquire();
    } 
    else 
    {
        g_pMouse->Unacquire();
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Function: UpdateInputState
//
// Description: 
//      Get the input device's state and display it.
//
//-----------------------------------------------------------------------------
HRESULT UpdateInputState( )
{
   
    if (NULL != g_pMouse) 
    {
        HRESULT hr;

        hr = DIERR_INPUTLOST;

        while ( DIERR_INPUTLOST == hr ) 
        {
       
            hr = g_pMouse->GetDeviceState( sizeof(DIMOUSESTATE), &dims );

            if ( hr == DIERR_INPUTLOST )
            {
                hr = g_pMouse->Acquire();
                if ( FAILED(hr) )  
                    return hr;
            }
        }

        if ( FAILED(hr) )  
            return hr;
   
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Function: FreeDirectInput
//
// Description: 
//      Initialize the DirectInput variables.
//
//-----------------------------------------------------------------------------
HRESULT FreeDirectInput()
{
    if (NULL != g_pMouse) 
    {

        g_pMouse->Release();
        g_pMouse = NULL;
    }
    
    if (NULL != g_pDI) 
    {
        g_pDI->Release();
        g_pDI = NULL;
    }

    return S_OK;
}


static void
ReleaseAllObjects(void)
{
	int i;
    if (g_pDD != NULL)
    {
        if (g_pDDSPrimary != NULL)
        {
            g_pDDSPrimary->Release();
            g_pDDSPrimary = NULL;
        }
        if (g_pDDSOne != NULL)
        {
            g_pDDSOne->Release();
            g_pDDSOne = NULL;
        }
        if (g_pDDPal != NULL)
        {
            g_pDDPal->Release();
            g_pDDPal = NULL;
        }
	
	if (Mouse) delete Mouse;
	if (Mouse2) delete Mouse2;
	if (KezeloP) delete KezeloP;
	if (FullScreen) delete FullScreen;
	if (FullScreen2) delete FullScreen2;
	if (MenuUp) delete MenuUp;
	if (Select) delete Select;
	if (Select2) delete Select2;
	if (Ian) delete Ian;
	if (Aradesh) delete Aradesh;
	if (MenuDown) delete MenuDown;
	if (SkillDex) delete SkillDex;
	
	if (MapInf) delete MapInf;
	if (WallInf) delete WallInf;
	if (ItemInf) delete ItemInf;
	for (i=0; i<10; i++)
	{
		if (Paraszt[i]) delete Paraszt[i];
	}
	for (i=0; i<8; i++)
	{
		if (MouseScr[i][0]) delete MouseScr[i][0];
		if (MouseScr[i][1]) delete MouseScr[i][1];
	}
	for (i=0; i<2; i++)
	{
		if (Buttons[i]) delete Buttons[i];
	}

        g_pDD->Release();
        g_pDD = NULL;
    }
	
	for (i=0; i<75; i++)
	{
		LINIStr[i]->~TIniStr();
		LINIStr[i] = NULL;
	}
	delete PlyAnim;
	FreeDirectInput();
}
