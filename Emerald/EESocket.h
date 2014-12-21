#pragma once
#ifndef _EE_SOCKET_H_
#define _EE_SOCKET_H_

#pragma comment(lib, "ws2_32.lib")
#include <winsock.h>

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EESocket
	//----------------------------------------------------------------------------------------------------
	class EESocket
	{
	protected:
		static bool InitializeSocket();

	protected:
		static bool s_isSocketInitialized;

	public:
		EESocket(char* _addr, u_short _port);
		EESocket(SOCKET _socket, const sockaddr_in& _port);
		EESocket(const EESocket& _server);
		~EESocket();
		
		SOCKET GetSocket();
		sockaddr_in GetAddr();

	protected:
		SOCKET m_socket;
		sockaddr_in m_addr;
	};

}

#endif