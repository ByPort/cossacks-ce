#pragma pack(1)
struct Radio{
	char* xi;
	char* yi;
	unsigned short N;
};
#define RRad 90
extern Radio Rarr[RRad];

void CreateAreas();
void InitAreas();
void EraseAreas();
extern int TopLx;
extern int TopLy;
extern int MaxTop;
extern int TopSH;
extern unsigned short* TopRef;

#define SI_Center	1
#define SI_Meln		2
#define SI_Sklad	4
#define SI_Shahta	8
#define SI_Tower	16
#define SI_ArtDepo	32
#define SI_House	64
#define SI_Barrack  128
#define SI_Farm		256
#define SI_Other	512
#define SI_Near     1024
#define SI_FarLand  8192
#define SI_Okrug    16384
#define SI_Central  32768
struct StrategyInfo{
public:
	unsigned short BuildInfo;
	unsigned char NPeasants;
	unsigned char NShortRange;
	unsigned char NLongRange;
	unsigned char NMortir;
	unsigned char NTowers;
	unsigned char NPushek;
};
struct Area{
	short x;
	short y;
	unsigned char  Importance;
	unsigned char NTrees;
	unsigned char NStones;
	//01-gold
	//02-iron
	//04-coal
	unsigned short  NMines;
	unsigned short* MinesIdx;
	unsigned short  NLinks;
	unsigned short* Link;
	unsigned short MaxLink;
	StrategyInfo SINF[8];
};
struct MediaTop{
	unsigned short* MotionLinks;
	unsigned short* LinksDist;
	unsigned short* TopRef;
	int NAreas;
	Area* TopMap;
};
extern MediaTop GTOP[2];
extern Area* TopMap;
extern int   NAreas;
extern int MaxArea;
extern unsigned short* MotionLinks;
extern unsigned short* LinksDist;
void CreateStrategyInfo();
//---------
#define WTopLx (TopLx)
#define WTopSH (TopSH)
#define WCLSH 2
#define WCLLX 4
extern unsigned short* WTopRef;
extern Area* WTopMap;
extern int   WNAreas;
extern int WMaxArea;
extern unsigned short* WMotionLinks;
extern unsigned short* WLinksDist;
//---------
void InitTopChange();
void ClearTopChange();
void AddChTop(unsigned short TopID);
void AddSurroundingAreas(unsigned short TopID);
void DynamicalLockTopCell(int x,int y);
void DynamicalUnLockTopCell(int x,int y);
void ProcessDynamicalTopology();