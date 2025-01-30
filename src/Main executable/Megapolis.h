#define N_KINDS 5
#define OrdDist 320
class AI_Army;
class Cell8x8
{
public:
	union
	{
		struct foo
		{
			unsigned char Peasants;//кол-во крестьян на этой клетке
			unsigned char Towers;//------//------башен
			unsigned char Warriors;
			unsigned char Farms;
			unsigned char Buildings;
			unsigned char Fly;
			unsigned char WaterPeas;
			unsigned char WaterObj;
		};
		unsigned char UnitsAmount[8];
	};
	//unsigned char P_MoveUp;//кол-во пересечений верхней границы
	//unsigned char P_MoveDn;
	//unsigned char P_MoveLf;
	//unsigned char P_MoveRi;
	//unsigned char Protection;  //=4*Towers+Warriors
	//unsigned char Agressivity; //=Warriors
	//unsigned char Importance;  //=Farms+Buildings
	unsigned short Neighbor;    //(0x001)  bit0 - UP
					  //(0x002)  bit1 - UP-RIGHT
					  //(0x004)  bit2 - RIGHT
					  //(0x008)  bit3 - DN-RIGHT
					  //(0x010)  bit4 - DN
					  //(0x020)  bit5 - DN-LEFT
					  //(0x040)  bit6 - LEFT
					  //(0x080)  bit7 - UP-LEFT
					  //(0x100)  bit8 - UP+2
					  //(0x200)  bit9 - RIGHT+2
					  //(0x400)  bit10- DN+2
					  //(0x800)  bit11- RIGHT+2
	//unsigned char Resrv[3];
	//unsigned short Enemy;
	//unsigned short EnemyTime;
	Cell8x8();
};
typedef Cell8x8 CellsInfo[64][64];
typedef CellsInfo TotalCInfo[8];
//extern TotalCInfo  TCInf;
class CityProject
{
public:

};
class ProposedProject
{
public:
	unsigned char PKind;//=0-Monster,==1-Upgrade
	unsigned short NIndex;//номер типа монстра/Upgrade index
	unsigned short ProducerIndex;//индекс свободного производителя
	unsigned short Percent;//процент денег, употребляемый на производство
	//int Cost;//цена 
	//int Useful[NBRANCH];
};
struct BuildProject
{
	bool Used : 1;
	bool PlaceFound : 1;
	bool Founded : 1;
	bool PeasantsCalled : 1;
	unsigned char Usage;
	short NearX;
	short NearY;
	int  x, y;
	int Options;
	unsigned short AttemptsToStand;
	unsigned short AttemptsToFindApprPlace;
	unsigned short Index;
	unsigned short NIndex;
	unsigned char MaxPeasants;
	unsigned char MinPeasants;
	unsigned char NPeasantsCalled;
};
class CityCell
{
public:
	int x, y;
	unsigned short WorkTimes;//==0xFFFF if inside the city
	CityCell* NextCell;
};
class CityCluster
{
public:
	CityCell* FirstCell;
	void AddCell( int x, int y );
};
typedef int Functional( int* x, int* y, int Lx, int Ly, unsigned char NI );
typedef int CheckWorkFn( OneObject* OB, City* CT );
class SmartGroup;
class Brigade;
class City;
typedef void BrigadeLink( Brigade* Brig );
class BrigadeOrder;
class BrigadeOrder
{
public:
	unsigned char OrdType;
	unsigned char Prio;
	unsigned short Size;
	char* Message;
	BrigadeOrder* Next;
	BrigadeLink* BLink;
};
struct BrigMemb
{
	unsigned short Peons;
	unsigned short Infantry;
	unsigned short Strelkov;
	unsigned short Mortir;
	unsigned short Pushek;
	unsigned short Grenaderov;
	unsigned short Other;
	unsigned short reserv;
};
int GetBMIndex( OneObject* OB );
class Brigade
{
public:
	City* CT;
	unsigned short* Memb;
	unsigned short* MembSN;
	unsigned short  SN;
	int* posX;
	int* posY;
	unsigned short NMemb;
	unsigned short MaxMemb;
	unsigned short MembID;
	char AddDamage;
	char AddShield;
	unsigned char WarType;
	unsigned char Direction;
	unsigned short ID;
	unsigned short BrigDelay;
	unsigned short LastTopology;
	int LastOrderTime;
	unsigned short ArmyID;
	unsigned short ErasureTime;
	bool Enabled : 1;
	bool PosCreated : 1;
	bool Precise : 1;
	bool Strelki : 1;
	bool AttEnm : 1;
	BrigMemb BM;
	CheckWorkFn* CFN;
	BrigadeOrder* BOrder;
	unsigned short LastEnemyID;
	unsigned short LastEnemySN;


