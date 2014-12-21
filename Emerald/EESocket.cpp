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
	EESocket::EESocket(char* _addr, u_short _port)
		:
		m_socket(),
		m_addr()
	{
		InitializeSocket();
	}

	//----------------------------------------------------------------------------------------------------
	EESocket::EESocket(SOCKET _socket, const sockaddr_in& _port)
		:
		m_socket(_socket),
		m_addr(_port)
	{
		InitializeSocket();
	}

	//----------------------------------------------------------------------------------------------------
	EESocket::EESocket(const EESocket& _server)
		:
		m_socket(_server.m_socket),
		m_addr(_server.m_addr)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EESocket::~EESocket()
	{
		closesocket(m_socket);
	}

	//----------------------------------------------------------------------------------------------------
	SOCKET EESocket::GetSocket()
	{
		return m_socket;
	}

	//----------------------------------------------------------------------------------------------------
	sockaddr_in EESocket::GetAddr()
	{
		return m_addr;
	}

}