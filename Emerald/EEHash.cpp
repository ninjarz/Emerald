#include "EEHash.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// RS Hash
	//----------------------------------------------------------------------------------------------------
	unsigned int RSHash(char* _str, unsigned int _len) 
	{
		unsigned int b = 378551;
		unsigned int a = 63689;
		unsigned int hash = 0;
		unsigned int i = 0;

		for (i = 0; i < _len; _str++, i++) {
			hash = hash * a + (*_str);
			a = a*b;
		}
		return hash;
	}

	// JS Hash
	//----------------------------------------------------------------------------------------------------
	unsigned int JSHash(char* _str, unsigned int _len)
	{
		unsigned int hash = 1315423911;
		unsigned int i = 0;

		for (i = 0; i<_len; _str++, i++) {
			hash ^= ((hash << 5) + (*_str) + (hash >> 2));
		}
		return hash;
	}

	// P. J. Weinberger Hash
	//----------------------------------------------------------------------------------------------------
	unsigned int PJWHash(char* _str, unsigned int _len)
	{
		const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int)* 8);
		const unsigned int ThreeQuarters = (unsigned int)((BitsInUnsignedInt * 3) / 4);
		const unsigned int OneEighth = (unsigned int)(BitsInUnsignedInt / 8);
		const unsigned int HighBits = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
		unsigned int hash = 0;
		unsigned int test = 0;
		unsigned int i = 0;

		for (i = 0; i<_len; _str++, i++) {
			hash = (hash << OneEighth) + (*_str);
			if ((test = hash & HighBits) != 0) {
				hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
			}
		}

		return hash;
	}

	// ELF Hash
	//----------------------------------------------------------------------------------------------------
	unsigned int ELFHash(char* _str, unsigned int _len)
	{
		unsigned int hash = 0;
		unsigned int x = 0;
		unsigned int i = 0;

		for (i = 0; i < _len; _str++, i++) {
			hash = (hash << 4) + (*_str);
			if ((x = hash & 0xF0000000L) != 0) {
				hash ^= (x >> 24);
			}
			hash &= ~x;
		}
		return hash;
	}

	// BKDR Hash
	//----------------------------------------------------------------------------------------------------
	unsigned int BKDRHash(char* _str, unsigned int _len)
	{
		unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
		unsigned int hash = 0;
		unsigned int i = 0;

		for (i = 0; i < _len; _str++, i++)
		{
			hash = (hash * seed) + (*_str);
		}

		return hash;
	}

	// SDBM Hash
	//----------------------------------------------------------------------------------------------------
	unsigned int SDBMHash(char* _str, unsigned int _len)
	{
		unsigned int hash = 0;
		unsigned int i = 0;

		for (i = 0; i < _len; _str++, i++) {
			hash = (*_str) + (hash << 6) + (hash << 16) - hash;
		}

		return hash;
	}

	// DJB Hash
	//----------------------------------------------------------------------------------------------------
	unsigned int DJBHash(char* _str, unsigned int _len)
	{
		unsigned int hash = 5381;
		unsigned int i = 0;

		for (i = 0; i < _len; _str++, i++) {
			hash = ((hash << 5) + hash) + (*_str);
		}

		return hash;
	}

	// DEK Hash
	//----------------------------------------------------------------------------------------------------
	unsigned int DEKHash(char* _str, unsigned int _len)
	{
		unsigned int hash = _len;
		unsigned int i = 0;

		for (i = 0; i < _len; _str++, i++) {
			hash = ((hash << 5) ^ (hash >> 27)) ^ (*_str);
		}
		return hash;
	}

	// BP Hash
	//----------------------------------------------------------------------------------------------------
	unsigned int BPHash(char* _str, unsigned int _len)
	{
		unsigned int hash = 0;
		unsigned int i = 0;
		for (i = 0; i < _len; _str++, i++) {
			hash = hash << 7 ^ (*_str);
		}

		return hash;
	}

	// FNV Hash
	//----------------------------------------------------------------------------------------------------
	unsigned int FNVHash(char* _str, unsigned int _len)
	{
		const unsigned int fnv_prime = 0x811C9DC5;
		unsigned int hash = 0;
		unsigned int i = 0;

		for (i = 0; i < _len; _str++, i++) {
			hash *= fnv_prime;
			hash ^= (*_str);
		}

		return hash;
	}

	// AP Hash
	//----------------------------------------------------------------------------------------------------
	unsigned int APHash(char* _str, unsigned int _len)
	{
		unsigned int hash = 0xAAAAAAAA;
		unsigned int i = 0;

		for (i = 0; i < _len; _str++, i++) {
			hash ^= ((i & 1) == 0) ? ((hash << 7) ^ (*_str) * (hash >> 3)) :
				(~((hash << 11) + (*_str) ^ (hash >> 5)));
		}

		return hash;
	}

	// HFLP Hash
	//----------------------------------------------------------------------------------------------------
	unsigned int HFLPHash(char *_str, unsigned int _len)
	{
		unsigned int n = 0;
		char* b = (char *)&n;
		for (unsigned int i = 0; i< _len; ++i) {
			b[i % 4] ^= _str[i];
		}
		return n%_len;
	}

	// HK Hash
	//----------------------------------------------------------------------------------------------------
	unsigned int HFHash(char* _str, unsigned int _len)
	{
		int result = 0;
		char* ptr = _str;
		int c;
		int i = 0;
		for (i = 1; c = *ptr++; i++)
			result += c * 3 * i;
		if (result<0)
			result = -result;
		return result%_len;
	}

	// Str Hash
	//----------------------------------------------------------------------------------------------------
	unsigned int StrHash(char *_str, unsigned int _len)
	{
		register unsigned int   h;
		register unsigned char *p;
		for (h = 0, p = (unsigned char *)_str; *p; p++) {
			h = 31 * h + *p;
		}

		return h;

	}

	// Tianl Hash
	//----------------------------------------------------------------------------------------------------
	unsigned int TianlHash(char *_str, unsigned int _len)
	{
		unsigned long urlHashValue = 0;
		unsigned char ucChar;
		if (!_len)  {
			return 0;
		}
		if (_len <= 256)  {
			urlHashValue = 16777216 * (_len - 1);
		}
		else {
			urlHashValue = unsigned long(42781900080);
		}
		if (_len <= 96) {
			for (unsigned int i = 1; i <= _len; i++) {
				ucChar = _str[i - 1];
				if (ucChar <= 'Z'&&ucChar >= 'A')  {
					ucChar = ucChar + 32;
				}
				urlHashValue += (3 * i*ucChar*ucChar + 5 * i*ucChar + 7 * i + 11 * ucChar) % 1677216;
			}
		}
		else  {
			for (unsigned int i = 1; i <= 96; i++)
			{
				ucChar = _str[i + _len - 96 - 1];
				if (ucChar <= 'Z'&&ucChar >= 'A')
				{
					ucChar = ucChar + 32;
				}
				urlHashValue += (3 * i*ucChar*ucChar + 5 * i*ucChar + 7 * i + 11 * ucChar) % 1677216;
			}
		}
		return urlHashValue;
	}
}