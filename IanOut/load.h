#ifndef __LOAD_H
#define __LOAD_H

#include "global.h"

extern std::string TempDir;

int LoadMusic(std::string filename);
int DeleteTempDir();
int InitializeMap();
int LoadLocation(int locnum, int entrnum);
int LoadFRMSingle(PFRMSingle *LoadIn, char* filename, int i);
int LoadFRMAnim(PFRMAnim *LoadIn, char* filename);
void CreateIanPalette();
void  LoadSplashScreen();
int InitApplication();

#endif //__LOAD_H