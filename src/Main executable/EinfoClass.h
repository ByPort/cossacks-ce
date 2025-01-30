#define MAXTOW 24
extern int NTows;
#define TSSHIFT (4+8)
struct HumanShip{
	unsigned short ID;
	unsigned short SN;
	int x,y;
	int Force;
	int Life;
	int R;
};
struct HShipsGroup{
	int xs,ys;
	int xL,yL;
	int xR,yR;
	int Ns, Force;
	int CTop;
	int R;
};
class EnemyInfo{
public:
	GlobalArmyInfo GAINF;
	unsigned char Mask;
	unsigned char BasicNation;

	unsigned char* ProtectionMap;
	unsigned short* SupMortBestID;
	unsigned short* SupMortLastTime;
	unsigned short EnmBuildList[128];
	unsigned short EnmBuildSN[128];
	int  NEnmBuild;
	int  NEnWalls;
	unsigned short WallsX[200];
	unsigned short WallsY[200];
	unsigned short TowsID[MAXTOW];
	unsigned short TowsSN[MAXTOW];
	unsigned short TowsMaxR[MAXTOW];
	unsigned char TowDanger[MAXTOW];
	unsigned short CurTow;
	unsigned short CurR;
	unsigned short CurXip;
	unsigned short MaxTowR;
	unsigned long* InflMap;
	DangerInfo DINF[40];
	int NDINF;
	int LastBuildSafetyTime;
	unsigned char* SafeMAP;
	int CurRStage;
	SafeCellInfo** SCINF;
	unsigned short WTopNear[64];
	int NWTopNear;
	unsigned short* NUN;
	unsigned char* TMAP;
	HumanShip SHIPS[128];
	HShipsGroup SGRP[64];
	int NSGRP;
	int NHSHIPS;
	int ShipsForce;
	unsigned short TopAreasDanger[1024];
	
	void InitInflMap();
	void ClearTow(OneObject* OB);
	void AddTow(OneObject* OB);
	void ProcessTow();
	void InitSuperMortiraCells();
	unsigned short ResearchCellForSupermortira(int cell,int MinDistance,int MaxDistance);
	unsigned short SearchDangerousPlaceForEnemy(int* utx,int* uty,int MINR,int MAXR);
	bool FindSafePlace(int* utx,int* uty);
	unsigned short SearchBestEnemyAndPlaceForSupermortira(OneObject* OB,int* BestX,int* BestY);
	void CreateEnmBuildList();
	void CreateProtectionMap();
	void CreateWallsList();
	bool CheckTowersPresence();
	void CreateListOfDangerObjects();
	void ResearchArmyDistribution();
	void InitBuildSafety();
	void CreateBuildSafetyMap();
	void RefreshSafeMap();
	unsigned char GetSafeVal(int x,int y);
	void ShowSafetyInfo(int x,int y);
	void AddSafePoint(int x,int y,unsigned short Index,unsigned short SN,unsigned short Prio);
	void ClearSafePoint(int x,int y,unsigned short Index);
	void InitSafeInfo();
	void CloseSafeInfo();
	void CheckSafeInfo();
	SafeCellInfo* GetCellInfo(int x,int y);
	void RegisterSafePoint(OneObject* OB,int x,int y);
	void InitResearchSafeCell();
	void ResearchSafeObject(OneObject* OB,int MinR,int MaxR,int pstart);
	void ResearchSafeCells(int MinR,int MaxR);
	void Clear();
	void ResearchHumanAttackPlaces();
	void ClearPlaces();
	void RegisterHumanShips();
	int  GetMaxForceOnTheWay(int TopStart,int FinalTop);
	void ResearchShipsGroups();
	int  GetShipsForce(int x,int y,int r);
	void ClearIslands();

	void ALLOCATE();
	void FREE();


};
class GlobalEnemyInfo{
public:
	EnemyInfo* EINF[8];
	void Clear();
	void Setup();
	void Process();
	void AddTow(OneObject* OB);
	void ClearTow(OneObject* OB);
	~GlobalEnemyInfo();
	GlobalEnemyInfo();
	bool FindNearestEnemy(unsigned char NNUM,int* x,int* y,bool TowerFear,int Min,bool Defence);
};
extern GlobalEnemyInfo GNFO;