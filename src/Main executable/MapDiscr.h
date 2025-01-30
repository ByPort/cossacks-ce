#pragma once

/*                    Map discription
 *
 * This file describes map cells, animations, monsters, buildings,
 * flying monsters, on-water monsters...
 */

 //Catch division by zero by "overloading" div()
 //(in case we will be able to link dynamically sometime, this causes no linking warnings)
div_t __cdecl SecureDivision( int const numerator, int const denominator );
#define div(x,y) SecureDivision(x,y)

#pragma pack(1)

#include "AntiBug.h"
#include "Icons.h"
#include <stdio.h>

#define NBRANCH 4
#define ULIMIT 65535
#define LULIMIT 65000

extern unsigned char MYNATION;

#define SetMyNation(x) {MYNATION=x^133;}
#define MyNation (MYNATION^133)

extern int ADDSH;

#include "gFile.h"

//Maximum size of cells map
extern int MAXCX;
#define MAXCY MAXCX

//maximal amount of units in cell
#define MAXINCELL 64

#define SHFCELL 6

//Size of cell
#define CELLSIZE 4

#define CELL2 2


extern int URESRC[8][8];

#define XRESRC(i,j) (URESRC[i][j]^134525)
#define SetXRESRC(i,j,k) URESRC[i][j]=(k^134525)
#define AddXRESRC(i,j,k) URESRC[i][j]=(((URESRC[i][j]^134525)+k)^134525)


extern int VAL_SHFCX;
extern int VAL_MAXCX;
extern int VAL_MAXCIOFS;
extern int VAL_SPRNX;
extern int VAL_SPRSIZE;
extern int VAL_MAPSX;
extern int MapShift;
extern int WLX;
extern int WMPSIZE;
extern int MaxWX;
extern int MaxWY;
extern int MAPSX;
extern int MAPSY;
extern int MAPSHF;
extern int BMSX;
extern int TopLx;
extern int TopLy;
extern int MaxTop;
extern int TopSH;
extern int B3SX;
extern int B3SY;
extern int B3SZ;
extern int MaxSector;
extern int MaxTH;
extern int MTHShift;
extern int VertInLine;
extern int SectInLine;
extern int MaxPointIndex;
extern int MaxLineIndex;
extern int StratLx;

extern short* THMap;// Map of heights in vertices
extern unsigned char* TexMap;//Map of textures in vertices
extern unsigned char* SectMap;//Map of sections on lines

extern int MAXCIOFS;
extern int TSX;

#define SINGLE_ORDER 0
#define HEAD_ORDER   1
#define TILE_ORDER   2

struct Coor3D
{
	int x, y, z;
};

typedef void HandlePro( int );

class Weapon;
class SelGroup;
class NewMonster;

//Описание одной клетки на карте(без картинки)
struct MapCell
{
	bool          WaterLock : 1;//Клетка недоступна для водных
	bool          AirLock : 1;	//Клетка недоступна для летающих
	bool          TempLock : 1;	//Признак временной недоступности
	unsigned char LayerID;		//Нижний слой на карте-объект типа МЕСТОРОЖДЕНИЕ
	unsigned short          MonsterID;	//Номер монстра в таблице объектов  
	unsigned short          BuildingID;	//Номер здания в таблице объектов
	unsigned short          FlyID;		//Летающий объект
};//8 bytes

//Элемент в таблице объектов
struct ObjectRef
{
	unsigned ObjID : 8;		//Тип объекта
	unsigned Location : 2;	//Наземный/Водный/Летающий/Всех типов
	void* lpObj;			//Ссылка на объект
};

//Максимальное количество объектов: 65535
#define MaxObject ULIMIT

//Массив объектов
typedef ObjectRef ObjArray[MaxObject];
class Brigade;

//Описание анимации
struct OneSlide
{
	unsigned short  FileID;	//Номер файла анимации
	char dx;		//Смещение спрайта по горизонтали
	char dy;		//Смещение спрайта по горизонтали
	unsigned short  spr;		//Номер спрайта в данном файле
	unsigned short SoundID;	//Номер звука
};

typedef OneSlide MovieType[256];
typedef MovieType* lpOneMovie;

struct  Octant
{
	unsigned char count;
	unsigned char ticks;
	char gdx;
	char gdy;
	OneSlide* Movie;
};

typedef Octant Animation[8];

//Информация о анимации и ее применении
struct MoreAnimation
{
	Octant* Anm;
	unsigned short WhatFor;	//Как будет использоваться данная анимация
	unsigned short Kind;
};

typedef MoreAnimation AnimArray[32];
typedef unsigned short WordArray[32768];
typedef WordArray* lpWordArray;

struct ObjIcon
{
	unsigned short spr;
};

#include "Upgrade.h"

//Описание типов объектов(идентификаторы)
#define EmptyID 0

//---------------------SYCRO------------------
int RandNew( char*, int );
void AddRandNew( char*, int, int );
void AddUN( char* File, int Line, int Param, int Type );

#define rando() RandNew(__FILE__,__LINE__)

//New monster format
//<ANMNAME> ShiftX ShiftY Rotations FileID NFrames frame1 ... frameN
//SETACTIVE <ANMNAME> ActiveFrame ActivePtX ActivePtY 
class NewFrame
{
public:
	unsigned short FileID;
	unsigned short SpriteID;
	short dx;
	short dy;
};

class NewAnimation
{
public:
	bool Enabled;

	unsigned char NFrames;
	unsigned char Parts;
	unsigned char PartSize;
	unsigned char ActiveFrame;

	short StartDx;
	short StartDy;
	short* ActivePtX;
	short* ActivePtY;
	short* LineInfo;

	unsigned char Rotations;
	unsigned char TicksPerFrame;
	unsigned char HotFrame;
	NewFrame* Frames;

	short SoundID;

	NewAnimation();
};

#define NAttTypes 4
struct FogRec
{
	unsigned short  NWeap;
	int   WProb;
	unsigned short* Weap;
};

class AdvCharacter;

class OrderClassDescription
{
public:
	char* ID;
	char* Message;

	int IconPos;
	int IconID;

	OrderClassDescription();
	~OrderClassDescription();
};

struct SingleGroup
{
	int ClassIndex;
	int NCommon;
	int NForms;

	unsigned char* IDX;
	unsigned short* Forms;
};

class FormGroupDescription
{
public:
	int NGrp;
	SingleGroup* Grp;

	FormGroupDescription();
	~FormGroupDescription();

	void Load( GFILE* f );
};

extern FormGroupDescription FormGrp;
class OrderDescription
{
public:
	char* ID;

	int NLines;
	short** Lines;
	unsigned short* LineNU;

	int NCom;
	short* ComX;
	short* ComY;
	short YShift;
	int NUnits;

	short BarX0;
	short BarY0;
	short BarX1;
	short BarY1;

	//symmetry groups
	unsigned short* Sym4f;
	unsigned short* Sym4i;
	unsigned short* SymInv;

	//additional parameters
	char AddDamage1;
	char AddShield1;
	char AddDamage2;
	char AddShield2;
	unsigned char GroupID;

	OrderDescription();
	~OrderDescription();
};

class StroiDescription
{
public:
	unsigned char ID;
	unsigned short NAmount;
	unsigned short* Amount;
	unsigned short* LocalID;
	unsigned short NUnits;
	unsigned short* Units;
};

class OfficerRecord
{
public:
	unsigned short BarabanID;
	unsigned short NStroi;
	StroiDescription SDES[3];
};

struct Flags3D
{
	int N;
	short Xr;
	short Points[48];
};

struct OneAddSprite
{
	unsigned short SpriteID;
	short SortX;
	short SortY;
};

struct OneAddStage
{
	unsigned short GPID;

	OneAddSprite Empty;
	OneAddSprite Stage1;
	OneAddSprite Ready;
	OneAddSprite Dead;

	unsigned short AddPoints;

	int NExplPoints;
	short* ExplCoor;

