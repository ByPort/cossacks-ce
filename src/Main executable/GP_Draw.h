#ifndef GP_USER
#define GP_API __declspec(dllexport)
#else
#define GP_API __declspec(dllimport)
#endif
#pragma pack(1)
typedef char* LPCHAR; 
#define MaxGPIdx 3000

class GP_API GP_Header
{
public:
	int NextPict;
	short dx;
	short dy;
	short Lx;
	short Ly;
	unsigned char* Pack;
	char  Options;
	unsigned long CData;
	short NLines;
	int GetLx();
	int GetLy();
	int GetDx();
	int GetDy();
};

struct GP_API GP_GlobalHeader
{
	int		 Sign;
	short	 NPictures;
	short    Reserved;
	int		 VocOffset;
	short    VocLength;
	//GP_Header* LGPH[256];
	unsigned long    LGPH[256];
};
struct OneUnicodeSet{
	int DY;
	int DX;
	unsigned char UseColor;
	int Start;
	int NSymbols;
	int GP_Start;
	int GPID;
};
struct UNICODETABLE{
	int NTables;
	OneUnicodeSet USET[4];
};
struct OneUniFont{
	char FontName[48];
	UNICODETABLE UTBL;
};
class GP_API UNIFONTS{
public:
	OneUniFont* UFONTS;
	int NFonts;
	UNIFONTS();
	~UNIFONTS();
	void LoadFonts();
	UNICODETABLE* FindFont(char* Name);
};
extern GP_API UNIFONTS UFONTS;
#define NO_PACK ((unsigned char*)0xFFFFFFFF)
void ErrM(char* s);
typedef GP_GlobalHeader* lpGP_GlobalHeader;

class GP_API GP_System
{
public:
	unsigned char* PackCash;
	int CashSize;
	int CashPos;

	//Current amount of loaded images in this instance
	int NGP;

	int NGPReady;

	//Array of pointers to buffers with loaded resource files
	GP_GlobalHeader** GPH;

	unsigned long** CASHREF;
	unsigned char* Mapping;
	char** GPNames;
	unsigned short* GPNFrames;
	RLCTable* RLCImage;
	RLCTable* RLCShadow;
	int* GPSize;
	int* GPLastTime;

	//Type of image resource file
	//0 = filed does not exist
	//1 = GP
	//2 = RLC
	//3 = RLC + Shadow 
	unsigned char* ImageType;

	short** ImLx;
	short** ImLy;
	char** ItDX;
	char** ItLX;
	UNICODETABLE** UNITBL;
	GP_System();
	~GP_System();
	unsigned char* GetCash(int Size);
	int PreLoadGPImage(char* Name);
	int PreLoadGPImage(char* Name,bool Shadow);
	bool LoadGP(int i);
	void  UnLoadGP(int i);
	int  GetGPWidth(int i,int n);
	int GetGPShift(int i,int n);
	int  GetGPHeight(int i,int n);
	bool GetGPSize(int i,int n,int* Lx,int* Ly);
	void ShowGP(int x,int y,int FileIndex,int SprIndex,unsigned char Nation);
	void ShowGPLayers(int x,int y,int FileIndex,int SprIndex,unsigned char Nation,int mask);
	void ShowGPTransparent(int x,int y,int FileIndex,int SprIndex,unsigned char Nation);
	void ShowGPTransparentLayers(int x,int y,int FileIndex,int SprIndex,unsigned char Nation,int mask);
	void ShowGPPal(int x,int y,int FileIndex,int SprIndex,unsigned char Nation,unsigned char* Table);
	void ShowGPPalLayers(int x,int y,int FileIndex,int SprIndex,unsigned char Nation,unsigned char* Table,int mask);
	void ShowGPRedN(int x,int y,int FileIndex,int SprIndex,unsigned char Nation,int N);
	void ShowGPDarkN(int x,int y,int FileIndex,int SprIndex,unsigned char Nation,int N);
	void ShowGPDark(int x,int y,int FileIndex,int SprIndex,unsigned char Nation);
	void ShowGPFired(int x,int y,int FileIndex,int SprIndex,unsigned char Nation);
	void ShowGPMutno(int x,int y,int FileIndex,int SprIndex,unsigned char Nation);
	void ShowGPGrad(int x,int y,int FileIndex,int SprIndex,unsigned char Antion,unsigned char* Table);
	void SetWhiteFont(int Gp_File);
	void SetRedFont(int Gp_File);
	void SetBlackFont(int Gp_File);
	void SetOptionalColor(int GP_File,int c);
	void FreeRefs(int i);
};

extern GP_API GP_System GPS;

class GP_API LocalGP
{
public:
	int GPID;
	bool Uniq;
//-----------------//
	LocalGP();
	LocalGP(char* Name);
	~LocalGP();
	void Load(char* Name);
	void UnLoad();
};

extern unsigned short GP_L_IDXS[MaxGPIdx];