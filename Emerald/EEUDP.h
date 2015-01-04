#pragma once
#ifndef _EE_UDP_H_
#define _EE_UDP_H_

#include "EESocket.h"
#include <vector>

#define EE_UDP_SIZE_MAX 1024
#define EE_UDP_TIME 0

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEUDP
	//----------------------------------------------------------------------------------------------------
	class EEUDP : public EESocket
	{
	public:
		EEUDP(char* _addr, u_short _port);
		EEUDP(const EEUDP& _server);
		~EEUDP();

	protected:
	};

	//EEUDPServer
	//----------------------------------------------------------------------------------------------------
	class EEUDPServer : public EEUDP
	{
	public:
		EEUDPServer(char* _addr, u_short _port);
		EEUDPServer(const EEUDPServer& _server);
		~EEUDPServer();

		bool Send(sockaddr_storage* _addr, const std::string _data);
		bool Send(sockaddr_in* _addr, const std::string _data);
		bool Recv(sockaddr_storage* _addr, std::string& _data);

	protected:

	};

	//EEUDPClient
	//----------------------------------------------------------------------------------------------------
	class EEUDPClient : public EEUDP
	{
	public:
		EEUDPClient(char* _addr, u_short _port);
		EEUDPClient(const EEUDPClient& _client);
		~EEUDPClient();

		bool Send(const std::string& _data);
		bool Recv(std::string& _data);

	protected:

	};




}


#endif