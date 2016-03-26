#include "EESecureHashAlgorithm.h"
#include <sstream>
#include <iomanip>
#include "EEHelper.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EESecureHashAlgorithm
	//----------------------------------------------------------------------------------------------------
	std::string EESecureHashAlgorithm::SHA_1(const std::string& _input)
	{
		// Preprocess
		std::vector<std::array<unsigned char, 64>> chunks;
		Preprocess(_input, chunks);

		// Process each 512-bit chunk
		unsigned int H[5] = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0 };
		for (std::array<unsigned char, 64>& chunk : chunks)
		{
			// Break chunk into sixteen 32-bit big-endian words
			std::vector<unsigned int> words;
			words.reserve(80);
			for (int i = 0; i < 64; i += 4)
			{
				words.push_back((chunk[i + 0] << 24) | (chunk[i + 1] << 16) | (chunk[i + 2] << 8) | (chunk[i + 3] << 0));
			}

			// Extend the sixteen 32-bit words into eighty 32-bit words
			for (int i = 16; i < 80; ++i)
			{
				words.push_back(RotateL((words[i - 3] ^ words[i - 8] ^ words[i - 14] ^ words[i - 16]), 1));
			}

			// Main loop
			unsigned int h[5];
			memcpy((char*)h, (char*)H, sizeof(H));
			for (int i = 0; i < 80; ++i)
			{
				unsigned int f;
				if (i <= 19)
					f = ((h[1] & h[2]) | ((~h[1]) & h[3])) + 0x5a827999;
				else if (i <= 39)
					f = (h[1] ^ h[2] ^ h[3]) + 0x6ed9eba1;
				else if (i <= 59)
					f = ((h[1] & h[2]) | (h[1] & h[3]) | (h[2] & h[3])) + 0x8f1bbcdc;
				else
					f = (h[1] ^ h[2] ^ h[3]) + 0xca62c1d6;

				f += RotateL(h[0], 5) + h[4] + words[i];
				h[4] = h[3];
				h[3] = h[2];
				h[2] = RotateL(h[1], 30);
				h[1] = h[0];
				h[0] = f;
			}

			// Add
			for (int i = 0; i < 5; ++i)
			{
				H[i] += h[i];
			}
		}

		// To Hex
		std::ostringstream stringStream;
		for (auto h : H) {
			stringStream << std::hex << std::setfill('0') << std::setw(8) << h;
		}

		return stringStream.str();
	}

	//----------------------------------------------------------------------------------------------------
	std::string EESecureHashAlgorithm::SHA_2_224(const std::string& _input)
	{
		// Preprocess
		std::vector<std::array<unsigned char, 64>> chunks;
		Preprocess(_input, chunks);

		// Process each 512-bit chunk
		const unsigned int k[64] = {
			0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
			0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
			0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
			0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
			0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
			0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
			0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
			0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };
		unsigned int H[8] = { 0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939, 0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4 };
		for (std::array<unsigned char, 64>& chunk : chunks)
		{
			// Break chunk into sixteen 32-bit big-endian words
			std::vector<unsigned int> words;
			words.reserve(64);
			for (int i = 0; i < 64; i += 4)
			{
				words.push_back((chunk[i + 0] << 24) | (chunk[i + 1] << 16) | (chunk[i + 2] << 8) | (chunk[i + 3] << 0));
			}

			// Extend the sixteen 32-bit words into sixty-four 32-bit words
			for (int i = 16; i < 64; ++i)
			{
				unsigned int s0 = RotateR(words[i - 15], 7) ^ RotateR(words[i - 15], 18) ^ ShiftR(words[i - 15], 3);
				unsigned int s1 = RotateR(words[i - 2], 17) ^ RotateR(words[i - 2], 19) ^ ShiftR(words[i - 2], 10);
				words.push_back(words[i - 16] + s0 + words[i - 7] + s1);
			}

			// Main loop
			unsigned int h[8];
			memcpy((char*)h, (char*)H, sizeof(H));
			for (int i = 0; i < 64; ++i)
			{
				unsigned int s1 = RotateR(h[4], 6) ^ RotateR(h[4], 11) ^ RotateR(h[4], 25);
				unsigned int ch = (h[4] & h[5]) ^ ((~h[4]) & h[6]);
				unsigned int temp1 = h[7] + s1 + ch + k[i] + words[i];
				unsigned int s0 = RotateR(h[0], 2) ^ RotateR(h[0], 13) ^ RotateR(h[0], 22);
				unsigned int maj = (h[0] & h[1]) ^ (h[0] & h[2]) ^ (h[1] & h[2]);
				unsigned int temp2 = s0 + maj;

				h[7] = h[6];
				h[6] = h[5];
				h[5] = h[4];
				h[4] = h[3] + temp1;
				h[3] = h[2];
				h[2] = h[1];
				h[1] = h[0];
				h[0] = temp1 + temp2;
			}

			// Add
			for (int i = 0; i < 8; ++i)
			{
				H[i] += h[i];
			}
		}

		// To Hex
		std::ostringstream stringStream;
		for (int i = 0; i < 7; ++i) {
			stringStream << std::hex << std::setfill('0') << std::setw(8) << H[i];
		}

		return stringStream.str();
	}

	//----------------------------------------------------------------------------------------------------
	std::string EESecureHashAlgorithm::SHA_2_256(const std::string& _input)
	{
		// Preprocess
		std::vector<std::array<unsigned char, 64>> chunks;
		Preprocess(_input, chunks);

		// Process each 512-bit chunk
		const unsigned int k[64] = {
			0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
			0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
			0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
			0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
			0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
			0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
			0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
			0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };
		unsigned int H[8] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
		for (std::array<unsigned char, 64>& chunk : chunks)
		{
			// Break chunk into sixteen 32-bit big-endian words
			std::vector<unsigned int> words;
			words.reserve(64);
			for (int i = 0; i < 64; i += 4)
			{
				words.push_back((chunk[i + 0] << 24) | (chunk[i + 1] << 16) | (chunk[i + 2] << 8) | (chunk[i + 3] << 0));
			}

			// Extend the sixteen 32-bit words into sixty-four 32-bit words
			for (int i = 16; i < 64; ++i)
			{
				unsigned int s0 = RotateR(words[i - 15], 7) ^ RotateR(words[i - 15], 18) ^ ShiftR(words[i - 15], 3);
				unsigned int s1 = RotateR(words[i - 2], 17) ^ RotateR(words[i - 2], 19) ^ ShiftR(words[i - 2], 10);
				words.push_back(words[i - 16] + s0 + words[i - 7] + s1);
			}

			// Main loop
			unsigned int h[8];
			memcpy((char*)h, (char*)H, sizeof(H));
			for (int i = 0; i < 64; ++i)
			{
				unsigned int s1 = RotateR(h[4], 6) ^ RotateR(h[4], 11) ^ RotateR(h[4], 25);
				unsigned int ch = (h[4] & h[5]) ^ ((~h[4]) & h[6]);
				unsigned int temp1 = h[7] + s1 + ch + k[i] + words[i];
				unsigned int s0 = RotateR(h[0], 2) ^ RotateR(h[0], 13) ^ RotateR(h[0], 22);
				unsigned int maj = (h[0] & h[1]) ^ (h[0] & h[2]) ^ (h[1] & h[2]);
				unsigned int temp2 = s0 + maj;

				h[7] = h[6];
				h[6] = h[5];
				h[5] = h[4];
				h[4] = h[3] + temp1;
				h[3] = h[2];
				h[2] = h[1];
				h[1] = h[0];
				h[0] = temp1 + temp2;
			}

			// Add
			for (int i = 0; i < 8; ++i)
			{
				H[i] += h[i];
			}
		}

		// To Hex
		std::ostringstream stringStream;
		for (auto h : H) {
			stringStream << std::hex << std::setfill('0') << std::setw(8) << h;
		}

		return stringStream.str();
	}

	//----------------------------------------------------------------------------------------------------
	std::string EESecureHashAlgorithm::SHA_2_512(const std::string& _input)
	{
		// Preprocess
		std::vector<std::array<unsigned char, 128>> chunks;
		Preprocess(_input, chunks);

		// Process each 1024-bit chunk
		const unsigned long long int k[80] = {
			0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc, 0x3956c25bf348b538,
			0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242, 0x12835b0145706fbe,
			0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2, 0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235,
			0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
			0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5, 0x983e5152ee66dfab,
			0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
			0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed,
			0x53380d139d95b3df, 0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
			0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218,
			0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8, 0x19a4c116b8d2d0c8, 0x1e376c085141ab53,
			0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373,
			0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
			0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b, 0xca273eceea26619c,
			0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba, 0x0a637dc5a2c898a6,
			0x113f9804bef90dae, 0x1b710b35131c471b, 0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc,
			0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817 };
		unsigned long long int H[8] = { 0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1, 0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b, 0x5be0cd19137e2179 };
		for (std::array<unsigned char, 128>& chunk : chunks)
		{
			// Break chunk into sixteen 64-bit big-endian words
			std::vector<unsigned long long int> words;
			words.reserve(80);
			for (int i = 0; i < 128; i += 8)
			{
				words.push_back(((unsigned long long int)chunk[i + 0] << 56) | ((unsigned long long int)chunk[i + 1] << 48) | ((unsigned long long int)chunk[i + 2] << 40) | ((unsigned long long int)chunk[i + 3] << 32) | (chunk[i + 4] << 24) | (chunk[i + 5] << 16) | (chunk[i + 6] << 8) | (chunk[i + 7] << 0));
			}

			// Extend the sixteen 64-bit words into eighty 64-bit words
			for (int i = 16; i < 80; ++i)
			{
				unsigned long long int s0 = RotateR(words[i - 15], 1) ^ RotateR(words[i - 15], 8) ^ ShiftR(words[i - 15], 7);
				unsigned long long int s1 = RotateR(words[i - 2], 19) ^ RotateR(words[i - 2], 61) ^ ShiftR(words[i - 2], 6);
				words.push_back(words[i - 16] + s0 + words[i - 7] + s1);
			}

			// Main loop
			unsigned long long int h[8];
			memcpy((char*)h, (char*)H, sizeof(H));
			for (int i = 0; i < 80; ++i)
			{
				unsigned long long int s1 = RotateR(h[4], 14) ^ RotateR(h[4], 18) ^ RotateR(h[4], 41);
				unsigned long long int ch = (h[4] & h[5]) ^ ((~h[4]) & h[6]);
				unsigned long long int temp1 = h[7] + s1 + ch + k[i] + words[i];
				unsigned long long int s0 = RotateR(h[0], 28) ^ RotateR(h[0], 34) ^ RotateR(h[0], 39);
				unsigned long long int maj = (h[0] & h[1]) ^ (h[0] & h[2]) ^ (h[1] & h[2]);
				unsigned long long int temp2 = s0 + maj;

				h[7] = h[6];
				h[6] = h[5];
				h[5] = h[4];
				h[4] = h[3] + temp1;
				h[3] = h[2];
				h[2] = h[1];
				h[1] = h[0];
				h[0] = temp1 + temp2;
			}

			// Add
			for (int i = 0; i < 8; ++i)
			{
				H[i] += h[i];
			}
		}

		// To Hex
		std::ostringstream stringStream;
		for (auto h : H) {
			stringStream << std::hex << std::setfill('0') << std::setw(8) << h;
		}

		return stringStream.str();
	}

	//----------------------------------------------------------------------------------------------------
	void EESecureHashAlgorithm::Preprocess(const std::string& _input, std::vector<std::array<unsigned char, 64>>& _chunks)
	{
		std::string input(_input);

		// Append the bit '1' to the message
		input.push_back((char)0x80);
		// Append the bit '0' to the message
		int rest = input.size() % 64;
		input.append(rest <= 56 ? 56 - rest : 64 - rest + 56, (char)0x00);
		// Append the input length to the message
		unsigned long long int length = _input.size() << 3;
		if (EEIsBigEndian())
		{
			input.append((char*)&length, 8);
		}
		else
		{
			for (int offset = 56; offset >= 0; offset -= 8)
			{
				input.push_back((length >> offset) & 0xff);
			}
		}
		// Break message into 512-bit chunks
		_chunks.reserve(input.size() >> 6);
		for (unsigned long long chunkOffset = 0; chunkOffset < input.size(); chunkOffset += 64)
		{
			std::array<unsigned char, 64> chunk;
			memcpy(chunk.data(), input.substr((size_t)chunkOffset, 64).data(), 64); // X
			_chunks.push_back(chunk);
		}
	}

	//----------------------------------------------------------------------------------------------------
	void EESecureHashAlgorithm::Preprocess(const std::string& _input, std::vector<std::array<unsigned char, 128>>& _chunks)
	{
		std::string input(_input);

		// Append the bit '1' to the message
		input.push_back((char)0x80);
		// Append the bit '0' to the message
		long long int rest = input.size() % 128;
		input.append((size_t)(rest <= 112 ? 112 - rest : 128 - rest + 112), (char)0x00); // X
		// Append the input length to the message
		unsigned long long int length = _input.size() << 3; // X
		if (EEIsBigEndian())
		{
			input.append(8, (char)0x00); // X
			input.append((char*)&length, 8);
		}
		else
		{
			input.append(8, (char)0x00); // X
			for (int offset = 56; offset >= 0; offset -= 8)
			{
				input.push_back((length >> offset) & 0xff);
			}
		}
		// Break message into 1024-bit chunks
		_chunks.reserve(input.size() >> 7);
		for (unsigned long long chunkOffset = 0; chunkOffset < input.size(); chunkOffset += 128)
		{
			std::array<unsigned char, 128> chunk;
			memcpy(chunk.data(), input.substr((size_t)chunkOffset, 128).data(), 128); // X
			_chunks.push_back(chunk);
		}
	}
}
