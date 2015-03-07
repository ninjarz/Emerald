#pragma once
#ifndef _EE_TCP_H_
#define _EE_TCP_H_

#include "EESocket.h"
#include <map>

#define EE_TCP_SIZE_MAX 1024
#define EE_TCP_TIME 0

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EETCP
	//----------------------------------------------------------------------------------------------------
	class EETCP : public EESocket
	{
	public:
		EETCP(char* _addr , u_short _port);
		EETCP(const EETCP& _server);
		virtual ~EETCP();

	protected:
	};

	//EETCPServer
	//----------------------------------------------------------------------------------------------------
	class EETCPServer : public EETCP
	{
	public:
		EETCPServer(char* _addr, u_short _port);
		EETCPServer(const EETCPServer& _server);
		virtual ~EETCPServer();

		virtual bool Process();
		SOCKET Accept();
		bool Send(const std::string& _data);
		bool Send(SOCKET _socket, const std::string _data);
		SOCKET Recv(std::string& _data);
		SOCKET Recv(std::string& _data, unsigned int _size);
		bool Recv(SOCKET _socket, std::string& _data);

	protected:
		std::map<SOCKET, EESocket> m_clients;
	};

	//EETCPClient
	//----------------------------------------------------------------------------------------------------
	class EETCPClient : public EETCP
	{
	public:
		EETCPClient(char* _addr, u_short _port);
		EETCPClient(const EETCPClient& _client);
		virtual ~EETCPClient();

		bool Send(const std::string& _data);
		bool Recv(std::string& _data);
		bool Recv(std::string& _data, unsigned int _size);

	protected:

	};
}


#endif