	short* FireX[2];
	short* FireY[2];
	short  NFires[2];

	int Cost[8];
};

#define MaxAStages 5
struct ComplexBuilding
{
	unsigned char Mask;
	OneAddStage Stages[MaxAStages];
};

class NewMonster
{
public:
	NewAnimation MotionL;//first part of motion
	NewAnimation MotionR;//second part of motion
	NewAnimation MotionLB;
	NewAnimation MotionRB;
	NewAnimation MiniStepL;
	NewAnimation MiniStepR;
	NewAnimation MiniStepLB;
	NewAnimation MiniStepRB;
	NewAnimation Fist;
	NewAnimation Death;
	NewAnimation DeathLie1;
	NewAnimation DeathLie2;
	NewAnimation DeathLie3;
	NewAnimation Stand;
	NewAnimation Work;
	NewAnimation Trans01;
	NewAnimation Trans10;
	NewAnimation Attack[NAttTypes];
	NewAnimation PAttack[NAttTypes];
	NewAnimation UAttack[NAttTypes];
	NewAnimation PStand[NAttTypes];
	NewAnimation PMotionL[NAttTypes];
	NewAnimation PMotionR[NAttTypes];
	NewAnimation PMotionLB[NAttTypes];
	NewAnimation PMotionRB[NAttTypes];
	NewAnimation PMiniStepL[NAttTypes];
	NewAnimation PMiniStepR[NAttTypes];
	NewAnimation PMiniStepLB[NAttTypes];
	NewAnimation PMiniStepRB[NAttTypes];

	NewAnimation StandHi;
	NewAnimation StandLo;
	NewAnimation Build;
	NewAnimation BuildHi;
	NewAnimation Damage;

	NewAnimation Rest;
	NewAnimation Rest1;
	NewAnimation RotateR;
	NewAnimation RotateL;

	NewAnimation WorkTree;
	NewAnimation WorkStone;
	NewAnimation WorkField;

	NewAnimation TransX3;
	NewAnimation Trans3X;

	unsigned char TransXMask;

	unsigned short AttackRadius1[NAttTypes];		//начальный радиус атаки в зависимости от типа
	unsigned short AttackRadius2[NAttTypes];		//конечный  радиус атаки в зависимости от типа
	unsigned short DetRadius1[NAttTypes];			//Радиус, по которому определяется какой тип атаки использовать
	unsigned short DetRadius2[NAttTypes];
	unsigned short AttackRadiusAdd[NAttTypes];	//добавка к радиусу атаки при условии, 
										//что юнит в сострянии немедленно атаковать
	Weapon* DamWeap[NAttTypes];			//оружие для поражения(пуля,ядро,...)

	unsigned char Rate[NAttTypes];			//16=x1 rate
	unsigned short AttackPause[NAttTypes];
	short AngleUp[NAttTypes];		//64=45degrees,32=arctan(1/2)degrees
	short AngleDn[NAttTypes];
	short MinDamage[NAttTypes];
	short MaxDamage[NAttTypes];
	short DamageRadius[NAttTypes];
	unsigned short  DamageDecr[NAttTypes];
	unsigned char  WeaponKind[NAttTypes];
	unsigned char  AttackMask[NAttTypes];

	unsigned short  MyIndex;
	short SrcZPoint;				//additional height of the weapon
	short DstZPoint;
	unsigned short NLockPt;
	unsigned char* LockX;
	unsigned char* LockY;

	unsigned short NSLockPt[MaxAStages];
	unsigned char* SLockX[MaxAStages];
	unsigned char* SLockY[MaxAStages];

	unsigned short NBLockPt;
	unsigned char* BLockX;
	unsigned char* BLockY;

	unsigned short NCheckPt;
	unsigned char* CheckX;
	unsigned char* CheckY;

	ComplexBuilding* CompxCraft;

	char* Message;
	bool Officer : 1;
	bool Baraban : 1;
	bool Building : 1;
	bool Peasant : 1;
	bool UnitAbsorber : 1;
	bool PeasantAbsorber : 1;
	bool Transport : 1;
	bool Producer : 1;
	bool SpriteObject : 1;
	bool Wall : 1;
	bool RiseUp : 1;
	bool SelfProduce : 1;
	bool WaterActive : 1;
	bool TwoParts : 1;
	bool Port : 1;
	bool Farm : 1;
	bool ShowDelay : 1;
	bool Capture : 1;
	bool NotHungry : 1;
	bool ShotForward : 1;
	bool Artilery : 1;
	bool Rinok : 1;
	bool SlowDeath : 1;
	bool AutoNoAttack : 1;
	bool AutoStandGround : 1;
	bool AttBuild : 1;
	bool CanStandGr : 1;
	bool Priest : 1;
	bool ResSubst : 1;
	bool Archer : 1;
	bool ArtDepo : 1;
	bool Artpodgotovka : 1;
	bool CanBeKilledInside : 1;
	bool CanBeCapturedWhenFree : 1;
	bool CanShoot : 1;
	bool CanStorm : 1;
	bool NoDestruct : 1;
	bool SlowRecharge : 1;

	unsigned char NInArtDepot;
	unsigned char NInFarm;
	unsigned char ArtSet;
	unsigned char TransMask[MaxAStages];
	NewAnimation* UpperPart;
	short UpperPartShift;
	NewAnimation* BuiAnm;
	unsigned char ArtCap[4];
	short BuiDist;
	short MaxPortDist;
	unsigned char NRiseFrames;
	unsigned char RiseStep;
	unsigned char ProdType;
	unsigned char SelfProduceStep;
	unsigned short FreeAdd;
	unsigned short PeasantAdd;
	unsigned short MaxInside;
	unsigned char ResConcentrator;
	unsigned char MaxZalp;
	unsigned char NoWaitMask;
	unsigned char UnitRadius;
	int Ves;

	short AddShotRadius;
	unsigned char  PromaxPercent;

	unsigned short NBuildPt;
	char* BuildPtX;
	char* BuildPtY;

	unsigned short NConcPt;
	char* ConcPtX;
	char* ConcPtY;

	unsigned short NDamPt;
	char* DamPtX;
	char* DamPtY;

	unsigned short NBornPt;
	char* BornPtX;
	char* BornPtY;

	unsigned short NShotPt;
	short* ShotPtX;
	short* ShotPtY;
	short* ShotPtZ;
	unsigned char* ShotDir;
	unsigned char* ShotDiff;
	unsigned short* ShotMinR;
	unsigned short* ShotMaxR;

	short* FireX[2];
	short* FireY[2];
	short  NFires[2];
	unsigned char MaxResPortion[8];
	int  NeedRes[8];
	int CenterMX;
	int CenterMY;
	int BRadius;
	unsigned short ProduceStages;
	unsigned short IconFileID;
	unsigned short IconID;
	int Radius1;
	int Radius2;
	int MotionDist;
	int OneStepDX[256];
	int OneStepDY[256];
	int POneStepDX[256];
	int POneStepDY[256];
	int PicDx, PicDy, PicLx, PicLy;
	short BuildX0, BuildY0, BuildX1, BuildY1;
	unsigned char KillMask;
	unsigned char MathMask;

	//rectangle around the monster
	int RectLx, RectLy, RectDx, RectDy;

	//Monster characteristics
	unsigned short Res1cost, Res2cost, Res3cost;
	unsigned short Life, Shield;
	unsigned short AttRange, VisRange;
	unsigned short MinAttRange, MaxAttRange, MaxNearRange;
	unsigned short AttType;
	unsigned short Time, Kind;
	short AnmUpShift;
	short* Bars3D;
	short  NBars;
	char* Name;

