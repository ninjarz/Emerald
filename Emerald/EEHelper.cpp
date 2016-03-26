#include "EEHelper.h"

#include <Windows.h>
#include <vector>
#include <fstream>

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//----------------------------------------------------------------------------------------------------
	bool EEIsBigEndian()
	{
		unsigned short test = 0x1234;

		if (*((unsigned char*)&test) == 0x12)
			return true;
		else
			return false;
	}

	//----------------------------------------------------------------------------------------------------
	void ResumeSpeed()
	{
		WORD* byte_pos = (WORD*)::timeGetTime;
		if (*byte_pos == 0x3D83) // cmp [83,3D] 指令  
			return;

		HANDLE hMyHandle = ::OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, ::GetCurrentProcessId());
		if (hMyHandle)
		{
			BYTE cmp_[7] = { 0x83, 0x3D, 0x14, 0x00, 0xB3, 0x76, 0x00 };
			::WriteProcessMemory(hMyHandle, (BYTE*)byte_pos, cmp_, 7, 0);
			::CloseHandle(hMyHandle);
		}
	}

	//----------------------------------------------------------------------------------------------------
	std::string EEOpenFile()
	{
		std::string filePos;
		filePos.resize(100);
		OPENFILENAMEA ofn = { 0 };
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = "文本文件(*.txt)\0*.txt\0CPP(*.cpp)\0*.cpp\0所有文件(*.*)\0*.*\0";
		ofn.lpstrInitialDir = NULL;
		ofn.lpstrFile = &filePos[0];
		ofn.nMaxFile = 100;
		ofn.nFilterIndex = 0;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

		int result = GetOpenFileNameA(&ofn);
		if (result) 
		{
			//return filePos;
		}

		return filePos;
	}

	//----------------------------------------------------------------------------------------------------
	std::wstring AnsiToUnicode(const char* _str)
	{
		int len = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _str, -1, NULL, 0);
		if (len <= 1)
		{
			return std::wstring();
		}
		std::wstring result;
		// memo: len - 1
		result.resize(len);
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _str, -1, &result[0], len);
		return result;
	}

	//----------------------------------------------------------------------------------------------------
	std::wstring AnsiToUnicode(const std::string& _str)
	{
		return AnsiToUnicode(_str.data());
	}

	//----------------------------------------------------------------------------------------------------
	std::string UnicodeToAnsi(const wchar_t* _str)
	{
		int len = WideCharToMultiByte(CP_ACP, 0, _str, -1, NULL, 0, NULL, NULL);
		if (len <= 1)
		{
			return std::string();
		}
		std::string result;
		result.resize(len);
		WideCharToMultiByte(CP_ACP, 0, _str, -1, &result[0], len, NULL, NULL);
		return result;
	}

	//----------------------------------------------------------------------------------------------------
	std::string UnicodeToAnsi(const std::wstring& _str)
	{
		return UnicodeToAnsi(_str.data());
	}

	//----------------------------------------------------------------------------------------------------
	std::wstring CopyEnvironmentVariable(const std::wstring& variable)
	{
		DWORD length = ::GetEnvironmentVariableW(variable.c_str(), 0, 0);
		if (!length)
			return std::wstring();
		std::vector<wchar_t> buffer(length);
		if (!GetEnvironmentVariableW(variable.c_str(), &buffer[0], buffer.size()) || !buffer[0])
			return std::wstring();
		return &buffer[0];
	}

	//----------------------------------------------------------------------------------------------------
	void TXTFilter(const std::string& _fin, const std::string& _fout)
	{
		std::ifstream fin(_fin);
		std::ofstream fout(_fout);
		if (fin.is_open() && fout.is_open())
		{
			while (!fin.eof())
			{
				std::string tmp;
				fin >> tmp >> tmp >> tmp;
				getline(fin, tmp);
				fout << trim(tmp) << '\n';
			}
			fin.close();
			fout.close();
		}
	}

	//----------------------------------------------------------------------------------------------------
	std::string& trim(std::string& _str)
	{
		_str.erase(0, _str.find_first_not_of(" "));
		_str.erase(_str.find_last_not_of(" ") + 1);
		return _str;
	}

	//----------------------------------------------------------------------------------------------------
	unsigned int RotateL(const unsigned int& value, const unsigned int& shift)
	{
		return (value << shift) | (value >> (32 - shift));
	}

	//----------------------------------------------------------------------------------------------------
	unsigned long long int RotateL(const unsigned long long int& value, const unsigned long long int& shift)
	{
		return (value << shift) | (value >> (64 - shift));
	}

	//----------------------------------------------------------------------------------------------------
	unsigned int RotateR(const unsigned int& value, const unsigned int& shift)
	{
		return (value << (32 - shift)) | (value >> shift);
	}

	//----------------------------------------------------------------------------------------------------
	unsigned long long int RotateR(const unsigned long long int& value, const unsigned long long int& shift)
	{
		return (value << (64 - shift)) | (value >> shift);
	}

	//----------------------------------------------------------------------------------------------------
	unsigned int ShiftR(const unsigned int& value, const unsigned int& shift)
	{
		return value >> shift;
	}

	//----------------------------------------------------------------------------------------------------
	unsigned long long int ShiftR(const unsigned long long int& value, const unsigned long long int& shift)
	{
		return value >> shift;
	}
}