#include "mouse.h"
//#include "engine.h"

/*HRESULT mouse::InitDirectInput( HWND hDlg,HINSTANCE g_hInst )
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
}*/

bool mouse::MouseIn(int x1,int y1,int x2,int y2)
{
	return ((MousX>=x1) && (MousX<=x2) && (MousY>=y1) && (MousY<=y2));
}

bool mouse::Pressed(int i)
{
	if ((i>=0) && (i<=2)) {
		return ((dims.buttons[i]==1) && !(olddims.buttons[i]==1)); 
	} else {
		if ((i>=3) && (i<=5)) {
			return ((olddims.buttons[i-3]==1) && !(dims.buttons[i-3]==1)); 
		} else {
			return (dims.buttons[i-6]==1); 
		}
	}
}

bool mouse::MInPr(int x1,int y1, int x2, int y2,int i)
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
/*HRESULT mouse::SetAcquire( )
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
*/
//-----------------------------------------------------------------------------
// Function: UpdateInputState
//
// Description: 
//      Get the input device's state and display it.
//
//-----------------------------------------------------------------------------
/*HRESULT mouse::UpdateInputState( )
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
*/



//-----------------------------------------------------------------------------
// Function: FreeDirectInput
//
// Description: 
//      Initialize the DirectInput variables.
//
//-----------------------------------------------------------------------------
/*HRESULT mouse::FreeDirectInput()
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

*/