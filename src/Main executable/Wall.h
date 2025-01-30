struct WallCell{
	unsigned char x;
	unsigned char y;
	unsigned short Stage;
	unsigned short Tile;
	unsigned short Health;
	unsigned short MaxHealth;
	unsigned short WorkerID;
	unsigned char WorkTime;
	unsigned char WorkNeed;
	unsigned short BuildProgress;
};
class WallCluster{
public:
	int ClusterSize;
	WallCell* Cells;
	unsigned char OwnerID;
	unsigned char Type;
	int Index;
	WallCluster();
void AddWall(unsigned char x,unsigned char y);
void PreArrangeTiles();
void ArrangeTiles();
int CheckPoint(unsigned char x,unsigned char y);
void ShowCluster();
void Stand();
void Init();
int	GetDataSize();
void CreateData(unsigned char NI,unsigned char* lpData);
int FindWorkPoint(unsigned char x,unsigned char y,unsigned short ID);
int FindDamagePoint(unsigned char x,unsigned char y,unsigned short ID);
};
class GWSys{
public:
	WallCluster* GWC;
	int MaxWall;
	GWSys();
	unsigned short AddCluster(unsigned char* Data);
	void ProcessClusters();
	void BuildCell(unsigned char x,unsigned char y,unsigned char Owner,unsigned char Incr,unsigned short WorkerID);
	void DamageCell(unsigned char x,unsigned char y,unsigned char Owner,unsigned char Incr);
};
extern WallCluster TMPCluster;
extern bool BuildWall;
extern bool WStarted;
void SetBuildWallMode();
void WallHandleMouse(int x,int y);
void LoadWalls();
extern GWSys GWALLS;
extern unsigned short Links[256][256];
extern unsigned short LIndex[256][256];