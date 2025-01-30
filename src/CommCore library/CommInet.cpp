// Network initialization part
//
#include "CommCore.h"

// ---------------------------------------------------------------------------------------------

int CCommCore::InitNetwork()
{
	_log_message("InitNetwork()");

	WSADATA WSAData;

	if(WSAStartup(MAKEWORD(2,2),&WSAData)!=0)
		return 0;

	if(!InitSocket())
		return 0;

	if(!InitHost())
		return 0;

	return 1;
}

// ---------------------------------------------------------------------------------------------

int CCommCore::InitHost()
{
	_log_message("InitHost()");

	if(gethostname(m_szUserName,255)==SOCKET_ERROR)
		return 0;

	HOSTENT * pHostEnt;

	pHostEnt=gethostbyname(m_szUserName);

	if(!pHostEnt)
		return 0;

	m_uAddrCount=0;

	while(pHostEnt->h_addr_list[m_uAddrCount]){
		memcpy(&m_dwAddrList[m_uAddrCount],pHostEnt->h_addr_list[m_uAddrCount],pHostEnt->h_length);
		m_uAddrCount++;
	};

	return 1;
}

// ---------------------------------------------------------------------------------------------

int CCommCore::CloseNetwork()
{
	_log_message("CloseNetwork()");

	if(!CloseSocket())
		return 0;

	if(WSACleanup()==SOCKET_ERROR)
		return 0;

	return 1;
}

// ---------------------------------------------------------------------------------------------

int CCommCore::InitSocket()
{
	_log_message("InitSocket()");

	m_DataSocket=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(m_DataSocket==INVALID_SOCKET)
		return 0;

	unsigned long	lArgP=0x01;
	if(ioctlsocket(m_DataSocket,FIONBIO,&lArgP)==SOCKET_ERROR)
		return 0;

	unsigned long	lMaxSize;
	int		iSizeOfMaxSize=sizeof(unsigned long);
	if(getsockopt(m_DataSocket,SOL_SOCKET,SO_MAX_MSG_SIZE,(char *)&lMaxSize,&iSizeOfMaxSize)==SOCKET_ERROR)
		return 0;

	m_uMaxMsgSize=(unsigned short)lMaxSize;

	sockaddr_in locaddr;

	locaddr.sin_family=AF_INET;
	locaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	locaddr.sin_port=htons(DATA_PORT);

	if(bind(m_DataSocket,(sockaddr *)&locaddr,sizeof(sockaddr_in))==SOCKET_ERROR)
		return 0;

	return 1;
}

// ---------------------------------------------------------------------------------------------

int CCommCore::CloseSocket()
{
	_log_message("CloseSocket()");

	if(closesocket(m_DataSocket)==SOCKET_ERROR)
		return 0;

	return 1;
}

// ---------------------------------------------------------------------------------------------
