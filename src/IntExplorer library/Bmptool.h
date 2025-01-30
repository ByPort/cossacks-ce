#define BMPTOOL
typedef unsigned char byte;
#pragma pack(1)
struct BMPformat{ // bmfh 
        unsigned short    bfType; 
        unsigned long   bfSize; 
        unsigned short    bfReserved1; 
        unsigned short    bfReserved2; 
		unsigned long   bfOffBits;
		unsigned long   biSize;
		long    biWidth; 
		long    biHeight; 
		unsigned short    biPlanes;
		unsigned short    biBitCount;
		unsigned long   biCompression;
		unsigned long   biSizeImage;
		long    biXPelsPerMeter; 
		long    biYPelsPerMeter; 
		unsigned long   biClrUsed;
		unsigned long   biClrImportant;
};
struct PAL3{
	unsigned char r,g,b;
};
struct PAL4{
	unsigned char b,g,r,reserved;
};
void SaveToBMP24(char* Name,int Lx,int Ly, unsigned char* data);
bool ReadBMP24(char* Name,BMPformat* BM, unsigned char** data);
bool ReadBMP8(char* Name,BMPformat* BM, unsigned char** data);
bool ReadBMP8TOBPX(char* Name, unsigned char** data);
void SaveToBMP8(char* Name,int Lx,int Ly, unsigned char* data,PAL3* pal);