	//fogging & fire
	FogRec Fogging;
	FogRec Destruct;
	FogRec Fire;
	unsigned short Protection[16];
	unsigned short Sprite;
	unsigned short SpriteVisual;
	unsigned char ExplosionMedia;
	unsigned char EMediaRadius;
	unsigned char LockType;		//0-Land,1-Water
	unsigned char MotionStyle;	//0-Soldiers,1-Cavalery,2-Sheeps,3-Fly
	NewAnimation* Veslo;
	NewAnimation* Reflection;
	unsigned char VisionType;
	short VesloRotX;
	short VesloRotY;
	unsigned char NVesel;
	short* VesloX;
	short* VesloY;
	short LinearLength;
	short* MultiWpX;
	short* MultiWpY;
	short NMultiWp;
	short MultiWpZ;
	short ResEff;
	short SelectSoundID;
	short AttackSoundID;
	short BornSoundID;
	short OrderSoundID;
	short DeathSoundID;
	short StrikeSoundID;

	short* HideTriX;
	short* HideTriY;
	short NHideTri;

	unsigned short  Razbros;
	unsigned short  ExplRadius;

	unsigned short ResConsumer;
	unsigned char ResConsID;

	unsigned char MaxAIndex;
	unsigned char Behavior;
	unsigned char ResAttType;
	unsigned char ResAttType1;
	unsigned char NShotRes;
	unsigned short* ShotRes;
	unsigned short CostPercent;
	unsigned char VesStart;
	unsigned char VesFin;
	unsigned char MinRotator;
	unsigned char FishSpeed;
	unsigned short FishAmount;
	unsigned char InfType;
	unsigned short PictureID;
	unsigned char Force;//for AI
	//for strongholds siege
	unsigned char MinOposit;
	unsigned char MaxOposit;
	unsigned char StormForce;

	short* WavePoints;
	unsigned char NWaves;
	char WaveDZ;
	unsigned char Usage;
	unsigned char BattleForce;
	Flags3D* FLAGS;
	char* MD_File;
	NewMonster();
	bool CreateFromFile( char* Name );
	NewAnimation* LoadNewAnimationByName( char* Name );
	AdvCharacter* AdvChar;
};

class AdvCharacter
{
public:
	unsigned short		AttackRadius1[NAttTypes];
	unsigned short		AttackRadius2[NAttTypes];
	unsigned short		DetRadius1[NAttTypes];
	unsigned short		DetRadius2[NAttTypes];
	unsigned short		AttackPause[NAttTypes];
	short		MaxDamage[NAttTypes];
	unsigned char		WeaponKind[NAttTypes];
	unsigned char		Rate[NAttTypes];
	unsigned char		Protection[16];
	int			NeedRes[8];
	unsigned short		MaxInside;
	unsigned short		ProduceStages;
	unsigned short		Life;
	unsigned short		Shield;
	unsigned char		FishSpeed;
	unsigned short		FishAmount;
	unsigned short        Razbros;

	unsigned short        MinR_Attack;
	unsigned short		MaxR_Attack;
	unsigned short        MaxDam;
	short       ResEff;

	bool		Changed;
};

class NewUpgrade
{
public:
	//information
	char* Name;
	char* Message;
	unsigned char Level;
	unsigned char Branch;
	unsigned short IconFileID;
	unsigned short IconSpriteID;
	char IconPosition;
	unsigned char NatID;
	unsigned short  Cost[8];
	unsigned char CtgUpgrade;
	unsigned char CtgType;
	union
	{
		int  NCtg;
		int  CtgValue;
	};
	unsigned short*    CtgGroup;
	unsigned char UnitType;
	union
	{
		int NUnits;
		int UnitValue;
	};
	unsigned short* UnitGroup;
	unsigned char ValueType;
	union
	{
		int  Value;
		int  NValues;
	};
	int* ValGroup;

	//mechanics
	bool Done : 1;
	bool Enabled : 1;
	bool PermanentEnabled : 1;
	bool IsDoing : 1;
	bool Gray : 1;
	bool Individual : 1;
	bool ManualDisable : 1;
	bool StageUp : 1;
	unsigned short NStages;
	unsigned short CurStage;
	unsigned char NAutoPerform;
	unsigned char NAutoEnable;
	unsigned short* AutoPerform;
	unsigned short* AutoEnable;
	unsigned char Options;
};

//Описание базового типа, общего для всех объектов
class GeneralObject
{
public:
	char* Message;

	bool NoSearchVictim : 1;
	bool Enabled : 1;
	bool CondEnabled : 1;
	bool Transport : 1;
	int Options : 1;
	bool WATT : 1;
	bool AATT : 1;
	bool P100 : 1;
	bool T3X3 : 1;
	bool FWEAP : 1;//огненное оружие
	bool AGold : 1;
	bool AWood : 1;
	bool Submarine : 1;
	bool RefreshLife : 1;
	bool CanFly : 1;
	bool CanAttWall : 1;
	bool CanFear : 1;
	bool canNucAttack : 1;
	bool AntiNuc : 1;
	bool UFO : 1;
	bool UFOTrans : 1;
	bool CanRepair : 1;
	bool ExtMenu : 1;
	bool CanDest : 1;
	bool ManualDisable : 1;
	unsigned char NatID;
	unsigned char Country;
	OfficerRecord* OFCR;
	NewMonster* newMons;
	char* MonsterID;
	unsigned short MagAgainst;
	unsigned short MagWith;
	unsigned short IDforUFO;
	unsigned char Kind;
	unsigned short MaxAutoAmount;
	unsigned short Useful[NBRANCH];//полезность объекта для каждой из отраслей
	unsigned short SpUsef[NBRANCH];//удельная полезность
	unsigned short AbsNeedPrio;
	unsigned short AbsNeedCount;
	unsigned short LifeShotLost;
	int	 cost;
	unsigned short delay;
	unsigned char WepSpeed;
	unsigned char WepDelay;
	unsigned short NUpgrades;
	unsigned short* Upg;
	char Wdx[8];
	char Wdy[8];
	unsigned char NIcons;
	unsigned short* IRefs;
	unsigned short NWeap;
	unsigned char NStages;
	Weapon* MWeap[12];
	unsigned char SWPIndex[12];
	short HitSound;
	short ClickSound;
	short OrderSound;
	short DeathSound;
	short BornSound;
	short AttackSound;
	short TreeSound;
	short GoldSound;
	unsigned short LockID;
	unsigned char NLockUnits;
	unsigned char Branch;

	unsigned char SizeX;
	unsigned char SizeY;

	unsigned char ResourceID[4];		//Индекс необходимого ресурса
	unsigned short ResAmount[4];		//Сколько необходимо для постройки 
	unsigned short NAnm;				//Количество доступных анимаций
	MoreAnimation *lpFAnim;	//Анимация с описанием применения 
	AdvCharacter* MoreCharacter;

	void GetMonsterCostString( char* st );
	void CloseGO();
};

class Visuals : public GeneralObject
{
};

//Upgrade-замена одного типа объектов на другой
class ObjectUpgrade : public GeneralObject
{
public:
	unsigned short SourceTypeIndex[8];
	unsigned short FinalTypeIndex[8];
};

//Upgrade - для Visuals-улучшение параметров для группы объектов;
class ParameterUpgrade : public GeneralObject
{
	unsigned short ObjectIndex[8];
	unsigned char ChangedParameter[8];
	unsigned char AdditionalValue[8];
};

class OneObject;
typedef void ReportFn( OneObject* Sender );

//Описание командного блока
//Приказы 1-го уровня
struct Order1
{
	//Указатель на отложенный приказ, если NULL то нет 
	//отложенного приказа 
	Order1* NextOrder;

	//Уровень приоритета выполнения команды
	//0-нижайший уровень приоритета
	//Данное задание может быть отложено только в том случае,
	//если приоритет прерывания выше приоритета выполнения
	unsigned char PrioryLevel;
	unsigned char OrderType;
	unsigned char OrderTime;//=0 if very fast
	//=1 - стоять на месте 
	//=2 - перейти в точку с координатами (x,y)
	//=3 - следовать за объектом (obj)
	//=4 - преследовать и атаковать объект (obj)
	//=5 - построить объект
	//=6 - создать объект
	//=7 - патрулировать

