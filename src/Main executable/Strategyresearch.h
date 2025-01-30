extern int StratLx;
extern int StratLy;
extern int StratSH;
class CellInfo{
public:
	unsigned short NSword;
	unsigned short NStrel;
	unsigned short NCaval;
	unsigned short NPush;
	unsigned short NMort;
	unsigned short NTow;
	unsigned short Total;
	unsigned char x;
	unsigned char y;
	unsigned short ArmyID;
	unsigned char Changed;
};
class ArmyInfo{
public:
	unsigned short NSword;
	unsigned short NStrel;
	unsigned short NCaval;
	unsigned short NPush;
	unsigned short NMort;
	unsigned short NTow;
	//unsigned short NPeon;
	unsigned char MinX;
	unsigned char MinY;
	unsigned char MaxX;
	unsigned char MaxY;
	unsigned short N;
};
class GlobalArmyInfo{
public:
	unsigned short* ArmDistr;
	CellInfo* CIN;
	int NCIN;
	int MaxCIN;
	ArmyInfo* AINF;
	int NArmy;
	int MaxArmy;
	GlobalArmyInfo();
	~GlobalArmyInfo();
	void Clear();
	void ResearchArmyDistribution(unsigned char NI);
	void Show(int x,int y);
};
