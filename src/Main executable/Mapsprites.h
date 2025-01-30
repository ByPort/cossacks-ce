//#define sprNx  (64<<ADDSH)
#define SprShf (6+ADDSH)
#define SprInCell 16
extern int MaxSprt;
class ObjCharacter{
public:
	unsigned char ResType;//0-wood,1-gold,2-stone,3-food,0xFE-no resource,0xFF-removable
	unsigned char WorkRadius;
	unsigned char ResPerWork;
	int WorkAmount;
	unsigned short WNextObj;
	unsigned short DamageAmount;
	unsigned short DNextObj;
    unsigned short TimeAmount;
    unsigned short TNextObj;
    unsigned char IntResType;
    unsigned short IntResPerWork;
    unsigned short IntWorkRadius;
    NewAnimation* Stand;
    NewAnimation* Shadow;
    short Z0;
    short DZ;
    unsigned char Parts;
    unsigned char Delay;
    unsigned char Frames;
	unsigned char Discret;
	unsigned char LockRadius;
	unsigned short NLockPt;
	short* LockX;
	short* LockY;

	char* Name;
};
class SprGroup{
public:
	int  NSpr;
	unsigned short FileID;
	unsigned short* Items;
	short* Dx;
	short* Dy;
	short* Radius;
	ObjCharacter* ObjChar;
	void LoadSprites(char* fname);
	int GetIndexByName(char* Name);
};
class OneSprite{
public:
	bool Enabled:1;
	bool Surrounded:1;
	int x;
	int y;
	short z;
	unsigned short Radius;
	SprGroup* SG;
	ObjCharacter* OC;
	int Index;
	unsigned short SGIndex;
	unsigned char WorkOver;
    unsigned short TimePassed;
	unsigned char Damage;
	int PerformWork(unsigned short);
    int PerformIntWork(int work);
	void PerformDamage(int Dam);
};
class TimeReq{
public:
    int NMembers;
    int MaxMembers;
    int* IDS;
    unsigned char* Kinds;
    TimeReq();
    ~TimeReq();
    void Handle();
    void Add(int ID,unsigned char Kind);
    void Del(int ID,unsigned char Kind);
};
extern TimeReq ObjTimer;
//extern unsigned char NSpri[sprNx*sprNx];
extern unsigned char* NSpri;
//extern unsigned short* SpRefs[sprNx*sprNx];
extern int** SpRefs
;
extern OneSprite* Sprites;
void InitSprites();
void addSprite(int x,int y,SprGroup* SG,unsigned short id);
int GetHeight(int x,int y);
void PreShowSprites();
void addScrSprite(int x,int y,SprGroup* SG,unsigned short id);
void addTree(int x,int y);
extern OneSprite* Sprites;
extern SprGroup TREES;
extern SprGroup STONES;
extern SprGroup HOLES;
extern SprGroup COMPLEX;
void ProcessSprites();
unsigned char DetermineResource(int x,int y);
unsigned char FindAnyResInCell(int x,int y,int cell,int* Dist,unsigned char Res);
bool CheckDist(int x,int y,unsigned short r);
void HideFlags();
//-----------------------L3--------------------------
//#define L3DX (MAPSX>>4)
//#define L3DY (MAPSY>>4)
//#define L3MAX (MAPSX<<2)
//#define L3SH (ADDSH+5)
//extern unsigned char* L3HIMap[L3DX*L3DY];
//void InitL3();
//void ClearL3();
//void SetL3Point(int x,int y,unsigned char H);
//int GetL3Height(int x,int y);
int GetUnitHeight(int x,int y);