	//Информация по каждому типу приказов
	ReportFn* DoLink;
	union
	{
		struct
		{
			unsigned char VisibilityRadius;
		} Stand;

		struct
		{
			int x;
			int y;
			unsigned short PrevDist;
			unsigned char Times;
		} MoveToXY;

		struct
		{
			unsigned char xd;
			unsigned char yd;
			unsigned char time;
			unsigned short BuildID;
			unsigned short BSN;
		} UFO;

		struct
		{
			unsigned short ox;
			unsigned short oy;
			unsigned short x, y, z;
			unsigned char wep;
		} AttackXY;

		struct
		{
			unsigned short ObjIndex;
			unsigned short SN;
			unsigned short PrevDist;
			unsigned char wep;
		} MoveToObj;

		struct
		{
			int ObjIndex;
			unsigned short SN;
			short ObjX;
			short ObjY;
			unsigned char AttMethod;
		} BuildObj;

		struct
		{
			unsigned short x;
			unsigned short y;
			unsigned short x1;
			unsigned short y1;
			unsigned char dir;
		} Patrol;

		struct
		{
			unsigned short ObjIndex;
			unsigned short Progress;
			unsigned short NStages;
			unsigned short ID;
			unsigned char PStep;
			unsigned short Power;
		} Produce;

		struct
		{
			unsigned short OldUpgrade;
			unsigned short NewUpgrade;
			unsigned short Stage;
			unsigned short NStages;
		} PUpgrade;

		struct
		{
			unsigned char dir;
		} MoveFrom;

		struct
		{
			int x;
			int y;
			int SprObj;
			unsigned char ResID;
		} TakeRes;

		struct
		{
			short LockX;
			short LockY;
			short EndX;
			short EndY;
		} DelBlock;

		struct
		{
			unsigned short x, y;
			short dx, dy;
			unsigned short NextX, NextY, NextTop;
		} SmartSend;

	} info;
};

class GOrder;
struct GlobalIconInfo
{
	HandlePro* HPLeft;
	HandlePro* HPRight;
	int LParam;
	int RParam;
	int IconSpriteID;
	char* Hint;
};

typedef bool GOrderFn( OneObject* OB, GOrder* GOR, int LParam, int RParam );
typedef int IconInfo( GOrder* GOR, int IcoIndex, OneObject* OB, GlobalIconInfo* GIN );

class GOrder
{
public:
	GOrderFn* CheckDisconnectionAbility;
	GOrderFn* Disconnect;
	GOrderFn* KillOrder;
	IconInfo* GetIcon;
	void*     Data;
	GOrder();
	~GOrder();
};

class Legion;
class Nation;

class FireObjectInfo
{
public:
	unsigned char* FireSprite;
	unsigned char* FireStage;
	unsigned char delay;
	short NeedFires;
	short RealFires;
	void  Erase();
};

class FireInfo
{
public:
	unsigned char BStage;
	FireObjectInfo* Objs[2];
	void Erase();
};

//Информация для каждого индивидуума
class OneObject
{
public:
	Nation* Nat;
	union
	{
		GeneralObject* General;
		Visuals* Visual;
		ObjectUpgrade* OUpgrade;
		ParameterUpgrade* PUpgrade;
	} Ref;
	NewMonster* newMons;
	//-----------new path algoritm variables----------------
	short* PathX;
	short* PathY;
	unsigned short Index;//номер этого монстра
//--------------------------------------------//
//-----------begin of saving cluster----------//
//--------------------------------------------//
	bool NeedPath;
	unsigned short CPdestX;
	unsigned short CPdestY;
	unsigned short NIPoints;
	unsigned short CurIPoint;
	unsigned short NIndex;	//индекс в национальном списке
	unsigned char Selected;
	unsigned char ImSelected;
	bool SelectedTemp : 1;
	bool Borg : 1;
	bool Invert : 1;
	bool Attack : 1;
	bool NoMotion : 1;		//не сдвигаться ни прт каких условиях,кроме приказа по армии
	bool NoFire : 1;		//не открывать огонь по врагу и не привлекать внимания
	bool NoInitiate : 1;	//не инициировать сражение
	bool WasInMobilZone : 1;//Был в зоне мобилизации
	bool TempFlag : 1;		//временный флаг, используемый для сортировки отряда
	bool Mobilised : 1;
	bool MoveInRect : 1;	//установлен, если среди риказов есть приказ о перемещении группы
	bool DrawUp : 1;		//СТРРPPОИТЬСЯ!!!
	bool PathBroken : 1;	//Если хоть раз была преграда на пути
	bool capBuilding : 1;
	bool capBuild : 1;
	bool capBase : 1;
	bool Ready : 1;
	bool NoSearchVictim : 1;
	bool AskMade : 1;
	bool SafeWall : 1;
	bool StandGround : 1;
	bool Invisible : 1;
	bool Zombi : 1;

	bool Transport : 1;
	bool Absent : 1;
	bool InFire : 1;
	bool RefreshLife : 1;
	bool DoWalls : 1;
	bool Use_AI : 1;
	bool AntiNuc : 1;

	bool NewMonst : 1;
	bool Collision : 1;
	bool LeftLeg : 1;
	//--------------New
	bool InMotion : 1;	//Выполняется перемещениe(New)
	bool BackMotion : 1;//шаг назад
	bool HasStatePA : 1;
	bool NewBuilding : 1;
	bool Hidden;
	bool Wall : 1;
	bool UnlimitedMotion : 1;
	bool LLock : 1;
	bool GLock : 1;
	bool AlwaysLock : 1;
	bool InternalLock : 1;
	bool TurnOff : 1;
	bool InArmy : 1;
	bool InPatrol : 1;

	//------------------for AI----------------
	bool NoBuilder : 1;
	bool DoNotCall : 1;
	bool AutoKill : 1;

	//-----------For best motion--------------
	short bm_DestX;
	short bm_DestY;
	short bm_NextX;
	short bm_NextY;
	short bm_dx;
	short bm_dy;
	short bm_PrevTopDst;
	unsigned short Guard;
	unsigned short bm_NextTop;
	void CreateSmartPath( int x, int y, int dx, int dy );
	void FindNextSmartPoint();
	//----------------------------------------

	unsigned char NZalp;
	unsigned char RotCntr;
	int NextForceX;
	int NextForceY;
	int BestNX;
	int BestNY;

	char OverEarth;
	unsigned short Kills;
	unsigned char LockType;//0-Land,1-Water
	unsigned char NothingTime;
	unsigned short BlockInfo;
	unsigned short Sdoxlo;
	unsigned char BackSteps;
	unsigned char BackReserv;
	unsigned char NewState;//0-Normal,1-prepare to attack
	unsigned char LocalNewState;
	unsigned char LeftVeslo;
	unsigned char RightVeslo;
	unsigned char Usage;
	char AddDamage;
	char AddShield;

	//------------Extended animation-----------//
	unsigned char CurAIndex;
	unsigned char MaxAIndex;
	unsigned char MoveStage;
	//-----------------End New-----------------//

	unsigned char BackDelay;

	unsigned char AbRes;

	short WallX;
	short WallY;

	unsigned short NUstages;
	unsigned short Ustage;
	unsigned short Serial;
	unsigned char Kind;
	short Lx;
	short Ly;

	unsigned short Life;
	unsigned short MaxLife;
	int x;
	int y;
	int DstX;
	int DstY;

	unsigned short delay;
	unsigned char PathDelay;
	unsigned short MaxDelay;

	unsigned char StandTime;
	unsigned char NStages;
	unsigned short Stage;
	unsigned char NNUM;

	unsigned short EnemyID;
	unsigned short EnemySN;
	unsigned char NMask;
	unsigned char RStage;
	unsigned char RType;
	unsigned char RAmount;
	unsigned short NearBase;

