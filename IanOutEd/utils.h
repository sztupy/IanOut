#include "afx.h"

#ifndef __utils_h
#define __utils_h

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif	/* __cplusplus */

extern HRESULT              InitFail(HWND hWnd, HRESULT hRet, LPCTSTR szError, ...);

#ifdef __cplusplus
}
#endif	/* __cplusplus */

class TIniStr
{
public:
	TIniStr() 
	{
		fname="";master="";slave="";buffer="";
	};
	virtual ~TIniStr() {};
	void Set(CString ifname, CString imaster, CString islave, CString ibuffer)
	{
		fname=ifname;master=imaster;slave=islave;buffer=ibuffer;
	}
	int FindIt(CString ifname, CString imaster, CString islave)
	{
		if ((ifname == fname) && (imaster == master) && (islave==slave)) return 0; else return 1;
	}
	CString Get()
	{
		return buffer;
	}

//private:
	CString fname;
	CString master;
	CString slave;
	CString buffer;
};

#endif // __utils_h