#include "EESecureHashAlgorithm.h"
#include "EEHelper.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EESecureHashAlgorithm
	//----------------------------------------------------------------------------------------------------
	std::string EESecureHashAlgorithm::SHA_1(const std::string& _input)
	{
		std::string input(_input);
		std::string output;

		// Append the bit '1' to the message
		input.push_back((char)0x80);
		// Append the bit '0' to the message
		int rest = input.size() % 64;
		input.append(std::string(rest <= 56 ? 56 - rest : 64 - rest + 56, (char)0x00));
		// Append input length
		unsigned long long int length = _input.size() << 3;
		if (!EEIsBigEndian())
		{
			//length = 
		}

		return output;
	}
}