	unsigned short BrigadeID;
	unsigned short BrIndex;
	unsigned char Media;//=0-terrain,=1-on water,=2-on air
	unsigned short AddInside;
	unsigned char PersonalDelay;
	short RZ;

	//Flying objects only:
	int  RealX;//1 pixel=16 length units
	int  RealY;
	int  DestX;
	int  DestY;
	int  RealVx;
	int  RealVy;
	int  BestDist;
	int  BestHeight;
	int	 Height;
	int  ForceX;
	int  ForceY;
	int  Radius1;
	int  Radius2;
	int  Speed;
	unsigned char GroundState;
	unsigned char SingleUpgLevel;
	unsigned char RealDir;
	unsigned char GraphDir;
	unsigned char NewCurSprite;
	unsigned char MotionDir;
	unsigned char PrioryLevel;
	unsigned short NInside;
	//-----------------------------------------------//
	//-------------end of saving cluster-------------//
	//-----------------------------------------------//

	unsigned short* Inside;

	//for all objects
	SelGroup* GroupIndex;
	FireInfo* IFire;
	//Указатель на текущий приказ 1-го уровня
	//Типы приказов:
	//	Перейти(x,y)->(x1,y1) по оптимальному пути;
	//	Атаковать объект(obj_index)- удаленный или близкий
	//	Умереть
	//	Создать экземпляр объекта(obj)...
	Order1* LocalOrder;
	GOrder* GlobalOrder;
	//Weapon* Weap;
	//New animation settings
	NewAnimation* HiLayer;
	NewAnimation* NewAnm;
	NewAnimation* LoLayer;

	//Methods declaration
	void DefaultSettings( GeneralObject* GO );
	void NewMonsterSendTo( int x, int y, unsigned char Prio, unsigned char OrdType );
	void NewMonsterPreciseSendTo( int x, int y, unsigned char Prio, unsigned char OrdType );
	void NewMonsterSmartSendTo( int x, int y, int dx, int dy, unsigned char Prio, unsigned char OrdType );
	void MakeProcess();
	void MakePreProcess();
	void WSendTo( int x2, int y2, int Prio );
	bool AttackObj( unsigned short OID, int Prio );
	bool AttackObj( unsigned short OID, int Prio, unsigned char OrdType );
	bool AttackObj( unsigned short OID, int Prio, unsigned char OrdType, unsigned short NTimes );
	void WAttackObj( unsigned short OID, int Prio );
	void FlyAttack( unsigned short OID, unsigned char Prio );
	void AttackPoint( unsigned char x, unsigned char y, unsigned char wep, int Prio );
	bool AttackPoint( int x, int y, int z, unsigned char Times, unsigned char Flags, unsigned char OrdType );
	bool NewAttackPoint( int x, int y, int Prio1, unsigned char OrdType, unsigned short NTimes );
	void ProcessFly();
	void CreatePath( int x1, int y1 );
	void CreateSimplePath( int x1, int y1 );
	bool CreatePrePath( int x1, int y1 );
	bool CreatePrePath2( int x1, int y1 );
	bool CreatePrePath4( int x1, int y1 );
	void ProcessNewMotion();
	void FreeAsmLink();
	void Die();
	void Eliminate();
	void MakeDamage( int Fundam, int Persist, OneObject* Sender );
	void MakeDamage( int Fundam, int Persist, OneObject* Sender, unsigned char AttType );
	void SearchVictim();
	void FreeOrdBlock( Order1* p );
	void ClearOrders();
	void ProcessMotion();
	void ProcessAttackMotion();
	void SendInGroup( unsigned char tx, unsigned char ty, unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1 );
	void NextStage();
	bool BuildObj( unsigned short OID, int Prio, bool LockPoint, unsigned char OrdType );
	int CheckAbility( unsigned short ID, const bool running_production );
	void Produce( unsigned short ID );
	void Produce( unsigned short ID, unsigned short GroupID );
	bool BuildWall( int xx, int yy, unsigned char Prio, unsigned char OrdType, bool TempBlock );
	bool DamageWall( unsigned short OID, int Prio );
	void PerformUpgrade( unsigned short NewU, unsigned short MakerID );
	void SetDstPoint( int x, int y );
	void Patrol( int x1, int y1, int x2, int y2, unsigned char prio );
	void EnableDoubleForce();
	void DisableDoubleForce();
	void EnableTripleForce();
	void DiasableTripleForce();
	void EnableQuadraForce();
	void DisableQuadraForce();
	void ContinueAttackPoint( unsigned char x, unsigned char y, int Prio );
	void ContinueAttackWall( unsigned char x, unsigned char y, int Prio );
	void MakeMeUFO();
	void WaitForRepair();
	void MakeMeSit();
	//-------New---------
	void BlockUnit();
	void WeakBlockUnit();
	void UnBlockUnit();
	void DeletePath();
	void CheckState();
	bool CheckLocking( int dx, int dy );
	void SetDestCoor( int x, int y );
	void EscapeLocking();
	bool FindPoint( int* x1, int* y1, unsigned char Flags );
	void ClearBuildPt();
	bool CheckBlocking();
	void DeleteBlocking();

	//returns points for damage
	bool GetDamagePoint( Coor3D* dp, int Precise );

	void TakeResourceFromSprite( int SID );
	void SetOrderedUnlimitedMotion( unsigned char OrdType );
	void ClearOrderedUnlimitedMotion( unsigned char OrdType );
	void ClearOrderedUnlimitedMotion( unsigned char OrdType, unsigned short GroupID );
	int TakeResource( int x, int y, unsigned char ResID, int Prio, unsigned char OrdType );
	unsigned short FindNearestBase();

	//Type:
	//0 - single order (previous orders will be erased)
	//1 - add order to the head of link
	//2 - add order to the tile of link
	Order1* CreateOrder( unsigned char Type );

	void DeleteLastOrder();
	void GetCornerXY( int* x, int* y );
	bool GoToMine( unsigned short ID, unsigned char Prio );
	bool GoToMine( unsigned short ID, unsigned char Prio, unsigned char Type );
	bool GoToTransport( unsigned short ID, unsigned char Prio );
	void LeaveMine( unsigned short Type );
	void LeaveTransport( unsigned short Type );
	void HideMe();
	void ShowMe();
	bool CheckOrderAbility( int LParam, int RParam );
	bool CheckOrderAbility();
	void GlobLock();
	void GlobUnlock();
	void Fishing();
	Brigade* GetBrigade();

	inline int DistTo( int xx, int yy )
	{
		__asm
		{
			mov		eax, xx
			mov		ebx, this
			mov		edx, [ebx]this.x
			sub		eax, edx
			jge		uui
			neg		eax
			uui : mov		ecx, yy
				  mov		edx, [ebx]this.y
				  sub		ecx, edx
				  jge		uux
				  neg		ecx
				  uux : cmp		ecx, eax
						jl		uuz
						mov		eax, ecx
						uuz :
		}
	};
	void CloseObject();
};

//Описание оружия
class Nation;
class ChildWeapon
{
public:
	Weapon* Child[4];
	unsigned char    NChildWeapon;
	unsigned char    MaxChild;
	unsigned char    MinChild;
	unsigned char    Type;
	void InitChild();
};

class Weapon
{
public:
	NewAnimation* NewAnm;
	ChildWeapon Default;
	ChildWeapon* CustomEx;
	unsigned char    NCustomEx;
	Weapon* ShadowWeapon;
	Weapon* TileWeapon[4];
	unsigned short    TileProbability;
	unsigned char	NTileWeapon;
	unsigned char    MinChild;
	unsigned char    MaxChild;
	unsigned char    HotFrame;
	Weapon* SyncWeapon[4];
	unsigned char	NSyncWeapon;
	char    dz;
	//see scripts
	short   Damage;
	short   Radius;
	short   Speed;
	short   Times;
	unsigned char    Transparency;
	unsigned char    Gravity;
	unsigned char    Propagation;
	bool    FullParent : 1;
	bool    HiLayer : 1;
	short	SoundID;
	unsigned short MyIndex;
};

