#define MaxPL 8

struct EXBUFFER
{
	unsigned long Size;
	bool Enabled;
	unsigned long Sign;//0xF376425E
	unsigned long RealTime;//if(??==0xFFFFFFFF)-empty buffer
	unsigned long RandIndex;
	unsigned char  Data[4096];
};

struct OnePing
{
	int FromTime;
	int ToTime;
	int BackTime;
};

class PingsSet
{
public:
	unsigned long DPID;
	int NPings;
	int MaxPings;
	OnePing* Pings;
};

class PingSumm
{
public:
	int NPL;
	PingsSet* PSET;
	PingSumm();
	~PingSumm();
	void ClearPingInfo();
	void AddPing( unsigned long DPID, unsigned long From, unsigned long To, unsigned long Back );
	void AddPlayer( unsigned long DPID );
	int GetTimeDifference( unsigned long DPID );
	int CheckPlayer( unsigned long DPID );
};
extern PingSumm PSUMM;

struct BACKUPSTR
{
	unsigned long  ID;
	unsigned long RealTime;
	unsigned char* Data;
	int   L;
};

class PLAYERSBACKUP
{
public:
	BACKUPSTR BSTR[32];
	int NBDATA;
	PLAYERSBACKUP();
	~PLAYERSBACKUP();
	void Clear();
	void AddInf( unsigned char* BUF, int L, unsigned long ID, int RT );
	void SendInfoAboutTo( unsigned long ID, unsigned long TO, unsigned long RT );
};

struct SingleRetr
{
	unsigned long IDTO;
	unsigned long IDFROM;
	unsigned long RT;
};

class RETRANS
{
public:
	SingleRetr* TOT;
	int NRET;
	int MaxRET;
	RETRANS();
	~RETRANS();
	void AddOneRet( unsigned long TO, unsigned long From, unsigned long RT );
	void AddSection( unsigned long TO, unsigned long From, unsigned long RT );
	void CheckRetr( unsigned long From, unsigned long RT );
	void Clear();
};

extern PLAYERSBACKUP PBACK;
extern RETRANS RETSYS;

struct RoomInfo
{
	char Name[128];
	char Nick[64];
	char RoomIP[32];
	unsigned long Profile;
	char GameID[64];
	int MaxPlayers;

	//Additional members to pass data from server to main exe / CommCore
	long player_id; //Necessary for host to send udp hole punching packets
	unsigned short port; //Udp hole punching port or real port of game host
	unsigned udp_interval; //Udp hole punching packet interval
	char udp_server[16]; //IP of udp hole punching server
};
extern RoomInfo GlobalRIF;

extern bool use_gsc_network_protocol;

// IChat library exports these
__declspec( dllimport ) int Process_GSC_ChatWindow( bool Active, RoomInfo* RIF );
__declspec( dllimport ) void LeaveGSCRoom();
__declspec( dllimport ) void StartGSCGame( char* Options, char* Map,
	int NPlayers, int* Profiles, char** Nations, int* Teams, int* Colors );

struct OnePlayerReport
{
	unsigned long Profile;
	unsigned char State;
	unsigned short Score;
	unsigned short Population;
	unsigned long ReachRes[6];
	unsigned short NBornP;
	unsigned short NBornUnits;
};

// IChat library exports these
__declspec( dllimport ) void ReportGSCGame( int time, int NPlayers, OnePlayerReport* OPR );
__declspec( dllimport ) void ReportAliveState( int NPlayers, int* Profiles );

void SETPLAYERNAME(char* name, bool);

#ifndef NODPLAY
bool MLP_CreateCompoundAddress(int selected_network_protocol, unsigned char* AddrBuf);
bool MLP_CreateBattleCompoundAddress(int selected_network_protocol, unsigned char* AddrBuf);
#endif

#ifndef NODPLAY
BOOL FAR PASCAL EnumAddressCallback1(
	REFGUID guidDataType,
	unsigned long dwDataSize,
	LPCVOID lpData,
	void* lpContext
);
#endif

// Custom DPID and two message types from dplay.h
// TODO: remove or make light structures
#ifndef NODPLAY
typedef DPID CDPID;

typedef LPDPMSG_CREATEPLAYERORGROUP CLPDPMSG_CREATEPLAYERORGROUP;
typedef LPDPMSG_DESTROYPLAYERORGROUP CLPDPMSG_DESTROYPLAYERORGROUP;

#else

typedef unsigned long CDPID;

typedef struct
{
	unsigned long   dwSize;             // Size of structure
	unsigned long   dwFlags;            // Not used. Must be zero.
	union
	{                           // The short or friendly name
		wchar_t*  lpszShortName;  // Unicode
		char*   lpszShortNameA; // ANSI
	};
	union
	{                           // The long or formal name
		wchar_t*  lpszLongName;   // Unicode
		char* lpszLongNameA;  // ANSI
	};

} CDPNAME;

typedef struct
{
	unsigned long       dwType;         // Message type
	unsigned long       dwPlayerType;   // Is it a player or group
	CDPID        dpId;           // ID of the player or group
	unsigned long       dwCurrentPlayers;   // current # players & groups in session
	void*      lpData;         // pointer to remote data
	unsigned long       dwDataSize;     // size of remote data
	CDPNAME      dpnName;        // structure with name info
	// the following fields are only available when using
	// the IDirectPlay3 interface or greater
	CDPID	    dpIdParent;     // id of parent group
	unsigned long		dwFlags;		// player or group flags
} * CLPDPMSG_CREATEPLAYERORGROUP;

typedef struct
{
	unsigned long       dwType;         // Message type
	unsigned long       dwPlayerType;   // Is it a player or group
	CDPID        dpId;           // player ID being deleted
	void*      lpLocalData;    // copy of players local data
	unsigned long       dwLocalDataSize; // sizeof local data
	void*      lpRemoteData;   // copy of players remote data
	unsigned long       dwRemoteDataSize; // sizeof remote data
	// the following fields are only available when using
	// the IDirectPlay3 interface or greater
	CDPNAME      dpnName;        // structure with name info
	CDPID	    dpIdParent;     // id of parent group	
	unsigned long		dwFlags;		// player or group flags
} * CLPDPMSG_DESTROYPLAYERORGROUP;
#endif

extern CDPID MyDPID;
extern PingSumm PSUMM;
extern EXBUFFER EBufs[MaxPL];
extern EXBUFFER EBufs1[MaxPL];
