#pragma once
#ifndef _EE_SOCKET_H_
#define _EE_SOCKET_H_

#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>
#include <string>

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EESocket
	//----------------------------------------------------------------------------------------------------
	class EESocket
	{
	protected:
		static bool InitializeSocket();

	protected:
		static bool s_isSocketInitialized;

	public:
		EESocket(char* _addr, char* _port);
		EESocket(char* _addr, u_short _port);
		EESocket(SOCKET _socket, const sockaddr_storage& _addr);
		EESocket(SOCKET _socket, const sockaddr_in& _addr);
		EESocket(const EESocket& _socket);
		virtual ~EESocket();
		
		SOCKET Socket();
		std::string IP();
		u_short Port();

	protected:
		SOCKET m_socket;
		sockaddr_storage m_addr;
		int m_addrLen;
	};

	// EESocket_APIs
	//----------------------------------------------------------------------------------------------------
	std::string EEGetIP(sockaddr_storage& _addr);
	u_short EEGetPort(sockaddr_storage& _addr);
}

#endif