//Описание элемента анимации(оружие в действии)
class AnmObject
{
public:
	NewAnimation* NewAnm;
	int x, y, z;//координаты
	int vx, vy, vz;//скорости
	int az;//ускорение
	int xd, yd, zd;//точка назначения
	short Frame;
	short NTimes;
	Weapon* Weap;
	unsigned short    Damage;
	OneObject* Sender;
	unsigned short ASerial;
	unsigned short DestObj;
	unsigned short DestSN;
	unsigned char AttType;
	char WeaponKind;
};

class City;
class Needness
{
public:
	unsigned char NeedType;//==0-monster,==1-Upgrade
	unsigned short MonID;
	unsigned char GroupSize;
	unsigned char Amount;
	unsigned short Probability;
	unsigned short MoneyPercent;
};

struct SWPAR
{
	unsigned short Range;
	bool Enabled : 1;
	bool Fly : 1;
};

struct sAI_Req
{
	unsigned char Kind;//0-unit,1-upgrade,2-group
	unsigned short ObjID;
	unsigned short Amount;//if upgrade:1-Done 2-Enabled
};

struct sAI_Devlp
{
	unsigned char Kind;//0-unit,1-upgrade
	unsigned char Source;//0-general,1-army,2-selo,3-science
	unsigned char ConKind;//0-unit,2-group
	unsigned short ObjID;
	unsigned short ConID;
	unsigned short Amount;
	unsigned short GoldPercent;
	unsigned short AtnPercent;
};

struct sAI_Cmd
{
	unsigned char Kind;//1-army,2-selo,3-science
	unsigned short Info[8];
};

class Branch
{
public:
	int  RESAM[8];
	unsigned short RESP[8];
	int  RESRM[8];
	void AddTo( unsigned char ResID, int Amount );
	void AddPerc( unsigned char ResID, int Amount, int perc );
	void AddEntire( unsigned char ResID, int Amount );
	void Check( unsigned char NI );
	void Init();
	int GetMonsterCostPercent( unsigned char NI, unsigned short NIndex );
	int GetUpgradeCostPercent( unsigned char NI, unsigned short NIndex );
};

//Описание нации в целом
struct U_Grp
{
	unsigned short N;
	unsigned short* UIDS;
	unsigned short* UVAL;
};

typedef void VoidProc();

class Nation
{
public:
	char SCRIPT[16];
	int NMon;
	bool GoldBunt : 1;
	GeneralObject* Mon[2048];
	unsigned short NKilled[2048];
	unsigned short NProduced[2048];
	unsigned char SoundMask[2048];
	unsigned char VictState;//0-? 1-defeat 2-victory

	//----Resource control-------
	int ResTotal[8];
	int ResOnUpgrade[8];
	int ResOnMines[8];
	int ResOnUnits[8];
	int ResOnBuildings[8];
	int ResOnLife[8];
	int ResBuy[8];
	int ResSell[8];
	//---------------------------

	City* CITY;
	int NGidot;
	int NFarms;
	unsigned short NArtdep;
	unsigned short NArtUnits[4];
	unsigned short* PAble[2048];
	unsigned short PACount[2048];
	char* AIndex[2048];
	char* AHotKey[2048];
	int BranchPercent[NBRANCH];
	int NUpgrades;
	NewUpgrade* UPGRADE[4096];
	int NOct;
	int NSlides;

	//AI Statements
	int CasheSize;
	int TAX_PERCENT;
	int CASH_PUSH_PROBABILITY;
	int NationalAI;//0..32768-determines speed of development
	int AI_Level_MIN;
	int AI_Level_MAX;
	int AI_Forward;
	short DangerSound;
	short VictorySound;
	short ConstructSound;
	short BuildDieSound;
	short UnitDieSound;
	unsigned short LastAttackTime;
	unsigned short Harch;
	unsigned short NLmenus;
	unsigned short* Lmenus;
	unsigned short NAmenus;
	unsigned short* Amenus;
	unsigned short NWmenus;
	unsigned short* Wmenus;
	unsigned short NCmenus;
	unsigned short* Cmenus;
	unsigned short NNeed;
	Needness NEED[1024];
	int ResRem[8];
	int ResSpeed[8];

	unsigned char NNUM;
	int  NFinf;
	unsigned char palette[256];
	unsigned char NMask;
	unsigned short NIcons;
	WIcon* wIcons[1024];
	unsigned short NCOND;
	unsigned short CLSize[4096];//Access controlling
	unsigned short* CLRef[4096];

	//Strange weapon prameters
	unsigned short SWRange[256];
	SWPAR SWP[256];

	//-------------NEW AI--------------
	unsigned short NGrp; //Groups of types definition
	unsigned short GRSize[32];
	unsigned short* GRRef[32];
	unsigned short  GAmount[32];//Result of calculation
	unsigned short N_AI_Levels;
	unsigned short N_AI_Req[256];
	unsigned short N_AI_Devlp[256];
	unsigned short N_AI_Cmd[256];
	sAI_Req* AI_Req[256];
	sAI_Devlp* AI_Devlp[256];
	sAI_Cmd* AI_Cmd[256];
	unsigned short AI_Level;
	unsigned short NPBal;
	unsigned short* PBalance;
	unsigned short NMineBL;
	unsigned short* PBL;

	int POnFood;
	int POnWood;
	int POnStone;

	char* DLLName;
	// TODO: replace with cross-platform alternative
	VoidProc* ProcessAIinDLL;
	void* hLibAI;

	//------------------SHAR----------------//StartSave
	unsigned char SharStage;
	int SearchRadius;
	int SharX;
	int SharY;
	int SharZ;
	int SharVx;
	int SharVy;
	int SharVz;
	int SharAx;
	int SharAy;
	int SharAz;
	bool Vision : 1;
	bool SharAllowed : 1;
	bool SharPlaceFound : 1;
	bool AI_Enabled : 1;
	//---------Upgradable properties--------//
	unsigned short FoodEff;
	unsigned short WoodEff;
	unsigned short StoneEff;
	bool Geology;
	//---------------Constants--------------//
	unsigned short UID_PEASANT;//EndSave
	unsigned short UID_WALL;
	unsigned short UID_MINE;
	unsigned short UID_HOUSE;

	U_Grp UGRP_MINEUP;

	unsigned short  MINE_CAPTURE_RADIUS;
	unsigned short  MINE_UPGRADE1_RADIUS;
	unsigned short  MINE_UPGRADE2_RADIUS;
	unsigned short  DEFAULT_MAX_WORKERS;
	unsigned short  MIN_PBRIG;

	unsigned short  MU1G_PERCENT[3];
	unsigned short  MU1I_PERCENT[3];
	unsigned short  MU1C_PERCENT[3];

	unsigned short  MU2G_PERCENT[3];
	unsigned short  MU2I_PERCENT[3];
	unsigned short  MU2C_PERCENT[3];

	unsigned short  MU3G_PERCENT[3];
	unsigned short  MU3I_PERCENT[3];
	unsigned short  MU3C_PERCENT[3];
	//--------------------------------------

	char** History;
	int NHistory;

	//-----------------XRONIKA--------------
	unsigned char ThereWasUnit;
	int NPopul;
	int MaxPopul;
	unsigned short* Popul;

	int NAccount;
	int MaxAccount;
	unsigned short* Account;

	int NUpgMade;
	int MaxUpgMade;
	unsigned short* UpgIDS;

	int*  UpgTime;
	void AddUpgrade( unsigned short ID, int time );
	void AddPopul( unsigned short N );

	//---------------NEW resource-----------
	Branch SELO;
	Branch ARMY;
	Branch SCIENCE;
	Branch GENERAL;
	//----------------------choose unit menu
	char***UnitNames;
	int*   NUnits;
	unsigned short** UnitsIDS;
	unsigned short FormUnitID;
	//---------------------------------

