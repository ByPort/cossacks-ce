#pragma once
struct GAMEOBJ;

typedef bool tpRegisterUnitType(GAMEOBJ* GOBJ, const char* Name);
tpRegisterUnitType* RegisterUnitType = nullptr;

typedef bool tpRegisterUpgrade(GAMEOBJ* GOBJ, const char* Name);
tpRegisterUpgrade* RegisterUpgrade = nullptr;

typedef void tpAI_Torg(unsigned char SellRes, unsigned char BuyRes, int SellAmount);
tpAI_Torg* AI_Torg = nullptr;

typedef bool tpTryUnit(GAMEOBJ* UnitType, int Max, unsigned char CostPercent, unsigned char Probability);
tpTryUnit* TryUnit = nullptr;

typedef bool tpTryUpgrade(GAMEOBJ* Upgrade, int CostPercent, int Probability);
tpTryUpgrade* TryUpgrade = nullptr;

typedef void tpSetMineBalanse(int N, unsigned short* Bal);
tpSetMineBalanse* SetMineBalanse = nullptr;

typedef void tpSetPDistribution(int OnFood, int OnWood, int OnStone);
tpSetPDistribution* SetPDistribution = nullptr;

typedef void tpAssignAmountOfMineUpgrades(int MU);
tpAssignAmountOfMineUpgrades* AssignAmountOfMineUpgrades = nullptr;

typedef bool tpAssignMineUpgrade(unsigned short U, const char* Str, unsigned val);
tpAssignMineUpgrade* AssignMineUpgrade = nullptr;

typedef void tpAssignMine(const char* Name);
tpAssignMine* AssignMine = nullptr;

typedef void tpAssignPeasant(const char* Name);
tpAssignPeasant* AssignPeasant = nullptr;

typedef void tpAssignHouse(const char* Name);
tpAssignHouse* AssignHouse = nullptr;

typedef void tpSET_MINE_CAPTURE_RADIUS(int x);
tpSET_MINE_CAPTURE_RADIUS* SET_MINE_CAPTURE_RADIUS = nullptr;

typedef void tpSET_MINE_UPGRADE1_RADIUS(int x);
tpSET_MINE_UPGRADE1_RADIUS* SET_MINE_UPGRADE1_RADIUS = nullptr;

typedef void tpSET_MINE_UPGRADE2_RADIUS(int x);
tpSET_MINE_UPGRADE2_RADIUS* SET_MINE_UPGRADE2_RADIUS = nullptr;

typedef void tpSET_DEFAULT_MAX_WORKERS(int x);
tpSET_DEFAULT_MAX_WORKERS* SET_DEFAULT_MAX_WORKERS = nullptr;

typedef void tpSET_MIN_PEASANT_BRIGADE(int x);
tpSET_MIN_PEASANT_BRIGADE* SET_MIN_PEASANT_BRIGADE = nullptr;

typedef int tpGetMoney(unsigned char id);
tpGetMoney* GetMoney = nullptr;

typedef int tpGetUnits(GAMEOBJ* UnitType);
tpGetUnits* GetUnits = nullptr;

typedef int tpGetReadyUnits(GAMEOBJ* UnitType);
tpGetReadyUnits* GetReadyUnits = nullptr;

typedef bool tpUpgIsDone(GAMEOBJ* Upgrade);
tpUpgIsDone* UpgIsDone = nullptr;

typedef bool tpUpgIsRun(GAMEOBJ* Upgrade);
tpUpgIsRun* UpgIsRun = nullptr;

typedef bool tpFieldExist();
tpFieldExist* FieldExist = nullptr;

typedef int tpGetDifficulty();
tpGetDifficulty* GetDifficulty = nullptr;

typedef int tpGetStartRes();
tpGetStartRes* GetStartRes = nullptr;

typedef int tpGetLandType();
tpGetLandType* GetLandType = nullptr;

typedef void tpAssignFormUnit(const char* Name);
tpAssignFormUnit* AssignFormUnit = nullptr;

typedef void tpSetAIRegister(int Reg, int Val);
tpSetAIRegister* SetAIRegister = nullptr;

typedef int tpGetAIRegister(int Reg);
tpGetAIRegister* GetAIRegister = nullptr;
