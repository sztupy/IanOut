#ifndef __LOAD_H
#define __LOAD_H

#include "global.h"

int LoadLevel(const char* filename);
int LoadFRMSingle(PFRMSingle *LoadIn, char* filename, int i);
void CreateIanPalette();
void  LoadSplashScreen();
int InitApplication();

#endif //__LOAD_H