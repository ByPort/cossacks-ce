struct BlockCell{
	unsigned short x;
	unsigned short y;
};

class BlockBars{
public:
	int NBars;
	int MaxBars;
	BlockCell* BC;
	BlockBars();
	~BlockBars();
	void Clear();
	bool Add(unsigned short x,unsigned short y);
	bool FastAdd(unsigned short x,unsigned short y);
	bool Delete(unsigned short x,unsigned short y);
};
void AddLockBar(unsigned short x,unsigned short y);
void AddUnLockbar(unsigned short x,unsigned short y);
void AddDefaultBar(unsigned short x,unsigned short y);
void SetTexturedRound(int x,int y,int r,unsigned char Tex);
int ConvScrY(int x,int y);
extern int NCurves;
void AddPointToCurve(int x,int y,bool Final,unsigned char Type);
extern unsigned short TexList[128];
extern unsigned short NTextures;