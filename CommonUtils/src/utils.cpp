#include "ddraw.h"
#include "io.h"
#include "discutil.h"
#include <string>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>

extern "C" int atoi2(const char * str)
{
	if (str) {
		return atoi(str);
	} else return 0;
}

extern "C" HRESULT
LogFail(HRESULT hRet, LPCTSTR szError,...)
{
	char                        szBuff[128];
	char						szBuff2[128];
	char						szBuff3[50];
    va_list                     vl;
	va_start(vl, szError);
    vsprintf(szBuff, szError, vl);

	if (hRet == DD_OK) wsprintf(szBuff3,"DDERR_UNKNOWN");
	if (hRet == DDERR_ALREADYINITIALIZED  ) wsprintf(szBuff3,"DDERR_ALREADYINITIALIZED ");
  if (hRet == DDERR_BLTFASTCANTCLIP  ) wsprintf(szBuff3,"DDERR_BLTFASTCANTCLIP ");
  if (hRet == DDERR_CANNOTATTACHSURFACE  ) wsprintf(szBuff3,"DDERR_CANNOTATTACHSURFACE ");
  if (hRet == DDERR_CANNOTDETACHSURFACE  ) wsprintf(szBuff3,"DDERR_CANNOTDETACHSURFACE ");
  if (hRet == DDERR_CANTCREATEDC  ) wsprintf(szBuff3,"DDERR_CANTCREATEDC ");
  if (hRet == DDERR_CANTDUPLICATE  ) wsprintf(szBuff3,"DDERR_CANTDUPLICATE ");
  if (hRet == DDERR_CANTLOCKSURFACE  ) wsprintf(szBuff3,"DDERR_CANTLOCKSURFACE ");
  if (hRet == DDERR_CANTPAGELOCK  ) wsprintf(szBuff3,"DDERR_CANTPAGELOCK ");
  if (hRet == DDERR_CANTPAGEUNLOCK  ) wsprintf(szBuff3,"DDERR_CANTPAGEUNLOCK ");
  if (hRet == DDERR_CLIPPERISUSINGHWND  ) wsprintf(szBuff3,"DDERR_CLIPPERISUSINGHWND ");
  if (hRet == DDERR_COLORKEYNOTSET  ) wsprintf(szBuff3,"DDERR_COLORKEYNOTSET ");
  if (hRet == DDERR_CURRENTLYNOTAVAIL  ) wsprintf(szBuff3,"DDERR_CURRENTLYNOTAVAIL ");
  if (hRet == DDERR_DDSCAPSCOMPLEXREQUIRED  ) wsprintf(szBuff3,"DDERR_DDSCAPSCOMPLEXREQUIRED ");
  if (hRet == DDERR_DCALREADYCREATED  ) wsprintf(szBuff3,"DDERR_DCALREADYCREATED ");
  if (hRet == DDERR_DEVICEDOESNTOWNSURFACE  ) wsprintf(szBuff3,"DDERR_DEVICEDOESNTOWNSURFACE ");
  if (hRet == DDERR_DIRECTDRAWALREADYCREATED  ) wsprintf(szBuff3,"DDERR_DIRECTDRAWALREADYCREATED ");
  if (hRet == DDERR_EXCEPTION  ) wsprintf(szBuff3,"DDERR_EXCEPTION ");
  if (hRet == DDERR_EXCLUSIVEMODEALREADYSET  ) wsprintf(szBuff3,"DDERR_EXCLUSIVEMODEALREADYSET ");
  if (hRet == DDERR_EXPIRED  ) wsprintf(szBuff3,"DDERR_EXPIRED ");
  if (hRet == DDERR_GENERIC  ) wsprintf(szBuff3,"DDERR_GENERIC ");
  if (hRet == DDERR_HEIGHTALIGN  ) wsprintf(szBuff3,"DDERR_HEIGHTALIGN ");
  if (hRet == DDERR_HWNDALREADYSET  ) wsprintf(szBuff3,"DDERR_HWNDALREADYSET ");
  if (hRet == DDERR_HWNDSUBCLASSED  ) wsprintf(szBuff3,"DDERR_HWNDSUBCLASSED ");
  if (hRet == DDERR_IMPLICITLYCREATED  ) wsprintf(szBuff3,"DDERR_IMPLICITLYCREATED ");
  if (hRet == DDERR_INCOMPATIBLEPRIMARY  ) wsprintf(szBuff3,"DDERR_INCOMPATIBLEPRIMARY ");
  if (hRet == DDERR_INVALIDCAPS  ) wsprintf(szBuff3,"DDERR_INVALIDCAPS ");
  if (hRet == DDERR_INVALIDCLIPLIST  ) wsprintf(szBuff3,"DDERR_INVALIDCLIPLIST ");
  if (hRet == DDERR_INVALIDDIRECTDRAWGUID  ) wsprintf(szBuff3,"DDERR_INVALIDDIRECTDRAWGUID ");
  if (hRet == DDERR_INVALIDMODE  ) wsprintf(szBuff3,"DDERR_INVALIDMODE ");
  if (hRet == DDERR_INVALIDOBJECT  ) wsprintf(szBuff3,"DDERR_INVALIDOBJECT ");
  if (hRet == DDERR_INVALIDPARAMS  ) wsprintf(szBuff3,"DDERR_INVALIDPARAMS ");
  if (hRet == DDERR_INVALIDPIXELFORMAT  ) wsprintf(szBuff3,"DDERR_INVALIDPIXELFORMAT ");
  if (hRet == DDERR_INVALIDPOSITION  ) wsprintf(szBuff3,"DDERR_INVALIDPOSITION ");
  if (hRet == DDERR_INVALIDRECT  ) wsprintf(szBuff3,"DDERR_INVALIDRECT ");
  if (hRet == DDERR_INVALIDSTREAM  ) wsprintf(szBuff3,"DDERR_INVALIDSTREAM ");
  if (hRet == DDERR_INVALIDSURFACETYPE  ) wsprintf(szBuff3,"DDERR_INVALIDSURFACETYPE ");
  if (hRet == DDERR_LOCKEDSURFACES  ) wsprintf(szBuff3,"DDERR_LOCKEDSURFACES ");
  if (hRet == DDERR_MOREDATA  ) wsprintf(szBuff3,"DDERR_MOREDATA ");
  if (hRet == DDERR_NEWMODE  ) wsprintf(szBuff3,"DDERR_NEWMODE ");
  if (hRet == DDERR_NO3D  ) wsprintf(szBuff3,"DDERR_NO3D ");
  if (hRet == DDERR_NOALPHAHW  ) wsprintf(szBuff3,"DDERR_NOALPHAHW ");
  if (hRet == DDERR_NOBLTHW  ) wsprintf(szBuff3,"DDERR_NOBLTHW ");
  if (hRet == DDERR_NOCLIPLIST  ) wsprintf(szBuff3,"DDERR_NOCLIPLIST ");
  if (hRet == DDERR_NOCLIPPERATTACHED  ) wsprintf(szBuff3,"DDERR_NOCLIPPERATTACHED ");
  if (hRet == DDERR_NOCOLORCONVHW  ) wsprintf(szBuff3,"DDERR_NOCOLORCONVHW ");
  if (hRet == DDERR_NOCOLORKEY  ) wsprintf(szBuff3,"DDERR_NOCOLORKEY ");
  if (hRet == DDERR_NOCOLORKEYHW  ) wsprintf(szBuff3,"DDERR_NOCOLORKEYHW ");
  if (hRet == DDERR_NOCOOPERATIVELEVELSET  ) wsprintf(szBuff3,"DDERR_NOCOOPERATIVELEVELSET ");
  if (hRet == DDERR_NODC  ) wsprintf(szBuff3,"DDERR_NODC ");
  if (hRet == DDERR_NODDROPSHW  ) wsprintf(szBuff3,"DDERR_NODDROPSHW ");
  if (hRet == DDERR_NODIRECTDRAWHW  ) wsprintf(szBuff3,"DDERR_NODIRECTDRAWHW ");
  if (hRet == DDERR_NODIRECTDRAWSUPPORT  ) wsprintf(szBuff3,"DDERR_NODIRECTDRAWSUPPORT ");
  if (hRet == DDERR_NODRIVERSUPPORT  ) wsprintf(szBuff3,"DDERR_NODRIVERSUPPORT ");
  if (hRet == DDERR_NOEMULATION  ) wsprintf(szBuff3,"DDERR_NOEMULATION ");
  if (hRet == DDERR_NOEXCLUSIVEMODE  ) wsprintf(szBuff3,"DDERR_NOEXCLUSIVEMODE ");
  if (hRet == DDERR_NOFLIPHW  ) wsprintf(szBuff3,"DDERR_NOFLIPHW ");
  if (hRet == DDERR_NOFOCUSWINDOW  ) wsprintf(szBuff3,"DDERR_NOFOCUSWINDOW ");
  if (hRet == DDERR_NOGDI  ) wsprintf(szBuff3,"DDERR_NOGDI ");
  if (hRet == DDERR_NOHWND  ) wsprintf(szBuff3,"DDERR_NOHWND ");
  if (hRet == DDERR_NOMIPMAPHW  ) wsprintf(szBuff3,"DDERR_NOMIPMAPHW ");
  if (hRet == DDERR_NOMIRRORHW  ) wsprintf(szBuff3,"DDERR_NOMIRRORHW ");
  if (hRet == DDERR_NOMONITORINFORMATION  ) wsprintf(szBuff3,"DDERR_NOMONITORINFORMATION ");
  if (hRet == DDERR_NONONLOCALVIDMEM  ) wsprintf(szBuff3,"DDERR_NONONLOCALVIDMEM ");
  if (hRet == DDERR_NOOPTIMIZEHW  ) wsprintf(szBuff3,"DDERR_NOOPTIMIZEHW ");
  if (hRet == DDERR_NOOVERLAYDEST  ) wsprintf(szBuff3,"DDERR_NOOVERLAYDEST ");
  if (hRet == DDERR_NOOVERLAYHW  ) wsprintf(szBuff3,"DDERR_NOOVERLAYHW ");
  if (hRet == DDERR_NOPALETTEATTACHED  ) wsprintf(szBuff3,"DDERR_NOPALETTEATTACHED ");
  if (hRet == DDERR_NOPALETTEHW  ) wsprintf(szBuff3,"DDERR_NOPALETTEHW ");
  if (hRet == DDERR_NORASTEROPHW  ) wsprintf(szBuff3,"DDERR_NORASTEROPHW ");
  if (hRet == DDERR_NOROTATIONHW  ) wsprintf(szBuff3,"DDERR_NOROTATIONHW ");
  if (hRet == DDERR_NOSTEREOHARDWARE  ) wsprintf(szBuff3,"DDERR_NOSTEREOHARDWARE ");
  if (hRet == DDERR_NOSTRETCHHW  ) wsprintf(szBuff3,"DDERR_NOSTRETCHHW ");
  if (hRet == DDERR_NOSURFACELEFT  ) wsprintf(szBuff3,"DDERR_NOSURFACELEFT ");
  if (hRet == DDERR_NOT4BITCOLOR  ) wsprintf(szBuff3,"DDERR_NOT4BITCOLOR ");
  if (hRet == DDERR_NOT4BITCOLORINDEX  ) wsprintf(szBuff3,"DDERR_NOT4BITCOLORINDEX ");
  if (hRet == DDERR_NOT8BITCOLOR  ) wsprintf(szBuff3,"DDERR_NOT8BITCOLOR ");
  if (hRet == DDERR_NOTAOVERLAYSURFACE  ) wsprintf(szBuff3,"DDERR_NOTAOVERLAYSURFACE ");
  if (hRet == DDERR_NOTEXTUREHW  ) wsprintf(szBuff3,"DDERR_NOTEXTUREHW ");
  if (hRet == DDERR_NOTFLIPPABLE  ) wsprintf(szBuff3,"DDERR_NOTFLIPPABLE ");
  if (hRet == DDERR_NOTFOUND  ) wsprintf(szBuff3,"DDERR_NOTFOUND ");
  if (hRet == DDERR_NOTINITIALIZED  ) wsprintf(szBuff3,"DDERR_NOTINITIALIZED ");
  if (hRet == DDERR_NOTLOADED  ) wsprintf(szBuff3,"DDERR_NOTLOADED ");
  if (hRet == DDERR_NOTLOCKED  ) wsprintf(szBuff3,"DDERR_NOTLOCKED ");
  if (hRet == DDERR_NOTPAGELOCKED  ) wsprintf(szBuff3,"DDERR_NOTPAGELOCKED ");
  if (hRet == DDERR_NOTPALETTIZED  ) wsprintf(szBuff3,"DDERR_NOTPALETTIZED ");
  if (hRet == DDERR_NOVSYNCHW  ) wsprintf(szBuff3,"DDERR_NOVSYNCHW ");
  if (hRet == DDERR_NOZBUFFERHW  ) wsprintf(szBuff3,"DDERR_NOZBUFFERHW ");
  if (hRet == DDERR_NOZOVERLAYHW  ) wsprintf(szBuff3,"DDERR_NOZOVERLAYHW ");
  if (hRet == DDERR_OUTOFCAPS  ) wsprintf(szBuff3,"DDERR_OUTOFCAPS ");
  if (hRet == DDERR_OUTOFMEMORY  ) wsprintf(szBuff3,"DDERR_OUTOFMEMORY ");
  if (hRet == DDERR_OUTOFVIDEOMEMORY  ) wsprintf(szBuff3,"DDERR_OUTOFVIDEOMEMORY ");
  if (hRet == DDERR_OVERLAPPINGRECTS  ) wsprintf(szBuff3,"DDERR_OVERLAPPINGRECTS ");
  if (hRet == DDERR_OVERLAYCANTCLIP  ) wsprintf(szBuff3,"DDERR_OVERLAYCANTCLIP ");
  if (hRet == DDERR_OVERLAYCOLORKEYONLYONEACTIVE  ) wsprintf(szBuff3,"DDERR_OVERLAYCOLORKEYONLYONEACTIVE ");
  if (hRet == DDERR_OVERLAYNOTVISIBLE  ) wsprintf(szBuff3,"DDERR_OVERLAYNOTVISIBLE ");
  if (hRet == DDERR_PALETTEBUSY  ) wsprintf(szBuff3,"DDERR_PALETTEBUSY ");
  if (hRet == DDERR_PRIMARYSURFACEALREADYEXISTS  ) wsprintf(szBuff3,"DDERR_PRIMARYSURFACEALREADYEXISTS ");
  if (hRet == DDERR_REGIONTOOSMALL  ) wsprintf(szBuff3,"DDERR_REGIONTOOSMALL ");
  if (hRet == DDERR_SURFACEALREADYATTACHED  ) wsprintf(szBuff3,"DDERR_SURFACEALREADYATTACHED ");
  if (hRet == DDERR_SURFACEALREADYDEPENDENT  ) wsprintf(szBuff3,"DDERR_SURFACEALREADYDEPENDENT ");
  if (hRet == DDERR_SURFACEBUSY  ) wsprintf(szBuff3,"DDERR_SURFACEBUSY ");
  if (hRet == DDERR_SURFACEISOBSCURED  ) wsprintf(szBuff3,"DDERR_SURFACEISOBSCURED ");
  if (hRet == DDERR_SURFACELOST  ) wsprintf(szBuff3,"DDERR_SURFACELOST ");
  if (hRet == DDERR_SURFACENOTATTACHED  ) wsprintf(szBuff3,"DDERR_SURFACENOTATTACHED ");
  if (hRet == DDERR_TESTFINISHED  ) wsprintf(szBuff3,"DDERR_TESTFINISHED ");
  if (hRet == DDERR_TOOBIGHEIGHT  ) wsprintf(szBuff3,"DDERR_TOOBIGHEIGHT ");
  if (hRet == DDERR_TOOBIGSIZE  ) wsprintf(szBuff3,"DDERR_TOOBIGSIZE ");
  if (hRet == DDERR_TOOBIGWIDTH  ) wsprintf(szBuff3,"DDERR_TOOBIGWIDTH ");
  if (hRet == DDERR_UNSUPPORTED  ) wsprintf(szBuff3,"DDERR_UNSUPPORTED ");
  if (hRet == DDERR_UNSUPPORTEDFORMAT  ) wsprintf(szBuff3,"DDERR_UNSUPPORTEDFORMAT ");
  if (hRet == DDERR_UNSUPPORTEDMASK  ) wsprintf(szBuff3,"DDERR_UNSUPPORTEDMASK ");
  if (hRet == DDERR_UNSUPPORTEDMODE  ) wsprintf(szBuff3,"DDERR_UNSUPPORTEDMODE ");
  if (hRet == DDERR_VERTICALBLANKINPROGRESS  ) wsprintf(szBuff3,"DDERR_VERTICALBLANKINPROGRESS ");
  if (hRet == DDERR_VIDEONOTACTIVE  ) wsprintf(szBuff3,"DDERR_VIDEONOTACTIVE ");
  if (hRet == DDERR_WASSTILLDRAWING  ) wsprintf(szBuff3,"DDERR_WASSTILLDRAWING ");
  if (hRet == DDERR_WRONGMODE  ) wsprintf(szBuff3,"DDERR_WRONGMODE ");
  if (hRet == DDERR_XALIGN  ) wsprintf(szBuff3,"DDERR_XALIGN ");
	
  wsprintf(szBuff2,"%s (%s)",szBuff,szBuff3);
    
  AddToLog(szBuff2);

  va_end(vl);
  return hRet;
}

