#include "EESocket.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EESocket
	//----------------------------------------------------------------------------------------------------
	bool EESocket::s_isSocketInitialized = false;

	//----------------------------------------------------------------------------------------------------
	bool EESocket::InitializeSocket()
	{
		if (!s_isSocketInitialized)
		{

			WSADATA  ws;
			if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
			{
				MessageBoxW(NULL, L"Init Windows Socket Failed!", L"ERROR", MB_OK);
				return false;
			}

			s_isSocketInitialized = true;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EESocket::EESocket(char* _addr, char* _port)
		:
		m_socket(),
		m_addr(),
		m_addrLen(0)
	{
		InitializeSocket();

		addrinfo* addr;
		if (getaddrinfo(_addr, _port, NULL, &addr) != 0) {
			memcpy(&m_addr, addr->ai_addr, addr->ai_addrlen);
			freeaddrinfo(addr);
		}
	}

	//----------------------------------------------------------------------------------------------------
	EESocket::EESocket(char* _addr, u_short _port)
		:
		m_socket(),
		m_addr(),
		m_addrLen(0)
	{
		InitializeSocket();

		std::string port;
		std::stringstream ss;
		ss << _port;
		ss >> port;
		addrinfo* addr;
		if (getaddrinfo(_addr, port.c_str(), NULL, &addr) == 0) {
			memcpy(&m_addr, addr->ai_addr, addr->ai_addrlen);
			m_addrLen = addr->ai_addrlen;
			freeaddrinfo(addr);
		}
	}

	//----------------------------------------------------------------------------------------------------
	EESocket::EESocket(SOCKET _socket, const sockaddr_storage& _addr)
		:
		m_socket(_socket),
		m_addr(_addr),
		m_addrLen(0)
	{
		InitializeSocket();

		switch (_addr.ss_family)
		{
		case AF_INET:
			m_addrLen = 16;
			break;
		case AF_INET6:
			m_addrLen = 28;
			break;
		}
	}

	//----------------------------------------------------------------------------------------------------
	EESocket::EESocket(SOCKET _socket, const sockaddr_in& _addr)
		:
		m_socket(_socket),
		m_addr(),
		m_addrLen(0)
	{
		InitializeSocket();

		memcpy(&m_addr, &_addr, sizeof(_addr));
		m_addrLen = sizeof(_addr);
	}

	//----------------------------------------------------------------------------------------------------
	EESocket::EESocket(const EESocket& _socket)
		:
		m_socket(_socket.m_socket),
		m_addr(_socket.m_addr)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EESocket::~EESocket()
	{
	}

	//----------------------------------------------------------------------------------------------------
	SOCKET EESocket::GetSocket()
	{
		return m_socket;
	}

}