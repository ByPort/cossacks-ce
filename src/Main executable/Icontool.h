class OneIcon{
public:
	unsigned short FileID;
	unsigned short SpriteID;
	//Type:
	//0  -simple icon
	//1  -icon with colored rectangle
	//2  -icon with Health bar
	//4  -icon with stage bar
	//8  -icon with integer value
	//16 -icon with hint
	//32 -right button active
	//64 -left  button active
    //128-memory for hint was allocated
	unsigned char  Color;
	int   Health;
	int   MaxHealth;
	int   Stage;
	int   MaxStage;
	int   IntVal;
	unsigned char  Level;
	bool  Disabled:1;
	bool  Selected:1;
	unsigned short MoreSprite;
    unsigned short Type;
	bool Visible:1;
	bool NoPress:1;
	char* Hint;
	char* HintLo;
    HandlePro* LPro;
    HandlePro* RPro;
	HandlePro* MoveOver;
    int   LParam;
    int   RParam;
	int   MoveParam;
	int   IntParam;

    void InitIcon();
    void AssignIcon(unsigned short FileID,unsigned short SpriteID);
    void AssignHint(char* str);
    void CreateHint(char* str);
	void CreateHintLo(char* str);
    void AssignColor(unsigned char c);
    void AssignHealth(int Health,int MaxHeath);
    void AssignStage(int Stage,int MaxStage);
    void AssignIntVal(int ival);
    void AssignLeft(HandlePro* Lpro,int param);
    void AssignRight(HandlePro* Hpro,int param);
	void AssignMoveOver(HandlePro* Hpro,int param);
	void AssignLevel(unsigned char Level);
	void AssignIntParam(int i);
	void AssignRedPulse();
	void SelectIcon();
	void Disable();
    void Draw(int x,int y);
};
class IconSet{
public:
    int Space;
    OneIcon* Icons;
    int NIcons;
    IconSet();
    ~IconSet();
    void InitIconSet();
    void ClearIconSet();
	OneIcon* AddIconFixed(unsigned short FileID,unsigned short SpriteID,int Index);
    OneIcon* AddIcon(unsigned short FileID,unsigned short SpriteID);
    OneIcon* AddIcon(unsigned short FileID,unsigned short SpriteID,char* Hint);
    OneIcon* AddIcon(unsigned short FileID,unsigned short SpriteID,char* Hint,unsigned char Color);
    OneIcon* AddIcon(unsigned short FileID,unsigned short SpriteID,unsigned char Color,char* Hint);
	int GetMaxX(int Lx);
    void DrawIconSet(int x,int y,int Nx,int Ny,int NyStart);
};