// DatExtract.cpp : Defines the entry point for the console application.
//

#include "datextract.h"
#include "windows.h"

TDATLocation::TDATLocation(std::string filename)
{
	fname = filename;
	h_dat = _open(filename.c_str(),_O_RDONLY|_O_BINARY);
    if (h_dat == -1) {
		return ;
    }

    _lseek(h_dat, 0l, SEEK_END);
    _lseek(h_dat, -8l, SEEK_CUR);
    _read(h_dat,&filelist_siz,4);
    _read(h_dat,&dat_siz,4);

	filelist_siz -= 4;
    f_pos = dat_siz - filelist_siz - 8;

    buf = (char *)malloc(filelist_siz);

	_lseek(h_dat,f_pos,SEEK_SET);
    byte_read = _read(h_dat,buf, filelist_siz);
	_close(h_dat);
}

TDATLocation::~TDATLocation()
{
	free(buf);
}

gzFile TDATLocation::OpenFile(std::string filename)
{
	ptr = buf;
	filename.erase(0,1);
	do {
		filename_len = *(int*)ptr;
		ptr += 4;
		ptr[filename_len] = 0;
		file_real_siz = *(int*)(ptr+filename_len+1);
		file_siz = *(int*)(ptr+filename_len+5);
		file_pos = *(int*)(ptr+filename_len+9);
		
		printf("%s\n",ptr);
		if (!lstrcmpi(filename.c_str(),ptr)) {
			h_dat = _open(fname.c_str(),_O_RDONLY|_O_BINARY);
			_lseek(h_dat,file_pos,SEEK_SET);
			return gzdopen(h_dat,"rb");
		}
		ptr+=(filename_len+13);
	} while (ptr-buf < byte_read);
	return NULL;
}


//
//int affix_len, affix_type;
////unsigned char gzheader[]={0x1f,0x8b,0x8,0x8,0x9f,0xe8,0xb7,0x36,0x2,0x3};
//
//void extract(char *filename,int h_dat,int file_pos,int file_siz) {
//	char *filenam;
//	char buf[100];
//	int i,len;
//
//	char* filename1 = "e:\\_DC_share_e\\Game\\Fallout_2_CD\\master.dat";
//
//	h_dat = _open(filename1,_O_RDONLY|_O_BINARY);
//
//	printf("Extracting %s\n",filename);
//
//	len = strlen(filename);
//	/* convert filenames */
//	filenam = (char *)malloc(len+5);
//	for (i = 0; i < len+1; i++) {
//		if (filename[i] == '\\') filenam[i] = '_';
//		else filenam[i] = filename[i];
//	}
//
//	sprintf(buf,"F:\\test\\%s",filenam);
//	
//
//	_lseek(h_dat,file_pos,SEEK_SET);
//	
//	gzFile gzstr;
//	int out;
//	out = _open(buf,_O_TRUNC | _O_CREAT |_O_RDWR| _O_BINARY, _S_IREAD | _S_IWRITE);
//	gzstr = gzdopen(h_dat,"rb");
//	char buf2[16384];
//    int err;
//    for (;;) {
//		err = sizeof(buf2);
//		if (file_siz<=err) err=file_siz;
//        len = gzread(gzstr, buf2, err);
//		file_siz-=len;
//		if (file_siz<=0) {
//			if (_write(out, buf2, (unsigned)len) != len) {
//				printf("Write ERROR (disk full?)\n");
//			}
//			break;
//		} 
//		if (len < 0) {
//			printf("ERROR while uncompressing\n");
//		};
//        if (len <= 0) break;
//
//		if (_write(out, buf2, (unsigned)len) != len) {
//			printf("Write ERROR (disk full?)\n");
//		}
//	}
//	_close(out);
//	gzclose(gzstr);
//}


//
//int _tmain(int argc, char *argv[]) {
//    char *buf, *ptr;
//	char *affix = "art\\intrface\\mainmenu.frm";
//    int h_dat, f_pos,filename_len, byte_read;
//    int file_siz, file_pos, file_real_siz, dat_siz, filelist_siz;
////    int i;
//
//	argc=2;
//	
//	char* filename1 = "e:\\_DC_share_e\\Game\\Fallout_2_CD\\master.dat";
//
//    h_dat = _open(filename1,_O_RDONLY|_O_BINARY);
//    if (h_dat == -1) {
//	printf("Cannot read %s\n",filename1);
//	return -1;
//    }
//
//    affix_type = PREFIX;
//
//    _lseek(h_dat, 0l, SEEK_END);
//    _lseek(h_dat, -8l, SEEK_CUR);
//    _read(h_dat,&filelist_siz,4);
//    _read(h_dat,&dat_siz,4);
//    filelist_siz -= 4;
//    f_pos = dat_siz - filelist_siz - 8;
//
//    buf = (char *)malloc(filelist_siz);
//    if (buf == NULL) {
//	printf("Memory not enough\n");
//	return -1;
//    }
//
//    ptr = buf;
//
//    /* read file list */
//    _lseek(h_dat,f_pos,SEEK_SET);
//    byte_read = _read(h_dat,buf, filelist_siz);
//	_close(h_dat);
//    do {
//	/* extract file info */
//	filename_len = *(int*)ptr;
//	ptr += 4;
//	ptr[filename_len] = 0;
//	file_real_siz = *(int*)(ptr+filename_len+1);
//	file_siz = *(int*)(ptr+filename_len+5);
//	file_pos = *(int*)(ptr+filename_len+9);
//	
//	printf("%s\n",ptr);
//	//if (!_strcmpi(ptr,affix)) 
//		extract(ptr,h_dat,file_pos,file_real_siz);
//	ptr+=(filename_len+13);
//
//    } while (ptr-buf < byte_read);
//	return 0;
//}

