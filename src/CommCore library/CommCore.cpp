

#include "CommCore.h"
#include <stdlib.h>
#include <stdio.h>

// ---------------------------------------------------------------------------------------------

int CCommCore::SetSessionName( const char* lpcszSessionName )
{
	_log_message( "SetSessionName()" );

	strcpy( m_szSessionName, lpcszSessionName );
	return 1;
}

// ---------------------------------------------------------------------------------------------

const char* CCommCore::GetUserName( PEER_ID PeerId )
{
	_log_message( "GetUserName()" );

	if (PeerId == m_piNumber)
		return m_szUserName;

	unsigned short uPeerNum = GetPeerById( PeerId );

	if (uPeerNum == BAD_PEER_ID)
		return NULL;

	return m_PeerList[uPeerNum].m_szUserName;
}

// ---------------------------------------------------------------------------------------------

int CCommCore::SetUserName( const char* lpcszUserName )
{
	_log_message( "SetUserName()" );

	strcpy( m_szUserName, lpcszUserName );

	return 1;//SendUserName();
}

// ---------------------------------------------------------------------------------------------

int CCommCore::SendUserName()
{
	_log_message( "SendUserName()" );

	if (m_bServer)
	{

		strcpy( m_PeerList[0].m_szUserName, m_szUserName );

		//		return SendServerList();
		return SendNewName( m_piNumber );

	}
	else
	{

		CC_PK_SEND_USER_NAME	SendUserNamePacket;

		strcpy( SendUserNamePacket.m_szUserName, m_szUserName );

		return SendRawPacket( m_paServAddr,
			m_paServPort,
			CC_PT_SEND_USER_NAME,
			(unsigned char*) &SendUserNamePacket,
			sizeof( CC_PK_SEND_USER_NAME ),
			1,
			0 );
	};
}

// ---------------------------------------------------------------------------------------------

int CCommCore::GetUserData( PEER_ID PeerId, unsigned char* lpbUserData, unsigned short * puUserDataSize )
{
	_log_message( "GetUserData()" );

	if (PeerId == m_piNumber)
	{
		memcpy( lpbUserData, m_lpbUserData, m_uUserDataSize );
		if (puUserDataSize)
		{
			*puUserDataSize = m_uUserDataSize;
		}
		return 1;
	}

	unsigned short uPeerNum = GetPeerById( PeerId );

	if (uPeerNum == BAD_PEER_ID)
	{
		if (puUserDataSize)
		{
			*puUserDataSize = 0;
		}
		return 0;
	}

	memcpy( lpbUserData, m_PeerList[uPeerNum].m_lpbUserData, m_PeerList[uPeerNum].m_uUserDataSize );

	if (puUserDataSize)
	{
		*puUserDataSize = m_PeerList[uPeerNum].m_uUserDataSize;
	}

	return 1;
}


// ---------------------------------------------------------------------------------------------

int CCommCore::SetUserData( const unsigned char* lpcbUserData, unsigned short uUserDataSize )
{
	_log_message( "SetUserData()" );

	if (m_lpbUserData)
	{
		free( m_lpbUserData );
	}

	m_lpbUserData = (unsigned char*) malloc( uUserDataSize );
	assert( m_lpbUserData );

	m_uUserDataSize = uUserDataSize;

	memcpy( m_lpbUserData, lpcbUserData, uUserDataSize );

	return 1;//SendUserData();
}

// ---------------------------------------------------------------------------------------------

