#include <string>

#ifndef __utils_h
#define __utils_h

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif	/* __cplusplus */

extern HRESULT              InitFail(HWND hWnd, HRESULT hRet, LPCTSTR szError, ...);
extern HRESULT				LogFail(HRESULT hRet, LPCTSTR szError,...);
extern int					atoi2(const char * str);

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
	void Set(std::string ifname, std::string imaster, std::string islave, std::string ibuffer)
	{
		fname=ifname;master=imaster;slave=islave;buffer=ibuffer;
	}
	int FindIt(std::string ifname, std::string imaster, std::string islave)
	{
		if ((ifname == fname) && (imaster == master) && (islave==slave)) return 0; else return 1;
	}
	std::string Get()
	{
		return buffer;
	}

//private:
	std::string fname;
	std::string master;
	std::string slave;
	std::string buffer;
};

#endif // __utils_h