extern "C" HRESULT
InitFail(HWND hWnd, HRESULT hRet, LPCTSTR szError,...)
{
	char                        szBuff[128];
	char						szBuff2[128];
	char						szBuff3[50];
    va_list                     vl;
 
    va_start(vl, szError);
    vsprintf(szBuff, szError, vl);
	if (hRet == DD_OK) wsprintf(szBuff3,"DDERR_UNKNOWN");
	if (hRet == DDERR_ALREADYINITIALIZED  ) wsprintf(szBuff3,"DDERR_ALREADYINITIALIZED ");
  if (hRet == DDERR_BLTFASTCANTCLIP  ) wsprintf(szBuff3,"DDERR_BLTFASTCANTCLIP ");
  if (hRet == DDERR_CANNOTATTACHSURFACE  ) wsprintf(szBuff3,"DDERR_CANNOTATTACHSURFACE ");
  if (hRet == DDERR_CANNOTDETACHSURFACE  ) wsprintf(szBuff3,"DDERR_CANNOTDETACHSURFACE ");
  if (hRet == DDERR_CANTCREATEDC  ) wsprintf(szBuff3,"DDERR_CANTCREATEDC ");
  if (hRet == DDERR_CANTDUPLICATE  ) wsprintf(szBuff3,"DDERR_CANTDUPLICATE ");
  if (hRet == DDERR_CANTLOCKSURFACE  ) wsprintf(szBuff3,"DDERR_CANTLOCKSURFACE ");
  if (hRet == DDERR_CANTPAGELOCK  ) wsprintf(szBuff3,"DDERR_CANTPAGELOCK ");
  if (hRet == DDERR_CANTPAGEUNLOCK  ) wsprintf(szBuff3,"DDERR_CANTPAGEUNLOCK ");
  if (hRet == DDERR_CLIPPERISUSINGHWND  ) wsprintf(szBuff3,"DDERR_CLIPPERISUSINGHWND ");
  if (hRet == DDERR_COLORKEYNOTSET  ) wsprintf(szBuff3,"DDERR_COLORKEYNOTSET ");
  if (hRet == DDERR_CURRENTLYNOTAVAIL  ) wsprintf(szBuff3,"DDERR_CURRENTLYNOTAVAIL ");
  if (hRet == DDERR_DDSCAPSCOMPLEXREQUIRED  ) wsprintf(szBuff3,"DDERR_DDSCAPSCOMPLEXREQUIRED ");
  if (hRet == DDERR_DCALREADYCREATED  ) wsprintf(szBuff3,"DDERR_DCALREADYCREATED ");
  if (hRet == DDERR_DEVICEDOESNTOWNSURFACE  ) wsprintf(szBuff3,"DDERR_DEVICEDOESNTOWNSURFACE ");
  if (hRet == DDERR_DIRECTDRAWALREADYCREATED  ) wsprintf(szBuff3,"DDERR_DIRECTDRAWALREADYCREATED ");
  if (hRet == DDERR_EXCEPTION  ) wsprintf(szBuff3,"DDERR_EXCEPTION ");
  if (hRet == DDERR_EXCLUSIVEMODEALREADYSET  ) wsprintf(szBuff3,"DDERR_EXCLUSIVEMODEALREADYSET ");
  if (hRet == DDERR_EXPIRED  ) wsprintf(szBuff3,"DDERR_EXPIRED ");
  if (hRet == DDERR_GENERIC  ) wsprintf(szBuff3,"DDERR_GENERIC ");
  if (hRet == DDERR_HEIGHTALIGN  ) wsprintf(szBuff3,"DDERR_HEIGHTALIGN ");
  if (hRet == DDERR_HWNDALREADYSET  ) wsprintf(szBuff3,"DDERR_HWNDALREADYSET ");
  if (hRet == DDERR_HWNDSUBCLASSED  ) wsprintf(szBuff3,"DDERR_HWNDSUBCLASSED ");
  if (hRet == DDERR_IMPLICITLYCREATED  ) wsprintf(szBuff3,"DDERR_IMPLICITLYCREATED ");
  if (hRet == DDERR_INCOMPATIBLEPRIMARY  ) wsprintf(szBuff3,"DDERR_INCOMPATIBLEPRIMARY ");
  if (hRet == DDERR_INVALIDCAPS  ) wsprintf(szBuff3,"DDERR_INVALIDCAPS ");
  if (hRet == DDERR_INVALIDCLIPLIST  ) wsprintf(szBuff3,"DDERR_INVALIDCLIPLIST ");
  if (hRet == DDERR_INVALIDDIRECTDRAWGUID  ) wsprintf(szBuff3,"DDERR_INVALIDDIRECTDRAWGUID ");
  if (hRet == DDERR_INVALIDMODE  ) wsprintf(szBuff3,"DDERR_INVALIDMODE ");
  if (hRet == DDERR_INVALIDOBJECT  ) wsprintf(szBuff3,"DDERR_INVALIDOBJECT ");
  if (hRet == DDERR_INVALIDPARAMS  ) wsprintf(szBuff3,"DDERR_INVALIDPARAMS ");
  if (hRet == DDERR_INVALIDPIXELFORMAT  ) wsprintf(szBuff3,"DDERR_INVALIDPIXELFORMAT ");
  if (hRet == DDERR_INVALIDPOSITION  ) wsprintf(szBuff3,"DDERR_INVALIDPOSITION ");
  if (hRet == DDERR_INVALIDRECT  ) wsprintf(szBuff3,"DDERR_INVALIDRECT ");
  if (hRet == DDERR_INVALIDSTREAM  ) wsprintf(szBuff3,"DDERR_INVALIDSTREAM ");
  if (hRet == DDERR_INVALIDSURFACETYPE  ) wsprintf(szBuff3,"DDERR_INVALIDSURFACETYPE ");
  if (hRet == DDERR_LOCKEDSURFACES  ) wsprintf(szBuff3,"DDERR_LOCKEDSURFACES ");
  if (hRet == DDERR_MOREDATA  ) wsprintf(szBuff3,"DDERR_MOREDATA ");
  if (hRet == DDERR_NEWMODE  ) wsprintf(szBuff3,"DDERR_NEWMODE ");
  if (hRet == DDERR_NO3D  ) wsprintf(szBuff3,"DDERR_NO3D ");
  if (hRet == DDERR_NOALPHAHW  ) wsprintf(szBuff3,"DDERR_NOALPHAHW ");
  if (hRet == DDERR_NOBLTHW  ) wsprintf(szBuff3,"DDERR_NOBLTHW ");
  if (hRet == DDERR_NOCLIPLIST  ) wsprintf(szBuff3,"DDERR_NOCLIPLIST ");
  if (hRet == DDERR_NOCLIPPERATTACHED  ) wsprintf(szBuff3,"DDERR_NOCLIPPERATTACHED ");
  if (hRet == DDERR_NOCOLORCONVHW  ) wsprintf(szBuff3,"DDERR_NOCOLORCONVHW ");
  if (hRet == DDERR_NOCOLORKEY  ) wsprintf(szBuff3,"DDERR_NOCOLORKEY ");
  if (hRet == DDERR_NOCOLORKEYHW  ) wsprintf(szBuff3,"DDERR_NOCOLORKEYHW ");
  if (hRet == DDERR_NOCOOPERATIVELEVELSET  ) wsprintf(szBuff3,"DDERR_NOCOOPERATIVELEVELSET ");
  if (hRet == DDERR_NODC  ) wsprintf(szBuff3,"DDERR_NODC ");
  if (hRet == DDERR_NODDROPSHW  ) wsprintf(szBuff3,"DDERR_NODDROPSHW ");
  if (hRet == DDERR_NODIRECTDRAWHW  ) wsprintf(szBuff3,"DDERR_NODIRECTDRAWHW ");
  if (hRet == DDERR_NODIRECTDRAWSUPPORT  ) wsprintf(szBuff3,"DDERR_NODIRECTDRAWSUPPORT ");
  if (hRet == DDERR_NODRIVERSUPPORT  ) wsprintf(szBuff3,"DDERR_NODRIVERSUPPORT ");
  if (hRet == DDERR_NOEMULATION  ) wsprintf(szBuff3,"DDERR_NOEMULATION ");
  if (hRet == DDERR_NOEXCLUSIVEMODE  ) wsprintf(szBuff3,"DDERR_NOEXCLUSIVEMODE ");
  if (hRet == DDERR_NOFLIPHW  ) wsprintf(szBuff3,"DDERR_NOFLIPHW ");
  if (hRet == DDERR_NOFOCUSWINDOW  ) wsprintf(szBuff3,"DDERR_NOFOCUSWINDOW ");
  if (hRet == DDERR_NOGDI  ) wsprintf(szBuff3,"DDERR_NOGDI ");
  if (hRet == DDERR_NOHWND  ) wsprintf(szBuff3,"DDERR_NOHWND ");
  if (hRet == DDERR_NOMIPMAPHW  ) wsprintf(szBuff3,"DDERR_NOMIPMAPHW ");
  if (hRet == DDERR_NOMIRRORHW  ) wsprintf(szBuff3,"DDERR_NOMIRRORHW ");
  if (hRet == DDERR_NOMONITORINFORMATION  ) wsprintf(szBuff3,"DDERR_NOMONITORINFORMATION ");
  if (hRet == DDERR_NONONLOCALVIDMEM  ) wsprintf(szBuff3,"DDERR_NONONLOCALVIDMEM ");
  if (hRet == DDERR_NOOPTIMIZEHW  ) wsprintf(szBuff3,"DDERR_NOOPTIMIZEHW ");
  if (hRet == DDERR_NOOVERLAYDEST  ) wsprintf(szBuff3,"DDERR_NOOVERLAYDEST ");
  if (hRet == DDERR_NOOVERLAYHW  ) wsprintf(szBuff3,"DDERR_NOOVERLAYHW ");
  if (hRet == DDERR_NOPALETTEATTACHED  ) wsprintf(szBuff3,"DDERR_NOPALETTEATTACHED ");
  if (hRet == DDERR_NOPALETTEHW  ) wsprintf(szBuff3,"DDERR_NOPALETTEHW ");
  if (hRet == DDERR_NORASTEROPHW  ) wsprintf(szBuff3,"DDERR_NORASTEROPHW ");
  if (hRet == DDERR_NOROTATIONHW  ) wsprintf(szBuff3,"DDERR_NOROTATIONHW ");
  if (hRet == DDERR_NOSTEREOHARDWARE  ) wsprintf(szBuff3,"DDERR_NOSTEREOHARDWARE ");
  if (hRet == DDERR_NOSTRETCHHW  ) wsprintf(szBuff3,"DDERR_NOSTRETCHHW ");
  if (hRet == DDERR_NOSURFACELEFT  ) wsprintf(szBuff3,"DDERR_NOSURFACELEFT ");
  if (hRet == DDERR_NOT4BITCOLOR  ) wsprintf(szBuff3,"DDERR_NOT4BITCOLOR ");
  if (hRet == DDERR_NOT4BITCOLORINDEX  ) wsprintf(szBuff3,"DDERR_NOT4BITCOLORINDEX ");
  if (hRet == DDERR_NOT8BITCOLOR  ) wsprintf(szBuff3,"DDERR_NOT8BITCOLOR ");
  if (hRet == DDERR_NOTAOVERLAYSURFACE  ) wsprintf(szBuff3,"DDERR_NOTAOVERLAYSURFACE ");
  if (hRet == DDERR_NOTEXTUREHW  ) wsprintf(szBuff3,"DDERR_NOTEXTUREHW ");
  if (hRet == DDERR_NOTFLIPPABLE  ) wsprintf(szBuff3,"DDERR_NOTFLIPPABLE ");
  if (hRet == DDERR_NOTFOUND  ) wsprintf(szBuff3,"DDERR_NOTFOUND ");
  if (hRet == DDERR_NOTINITIALIZED  ) wsprintf(szBuff3,"DDERR_NOTINITIALIZED ");
  if (hRet == DDERR_NOTLOADED  ) wsprintf(szBuff3,"DDERR_NOTLOADED ");
  if (hRet == DDERR_NOTLOCKED  ) wsprintf(szBuff3,"DDERR_NOTLOCKED ");
  if (hRet == DDERR_NOTPAGELOCKED  ) wsprintf(szBuff3,"DDERR_NOTPAGELOCKED ");
  if (hRet == DDERR_NOTPALETTIZED  ) wsprintf(szBuff3,"DDERR_NOTPALETTIZED ");
  if (hRet == DDERR_NOVSYNCHW  ) wsprintf(szBuff3,"DDERR_NOVSYNCHW ");
  if (hRet == DDERR_NOZBUFFERHW  ) wsprintf(szBuff3,"DDERR_NOZBUFFERHW ");
  if (hRet == DDERR_NOZOVERLAYHW  ) wsprintf(szBuff3,"DDERR_NOZOVERLAYHW ");
  if (hRet == DDERR_OUTOFCAPS  ) wsprintf(szBuff3,"DDERR_OUTOFCAPS ");
  if (hRet == DDERR_OUTOFMEMORY  ) wsprintf(szBuff3,"DDERR_OUTOFMEMORY ");
  if (hRet == DDERR_OUTOFVIDEOMEMORY  ) wsprintf(szBuff3,"DDERR_OUTOFVIDEOMEMORY ");
  if (hRet == DDERR_OVERLAPPINGRECTS  ) wsprintf(szBuff3,"DDERR_OVERLAPPINGRECTS ");
  if (hRet == DDERR_OVERLAYCANTCLIP  ) wsprintf(szBuff3,"DDERR_OVERLAYCANTCLIP ");
  if (hRet == DDERR_OVERLAYCOLORKEYONLYONEACTIVE  ) wsprintf(szBuff3,"DDERR_OVERLAYCOLORKEYONLYONEACTIVE ");
  if (hRet == DDERR_OVERLAYNOTVISIBLE  ) wsprintf(szBuff3,"DDERR_OVERLAYNOTVISIBLE ");
  if (hRet == DDERR_PALETTEBUSY  ) wsprintf(szBuff3,"DDERR_PALETTEBUSY ");
  if (hRet == DDERR_PRIMARYSURFACEALREADYEXISTS  ) wsprintf(szBuff3,"DDERR_PRIMARYSURFACEALREADYEXISTS ");
  if (hRet == DDERR_REGIONTOOSMALL  ) wsprintf(szBuff3,"DDERR_REGIONTOOSMALL ");
  if (hRet == DDERR_SURFACEALREADYATTACHED  ) wsprintf(szBuff3,"DDERR_SURFACEALREADYATTACHED ");
  if (hRet == DDERR_SURFACEALREADYDEPENDENT  ) wsprintf(szBuff3,"DDERR_SURFACEALREADYDEPENDENT ");
  if (hRet == DDERR_SURFACEBUSY  ) wsprintf(szBuff3,"DDERR_SURFACEBUSY ");
  if (hRet == DDERR_SURFACEISOBSCURED  ) wsprintf(szBuff3,"DDERR_SURFACEISOBSCURED ");
  if (hRet == DDERR_SURFACELOST  ) wsprintf(szBuff3,"DDERR_SURFACELOST ");
  if (hRet == DDERR_SURFACENOTATTACHED  ) wsprintf(szBuff3,"DDERR_SURFACENOTATTACHED ");
  if (hRet == DDERR_TESTFINISHED  ) wsprintf(szBuff3,"DDERR_TESTFINISHED ");
  if (hRet == DDERR_TOOBIGHEIGHT  ) wsprintf(szBuff3,"DDERR_TOOBIGHEIGHT ");
  if (hRet == DDERR_TOOBIGSIZE  ) wsprintf(szBuff3,"DDERR_TOOBIGSIZE ");
  if (hRet == DDERR_TOOBIGWIDTH  ) wsprintf(szBuff3,"DDERR_TOOBIGWIDTH ");
  if (hRet == DDERR_UNSUPPORTED  ) wsprintf(szBuff3,"DDERR_UNSUPPORTED ");
  if (hRet == DDERR_UNSUPPORTEDFORMAT  ) wsprintf(szBuff3,"DDERR_UNSUPPORTEDFORMAT ");
  if (hRet == DDERR_UNSUPPORTEDMASK  ) wsprintf(szBuff3,"DDERR_UNSUPPORTEDMASK ");
  if (hRet == DDERR_UNSUPPORTEDMODE  ) wsprintf(szBuff3,"DDERR_UNSUPPORTEDMODE ");
  if (hRet == DDERR_VERTICALBLANKINPROGRESS  ) wsprintf(szBuff3,"DDERR_VERTICALBLANKINPROGRESS ");
  if (hRet == DDERR_VIDEONOTACTIVE  ) wsprintf(szBuff3,"DDERR_VIDEONOTACTIVE ");
  if (hRet == DDERR_WASSTILLDRAWING  ) wsprintf(szBuff3,"DDERR_WASSTILLDRAWING ");
  if (hRet == DDERR_WRONGMODE  ) wsprintf(szBuff3,"DDERR_WRONGMODE ");
  if (hRet == DDERR_XALIGN  ) wsprintf(szBuff3,"DDERR_XALIGN ");

	wsprintf(szBuff2,"%s (%s)",szBuff,szBuff3);
    AddToLog(szBuff2);
    MessageBox(hWnd, szBuff2, "IanOut: A Post-Nuclear Ian-Playing Game", MB_OK);
	PostMessage(hWnd, WM_CLOSE, 0, 0);
    //DestroyWindow(hWnd);
    va_end(vl);
    return hRet;
}