int CCommCore::SendUserData()
{
	_log_message( "SendUserData()" );

	if (!m_lpbUserData)
	{
		return 0;
	}

	if (m_bServer)
	{

		free( m_PeerList[0].m_lpbUserData );

		m_PeerList[0].m_uUserDataSize = m_uUserDataSize;
		m_PeerList[0].m_lpbUserData = (unsigned char*) malloc( m_uUserDataSize );
		assert( m_PeerList[0].m_lpbUserData );

		memcpy( m_PeerList[0].m_lpbUserData, m_lpbUserData, m_uUserDataSize );
		m_PeerList[0].m_uUserDataSize = m_uUserDataSize;

		//		return SendServerList();
		return SendNewData( m_piNumber );
	}
	else
	{

		LPCC_PK_SEND_USER_DATA	lpSendUserDataPacket;

		lpSendUserDataPacket = (LPCC_PK_SEND_USER_DATA) malloc( sizeof( CC_PK_SEND_USER_DATA ) + m_uUserDataSize );
		assert( lpSendUserDataPacket );

		lpSendUserDataPacket->m_uUserDataSize = m_uUserDataSize;

		memcpy( lpSendUserDataPacket->m_UserData, m_lpbUserData, m_uUserDataSize );

		int bRes = SendRawPacket( m_paServAddr,
			m_paServPort,
			CC_PT_SEND_USER_DATA,
			(unsigned char*) lpSendUserDataPacket,
			sizeof( CC_PK_SEND_USER_DATA ) + m_uUserDataSize,
			1,
			0 );
		free( lpSendUserDataPacket );

		return bRes;
	};
}

// ---------------------------------------------------------------------------------------------

int CCommCore::SendDropClient( PEER_ID PeerID )
{
	_log_message( "SendDropClient()" );

	CC_PK_HOST_DROP		HostDropPacket;

	unsigned short	uPeerNum;

	uPeerNum = GetPeerById( PeerID );

	if (BAD_PEER_ID == uPeerNum)
	{
		return 0;
	}

	HostDropPacket.m_dwReserved = 0xFF;

	return SendRawPacket( m_PeerList[uPeerNum].m_ex_Addr,
		m_PeerList[uPeerNum].m_ex_Port,
		CC_PT_HOST_DROP,
		(unsigned char*) &HostDropPacket,
		sizeof( CC_PK_HOST_DROP ),
		1,
		0 );
}

// ---------------------------------------------------------------------------------------------

int CCommCore::SendToAll( unsigned char* lpbBuffer, unsigned short u_Size, int bSecure )
{
	_log_message( "SendToAll()" );

	for (int i = 0; i < m_uPeerCount; i++)
		if (!SendToPeer( m_PeerList[i].m_Id, lpbBuffer, u_Size, bSecure ))
			return 0;

	return 1;
}

// ---------------------------------------------------------------------------------------------

int CCommCore::SendToPeer( PEER_ID piNumber, unsigned char* lpbBuffer, unsigned short uSize, int bSecure )
{
	_log_message( "SendToPeer()" );

	if (piNumber == m_piNumber)
		return 1;

	LPCC_PK_SEND_DATA	lpFrame;
	unsigned short				uFrameSize;

	uFrameSize = sizeof( CC_PK_SEND_DATA ) + uSize;

	lpFrame = (LPCC_PK_SEND_DATA) malloc( uFrameSize );

	lpFrame->m_uSize = uSize;
	memcpy( lpFrame->m_bData, lpbBuffer, uSize );

	unsigned short	uPeerNum = GetPeerById( piNumber );
	if (uPeerNum == BAD_PEER_ID)
		return 0;

	int bRes = SendRawPacket( ( uPeerNum == 0 ) ? m_paServAddr : m_PeerList[uPeerNum].m_ex_Addr,
		m_PeerList[uPeerNum].m_ex_Port,
		CC_PT_SEND_DATA,
		(unsigned char*) lpFrame,
		uFrameSize,
		bSecure,
		0 );

	free( lpFrame );

	return bRes;
}

// ---------------------------------------------------------------------------------------------

int CCommCore::SendConfirmDataPacket( sockaddr_in *lpSender, unsigned long lStamp )
{
	_log_message( "SendConfirmDataPacket()" );

	CC_PK_FRAME_CONFIRM		ConfPacket;

	ConfPacket.m_lConfirmStamp = lStamp;

	if (!SendRawPacket( lpSender->sin_addr,
		lpSender->sin_port,
		CC_PT_FRAME_CONFIRM,
		(unsigned char*) &ConfPacket,
		sizeof( CC_PK_FRAME_CONFIRM ),
		0,
		0 ))
		return 0;

	return 1;
}

