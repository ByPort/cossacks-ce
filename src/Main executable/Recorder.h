class InternetStream{
public:
	bool LocalFile;
	int CurPosition;
	char StreamName[128];
	char Server[8];
	char HashName[128];
	bool Open(char* Name);
	void Close();
	int ReadSomething(unsigned char* Buf,int MaxLen,bool Scroll,bool Blocking);
	int Error();
};
class RecordGame{
public:
	InternetStream STREAM;
	unsigned char* Stream;
	int Size;
	int MaxSize;
	int Pos;
	int LastSentPos;
	char MapName[64];
	char RecName[64];
	void StartRecord(char* MapName);
	void CloseRecord();
	void AddRecord();
	void AddEmptyRecord();
	void AddByte(unsigned char v);
	void AddShort(short v);
	void AddBuf(unsigned char* Data,int Size);
	void ReadStream(char* Name);
	void TryToFlushNetworkStream(bool Final);
	bool Extract();
	void Save();
	void RunRecordedGame(char* Name);
	RecordGame();
	~RecordGame();
};
extern RecordGame RGAME;
extern bool RecordMode;