	void CreateNation( unsigned char NMask, unsigned char NIndex );
	int  CreateNewMonsterAt( int x, int y, int n, bool Anyway );
	void AssignWeapon( Weapon* Wpn, int i );
	int CreateBuilding( unsigned short ID, unsigned char x, unsigned char y );
	bool CheckBuilding( unsigned short ID, unsigned char x, unsigned char y );
	void GetUpgradeCostString( char* st, unsigned short UI );
	void CloseNation();
	void AddResource( unsigned char Rid, int Amount );
	void ControlProduce( unsigned char Branch, unsigned char ResID, int Amount );
};

typedef char** lplpCHAR;
typedef char*  lpCHAR;
typedef int*   lpINT;

class SelGroup
{
public:
	unsigned short* Member;
	unsigned short* SerialN;
	unsigned short NMemb;
	bool CanMove : 1;
	bool CanSearchVictim : 1;
	bool CanHelpToFriend : 1;
	bool Egoizm : 1;
	SelGroup();
	void CreateFromSelection( unsigned char NI );
	void SelectMembers( unsigned char NI, bool Shift );
	void DeleteMembers();
	void ImSelectMembers( unsigned char NI, bool Shift );
};

//Массив все монстров на карте
#define MaxObj ULIMIT
#define maximage 2048

extern OneObject* Group[ULIMIT];
extern RLCTable MImage[maximage];
extern RLCTable miniMImage[maximage];

void LoadMonsters();

#define maxmap 256//(128<<1)//ADDSH)  //Don't change it!

void LoadLock();

#define MaxAsmCount 16384
#define OneAsmSize 256
#define OneAShift 8;
#define MaxOrdCount 32768
#define OneOrdSize 32;
#define OneOShift 5;
char* GetAsmBlock();
void FreeAsmBlock( char* p );
void InitAsmBuf();
Order1* GetOrdBlock();
void InitOrdBuf();
extern Order1 OrdBuf[MaxOrdCount];
extern bool	AsmUsage[MaxAsmCount];
extern int	msx;
extern int msy;
extern void Except();

//Размер очереди на выполнение(2^n only !)
#define StSize 8192
#define StMask StSize-1;
extern unsigned short ComStc[StSize];

#define FreeTime 600;
void CarryOutOrder();

void doooo();
extern unsigned short Creator;
extern Nation NAT;
extern int	smapx;
extern int	smapy;
extern int	smaplx;
extern int	smaply;
extern int minix;
extern int	miniy;

void MakePostProcess();
void MakeWPostProcess();
void PrepareProcessing();

extern int Flips;
extern int	mapx;
extern int	mapy;

void InitExplosions();
void ProcessExpl();
extern Weapon FlyFire1;
extern Weapon Vibux1;
void CloseExplosions();
extern unsigned char PlayerMask;
extern bool EgoFlag;
void AddAsk( unsigned short ReqID, unsigned char x, unsigned char y, char zdx, char zdy );
extern SelGroup SelSet[80];
extern Weapon* WPLIST[1024];

void InitZones();
int CreateZone( int x, int y, int lx, int ly, HandlePro* HPro, int Index, char* Hint );
void ControlZones();
void DeleteZone( int i );
void ShowProp();
void InitPrpBar();
void ShowAbility();

extern unsigned short* Selm[8];
extern unsigned short* SerN[8];
extern unsigned short* ImSelm[8];
extern unsigned short* ImSerN[8];
extern unsigned short ImNSL[8];
extern unsigned short NSL[8];

void CmdCreateSelection( unsigned char NI, unsigned char x, unsigned char y, unsigned char x1, unsigned char y1 );
void CmdSendToXY( unsigned char NI, int x, int y, short Dir );
void CmdAttackObj( unsigned char NI, unsigned short ObjID, short DIR );
void CmdCreateTerrain( unsigned char NI, unsigned char x, unsigned char y, unsigned short Type );
void CmdCreateBuilding( unsigned char NI, int x, int y, unsigned short Type );
void CmdProduceObj( unsigned char NI, unsigned short Type );
void CmdMemSelection( unsigned char NI, unsigned char Index );
void CmdRememSelection( unsigned char NI, unsigned char Index );
void CmdBuildObj( unsigned char NI, unsigned short ObjID );
void CmdBuildWall( unsigned char NI, short xx, short yy );
void CmdRepairWall( unsigned char NI, short xx, short yy );
void CmdDamageWall( unsigned char NI, unsigned short LIN );
void CmdTakeRes( unsigned char NI, int x, int y, unsigned char ResID );
void CmdPerformUpgrade( unsigned char NI, unsigned short UI );
void CmdCreateKindSelection( unsigned char NI, unsigned char x, unsigned char y, unsigned char x1, unsigned char y1, unsigned char Kind );
void CmdCreateTypeSelection( unsigned char NI, unsigned char x, unsigned char y, unsigned char x1, unsigned char y1, unsigned short Type );
void CmdCreateGoodSelection( unsigned char NI, unsigned short x, unsigned short y, unsigned short x1, unsigned short y1 );
void CmdCreateGoodKindSelection( unsigned char NI, unsigned short x, unsigned short y, unsigned short x1, unsigned short y1, unsigned char Kind );
void CmdCreateGoodTypeSelection( unsigned char NI, unsigned short x, unsigned short y, unsigned short x1, unsigned short y1, unsigned short Type );
void CmdSetDst( unsigned char NI, int x, int y );
void CmdSendToPoint( unsigned char NI, unsigned char x, unsigned char y );
void CmdAttackToXY( unsigned char NI, unsigned char x, unsigned char y );
void CmdStop( unsigned char NI );
void CmdStandGround( unsigned char NI );
void CmdPatrol( unsigned char NI, int x, int y );
void CmdRepair( unsigned char NI, unsigned char x, unsigned char y );
void CmdGetResource( unsigned char NI, unsigned char x, unsigned char y );
void CmdSendToTransport( unsigned char NI, unsigned short ID );
void CmdUnload( unsigned char NI, unsigned char x, unsigned char y );
void CmdDie( unsigned char NI );
void CmdContinueAttackPoint( unsigned char NI, unsigned char x, unsigned char y );
void CmdContinueAttackWall( unsigned char NI, unsigned char x, unsigned char y );
void CmdSitDown( unsigned char NI );
void CmdNucAtt( unsigned char NI, unsigned char x, unsigned char y );
void CmdChooseSelType( unsigned char NI, unsigned short ID );
void CmdChooseUnSelType( unsigned char NI, unsigned short ID );
void CmdGoToMine( unsigned char NI, unsigned short ID );
void CmdLeaveMine( unsigned char NI, unsigned short Type );
void CmdErseBrigs( unsigned char NI );
void CmdChooseSelBrig( unsigned char NI, unsigned short ID );
void CmdChooseUnSelBrig( unsigned char NI, unsigned short ID );
void CmdMakeStandGround( unsigned char NI );
void CmdCancelStandGround( unsigned char NI );
void CmdCrBig( unsigned char NI, int i );
void CmdSelBrig( unsigned char NI, unsigned char Type, unsigned short ID );

extern Nation NATIONS[8];

void InitEBuf();
void ExecuteBuffer();
extern char Prompt[80];
extern int PromptTime;
void CreateWaterMap();
extern __declspec( dllexport ) int SCRSizeX;
extern __declspec( dllexport ) int SCRSizeY;
extern __declspec( dllexport ) int RSCRSizeX;
extern __declspec( dllexport ) int RSCRSizeY;
extern __declspec( dllexport ) int COPYSizeX;
void CmdGetOil( unsigned char NI, unsigned short UI );

extern unsigned char NLocks[64][64];
void SetLock( int x, int y, char val );

inline void IncLock( unsigned char x, unsigned char y )
{
	NLocks[y >> 2][x >> 2]++;
	SetLock( x, y, 1 );
}

inline void DecLock( unsigned char x, unsigned char y )
{
	NLocks[y >> 2][x >> 2]--;
	SetLock( x, y, 1 );
}

