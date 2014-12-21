#include "EETCP.h"



//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EETCP
	//----------------------------------------------------------------------------------------------------
	EETCP::EETCP(char* _addr, u_short _port)
		:
		EESocket(_addr, _port)
	{
		m_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

		m_addr.sin_family = AF_INET;
		m_addr.sin_addr.s_addr = inet_addr(_addr);
		m_addr.sin_port = htons(_port);
		memset(m_addr.sin_zero, 0x00, 8);
	}

	//----------------------------------------------------------------------------------------------------
	EETCP::EETCP(const EETCP& _server)
		:
		EESocket(_server)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EETCP::~EETCP()
	{
		closesocket(m_socket);
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
					m_clients.insert(std::pair<SOCKET, EESocket>(clientSocket, EESocket(clientSocket, clientAddr)));
					return clientSocket;
				}
			}
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EETCPServer::Send(const std::string& _data)
	{
		for (std::map<SOCKET, EESocket>::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
		{
			if (send(it->first, _data.c_str(), _data.size(), 0) == -1)
			{
				//delete the socket
			}
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EETCPServer::Send(SOCKET _socket, const std::string _data)
	{
		if (send(_socket, _data.c_str(), _data.size(), 0) == -1)
			return false;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	SOCKET EETCPServer::Recv(std::string& _data)
	{
		fd_set rds;
		FD_ZERO(&rds);
		for (std::pair<SOCKET, EESocket> client : m_clients)
		{
			FD_SET(client.first, &rds);
		}
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
				for (std::pair<SOCKET, EESocket> client : m_clients)
				{
					if (FD_ISSET(client.first, &rds))
					{
						char buff[EE_TCP_SIZE_MAX];
						size_t size = recv(client.first, buff, EE_TCP_SIZE_MAX, 0);
						if (size != -1)
						{
							_data.clear();
							_data.append(buff, size);
							return client.first;
						}
					}
				}
			}
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	SOCKET EETCPServer::Recv(std::string& _data, unsigned int _size)
	{
		fd_set rds;
		FD_ZERO(&rds);
		for (std::pair<SOCKET, EESocket> client : m_clients)
		{
			FD_SET(client.first, &rds);
		}
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
				for (std::pair<SOCKET, EESocket> client : m_clients)
				{
					if (FD_ISSET(client.first, &rds))
					{
						char buff[EE_TCP_SIZE_MAX];
						size_t size = recv(client.first, buff, _size < EE_TCP_SIZE_MAX ? _size : EE_TCP_SIZE_MAX, 0);
						if (size != -1)
						{
							_data.clear();
							_data.append(buff, size);
							return client.first;
						}
					}
				}
			}
		}

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
	bool EETCPClient::Send(const std::string& _data)
	{
		if (send(m_socket, _data.c_str(), _data.size(), 0) == -1)
			return false;

		return true;
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

	//----------------------------------------------------------------------------------------------------
	bool EETCPClient::Recv(std::string& _data, unsigned int _size)
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
					size_t size = recv(m_socket, buff, _size < EE_TCP_SIZE_MAX ? _size : EE_TCP_SIZE_MAX, 0);
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