#include "ddraw.h"
#include "discutil.h"
#include "io.h"
#include <string>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>

#include "datextract.h"

std::string FileSorrend[16];
PDATLocation DATSorrend[16];
int ActualLogLevel = 0;

void SetLogLevel(int newlevel)
{
	if (newlevel<0) newlevel=0;
	ActualLogLevel = newlevel;
	AddToLog(0,"Global> Entering loglevel %i",newlevel);
}

extern void AddToLog(int loglevel, const char* text,...)
{
	if (loglevel<=ActualLogLevel) {
	FILE *f;
	if ((f = fopen("log.txt","a+")) == NULL)
		return;

	char szBuff[150];

	va_list                vl;
	va_start(vl, text);
    vsprintf(szBuff, text, vl);
	va_end(vl);

	if (loglevel>0) fprintf(f,"%s (%i)\n",szBuff,loglevel); else fprintf(f,"%s\n",szBuff);
	if (loglevel>0) printf("%s (%i)\n",szBuff,loglevel); else printf("%s\n",szBuff);
	fclose(f);
	}
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
	if (CanOpen((filename).c_str())) return true;
	for (i=0; i<16; i++) 
	if (FileSorrend[i]!="")
	{
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
		FileSorrend[i] = "-";
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
	if (CanOpen((filename).c_str())) {
			return gzopen((filename).c_str(),mode);
		}
	for (i=0; i<16; i++)
	if (FileSorrend[i]!="")
	{
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
	if (CanOpen((filename).c_str())) {
			return filename;
		}
	for (i=0; i<16; i++)
	if (FileSorrend[i]!="")
	{
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

extern std::string GetMusicFile(std::string fname)
{
	int i;
	gzFile file;
	std::string filename;

	filename = fname+".WAV";
	if (CanOpen((filename).c_str())) {
			return filename;
		}
	for (i=0; i<16; i++)
	if (FileSorrend[i]!="")
	{
		if (DATSorrend[i] != NULL) {
			file = DATSorrend[i]->OpenFile(filename);
			if (file != NULL) { gzclose(file); return DATSorrend[i]->fname+filename; }
		} else
		if (CanOpen((FileSorrend[i]+filename).c_str())) {
			return FileSorrend[i]+filename;
		}
	}

	filename = fname+".OGG";
	if (CanOpen((filename).c_str())) {
			return filename;
		}
	for (i=0; i<16; i++)
	if (FileSorrend[i]!="")
	{
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