#include "EEDNS.h"
#include <fstream>

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEDNSServer
	//----------------------------------------------------------------------------------------------------
	EEDNSServer::EEDNSServer(char* _addr, u_short _port)
		:
		EEUDPServer(_addr, _port),
		m_loopup(DNS_SERVER, 53)
	{
		LoadHostes(L"hosts");
	}

	//----------------------------------------------------------------------------------------------------
	EEDNSServer::EEDNSServer(const EEUDPServer& _server)
		:
		EEUDPServer(_server),
		m_loopup(DNS_SERVER, 53)
	{
		LoadHostes(L"hosts");
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
		if (Recv(&addr, data))
		{
			printf("---------------------------------------------------\n");
			printf("Recv from:%s %d\n", EEGetIP(addr).data(), EEGetPort(addr));
			// header
			EEDNSHeader header;
			header.Load(data.data());
			header.Print();

			char *content = (char*)data.data() + header.Size();
			// question
			std::vector<EEDNSQuestion> questions(header.QDCOUNT);
			for (EEDNSQuestion &question : questions)
			{
				question.Load(content);
				content += question.Size();
				question.Print();
				if (question.TYPE != 1)
				{
					header.QR = 1;
					header.RA = 1;
					header.RCODE = 4;
					std::string result = header.NetString() + questions[0].NetString();
					Send(&addr, result);

					return false;
				}
			}

			// make answers
			std::vector<EEDNSAnswer> answers(header.QDCOUNT);
			for (int i = 0; i < header.QDCOUNT; ++i)
			{
				++header.ANCOUNT;
				answers[i].NAME = questions[i].NAME;
				//answer.NAME += (char)0xc0;
				//answer.NAME += (char)(header.Size());
				answers[i].TYPE = 1;
				answers[i].CLASS = 1;
				answers[i].TTL = 6000;
				answers[i].RDLENGTH = 4;
				answers[i].RDATA.resize(answers[i].RDLENGTH);
				unsigned int *rdata = (unsigned int*)&answers[i].RDATA[0];
				auto iter = m_hosts.find(questions[i].Name());
				if (iter != m_hosts.end())
					*rdata = inet_addr(iter->second.data());
				else
				{
					m_clients.insert(std::pair<unsigned int, sockaddr_storage>(header.ID, addr));
					m_loopup.Send(data);

					return false;
				}
			}

			// respond
			header.QR = 1;
			header.RA = 1;
			std::string result = header.NetString();
			for (EEDNSQuestion &question : questions)
				result += question.NetString();
			for (EEDNSAnswer &answer : answers)
				result += answer.NetString();
			Send(&addr, result);

			return true;
		}

		Loopup();

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEDNSServer::Loopup()
	{
		std::string data;
		if (m_loopup.Recv(data))
		{
			EEDNSHeader header;
			header.Load(data.data());
			sockaddr_storage addr = m_clients[header.ID];
			m_clients.erase(header.ID);
			printf("**************************************************\n");
			printf("Sent to:%s %d\n", EEGetIP(addr).data(), EEGetPort(addr));
			header.Print();

			char *content = (char*)data.data() + header.Size();
			// question
			std::vector<EEDNSQuestion> questions(header.QDCOUNT);
			for (EEDNSQuestion &question : questions)
			{
				question.Load(content);
				content += question.Size();

				question.Print();
			}

			Send(&addr, data);
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
			while (!fin.eof())
			{
				fin >> ip >> name;
				m_hosts.insert(std::pair<std::string, std::string>(name, ip));
			}
			fin.close();
			return true;
		}

		return false;
	}

	// todo (memo: difficult)
	//----------------------------------------------------------------------------------------------------
	std::string EEDNSServer::GetDomainRoot(std::string _domain)
	{
		int pos = _domain.find("http://");
		if (pos == 0)
			_domain.erase(0, 7);
		pos = _domain.find("/");
		if (pos != std::string::npos)
			_domain.erase(pos, _domain.size() - pos);

		return _domain;
	}
}