extern bool FASTMODE;
extern unsigned short MAXOBJECT;

void SetupHint();
void AssignHint( char* s, int time );
void GetChar( GeneralObject* GO, char* s );

extern OneObject OBJECTS[ULIMIT];

extern short TSin[257];
extern short TCos[257];
extern short TAtg[257];

void SetFlyMarkers();
void ClearFlyMarkers();

typedef void UniqMethood( int x, int y );

void HandleSW();
void CreateStrangeObject( int i, unsigned char NI, int x, int y, unsigned short ID );
void ShowRLCItemMutno( int x, int y, lpRLCTable lprt, int n );
void ShowRLCItemFired( int x, int y, lpRLCTable lprt, int n );
bool CheckAttAbility( OneObject* OB, unsigned short Patient );
void PrepareToEdit();
void PrepareToGame();
extern int MaxSizeX;
extern int MaxSizeY;
extern bool MiniMode;
void SetMiniMode();
void ClearMiniMode();
extern int Shifter;
extern int Multip;
void InitAllGame();

//x,y-coordinates of point on the 2D plane (unit:pix)
//returnfs index of building,otherwise 0xFFFF
unsigned short DetermineBuilding( int x, int y, unsigned char NMask );
bool Create3DAnmObject( Weapon* Weap, int xs, int ys, int zs1,
	int xd, int yd, int zd,
	OneObject* OB, unsigned char AttType, unsigned short DestObj );
bool Create3DAnmObject( Weapon* Weap, int xs, int ys, int zs1,
	int xd, int yd, int zd,
	OneObject* OB, unsigned char AttType, unsigned short DestObj, char dz );
int div24( int y );
int Prop43( int y );

int GETV( char* Name );
char* GETS( char* Name );

void LoadAllNations( unsigned char NIndex );

int GetExMedia( char* Name );
extern NewAnimation** FiresAnm[2];
extern NewAnimation** PreFires[2];
extern NewAnimation** PostFires[2];
extern int            NFiresAnm[2];
typedef NewAnimation* lpNewAnimation;
extern int UnitsPerFarm;
extern int ResPerUnit;
extern int EatenRes;

#include "UnSyncro.h"

extern unsigned short* fmap;
extern unsigned char* MCount;
extern unsigned short* BLDList;
extern unsigned char* NPresence;

//------------sorting by nations-------------
extern unsigned short* NatList[8];
extern int   NtNUnits[8];
extern int   NtMaxUnits[8];
void SetupNatList();
void InitNatList();
void AddObject( OneObject* OB );
void DelObject( OneObject* OB );
void PlayAnimation( NewAnimation* NA, int Frame, int x, int y );
void MakeOrderSound( OneObject* OB, unsigned char SMask );

extern int GoldID;
extern int FoodID;
extern int StoneID;
extern int TreeID;
extern int CoalID;
extern int IronID;
void UpdateAttackR( AdvCharacter* ADC );

//------------------IDS-----------------//
#define MelnicaID	0x01
#define MelnicaIDS  "MELNICA"

#define FarmID		0x02
#define FarmIDS		"FARM"

#define CenterID	0x03
#define CenterIDS	"CENTER"

#define SkladID		0x04
#define SkladIDS	"SKLAD"

#define TowerID		0x05
#define TowerIDS	"TOWER"

#define FieldID		0x06
#define FieldIDS	"FIELD"

#define MineID		0x07
#define MineIDS		"MINE"

#define FastHorseID	0x08
#define FastHorseIDS "FASTHORSE"

#define MortiraID	0x09
#define MortiraIDS  "MORTIRA"

#define PushkaID	0x0A
#define PushkaIDS   "PUSHKA"

#define GrenaderID  0x0B
#define GrenaderIDS "GRENADER"

#define HardWallID  0x0C
#define HardWallIDS "HARDWALL"

#define WeakWallID  0x0D
#define WeakWallIDS "WEAKWALL"

#define LinkorID	0x0E
#define LinkorIDS	"LINKOR"

#define WeakID		0x0F
#define WeakIDS	    "WEAK"

#define FisherID	0x10
#define FisherIDS	"FISHER"

#define ArtDepoID	0x11
#define ArtDepoIDS  "ARTDEPO"

#define SupMortID	0x12
#define SupMortIDS	"SUPERMORTIRA"

#define PortID		0x13
#define PortIDS	    "PORT"

#define LightInfID	0x14
#define LightInfIDS	"LIGHTINFANTRY"

#define StrelokID	0x15
#define StrelokIDS	"STRELOK"

#define HardHorceID	0x16
#define HardHorceIDS "HARDHORCE"

#define PeasantID	0x17
#define PeasantIDS	"PEASANT"

#define HorseStrelokID	0x18
#define HorseStrelokIDS "HORSE-STRELOK"

#define FregatID	0x19
#define FregatIDS   "FREGAT"

#define GaleraID	0x1B
#define GaleraIDS   "GALERA"

#define IaxtaID	    0x1C
#define IaxtaIDS    "IAXTA"

#define ShebekaID	 0x1E
#define ShebekaIDS   "SHEBEKA"

#define ParomID      0x1F
#define ParomIDS     "PAROM"

#define ArcherID    0x20
#define ArcherIDS   "ARCHER"

#define MultiCannonID 0x1E
#define MultiCannonIDS "MCANNON"

extern OrderClassDescription OrderDesc[16];
extern int NOClasses;
extern OrderDescription ElementaryOrders[128];
extern int NEOrders;

int OScale( int x );
extern short LastDirection;
#define MobilR 1024
void SetTrianglesState( int xc, int yc, short* xi, short* yi, int NP, bool State );

extern unsigned long LOADNATMASK;
extern char NatCharLo[32][8];
extern char NatCharHi[32][8];
void RunPF( int i );
void StopPF( int i );

extern bool GoAndAttackMode;
extern int FrmDec;
extern int SpeedSh;
extern int REALTIME;

void CreateTimedHint( char* s, int time );
void CreateTimedHintEx( char* s, int time, unsigned char opt );

//-----------------New text files------------------//
extern int LX_fmap;
unsigned short GetV_fmap( int x, int y );
extern int VAL_SHFCX;
extern int VAL_MAXCX;
extern int VAL_MAXCIOFS;
extern short randoma[8192];
extern unsigned short TexFlags[256];
int AddTHMap( int );

#define SECTMAP(i) (SectMap?SectMap[i]:(unsigned short(randoma[unsigned short(i&8191)])%3))

unsigned short GetNMSL( int i );
void SetNMSL( int i, unsigned short W );
void CleanNMSL();
extern int LastActionX;
extern int LastActionY;
extern unsigned char NatRefTBL[8];

#define GM(x) (1<<x)
#define INITBEST 0x0FFFFFFF

/* FTW?
#define DeleteLastOrder() DeleteLastOrder();
#define CreatePath(x1,y1) CreatePath(x1,y1);
#define NewMonsterSendTo(x,y,Prio,OrdType) NewMonsterSendTo(x,y,Prio,OrdType);
#define NewMonsterPreciseSendTo(x,y,Prio,OrdType) NewMonsterPreciseSendTo(x,y,Prio,OrdType);
#define NewMonsterSmartSendTo(x,y,dx,dy,Prio,OrdType) NewMonsterSmartSendTo(x,y,dx,dy,Prio,OrdType);
*/

extern unsigned short* TopRef;
extern int TopLx;

__forceinline unsigned short SafeTopRef( int x, int y )
{
	if ( x >= 0 && y >= 0 && x < TopLx&&y < TopLx )
	{
		return TopRef[x + ( y << TopSH )];
	}
	else
	{
		return 0xFFFF;
	}
}

__forceinline void SafeSetTopRef( int x, int y, unsigned short Val )
{
	if ( x >= 0 && y >= 0 && x < TopLx&&y < TopLx )
	{
		TopRef[x + ( y << TopSH )] = Val;
	}
};