	void SetIndex();
	void Init( City* CT, unsigned short ID );
	void AddObject( OneObject* OB );
	void CheckMembers( City* CT );
	void RemoveObjects( int NObj, Brigade* Dest );
	bool RemoveOne( int Index, Brigade* Dest );
	void RemovePeasants( Brigade* Dest );
	void FreeMember( int Idx );
	void DeleteAll();
	void CreateSquare();
	void CreateConvoy( unsigned char Type );
	int AddInRadius( int x, int y, int r, BrigMemb* BMem, Brigade* Dest );
	int AddInRadius( int x, int y, int r, BrigMemb* BMemb );
	BrigadeOrder* CreateOrder( unsigned char OrdType, int Size );
	int SelectPeasants( unsigned char NI );
	void Rospusk();
	bool GetCenter( int* x, int* y );
	//----------------------ORDERS----------------------//
	bool LocalSendTo( int x, int y, unsigned char prio, unsigned char OrdType );
	bool LinearLocalSendTo( int x, int y, unsigned char prio, unsigned char OrdType );
	bool WideLocalSendTo( int x, int y, unsigned char prio, unsigned char OrdType );
	bool GlobalSendTo( int x, int y, unsigned char prio, unsigned char OrdType );
	bool CaptureMine( int SID, unsigned char prio, unsigned char OrdType );
	bool AttackEnemy( int x, int y, unsigned char prio, unsigned char OrdType );
	void MakeBattle();
	void ProtectFarMines();
	//-----------------Human functions------------------//
	bool CreateNearOfficer( OneObject* OB, unsigned short Type, int ODIndex );
	void CreateOrderedPositions( int x, int y, char dir );
	void CreateSimpleOrderedPositions( int x, int y, char dir );
	void HumanCheckUnits();
	void HumanLocalSendTo( int x, int y, short Dir, unsigned char Prio, unsigned char OrdType );
	void HumanGlobalSendTo( int x, int y, short Dir, unsigned char Prio, unsigned char OrdType );
	void KeepPositions( unsigned char OrdType, unsigned char Prio );
	void Bitva();
	//--------------------------------------------------//
	void ClearBOrders();
	void DeleteBOrder();
};
typedef void GroupMaker( SmartGroup* GS );
class GroupOrder;
class GroupOrder
{
public:
	unsigned char		OrderType;
	GroupOrder* NextOrder;
	GroupMaker* GroupLink;
	void*       GroupData;
	int         GDataSize;
};