// ---------------------------------------------------------------------------------------------

int CCommCore::SendConnectReject( sockaddr_in *lpSender, unsigned short uReason )
{
	_log_message( "SendConnectReject()" );

	CC_PK_CONNECT_REJECT	RejectPacket;

	RejectPacket.m_uReason = uReason;

	if (!SendRawPacket( lpSender->sin_addr,
		lpSender->sin_port,
		CC_PT_CONNECT_REJECT,
		(unsigned char*) &RejectPacket,
		sizeof( CC_PK_CONNECT_REJECT ),
		1,
		0 ))
		return 0;

	return 1;
}

// ---------------------------------------------------------------------------------------------

int CCommCore::SendConnectOk( sockaddr_in *lpSender, PEER_ID PeerId )
{
	_log_message( "SendConnectOk()" );

	CC_PK_CONNECT_OK	ConnectOkPacket;

	ConnectOkPacket.m_Id = PeerId;
	ConnectOkPacket.m_dwOptions = m_dwOptions;

	strcpy( ConnectOkPacket.m_szSessionName, m_szSessionName );

	if (!SendRawPacket( lpSender->sin_addr,
		lpSender->sin_port,
		CC_PT_CONNECT_OK,
		(unsigned char*) &ConnectOkPacket,
		sizeof( CC_PK_CONNECT_OK ),
		1,
		0 ))
		return 0;

	return 1;
}

// ---------------------------------------------------------------------------------------------

int CCommCore::SendDropOk( sockaddr_in *lpSender )
{
	_log_message( "SendDropOk()" );

	CC_PK_HOST_DROP_OK	HostDropOkPacket;

	HostDropOkPacket.m_dwReserved = 0x00;

	if (!SendRawPacket( lpSender->sin_addr,
		lpSender->sin_port,
		CC_PT_HOST_DROP_OK,
		(unsigned char*) &HostDropOkPacket,
		sizeof( CC_PK_HOST_DROP_OK ),
		1,
		0 ))
		return 0;

	return 1;
}

// ---------------------------------------------------------------------------------------------

void CCommCore::SetCommCoreUID( const char* lpcszCCUID )
{
	_log_message( "SetCommCoreUID()" );

	HKEY	hKey;

	RegCreateKeyEx( HKEY_CURRENT_USER,
		"Software\\GSC Game World",
		0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, NULL );

	RegSetValueEx( hKey, "CCUID", 0, REG_SZ, (unsigned char*) lpcszCCUID, strlen( lpcszCCUID ) + 1 );

	RegCloseKey( hKey );
}

// ---------------------------------------------------------------------------------------------

void CCommCore::NewCommCoreUID( char* lpszCCUID )
{
	_log_message( "NewCommCoreUID()" );

	char	szComputerName[255];
	char	szCCUID[255];
	unsigned long	dwSize = 64;
	unsigned long	dwTicks;
	int		iRand;

	GetComputerName( szComputerName, &dwSize );

	for (iRand = dwSize; iRand < 8; iRand++)
		szComputerName[iRand] = '0';

	szComputerName[iRand] = '\0';

	dwTicks = GetTickCount();

	srand( dwTicks );

	iRand = rand();

	sprintf( szCCUID, "%-8.8s-%8.8X-%4.4X", szComputerName, dwTicks, iRand );

	SetCommCoreUID( szCCUID );

	strcpy( lpszCCUID, szCCUID );
}

// ---------------------------------------------------------------------------------------------

void CCommCore::GetCommCoreUID( char* lpszCCUID )
{
	_log_message( "GetCommCoreUID()" );

	HKEY	hKey;
	char	szCCUID[64];
	unsigned long	dwSize = 64;

	if (RegOpenKeyEx( HKEY_CURRENT_USER,
		"Software\\GSC Game World",
		0,
		KEY_ALL_ACCESS,
		&hKey ) != ERROR_SUCCESS)
	{
		NewCommCoreUID( lpszCCUID );
		return;
	};

	if (RegQueryValueEx( hKey,
		"CCUID",
		0,
		NULL,
		(unsigned char*) szCCUID,
		&dwSize ) != ERROR_SUCCESS)
	{
		RegCloseKey( hKey );
		NewCommCoreUID( lpszCCUID );
		return;
	};

	RegCloseKey( hKey );

	strcpy( lpszCCUID, szCCUID );
}

