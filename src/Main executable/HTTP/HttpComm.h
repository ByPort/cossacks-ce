// HttpComm.h: interface for the CHttpComm class.
//
//////////////////////////////////////////////////////////////////////

enum ConnectionStage{csConnecting,csSending,csReceiving,csWaiting,csError};

#define RECV_BUFFER_SIZE	16*1024

typedef struct SHttpQuery
{
	unsigned long				m_dwHandle;			// handle
	SOCKET				m_Socket;			// socket
	LPSTR				m_lpszQuery;		// query
	void*				m_lpvBuffer;		// receive buffer
	unsigned long				m_dwDataSize;		// received data size
	ConnectionStage		m_Stage;			// connection stage
}*PHttpQuery;

class CHttpComm  
{
public:
	VOID		ProcessRequests();
	unsigned long		AddRequest(const char* lpcszURL);
	int			GetData(unsigned long dwHandle, void* lpvBuffer, unsigned long dwBufferSize);
	VOID		FreeData(unsigned long dwHandle);
	CHttpComm();
	virtual ~CHttpComm();
protected:
	BOOL		m_bInitialized;
	CHAR		m_szProxyAddr[255];
	unsigned long		m_bUseProxy;
	unsigned long		m_dwProxyPort;
	unsigned long		m_dwRequestCount;
	unsigned long		m_dwHandleAuto;
	PHttpQuery	m_pRequestList;
};