class Idea;
typedef void IdeaBrain( Idea* IDEA );
typedef void FreeIdea( Idea* ID );
class Idea
{
public:
	City* CT;
	Idea* PrevIdea;
	Idea* NextIdea;
	IdeaBrain* Brain;
	void* IdeaData;
	FreeIdea* FI;
	int   DataSize;
	void ClearIdea();
};
class Inform;
//typedef void I_Save(SaveBuf* SB,Inform* Inf);
//typedef void I_Load(SaveBuf* SB,Inform** Inf);
typedef void I_Clear( Inform* Inf );
class Inform
{
public:
	unsigned short ID;
	unsigned short Essence;
	Inform* Next;
	Inform* Previous;
	City*   CT;
	I_Clear* IClr;
	unsigned short Size;
};
class MineBase :public Inform
{
public:
	unsigned short   M_ID;
	unsigned short   M_SN;
	short  topx;
	short  topy;
	unsigned char   ResKind;
	unsigned char   UpgLevel;
	unsigned char   PSend;
	unsigned short   DefendersID;
	unsigned short   DefendersSN;
	unsigned short   MinersID;
	unsigned short   MinersSN;
	/*
	unsigned short   TowID;
	unsigned short   TowSN;
	unsigned short   NWalls;
	short* Walls;
	unsigned short   NGateProj;
	short* GateProj;
	unsigned short   NGates;
	short* Gates;
	*/
};
class PRM_Info :public Inform
{
public:
	int NBrigs;
	unsigned short BrigsID[5];
	unsigned short BrigsSN[5];
};
class SmartGroup : public Brigade
{
	bool Enabled;
	unsigned short Index;
	unsigned short NFarInf;
	unsigned short NNearInf;
	unsigned short NFarCav;
	unsigned short NNearCav;
	short CenterX;
	short CenterY;
	GroupOrder* Order;
};
#define MBR4    16
#define MaxBrig (MBR4*32)
#define MaxArm 128
class DefendInfo
{
public:
	unsigned char  x;
	unsigned char  y;
	unsigned char  Importance;
	unsigned short  NDefenders;
	unsigned short  MaxDefs;
	unsigned short* Def;
	unsigned short* DefSN;
	void AddUnit( OneObject* OB );
};
class AI_Army;
//----------------------ARMY--------------------//

#define PolkSize   36
#define NBrigTypes 6
class ExtendedBrigade
{
public:
	unsigned char BrigadeType;
	//0-Short range infantry
	//1-Long range infantry
	//2-Fast cavalry
	//3-Hard cavalry
	//4-Long range cavalry
	//5-Artillery
	//6-Sapers
	//7-Builders
	int Force;
	int NeedMembers;
	unsigned short NextBrigade;
	Brigade* Brig;
};
typedef void ArmyLink( AI_Army* ARM );
class ArmyOrder;
class ArmyOrder
{
public:
	unsigned char OrdType;
	unsigned char Prio;
	unsigned short Size;
	char* Message;
	ArmyOrder* Next;
	ArmyLink*  ALink;
};
struct FormInfo
{
	unsigned short ComID;
	unsigned short ComSN;
	unsigned short BarID;
	unsigned short BarSN;
};
class AI_Army
{
public:
	int NExBrigs;
	int MaxExBrigs;
	bool Enabled : 1;
	bool SpecialOrder : 1;
	int TopPos;
	int x, y;
	int Force;
	unsigned char Spec;
	unsigned short ArmyID;
	ExtendedBrigade* ExBrigs;
	//Additional information
	City* CT;
	Nation* NT;
	unsigned char NI;
	int LastBitvaTime;
	unsigned short FirstBrig;
	//Order information
	ArmyOrder* AOrder;
	//Formation info;
	unsigned short ComID[8];
	unsigned short ComSN[8];
	unsigned short NCom;
	unsigned short BarID[8];
	unsigned short BarSN[8];
	unsigned short NBar;
	unsigned short NFreeUnits;
	//Functions
	void InitArmy( City* CT );
	void ClearArmy();
	void CreateMinimalArmyFromBrigade( Brigade* BR, int Type );
	void AddBrigade( Brigade* BR );
	void AddUnit( OneObject* OB );
	void CloseArmy();
	void MoveArmyTo( int x, int y );
	void CreateBrigLink();
	void AttackArea( int x, int y );
	ArmyOrder* CreateOrder( unsigned char OrdType, int Size );
	void DeleteAOrder();
	void ClearAOrders();
	void LocalSendTo( int x, int y, unsigned char Prio, unsigned char OrdType );
	int  GetArmyDanger( int x, int y );
	void SendToMostSafePosition();
	void WideLocalSendTo( int x, int y, unsigned char Prio, unsigned char OrdType );
	void Parad();
	void ConnectToArmy( int ID, unsigned char Prio, unsigned char OrdType );
	void MakeBattle();
	void Bitva();
	void Diversia();
	void SetZombi();
	void ClearZombi();
	bool CheckArtillery();
	int  GetAmountOfBrigs( unsigned char Type );
	void SendGrenaders();
};
class City
{
public:
	Nation* Nat;
	unsigned char NI;
	//int InX,InY,InX1,InY1,OutX,OutY,OutX1,OutY1;
	//int TryTimes;
	//int ExpandSize;
	int Amount;
	//int GoldPeasPart;
	//int WoodPeasPart;
	//int BuildPeasPart;
	//unsigned short KindMax[N_KINDS];//макс. кол-во тварей типа Kind
	//unsigned short *MKList[N_KINDS];//массив указателей на массивы с индексами монстров данного типа
	//Functional* FNC[N_KINDS];//массив функционалов для выбора места, если место вообще не подходит, то возвр. -1
	//unsigned short BranchPart[NBRANCH];
	int AutoEraseTime;
	unsigned short BestProj[128];
	unsigned short NBestProjects;
	unsigned char NWoodSklads;
	unsigned char NStoneSklads;
	unsigned short WoodSkladID[8];
	unsigned short WoodSkladSN[8];
	unsigned short StoneSkladID[8];
	unsigned short StoneSkladSN[8];
	char AIID[48];
	unsigned char StartRes;
	unsigned char ResOnMap;
	unsigned char Difficulty;
	unsigned char LandType;
	unsigned short TransportID;
	unsigned short TransportSN;