// ---------------------------------------------------------------------------------------------

int CCommCore::SendNewName( PEER_ID PeerId )			// Отсылает информацию о имени	(сервер)
{
	_log_message( "SendNewName()" );

	CC_PK_SEND_NEW_NAME			SendNewNamePacket;
	unsigned short						uPeerNum;

	uPeerNum = GetPeerById( PeerId );

	SendNewNamePacket.m_PeerId = PeerId;
	strcpy( SendNewNamePacket.m_szUserName, m_PeerList[uPeerNum].m_szUserName );

	for (int i = 1; i < m_uPeerCount; i++)
		if (m_PeerList[i].m_Id != PeerId)
			SendRawPacket( m_PeerList[i].m_ex_Addr,
				m_PeerList[i].m_ex_Port,
				CC_PT_SEND_NEW_NAME,
				(unsigned char*) &SendNewNamePacket,
				sizeof( CC_PK_SEND_NEW_NAME ),
				1,
				0 );

	return 1;
}
// ---------------------------------------------------------------------------------------------

int CCommCore::SendNewData( PEER_ID PeerId )			// Отсылает информацию о дате	(сервер)
{
	_log_message( "SendNewData()" );

	LPCC_PK_SEND_NEW_DATA		pSendNewDataPacket = NULL;
	unsigned short						uPacketSize = 0;
	unsigned short						uPeerNum;

	uPeerNum = GetPeerById( PeerId );

	if (uPeerNum == BAD_PEER_ID)
		return 0;

	uPacketSize = sizeof( CC_PK_SEND_NEW_DATA ) + m_PeerList[uPeerNum].m_uUserDataSize;

	pSendNewDataPacket = (LPCC_PK_SEND_NEW_DATA) malloc( uPacketSize );

	pSendNewDataPacket->m_PeerId = PeerId;
	pSendNewDataPacket->m_uUserDataSize = m_PeerList[uPeerNum].m_uUserDataSize;
	memcpy( pSendNewDataPacket->m_UserData,
		m_PeerList[uPeerNum].m_lpbUserData,
		m_PeerList[uPeerNum].m_uUserDataSize );

	for (int i = 1; i < m_uPeerCount; i++)
		if (m_PeerList[i].m_Id != PeerId)
		{
			SendRawPacket( m_PeerList[i].m_ex_Addr,
				m_PeerList[i].m_ex_Port,
				CC_PT_SEND_NEW_DATA,
				(unsigned char*) pSendNewDataPacket,
				uPacketSize,
				1,
				0 );

		};

	free( pSendNewDataPacket );

	return 1;
}

// ---------------------------------------------------------------------------------------------

int CCommCore::IsOverNAT( PEER_ID PeerId )
{
	_log_message( "IsOverNAT()" );

	if (PeerId == 1)
		return 0;

	unsigned short uPeerNum;

	uPeerNum = GetPeerById( PeerId );

	if (uPeerNum == BAD_PEER_ID)
		return 0;

	return m_PeerList[uPeerNum].m_bOverNAT;
}

// ---------------------------------------------------------------------------------------------
void CCommCore::GetServerAddress( char* lpszServerAddress )
{
	if (lpszServerAddress)
	{
		strcpy( lpszServerAddress, inet_ntoa( m_paServAddr ) );
	}
}
// ---------------------------------------------------------------------------------------------

int CCommCore::SendUdpHolePunch( sockaddr *server, char *content, const int content_len )
{
	const int res = sendto( m_DataSocket, content, content_len, 0, server, sizeof( sockaddr_in ) );

	if (SOCKET_ERROR == res)
	{
		return 0;
	}

	return 1;
}
