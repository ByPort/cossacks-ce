class WallCharacter
{
public:
	char* Name;
	unsigned short RIndex;
	unsigned short NBuild;
	unsigned short Ndamage;
	short dx, dy;
	unsigned short GateFile;
	short GateDx;
	short GateDy;
	short UpgradeGateIcon;
	short OpenGateIcon;
	short CloseGateIcon;
	short UpgradeTower1;
	short UpgradeTower2;
	unsigned short  GateCost[8];
	short TexRadius;
	short NTex;
	short TexPrec;
	short* Tex;
	short OpenGateSoundID;
	short CloseGateSoundID;
	unsigned char  OpenKeyFrame;
	unsigned char  CloseKeyFrame;
};

class WallCluster;

class WallCell
{
public:
	short x;
	short y;
	unsigned char DirMask;
	unsigned char Type;
	unsigned char NI;
	unsigned char Stage;
	unsigned char MaxStage;
	unsigned short Health;
	unsigned short MaxHealth;
	unsigned char Sprite;
	unsigned char SprBase;
	unsigned short ClusterIndex;
	unsigned short OIndex;
	unsigned short GateIndex;
	int Locks;
	bool Visible;
	bool CheckPosition();
	int GetLockStatus();
	void SetLockStatus();
	bool StandOnLand( WallCluster* WC );
	void CreateLocking( WallCluster* WC );
	void Landing( WallCluster* WC );
	void ClearLocking();
};

class WallSystem;

class WallCluster
{
public:
	unsigned char Type;
	int NCornPt;
	unsigned short* CornPt;
	int NCells;
	WallCell* Cells;
	WallSystem* WSys;
	short LastX;
	short LastY;
	short FinalX;
	short FinalY;
	NewMonster* NM;
	unsigned short  NIndex;
	unsigned char  NI;
	//------------------//
	WallCluster();
	~WallCluster();
	void SetSize( int N );
	void ConnectToPoint( short x, short y );
	void ConnectToPoint( short x, short y, bool Vis );
	void UndoSegment();
	void SetPreviousSegment();
	void KeepSegment();
	void CreateSprites();
	void AddPoint( short x, short y, bool Vis );
	void Preview();
	void View();
	int  CreateData( unsigned short* Data, unsigned short Health );
	void CreateByData( unsigned short* Data );
	void SendSelectedToWork( unsigned char NI, unsigned char OrdType );
};

class WallSystem
{
public:
	int NClusters;
	WallCluster** WCL;
	//-------------------//
	WallSystem();
	~WallSystem();
	void AddCluster( WallCluster* );
	void WallSystem::Show();
};

void WallHandleDraw();
void WallHandleMouse();
void SetWallBuildMode( unsigned char NI, unsigned short NIndex );
void LoadAllWalls();

inline int GetLI( int x, int y )
{
	return x + ( y << ( VAL_SHFCX + 1 ) );
}

extern int MaxLI;
extern int MaxLIX;
extern int MaxLIY;
extern int NChar;

extern WallCharacter WChar[32];

//extern WallCell* WRefs[MAXCX*MAXCY*4];

extern WallCell** WRefs;
extern WallSystem WSys;

void DetermineWallClick( int x, int y );
void SetLife( WallCell* WC, int Health );

//gates
#define NGOpen 9

class Gate
{
public:
	short x;
	short y;
	unsigned char NI;
	unsigned char NMask;
	unsigned char State;
	unsigned char delay;
	unsigned char Locked;
	unsigned char CharID;
};

extern Gate* Gates;
extern int NGates;
extern int MaxGates;

void SetupGates();
void InitGates();
int AddGate( short x, short y, unsigned char NI );
void DelGate( int ID );