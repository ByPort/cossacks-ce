#pragma once
#include <iterator>
#include <vector>
#include <string>
#include <cstring>
#include <Winsock2.h>

/*
	Helper class for setting up and keeping alive a dummy udp connection to server.
	Uses CCommCore UDP socket to send packets.
	Useful for the player who wants to host an internet game but is behind a NAT.
*/
class UdpHolePuncher
{
public:
	void Init( const char *server_addr , const unsigned short port, 
		const uint64_t interval , const long player_id, const char *access_key );
	void KeepAlive();
private:
	bool ready_for_punching_;
	std::vector<char> packet_;
	int packet_size_;
	uint64_t last_send_time_;
	uint64_t interval_;
	sockaddr_in server_addr_;
};

// IntExplorer library exports this
__declspec( dllimport ) char* GetAccessKey( int Index );
