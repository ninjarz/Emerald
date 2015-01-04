#include "EEUDP.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEUDP
	//----------------------------------------------------------------------------------------------------
	EEUDP::EEUDP(char* _addr, u_short _port)
		:
		EESocket(_addr, _port)
	{
		m_socket = socket(m_addr.ss_family, SOCK_DGRAM, IPPROTO_UDP);
	}

	//----------------------------------------------------------------------------------------------------
	EEUDP::EEUDP(const EEUDP& _server)
		:
		EESocket(_server)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEUDP::~EEUDP()
	{

	}

	//EEUDPServer
	//----------------------------------------------------------------------------------------------------
	EEUDPServer::EEUDPServer(char* _addr, u_short _port)
		:
		EEUDP(_addr, _port)
	{
		if (-1 == bind(m_socket, (sockaddr*)&m_addr, m_addrLen))
		{
			MessageBoxW(NULL, L"UDP bind failed!", L"ERROR", MB_OK);
		}

		DWORD mode(1);
		ioctlsocket(m_socket, FIONBIO, &mode);
	}

	//----------------------------------------------------------------------------------------------------
	EEUDPServer::EEUDPServer(const EEUDPServer& _server)
		:
		EEUDP(_server)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEUDPServer::~EEUDPServer()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEUDPServer::Send(sockaddr_storage* _addr, const std::string _data)
	{
		switch (_addr->ss_family)
		{
		case AF_INET:
			if (sendto(m_socket, _data.c_str(), _data.size(), 0, (sockaddr*)_addr, sizeof(sockaddr_in)) == -1)
				return false;
			break;

		case AF_INET6:
			if (sendto(m_socket, _data.c_str(), _data.size(), 0, (sockaddr*)_addr, sizeof(sockaddr_in6)) == -1)
				return false;
			break;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEUDPServer::Send(sockaddr_in* _addr, const std::string _data)
	{
		if (sendto(m_socket, _data.c_str(), _data.size(), 0, (sockaddr*)_addr, sizeof(sockaddr_in)) == -1)
			return false;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEUDPServer::Recv(sockaddr_storage* _addr, std::string& _data)
	{
		fd_set rds;
		FD_ZERO(&rds);
		FD_SET(m_socket, &rds);
		timeval timeout = { 0, 0 };
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
				if (FD_ISSET(m_socket, &rds))
				{
					int addrLen = sizeof(sockaddr_storage);
					char buff[EE_UDP_SIZE_MAX];
					size_t size = recvfrom(m_socket, buff, EE_UDP_SIZE_MAX, 0, (sockaddr*)_addr, &addrLen);
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

	//EEUDPClient
	//----------------------------------------------------------------------------------------------------
	EEUDPClient::EEUDPClient(char* _addr, u_short _port)
		:
		EEUDP(_addr, _port)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEUDPClient::EEUDPClient(const EEUDPClient& _client)
		:
		EEUDP(_client)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEUDPClient::~EEUDPClient()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEUDPClient::Send(const std::string& _data)
	{
		if (sendto(m_socket, _data.c_str(), _data.size(), 0, (sockaddr*)&m_addr, m_addrLen) == -1)
			return false;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEUDPClient::Recv(std::string& _data)
	{
		fd_set rds;
		FD_ZERO(&rds);
		FD_SET(m_socket, &rds);
		timeval timeout = { EE_UDP_TIME, 0 };
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
					sockaddr_storage addr;
					int addrLen = sizeof(sockaddr_storage);
					char buff[EE_UDP_SIZE_MAX];
					size_t size = recvfrom(m_socket, buff, EE_UDP_SIZE_MAX, 0, (sockaddr*)&addr, &addrLen);
					//if (addr.sin_addr.S_un.S_addr == m_addr.sin_addr.S_un.S_addr)
					{
						if (size != -1)
						{
							_data.clear();
							_data.append(buff, size);
							return true;
						}
					}
				}
			}
		}

		return false;
	}
}