// Peers manipulation routines
//
#include "CommCore.h"

// ---------------------------------------------------------------------------------------------
unsigned short CCommCore::GetPeerByCCUID(const char* lpcszCCUID)
{
	//	_log_message("GetPeerByCCUID()");

	for (unsigned short i = 0; i < m_uPeerCount; i++)
	{
		if (memcmp(m_PeerList[i].m_szCCUID, lpcszCCUID, 22) == 0)
		{
			return i;
		}
	}

	return BAD_PEER_ID;
}

// ---------------------------------------------------------------------------------------------

unsigned short CCommCore::GetPeerById(PEER_ID PeerId)
{
	//	_log_message("GetPeerById()");

	for (unsigned short i = 0; i < m_uPeerCount; i++)
	{
		if (m_PeerList[i].m_Id == PeerId)
		{
			return i;
		}
	}

	return BAD_PEER_ID;
}

// ---------------------------------------------------------------------------------------------

int CCommCore::DropPeer(unsigned short uPeer)
{
	_log_message("DropPeer()");

	memmove(&m_PeerList[uPeer], &m_PeerList[uPeer + 1], (m_uPeerCount - uPeer - 1) * sizeof(PEER_ENTRY));

	m_uPeerCount--;

	if (m_bServer)
		SendServerList();

	return 1;
}

// ---------------------------------------------------------------------------------------------

int CCommCore::DeletePeer(PEER_ID piNumber)
{
	_log_message("DeletePeer()");

	unsigned short
		uPeer = GetPeerById(piNumber);

	if (uPeer == BAD_PEER_ID)
		return 0;

	return DropPeer(uPeer);
}

// ---------------------------------------------------------------------------------------------

int CCommCore::EnumPeers()
{
	//	_log_message("EnumPeers()");

	if (lpEnumProc)
	{
		for (int i = 0; i < m_uPeerCount; i++)
		{
			if (m_PeerList[i].m_Id == m_piNumber)
			{
				lpEnumProc( m_piNumber, m_szUserName );
			}
			else
			{
				lpEnumProc( m_PeerList[i].m_Id, m_PeerList[i].m_szUserName );
			}
		}
	}

	return 1;
}

// ---------------------------------------------------------------------------------------------
