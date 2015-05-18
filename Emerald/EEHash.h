#pragma once
#ifndef _EE_HASH_H_
#define _EE_HASH_H_

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	unsigned int RSHash(char* _str, unsigned int _len);
	unsigned int JSHash(char* _str, unsigned int _len);
	unsigned int PJWHash(char* _str, unsigned int _len);
	unsigned int ELFHash(char* _str, unsigned int _len);
	unsigned int BKDRHash(char* _str, unsigned int _len);
	unsigned int SDBMHash(char* _str, unsigned int _len);
	unsigned int DJBHash(char* _str, unsigned int _len);
	unsigned int DEKHash(char* _str, unsigned int _len);
	unsigned int BPHash(char* _str, unsigned int _len);
	unsigned int FNVHash(char* _str, unsigned int _len);
	unsigned int APHash(char* _str, unsigned int _len);
	unsigned int HFLPHash(char* _str, unsigned int _len);
	unsigned int HFHash(char* _str, unsigned int _len);
	unsigned int StrHash(char* _str, unsigned int _len);
	unsigned int TianlHash(char* _str, unsigned int _len);
}

#endif