#ifndef __DATEXTRACT_H
#define __DATEXTRACT_H

#include "io.h"
#include <string>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>

#include "../zlib/zlib.h"

class TDATLocation;

typedef TDATLocation *PDATLocation;

class TDATLocation {
public:
	TDATLocation(std::string filename);
	~TDATLocation();
	gzFile OpenFile(std::string filename);

	std::string fname;
private:
    char *buf, *ptr;
    int h_dat, f_pos,filename_len, byte_read;
    int file_siz, file_pos, file_real_siz, dat_siz, filelist_siz;
};

#endif //__DATEXTRACT_H