#ifndef NODPLAY
#include "Dplay.h"
#endif

#define MaxPL 8

struct EXBUFFER
{
	DWORD Size;
	bool Enabled;
	DWORD Sign;//0xF376425E
	DWORD RealTime;//if(??==0xFFFFFFFF)-empty buffer
	DWORD RandIndex;
	byte  Data[4096];
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
	DWORD DPID;
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
	void AddPing( DWORD DPID, DWORD From, DWORD To, DWORD Back );
	void AddPlayer( DWORD DPID );
	int GetTimeDifference( DWORD DPID );
	int CheckPlayer( DWORD DPID );
};
extern PingSumm PSUMM;

struct BACKUPSTR
{
	DWORD  ID;
	DWORD RealTime;
	byte* Data;
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
	void AddInf( byte* BUF, int L, DWORD ID, int RT );
	void SendInfoAboutTo( DWORD ID, DWORD TO, DWORD RT );
};

struct SingleRetr
{
	DWORD IDTO;
	DWORD IDFROM;
	DWORD RT;
};

class RETRANS
{
public:
	SingleRetr* TOT;
	int NRET;
	int MaxRET;
	RETRANS();
	~RETRANS();
	void AddOneRet( DWORD TO, DWORD From, DWORD RT );
	void AddSection( DWORD TO, DWORD From, DWORD RT );
	void CheckRetr( DWORD From, DWORD RT );
	void Clear();
};

extern PLAYERSBACKUP PBACK;
extern RETRANS RETSYS;

struct RoomInfo
{
	char Name[128];
	char Nick[64];
	char RoomIP[32];
	DWORD Profile;
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
	DWORD Profile;
	byte State;
	word Score;
	word Population;
	DWORD ReachRes[6];
	word NBornP;
	word NBornUnits;
};

// IChat library exports these
__declspec( dllimport ) void ReportGSCGame( int time, int NPlayers, OnePlayerReport* OPR );
__declspec( dllimport ) void ReportAliveState( int NPlayers, int* Profiles );

void SETPLAYERNAME(char* name, bool);

#ifndef NODPLAY
bool MLP_CreateCompoundAddress(int selected_network_protocol, byte* AddrBuf);
bool MLP_CreateBattleCompoundAddress(int selected_network_protocol, byte* AddrBuf);
#endif

#ifndef NODPLAY
BOOL FAR PASCAL EnumAddressCallback1(
	REFGUID guidDataType,
	DWORD dwDataSize,
	LPCVOID lpData,
	LPVOID lpContext
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
	DWORD   dwSize;             // Size of structure
	DWORD   dwFlags;            // Not used. Must be zero.
	union
	{                           // The short or friendly name
		LPWSTR  lpszShortName;  // Unicode
		LPSTR   lpszShortNameA; // ANSI
	};
	union
	{                           // The long or formal name
		LPWSTR  lpszLongName;   // Unicode
		LPSTR   lpszLongNameA;  // ANSI
	};

} CDPNAME;

typedef struct
{
	DWORD       dwType;         // Message type
	DWORD       dwPlayerType;   // Is it a player or group
	CDPID        dpId;           // ID of the player or group
	DWORD       dwCurrentPlayers;   // current # players & groups in session
	LPVOID      lpData;         // pointer to remote data
	DWORD       dwDataSize;     // size of remote data
	CDPNAME      dpnName;        // structure with name info
	// the following fields are only available when using
	// the IDirectPlay3 interface or greater
	CDPID	    dpIdParent;     // id of parent group
	DWORD		dwFlags;		// player or group flags
} * CLPDPMSG_CREATEPLAYERORGROUP;

typedef struct
{
	DWORD       dwType;         // Message type
	DWORD       dwPlayerType;   // Is it a player or group
	CDPID        dpId;           // player ID being deleted
	LPVOID      lpLocalData;    // copy of players local data
	DWORD       dwLocalDataSize; // sizeof local data
	LPVOID      lpRemoteData;   // copy of players remote data
	DWORD       dwRemoteDataSize; // sizeof remote data
	// the following fields are only available when using
	// the IDirectPlay3 interface or greater
	CDPNAME      dpnName;        // structure with name info
	CDPID	    dpIdParent;     // id of parent group	
	DWORD		dwFlags;		// player or group flags
} * CLPDPMSG_DESTROYPLAYERORGROUP;
#endif

extern CDPID MyDPID;
extern PingSumm PSUMM;
extern EXBUFFER EBufs[MaxPL];
extern EXBUFFER EBufs1[MaxPL];
