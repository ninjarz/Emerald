#include "EETCP.h"



//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EETCP
	//----------------------------------------------------------------------------------------------------
	bool EETCP::s_isTCPInitialized = false;

	//----------------------------------------------------------------------------------------------------
	bool EETCP::TCPInitialize()
	{
		if (!s_isTCPInitialized)
		{

			WSADATA  ws;
			if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
			{
				MessageBoxW(NULL, L"Init Windows Socket Failed!", L"ERROR", MB_OK);
				return false;
			}

			s_isTCPInitialized = true;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EETCP::EETCP(char* _addr, u_short _port)
		:
		m_socket(),
		m_addr()
	{
		TCPInitialize();

		m_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

		m_addr.sin_family = AF_INET;
		m_addr.sin_addr.s_addr = inet_addr(_addr);
		m_addr.sin_port = htons(_port);
		memset(m_addr.sin_zero, 0x00, 8);
	}

	//----------------------------------------------------------------------------------------------------
	EETCP::EETCP(const EETCP& _server)
		:
		m_socket(_server.m_socket),
		m_addr(_server.m_addr)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EETCP::~EETCP()
	{

	}

	//EETCPServer
	//----------------------------------------------------------------------------------------------------
	EETCPServer::EETCPServer(char* _addr, u_short _port)
		:
		EETCP(_addr, _port)
	{
		bind(m_socket, (sockaddr*)&m_addr, sizeof(sockaddr));
		listen(m_socket, SOMAXCONN);

		DWORD mode(1);
		ioctlsocket(m_socket, FIONBIO, &mode);
	}

	//----------------------------------------------------------------------------------------------------
	EETCPServer::EETCPServer(const EETCPServer& _server)
		:
		EETCP(_server)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EETCPServer::~EETCPServer()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EETCPServer::Process()
	{
		Accept();

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	SOCKET EETCPServer::Accept()
	{
		fd_set rds;
		FD_ZERO(&rds);
		FD_SET(m_socket, &rds);
		timeval timeout = { EE_TCP_TIME, 0 };
		if (int result = select(0, &rds, NULL, NULL, &timeout))
		{
			if (result == 0)
			{
				//Time out
			}
			else if (result == SOCKET_ERROR)
			{

			}
			else
			{
				if (FD_ISSET(m_socket, &rds))
				{
					SOCKET clientSocket;
					sockaddr_in clientAddr;
					int size = sizeof(clientAddr);
					clientSocket = accept(m_socket, (struct sockaddr*)&clientAddr, &size);
					m_clients.insert(std::pair<SOCKET, EETCPClientData>(clientSocket, EETCPClientData(clientSocket, clientAddr)));
					return clientSocket;
				}
			}
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EETCPServer::Send(const std::string _data)
	{
		for (std::map<SOCKET, EETCPClientData>::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
		{
			send(it->second.clientSocket, _data.c_str(), _data.size(), 0);
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EETCPServer::Send(SOCKET _socket, const std::string _data)
	{
		if (send(_socket, _data.c_str(), _data.size(), 0) != -1)
			return true;

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EETCPServer::Recv(SOCKET _socket, std::string& _data)
	{
		fd_set rds;
		FD_ZERO(&rds);
		FD_SET(_socket, &rds);
		timeval timeout = { EE_TCP_TIME, 0 };
		if (int result = select(0, &rds, NULL, NULL, &timeout))
		{
			if (result == 0)
			{
				//Time out
			}
			else if (result == SOCKET_ERROR)
			{
				//Error
			}
			else
			{
				if (FD_ISSET(_socket, &rds))
				{
					char buff[EE_TCP_SIZE_MAX];
					size_t size = recv(_socket, buff, EE_TCP_SIZE_MAX, 0);
					if (size != -1)
					{
						_data.clear();
						_data.append(buff, size);
						return true;
					}
				}
			}
		}

		return false;
	}

	//EETCPClient
	//----------------------------------------------------------------------------------------------------
	EETCPClient::EETCPClient(char* _addr, u_short _port)
		:
		EETCP(_addr, _port)
	{
		connect(m_socket, (sockaddr*)&m_addr, sizeof(sockaddr));
	}

	//----------------------------------------------------------------------------------------------------
	EETCPClient::EETCPClient(const EETCPClient& _client)
		:
		EETCP(_client)
	{

	}
	
	//----------------------------------------------------------------------------------------------------
	EETCPClient::~EETCPClient()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EETCPClient::Send(const std::string _data)
	{
		if (send(m_socket, _data.c_str(), _data.size(), 0) != -1)
			return true;

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EETCPClient::Recv(std::string& _data)
	{
		fd_set rds;
		FD_ZERO(&rds);
		FD_SET(m_socket, &rds);
		timeval timeout = { EE_TCP_TIME, 0 };
		if (int result = select(0, &rds, NULL, NULL, &timeout))
		{
			if (result == 0)
			{
				//Timeout
			}
			else if (result == SOCKET_ERROR)
			{
				//Error
			}
			else
			{
				if (FD_ISSET(m_socket, &rds))
				{
					char buff[EE_TCP_SIZE_MAX];
					size_t size = recv(m_socket, buff, EE_TCP_SIZE_MAX, 0);
					if (size != -1)
					{
						_data.clear();
						_data.append(buff, size);
						return true;
					}
				}
			}
		}

		return false;
	}
}