#pragma warning(disable:4996) 

#include "RWM.h"

#define X3_NOT_INITIALIZED -536805375

typedef int32_t(__stdcall *t_x3_Dispatch)(OUT void *Function, IN uint32_t Type);
typedef void(__thiscall* tPrintChatMessage)(DWORD, LPCSTR, LPCSTR, BYTE, bool, bool, BYTE);
typedef short(__cdecl* tSendChatMessage)(BYTE, LPCSTR, LPCSTR);
static t_x3_Dispatch o_x3_Dispatch = nullptr;
static tSendChatMessage oSendChatMessage = nullptr;
static tPrintChatMessage oPrintChatMessage = reinterpret_cast<tPrintChatMessage>(0x00581AC0);
int playerIndex = 0;

bool bName = false;
std::vector<std::string> Split(const std::string &strText, const std::string &strDelimiter)
{
	std::vector<std::string> Tokens;
	size_t szPrevious = 0, szPosition = 0;
	do
	{
		szPosition = strText.find(strDelimiter, szPrevious);
		if (szPosition == std::string::npos) szPosition = strText.length();
		std::string token = strText.substr(szPrevious, szPosition - szPrevious);
		if (!token.empty()) Tokens.push_back(token);
		szPrevious = szPosition + strDelimiter.length();
	} while (szPosition < strText.length() && szPrevious < strText.length());
	return Tokens;
}
std::string Second;
short __cdecl hkSendChatMessage(byte bType, LPCSTR lpName, LPCSTR lpText)
{
	LPCSTR savedMessage = lpText;
	std::string splitMessage(savedMessage);


	std::vector<std::string> Message = Split(splitMessage, " ");
	std::string First = Message.at(0);
	Second = Message.at(1);

	if (_stricmp(First.c_str(), "/setname") == 0)
	{
		
		return 0;
	}
	return oSendChatMessage(bType, lpName, lpText);
}

void __stdcall DllMain()
{
	MessageBeep(1000);
	//oSendChatMessage = reinterpret_cast<tSendChatMessage>(RWM::Funcs::DetourFunction((void*)0x00B389F0, hkSendChatMessage, 5));
	
}
unsigned __stdcall MainThread(void* pReserved)
{
	while (true)
	{
		
			for (int i = 0; i < 16; i++)
			{
				DWORD nameoffs[4] = { (DWORD)(0x54 + 4), 0x58, 0x14, 0x64 };
				DWORD namebase = RWM::Funcs::GetDMA(0x11B5FC0, nameoffs, 4);


				char *name = "Aceen";
				if (namebase == NULL)break;
				memcpy((char*)namebase, name, strlen(name));

				char *level = "ICONTAG_USER_GRADE_26";
				if (0x01018C60 == NULL)break;
				memcpy((char*)0x01018C60, level, strlen(level));
			

			}
	}
	
}
__declspec(dllexport) int32_t __stdcall x3_1(void *FunctionAddress, uint32_t Type)
{
	if (o_x3_Dispatch == nullptr)
	{
		std::string ModulePath;
		ModulePath.resize(MAX_PATH);

		if (!GetModuleFileNameA(NULL, const_cast<LPSTR>(ModulePath.data()), MAX_PATH))
		{
			MessageBoxA(0, "GetModuleFileNameA failed!", "Error", 0);
			return X3_NOT_INITIALIZED;
		}
		HMODULE hX3 = LoadLibraryW(L"XIGNCODE\\x3.dummy");
		if (hX3 == nullptr)
		{
			MessageBoxA(0, "LoadLibraryA failed!", "Error", 0);
			return X3_NOT_INITIALIZED;
		}

		o_x3_Dispatch = reinterpret_cast<t_x3_Dispatch>(GetProcAddress(hX3, reinterpret_cast<LPCSTR>(1)));
		if (o_x3_Dispatch == nullptr)
		{
			MessageBoxA(0, "GetProcAddress failed!", "Error", 0);
			return X3_NOT_INITIALIZED;
		}

		if (!_beginthreadex(nullptr, 0, &MainThread, nullptr, 0, nullptr))
		{
			char Buffer[512];
			RtlZeroMemory(Buffer, 512);
			strcpy(Buffer, "Thread creation failed\nError: ");
			strerror_s(Buffer + strlen(Buffer), 512 - strlen(Buffer), errno);

			MessageBoxA(NULL, Buffer, "Error", 0);
		}

		DllMain();

	}

	return o_x3_Dispatch(FunctionAddress, Type);
}