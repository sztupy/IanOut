#ifndef __discutil_h
#define __discutil_h

#include <string>
#include "datextract.h"

void					SetLogLevel(int newlevel);
bool					CanOpen(const char* fname);
bool					CanOpen2(std::string filename);
void					AddToLog(int loglevel,const char* text,...);
void					ClearLogFile(void);
void					SetFile(int i, std::string filename);
std::string				GetFile(std::string filename);
std::string				GetMusicFile(std::string fname);
gzFile					__IOopen(std::string filename, const char* mode);

#endif // __utils_h