	unsigned char NMeln;
	unsigned short MelnID[4];
	unsigned short MelnSN[4];

	int  MelnCoorX[4];
	int  MelnCoorY[4];
	int  NMelnCoor;

	int  FieldsID[512];
	unsigned short FieldsSN[512];
	unsigned short NFields;

	unsigned short Construction[64];
	unsigned short ConstructionSN[64];
	unsigned short ConstructionTime[64];
	unsigned short NConstructions;

	unsigned short FarmsUnderConstruction;
	int Account;

	bool FieldReady;
	bool AllowWood;
	bool AllowStone;
	bool AllowMine;
	bool DefenceStage;

	unsigned short NeedOnField;
	unsigned short NeedOnStone;
	unsigned short NeedOnWood;

	unsigned short** GroupsSet;
	unsigned short*  NGroupsInSet;
	int    NGroups;

	Brigade Brigs[MaxBrig - 11];
	Brigade Settlers;
	Brigade Defenders;
	Brigade Guards;
	Brigade Agressors;
	Brigade OnField;
	Brigade OnStone;
	Brigade OnWood;
	Brigade Builders;
	Brigade Free;
	Brigade InMines;
	Brigade FreeArmy;
	unsigned char N_mineUp[10];
	unsigned short MaxFields;
	unsigned short BestOnField;
	unsigned short WaterBrigs[128];
	//------------------ARMIES-------------------//
	AI_Army ARMS[MaxArm];
	unsigned short* DefArms;
	int NDefArms;
	int MaxDefArms;
	int NDivr;
	unsigned char LockUpgrade[8];
	unsigned char MyIsland;
	//-------------Peasants balance--------------//
	short NPeas;
	short NeedPF;
	short NeedPW;
	short NeedPS;
	short FreePS;
	int   WoodAmount;
	int   StoneAmount;
	int   FoodAmount;
	int   LastTime;
	int  WoodSpeed;
	int  StoneSpeed;
	int  FoodSpeed;
	//-------------------------------------------//
	int   LFarmIndex;
	int   LFarmRadius;
	short FarmLX;
	short FarmLY;
	//-------------------------------------------//
	unsigned short  MAX_WORKERS;
	//-------------------------------------------//
	bool AI_DEVELOPMENT : 1;
	bool AI_WATER_BATTLE : 1;
	bool AI_LAND_BATTLE : 1;
	bool AI_MINES_CPTURE : 1;
	bool AI_TOWN_DEFENCE : 1;
	bool AI_MINES_UPGRADE : 1;
	bool AI_FAST_DIVERSION : 1;
	//-----------DEFENDING THE BASE--------------//
	DefendInfo* DefInf;
	int NDefn;
	int MaxDefn;
	int MaxGuards;
	int AbsMaxGuards;
	//-------------------------------------------//
	unsigned short NP;
	unsigned short NAgressors;
	unsigned short MinType;
	unsigned short MaxType;
	unsigned short UnitAmount[2048];//Amount of units of some kind
	unsigned short ReadyAmount[2048];
	unsigned short PRPIndex[2048];//index of project to produce this type of monster,if not proposed,0xFFFF
	unsigned short UPGIndex[2048];
	unsigned short UnBusyAmount[2048];//сколько модулей простаивает
	unsigned short Producer[2048];
	bool PresentProject : 1;
	bool IntellectEnabled : 1;
	bool CenterFound : 1;
	short CenterX;
	short CenterY;
	BuildProject BPR;
	unsigned short NProp;
	ProposedProject Prop[100];
	Idea* IDEA;
	Inform* INFORM;
	Idea* AddIdea( IdeaBrain* IBR, bool Dup );
	void AddInform( Inform* Inf, I_Clear* ICL );
	void DelInform();
	void DelInform( Inform* Inf );
	Inform* SearchInform( unsigned short ID, unsigned short Essence, Inform* inf );
	Inform* SearchInform( unsigned short ID, Inform* inf );
	void HandleIdeas();
	void DelIdeas();
	unsigned short EnemyList[32];
	void CreateCity( unsigned char NI );
	bool CheckTZone( int x, int y, int Lx, int Ly );
	bool TryToFindPlace( int* x, int* y, int Lx, int Ly, unsigned char Kind );
	void EnumUnits();
	void AddProp( unsigned short NIN, GeneralObject* GO, unsigned short prop, unsigned short per );
	void AddUpgr( unsigned short NIN, unsigned short prod, unsigned short per );
	void EnumProp();
	void ProcessCreation();
	void HelpMe( unsigned short ID );
	void HandleFly();
	unsigned short FindNeedProject();
	void RefreshAbility();
	bool FindApproximateBuildingPlace( GeneralObject* GO );
	bool FindPreciseBuildingPlace( GeneralObject* GO );
	void AddConstruction( OneObject* OB );
	void HandleConstructions();
	void MarkUnusablePlace();
	void CloseCity();
	void HandleFields();
	void MakeZasev();
	void MakeSlowZasev();
	void HandleBrigades( int NP );
	void RegisterNewUnit( OneObject* OB );
	int  GetFreeBrigade();
	void ExecuteBrigades();
	void CalculateBalance();
	void GetNextPlaceForFarm( int* x, int* y );
	void UnRegisterNewUnit( OneObject* OB );
	void RegisterWaterBrigade( int ID );
	void UnRegisterWaterBrigade( int ID );
	//---------------Armies--------------
	int GetFreeArmy();

