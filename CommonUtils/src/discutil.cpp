#include "ddraw.h"
#include "io.h"
#include <string>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>

#include "datextract.h"

std::string FileSorrend[10];
PDATLocation DATSorrend[10];

extern void AddToLog(const char* text,...)
{
	FILE *f;
	if ((f = fopen("log.txt","a+")) == NULL)
		return;

	char szBuff[150];

	va_list                vl;
	va_start(vl, text);
    vsprintf(szBuff, text, vl);
	va_end(vl);

	fprintf(f,"%s\n",szBuff);
	fclose(f);
}

extern bool CanOpen(const char* fname)
{
	int stream;
	if ((stream = _open( fname,_O_BINARY)) == -1)
		return false;
	_close(stream);
	return true;
}

extern bool CanOpen2(std::string filename)
{
	int i;
	gzFile file;
	for (i=0; i<10; i++) {
		if (DATSorrend[i] != NULL) {
			file = DATSorrend[i]->OpenFile(filename);
			if (file != NULL) { gzclose(file); return true; }
		} else
		if (CanOpen((FileSorrend[i]+filename).c_str())) {
			return true;
		}
	}
	return false;	
}

extern void ClearLogFile(void)
{
	FILE *f;
	if ((f = fopen("log.txt","w")) == NULL)
		return;
	fclose(f);
}

extern void SetFile(int i, std::string filename)
{
	if (CanOpen(filename.c_str())) {
		FileSorrend[i] = "";
		DATSorrend[i] = new TDATLocation(filename);
	} else {
		FileSorrend[i] = filename;	
		DATSorrend[i] = NULL;
	}
}

extern gzFile __IOopen(std::string filename, const char* mode)
{
	int i;
	gzFile file;
	for (i=0; i<10; i++) {
		if (DATSorrend[i] != NULL) {
			file = DATSorrend[i]->OpenFile(filename);
			if (file != NULL) return file;
		} else
		if (CanOpen((FileSorrend[i]+filename).c_str())) {
			return gzopen((FileSorrend[i]+filename).c_str(),mode);
		}
	}
	return NULL;
}

extern std::string GetFile(std::string filename)
{
	int i;
	gzFile file;
	for (i=0; i<10; i++) {
		if (DATSorrend[i] != NULL) {
			file = DATSorrend[i]->OpenFile(filename);
			if (file != NULL) { gzclose(file); return DATSorrend[i]->fname+filename; }
		} else
		if (CanOpen((FileSorrend[i]+filename).c_str())) {
			return FileSorrend[i]+filename;
		}
	}
	return "";
}