#include "EEDNS.h"
#include <fstream>

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEDNSServer
	//----------------------------------------------------------------------------------------------------
	EEDNSServer::EEDNSServer(char* _addr, u_short _port)
		:
		EEUDPServer(_addr, _port)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEDNSServer::EEDNSServer(const EEUDPServer& _server)
		:
		EEUDPServer(_server)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEDNSServer::~EEDNSServer()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEDNSServer::Process()
	{
		sockaddr_storage addr;
		std::string data;
		if (Recv(&addr, data) || data.size() >= sizeof(EEDNSHeader))
		{
			/*addrinfo* addrinfo;
			sockaddr_storage host;
			if (getaddrinfo("8.8.8.8", "53", NULL, &addrinfo) != 0) {
				memcpy(&host, addrinfo->ai_addr, addrinfo->ai_addrlen);
				freeaddrinfo(addrinfo);
			}
			if (EEGetIP(addr) != "8.8.8.8")
				Send(&host, data);*/

			printf("Recv from:%s %d\n", EEGetIP(addr).data(), EEGetPort(addr));
			// header
			EEDNSHeader header;
			header.QR = 1;
			header.Load(data.data());
			printf("ID:%d \tFLAG:%d\n", header.ID, header.FLAG);
			printf("QDCOUNT:%d \tANCOUNT:%d\n", header.QDCOUNT, header.ANCOUNT);
			printf("NSCOUNT:%d \tARCOUNT:%d\n", header.NSCOUNT, header.ARCOUNT);

			// store the client
			//m_clients.insert(std::pair<unsigned int, sockaddr_storage>(header.ID, addr));

			char *content = (char*)data.data() + header.Size();
			// question
			std::vector<EEDNSQuestion> questions(header.QDCOUNT);
			for (EEDNSQuestion &question : questions)
			{
				question.NAME = content;
				content += question.NAME.size() + 1;
				question.TYPE = ntohs(*(unsigned short*)content);
				content += 2;
				question.CLASS = ntohs(*(unsigned short*)content);
				content += 2;

				printf("NAME:%s\n", question.Name().data());
			}

			// make answers
			std::vector<EEDNSAnswer> answers(header.QDCOUNT);
			for (EEDNSAnswer &answer : answers)
			{
				++header.ANCOUNT;
				answer.NAME += (char)0xc0;
				answer.NAME += (char)(header.Size());
				answer.TYPE = 1;
				answer.CLASS = 1;
				answer.TTL = 60;
				answer.RDLENGTH = 4;
				answer.RDATA.resize(answer.RDLENGTH);
				unsigned int *data = (unsigned int*)&answer.RDATA[0];
				*data = inet_addr("123.125.114.144");
			}
			std::string result = header.NetString() + questions[0].NetString() + answers[0].NetString();
			Send(&addr, result);
			return true;
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEDNSServer::LoadHostes(wchar_t* _file)
	{
		std::ifstream fin(_file);
		if (fin.is_open())
		{
			std::string name;
			std::string ip;
			fin >> ip >> name;
			while (!fin.eof())
			{
				m_hosts.insert(std::pair<std::string, std::string>(name, ip));
				fin >> ip >> name;
			}
			fin.close();
			return true;
		}

		return false;
	}
}