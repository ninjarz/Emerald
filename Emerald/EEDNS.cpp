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
			//header
			EEDNSHeader header;
			header.Load(data.data());
			printf("ID:%d \tFLAG:%d\n", header.ID, header.FLAG);
			printf("QDCOUNT:%d \tANCOUNT:%d\n", header.QDCOUNT, header.ANCOUNT);
			printf("NSCOUNT:%d \tARCOUNT:%d\n", header.NSCOUNT, header.ARCOUNT);

			char *content = (char*)data.data() + header.Size();
			//question
			std::vector<EEDNSQuestion> questions(header.QDCOUNT);
			for (EEDNSQuestion &question : questions)
			{
				question.NAME = content;
				content += question.NAME.size() + 1;
				question.TYPE = ntohs(*(unsigned short*)content);
				content += 2;
				question.CLASS = ntohs(*(unsigned short*)content);
				content += 2;

				printf("NAME:%s\n", question.Name());
			}

			//make answers
			std::vector<EEDNSAnswer> answers(header.QDCOUNT);
			for (EEDNSAnswer &answer : answers)
			{
				answer.NAME += (char)0xc0;
				
				answer.TYPE;
				answer.CLASS;
			}

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
			fin >> name >> ip;
			while (!fin.eof())
			{
				//m_hosts.insert(name, ip);
				fin >> ip >> name;
			}
			fin.close();
			return true;
		}

		return false;
	}
}