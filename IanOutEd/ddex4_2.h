//-----------------------------------------------------------------------------
// Name: ReleaseAllObjects()
// Desc: Finished with all objects we use; release them
//-----------------------------------------------------------------------------

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
	if (FullScreen) delete FullScreen;
	if (MenuUp) delete MenuUp;
	if (Select) delete Select;
	if (Select2) delete Select2;
	if (MenuDown) delete MenuDown;
	
	if (MapInf) delete MapInf;
	if (WallInf) delete WallInf;
	if (ItemInf) delete ItemInf;
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

	FreeDirectInput();
}
