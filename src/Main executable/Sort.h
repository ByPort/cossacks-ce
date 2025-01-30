class SortClass{
public:
    unsigned short* Uids;
    //unsigned short* Usn;
    int*  Parms;
    int NUids;
    int MaxUID;
    SortClass();
    ~SortClass();
    void Sort();
    void CheckSize(int Size); 
    unsigned short CreateByX(unsigned short* UIDS,int NUIDS,bool Direction);
    unsigned short CreateByY(unsigned short* UIDS,int NUIDS,bool Direction);
    unsigned short CreateByR(unsigned short* UIDS,int NUIDS,bool Direction,int x,int y);
	unsigned short CreateByLine(unsigned short* UIDS,int NUIDS,int dx,int dy);
	unsigned short CreateByLineForShips(unsigned short* UIDS,int NUIDS,int dx,int dy);
    void CopyToDst(unsigned short* Dst,unsigned short* Sns);
    void Copy(unsigned short* Dst);
};
extern SortClass UNISORT;
unsigned short SortUnitsByR(unsigned short* urefs,unsigned short* usn,int nu,int x,int y);
class PositionOrder{
public:
    int NUnits;
    int MaxUnit;
	int CenterX;
	int CenterY;
    unsigned short* Ids;
    int* px;
    int* py;
    PositionOrder();
    ~PositionOrder();
    void CheckSize(int Size);
    void Create(unsigned short* IDS,int NIDS);
    unsigned short CreatePositions(int x,int y,unsigned short* IDS,int NIDS);
	unsigned short CreateLinearPositions(int x,int y,unsigned short* IDS,int NIDS,int dx,int dy);
	unsigned short CreateRotatedPositions(int x,int y,unsigned short* IDS,int NIDS,int dx,int dy);
	unsigned short CreateRotatedPositions2(int x,int y,unsigned short* IDS,int NIDS,int dx,int dy);
	unsigned short CreateOrdPos(int x,int y,unsigned char dir,int NIDS,unsigned short* IDS,OrderDescription* ODS);
	unsigned short CreateSimpleOrdPos(int x,int y,unsigned char dir,int NIDS,unsigned short* IDS,OrderDescription* ODS);
    void SendToPosition(unsigned char Prio,unsigned char OrdType);
	void Patrol();
};
void GroupSendSelectedTo(unsigned char NI,int x,int y,unsigned char Prio,unsigned char OrdType);
extern PositionOrder PORD;