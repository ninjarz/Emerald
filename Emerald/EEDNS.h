#pragma once
#ifndef _EE_DNS_H_
#define _EE_DNS_H_

#include <map>
#include "EEHelper.h"
#include "EEUDP.h"
#include "EETCP.h"

#define DNS_SERVER "114.114.114.114"
/*	
message:

    +---------------------+
    |        Header       |
    +---------------------+
    |       Question      | the question for the name server
    +---------------------+
    |        Answer       | RRs answering the question
    +---------------------+
    |      Authority      | RRs pointing toward an authority
    +---------------------+
    |      Additional     | RRs holding additional information
    +---------------------+	
*/
/*
The header contains the following fields:

                                    1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      ID                       |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    QDCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ANCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    NSCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ARCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

where:

ID              A 16 bit identifier assigned by the program that
                generates any kind of query.  This identifier is copied
                the corresponding reply and can be used by the requester
                to match up replies to outstanding queries.
	
QR              A one bit field that specifies whether this message is a
                query (0), or a response (1).
	
OPCODE          A four bit field that specifies kind of query in this
                message.  This value is set by the originator of a query
                and copied into the response.  The values are:

                0               a standard query (QUERY)
                1               an inverse query (IQUERY)
                2               a server status request (STATUS)
                3-15            reserved for future use

AA              Authoritative Answer - this bit is valid in responses,
                and specifies that the responding name server is an
                authority for the domain name in question section.

                Note that the contents of the answer section may have
                multiple owner names because of aliases.  The AA bit
                corresponds to the name which matches the query name, or
                the first owner name in the answer section.

TC              TrunCation - specifies that this message was truncated
                due to length greater than that permitted on the
                transmission channel.

RD              Recursion Desired - this bit may be set in a query and
                is copied into the response.  If RD is set, it directs
                the name server to pursue the query recursively.
                Recursive query support is optional.

RA              Recursion Available - this be is set or cleared in a
                response, and denotes whether recursive query support is
                available in the name server.

Z               Reserved for future use.  Must be zero in all queries
                and responses.

RCODE           Response code - this 4 bit field is set as part of
                responses.  The values have the following
                interpretation:

                0               No error condition
                1               Format error - The name server was
                                unable to interpret the query.
                2               Server failure - The name server was
                                unable to process this query due to a
                                problem with the name server.
                3               Name Error - Meaningful only for
                                responses from an authoritative name
                                server, this code signifies that the
                                domain name referenced in the query does
                                not exist.
                4               Not Implemented - The name server does
                                not support the requested kind of query.
                5               Refused - The name server refuses to
                                perform the specified operation for
                                policy reasons.  For example, a name
                                server may not wish to provide the
                                information to the particular requester,
                                or a name server may not wish to perform
                                a particular operation (e.g., zone
                                transfer) for particular data.
                6-15            Reserved for future use.

QDCOUNT         an unsigned 16 bit integer specifying the number of
                entries in the question section.

ANCOUNT         an unsigned 16 bit integer specifying the number of
                resource records in the answer section.

NSCOUNT         an unsigned 16 bit integer specifying the number of name
                server resource records in the authority records
                section.

ARCOUNT         an unsigned 16 bit integer specifying the number of
                resource records in the additional records section.
*/
/*
Question section format:

                                    1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                     QNAME                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QTYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QCLASS                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

where:

QNAME           a domain name represented as a sequence of labels, where
                each label consists of a length octet followed by that
                number of octets.  The domain name terminates with the
                zero length octet for the null label of the root.  Note
                that this field may be an odd number of octets; no
                padding is used.

QTYPE           a two octet code which specifies the type of the query.
                The values for this field include all codes valid for a
                TYPE field, together with some more general codes which
                can match more than one type of RR.

QCLASS          a two octet code that specifies the class of the query.
                For example, the QCLASS field is IN for the Internet.	
*/
/*
Resource record format:
                                    1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                                               /
    /                      NAME                     /
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     CLASS                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TTL                      |
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                   RDLENGTH                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
    /                     RDATA                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

where:

NAME            a domain name to which this resource record pertains.

TYPE            two octets containing one of the RR type codes.  This
                field specifies the meaning of the data in the RDATA
                field.

CLASS           two octets which specify the class of the data in the
                RDATA field.

TTL             a 32 bit unsigned integer that specifies the time
                interval (in seconds) that the resource record may be
                cached before it should be discarded.  Zero values are
                interpreted to mean that the RR can only be used for the
                transaction in progress, and should not be cached.

RDLENGTH        an unsigned 16 bit integer that specifies the length in
                octets of the RDATA field.

RDATA           a variable length string of octets that describes the
                resource.  The format of this information varies
                according to the TYPE and CLASS of the resource record.
                For example, the if the TYPE is A and the CLASS is IN,
                the RDATA field is a 4 octet ARPA Internet address.
	
*/
/*
MX RDATA format:

    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                  PREFERENCE                   |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    /                   EXCHANGE                    /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

where:

PREFERENCE      A 16 bit integer which specifies the preference given to
                this RR among others at the same owner.  Lower values
                are preferred.
EXCHANGE        A <domain-name> which specifies a host willing to act as
                a mail exchange for the owner name.
*/

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEDNSHeader
	//----------------------------------------------------------------------------------------------------
	struct EEDNSHeader
	{
		unsigned short ID;
		union
		{
			unsigned short FLAG;
			struct
			{
				unsigned short RCODE : 4;
				unsigned short Z  : 3;
				unsigned short RA : 1;
				unsigned short RD : 1;
				unsigned short TC : 1;
				unsigned short AA : 1;
				unsigned short OPCODE : 4;
				unsigned short QR : 1;
			};
		};
		unsigned short QDCOUNT;
		unsigned short ANCOUNT;
		unsigned short NSCOUNT;
		unsigned short ARCOUNT;

		inline int Size()
		{
			return 12;
		}
		inline bool Load(const char* _data)
		{
			if (_data)
			{
				if (EEIsBigEndian()) // fix the endianess issue
					memcpy(this, _data, sizeof(EEDNSHeader));
				else
				{
					unsigned short *headerData = (unsigned short*)_data;
					ID = ntohs(*(headerData++));
					FLAG = ntohs(*(headerData++));
					QDCOUNT = ntohs(*(headerData++));
					ANCOUNT = ntohs(*(headerData++));
					NSCOUNT = ntohs(*(headerData++));
					ARCOUNT = ntohs(*(headerData++));
				}
				return true;
			}

			return false;
		}
		inline std::string NetString()
		{
			std::string result;
			result.resize(12);
			unsigned short *dst = (unsigned short *)&result[0];
			dst[0] = htons(ID);
			dst[1] = htons(FLAG);
			dst[2] = htons(QDCOUNT);
			dst[3] = htons(ANCOUNT);
			dst[4] = htons(NSCOUNT);
			dst[5] = htons(ARCOUNT);
			return result;
		}
		inline void Print()
		{
			printf("Header\n");
			printf("ID:%d \tFLAG:%d\n", ID, FLAG);
			printf("QR:%d \tOPCODE:%d \tAA:%d \tTC:%d\n", QR, OPCODE, AA, TC);
			printf("RD:%d \tRA:%d \tZ:%d \tRCODE:%d\n", RD, RA, Z, RCODE);
			printf("QDCOUNT:%d \tANCOUNT:%d\n", QDCOUNT, ANCOUNT);
			printf("NSCOUNT:%d \tARCOUNT:%d\n", NSCOUNT, ARCOUNT);
		}
	};

	//EEDNSQuestion
	//----------------------------------------------------------------------------------------------------
	struct EEDNSQuestion
	{
		std::string NAME;
		unsigned short TYPE;
		unsigned short CLASS;

		inline int Size()
		{
			return NAME.size() + 1 + 4;
		}
		inline bool Load(const char* _data)
		{
			NAME = _data;
			_data += NAME.size() + 1;
			TYPE = ntohs(*(unsigned short*)_data);
			_data += 2;
			CLASS = ntohs(*(unsigned short*)_data);
			_data += 2;
			return true;
		}
		inline std::string Name()
		{
			std::string result;
			char *content = (char*)NAME.data();
			while (*content)
			{
				result.append(content + 1, *content);
				result += '.';
				content += *content + 1;
			}
			result.pop_back();
			return result;
		}
		inline std::string NetString()
		{
			std::string result;
			result += NAME;
			result += '\0';
			result.resize(result.size() + 4);
			unsigned short *dst = (unsigned short *)&result[result.size() - 4];
			dst[0] = htons(TYPE);
			dst[1] = htons(CLASS);
			return result;
		}
		inline void Print()
		{
			printf("Question\n");
			printf("NAME:%s\n", Name().data());
			printf("TYPE:%d \tCLASS:%d\n", TYPE, CLASS);
		}
	};

	// EEDNSAnswer
	//----------------------------------------------------------------------------------------------------
	struct EEDNSAnswer
	{
		std::string NAME;
		unsigned short TYPE;
		unsigned short CLASS;
		unsigned int TTL;
		unsigned short RDLENGTH;
		std::string RDATA;

		inline std::string NetString()
		{
			std::string result;
			result += NAME;
			result += '\0';
			result.resize(result.size() + 10);
			unsigned short *dst0 = (unsigned short *)&result[result.size() - 10];
			dst0[0] = htons(TYPE);
			dst0[1] = htons(CLASS);
			dst0[4] = htons(RDLENGTH);
			unsigned int *dst1 = (unsigned int*)&dst0[2];
			dst1[0] = htonl(TTL);
			result += RDATA;
			return result;
		}
		inline void Print()
		{
			printf("Answer\n");
			printf("NAME:%s\n", NAME.data());
			printf("TYPE:%d \tCLASS:%d\n", TYPE, CLASS);
			printf("TTL:%d\n", TTL);
			printf("RDLENGTH:%d\n", RDLENGTH);
			printf("RDATA:%s\n", RDATA.data());
		}
	};

	// EEDNSServer
	//----------------------------------------------------------------------------------------------------
	class EEDNSServer : public EEUDPServer
	{
	public:
		EEDNSServer(char* _addr, u_short _port);
		EEDNSServer(const EEUDPServer& _server);
		virtual ~EEDNSServer();

		virtual bool Process();
		bool Lookup();
		bool LoadHostes(wchar_t* _file);
		std::string GetDomainRoot(std::string _domain);

	protected:
		//std::vector<std::string>
		std::map<std::string, std::string> m_hosts;
		std::map<unsigned int, sockaddr_storage> m_clients;

		EEUDPClient m_loopup;
	};
}


#endif