	//--------------Ideas----------------
	void TakeNewMine();
	void UpgradeSomeMine();
	void CreateWallNearMine();
	void BuildTower();
	void UpgradeMines();
	void SendAgressors();
	void ProtectMine();
	void BuildWallsNearMines();
	//----------------defending--------------//
	void HandleDefending();
	int  CheckDefending( unsigned char x, unsigned char y );
	void AddDefending( unsigned char x, unsigned char y, unsigned char Importance );
	void AddUnitDefender( OneObject* OB );
};
class Forces
{
public:
	int MinX;
	int MinY;
	int MaxX;
	int MaxY;
	int Damage;
	int Life;
	int Shield;
	int Protection;
	int Population;
	int StrongFactor;
	int CenterX;
	int CenterY;
	int NBuild;
	void SetDefaults();
};
#define SendPTime 10000
class SendPInform :public Inform
{
public:
	int time;
};
struct DangerInfo
{
	unsigned short ID;
	unsigned short SN;
	unsigned short Kind;
	unsigned short UpgradeLevel;
	unsigned short Life;
	unsigned short MaxLife;
	int x, y, z;
	int MinR;
	int MaxR;
};

extern Forces NForces[8];
extern int BestForce;
extern int BestNat;
extern int NextBest;
void OutCInf( unsigned char x, unsigned char y );
extern int COUNTER;
extern unsigned long BMASK[MBR4];
void InitSuperMortiraCells();
void CreateEnmBuildList();
void CreateProtectionMap();
struct CostPlace
{
	int xw, yw;
	int xc, yc;
	unsigned char Island;
	unsigned char R;
	unsigned char Access;
	unsigned short Transport;
};
extern CostPlace* COSTPL;
extern int NCost;
extern int MaxCost;