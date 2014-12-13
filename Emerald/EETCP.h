#pragma once
#ifndef _EE_TCP_H_
#define _EE_TCP_H_

#pragma comment(lib, "ws2_32.lib")
#include <winsock.h>
#include <vector>
#include <map>

#define EE_TCP_SIZE_MAX 1024
#define EE_TCP_TIME 0

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EETCP
	//----------------------------------------------------------------------------------------------------
	class EETCP
	{
	protected:
		static bool TCPInitialize();

	protected:
		static bool s_isTCPInitialized;

	public:
		EETCP(char* _addr , u_short _port);
		EETCP(const EETCP& _server);
		~EETCP();

	protected:
		SOCKET m_socket;
		sockaddr_in m_addr;
	};

	//EETCPClientData
	//----------------------------------------------------------------------------------------------------
	struct EETCPClientData
	{
		inline EETCPClientData(const SOCKET& _socket, const sockaddr_in& _addr) : clientSocket(_socket), clientAddr(_addr) {}

		SOCKET clientSocket;
		sockaddr_in clientAddr;
	};

	//EETCPServer
	//----------------------------------------------------------------------------------------------------
	class EETCPServer : public EETCP
	{
	public:
		EETCPServer(char* _addr, u_short _port);
		EETCPServer(const EETCPServer& _server);
		~EETCPServer();

		virtual bool Process();
		SOCKET Accept();
		bool Send(const std::string _data);
		bool Send(SOCKET _socket, const std::string _data);
		bool Recv(SOCKET _socket, std::string& _data);

	protected:
		std::map<SOCKET, EETCPClientData> m_clients;
	};

	//EETCPClient
	//----------------------------------------------------------------------------------------------------
	class EETCPClient : public EETCP
	{
	public:
		EETCPClient(char* _addr, u_short _port);
		EETCPClient(const EETCPClient& _client);
		~EETCPClient();

		bool Send(const std::string _data);
		bool Recv(std::string& _data);

	protected:

	};
}


#endif