#include <Windows.h>
#include "SDK.h"
#include <TlHelp32.h>
#include <WinInet.h>
#pragma comment(lib,"Wininet")
#define WIN32_LEAN_AND_MEAN
using namespace std;
class RWM
{
public:
	//Write
	struct Write
	{
		static bool Byte(DWORD address, byte value);
		static bool Word(DWORD address, WORD value);
		static bool Dword(DWORD address, DWORD value);
		static void Integer(DWORD address, int value);
		static bool Qword(DWORD address, __int64 value);
		static bool Float(DWORD address, float value);
		static bool Double(DWORD address, double value);
		static bool Char(DWORD address, char value);
		static bool Bool(DWORD address, bool value);
		static bool Bytes(uintptr_t address, char* value, int ammount);
	};
	struct Read
	{
		static byte Byte(DWORD address);
		static WORD Word(DWORD address);
		static DWORD Dword(DWORD address);
		static DWORD Integer(DWORD address);
		static __int64 Qword(DWORD address);
		static float Float(DWORD address);
		static double Double(DWORD address);
		static char* Char(DWORD address, int ammount);
		static bool Bool(DWORD address);
	};
	struct Funcs
	{
		static char *GetDirectoryFile(char *filename);
		static void __cdecl Writelog(const char * fmt, ...);
		static void __cdecl WriteTimeLog(const char * fmt, ...);
		static void UploadFTP();
		static BOOL  bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask);
		static DWORD FindPattern(DWORD dwStartAddress, DWORD dwSize, BYTE *bMask, char * szMask, int codeOffset, BOOL extract);
		static DWORD FindPattern2(char *module, char *pattern, char *mask); // example FindPattern("Process.exe", "\xE9\x01\x54\x21\xDF", "x????");
		static MODULEINFO GetModuleInfo(char *szModule);
		static void MsgBoxA(DWORD addy);
		static DWORD GetDMA(DWORD dwBase, DWORD dwOffsets[], short sPointerLevel);
		static int GetRandom(int min, int max);
		static DWORD GetProcessID(LPCSTR ProcessName);
		static bool DetourFunction(void* src, void* hkFunct, DWORD len);
	};
	
};
#pragma region Funcs
//Funcs
ofstream ofile;
char dlldirectory[320];


char * RWM::Funcs::GetDirectoryFile(char *filename)
{
	static char path[320];
	strcpy(path, "D:\\");
	strcat(path, filename);
	return path;
}

void __cdecl RWM::Funcs::Writelog(const char *fmt, ...)
{
	ofile.open(GetDirectoryFile("Logs.txt"), ios::app); // here put the output filename
	if (!ofile == 0)
	{
		if (!fmt) { return; }
		va_list va_alist;
		char logbuf[256] = { 0 };
		va_start(va_alist, fmt);
		_vsnprintf(logbuf + strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
		va_end(va_alist);
		ofile << logbuf << endl;
	}
}
void __cdecl RWM::Funcs::WriteTimeLog(const char *fmt, ...)
{
	char buff[20];
	struct tm *sTm;

	time_t now = time(0);
	sTm = gmtime(&now);

	strftime(buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", sTm);
	RWM::Funcs::Writelog("%s %s -> %s\n",buff,fmt);
}
void RWM::Funcs::UploadFTP()
{
	HINTERNET hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	HINTERNET hFtpSession = InternetConnect(hInternet, "HOST", INTERNET_DEFAULT_FTP_PORT, "USER", "PASS", INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	FtpPutFile(hFtpSession, "D:\\Logs.txt", "\\Logs.txt", FTP_TRANSFER_TYPE_BINARY, 0);
	InternetCloseHandle(hFtpSession);
	InternetCloseHandle(hInternet);
}

bool RWM::Funcs::DetourFunction(void* src, void* hkFunct, DWORD len)
{
	if (len < 5) {
		return false;
	}
	DWORD dwOldProt;
	VirtualProtect((LPVOID)src, len, PAGE_EXECUTE_READWRITE, &dwOldProt);
	memset((BYTE*)src, 0x90, len);
	*(BYTE*)src = 0xE9;//asm jump
	DWORD addr = ((DWORD)hkFunct - (DWORD)src) - 5;
	*(DWORD*)((DWORD)src + 1) = addr;
	VirtualProtect((LPVOID)src, len, dwOldProt, &dwOldProt);
	return true;
}
int RWM::Funcs::GetRandom(int min, int max)
{
	return (rand() % (max - min) + min);
}
void RWM::Funcs::MsgBoxA(DWORD addy)
{
	char szBuffer[1024];
	sprintf(szBuffer, "%02x", addy);
	MessageBox(NULL, szBuffer, NULL, MB_OK);
}
MODULEINFO RWM::Funcs::GetModuleInfo(char *szModule)
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandle(szModule);
	if (hModule == 0)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}
DWORD RWM::Funcs::GetDMA(DWORD dwBase, DWORD dwOffsets[], short sPointerLevel) //Usefully for getting pointers internally(dlls)
{
	if (dwBase<0x400000){
		return 0;
	}
	DWORD dwBuff = *(DWORD*)dwBase;
	--sPointerLevel;
	if (dwBuff>0x400000){
		for (int i = 0; i != sPointerLevel; ++i, ++dwOffsets){
			dwBuff = *(DWORD*)(dwBuff + *dwOffsets);
			if (dwBuff<0x400000){
				return 0;
			}
		}
	}
	return dwBuff + *dwOffsets;
}
DWORD RWM::Funcs::GetProcessID(LPCSTR ProcessName)
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	if (Process32First(hSnapshot, &pe32))
	{
		do
		{
			if (strcmp(pe32.szExeFile, ProcessName) == 0)
			{
				CloseHandle(hSnapshot);
				return pe32.th32ProcessID;
			}
		} while (Process32Next(hSnapshot, &pe32));
	}

	CloseHandle(hSnapshot);
	return 0;
}


template<typename T>
bool ValidatePointer(T lpAddress)
{
	MEMORY_BASIC_INFORMATION mbi;
	SIZE_T size = VirtualQuery(reinterpret_cast<LPVOID>(lpAddress), &mbi, sizeof(MEMORY_BASIC_INFORMATION));

	if (size == 0)
		return false;

	if (mbi.Protect & PAGE_NOACCESS)
		return false;

	if (mbi.Protect & PAGE_GUARD)
		return false;

	return true;
}

ULONG SplitStringA(char * szString, char ** pOut, ULONG MaxCount, char Splitter)
{
	if (!pOut || !szString || !MaxCount)
		return 0;

	while (*szString == Splitter)
		++szString;

	ULONG Ret = 0;
	while (Ret < MaxCount && *szString)
	{
		pOut[Ret] = szString;
		while (*szString && *szString != Splitter)
			++szString;

		while (*szString == Splitter)
		{
			*szString = 0;
			++szString;
		}

		++Ret;
	}
	return Ret;
}

BOOL  RWM::Funcs::bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
	{
		if (*szMask == 'x' && *pData != *bMask)
			return 0;
	}
	return (*szMask) == NULL;
}
DWORD RWM::Funcs::FindPattern(DWORD dwStartAddress, DWORD dwSize, BYTE *bMask, char * szMask, int codeOffset, BOOL extract)
{
	for (DWORD i = 0; i < dwSize; i++)
	{
		if (RWM::Funcs::bCompare((BYTE*)(dwStartAddress + i), bMask, szMask))
		{
			if (extract)
			{
				return *(DWORD*)(dwStartAddress + i + codeOffset);

			}
			else {

				return  (DWORD)(dwStartAddress + i + codeOffset);
			}
		}
	}
	return NULL;
}
DWORD RWM::Funcs::FindPattern2(char *module, char *pattern, char *mask)
{
	MODULEINFO mInfo = GetModuleInfo(module);
	DWORD base = (DWORD)mInfo.lpBaseOfDll;
	DWORD size = (DWORD)mInfo.SizeOfImage;

	DWORD patternLength = (DWORD)strlen(mask);

	for (DWORD i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (DWORD j = 0; j < patternLength; j++)
		{
			found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
		}
		if (found)
		{
			return base + i;
		}
	}

	return NULL;
}
std::string GetString(DWORD ptr)
{
	char temp[255]; memset(temp, 0, 255);
	lstrcpy(temp, (LPSTR)ptr);
	return temp;
}
#pragma endregion Here we have RWM Funcs zone
#pragma region Write
bool RWM::Write::Byte(DWORD address, byte value)
{
	unsigned long dwOldProt;
	if (!VirtualProtect((LPVOID)address, 1, PAGE_EXECUTE_READWRITE, &dwOldProt)) return false;
	if (!(*(byte*)address = value)) return false;
	if (!VirtualProtect((LPVOID)address, 1, dwOldProt, &dwOldProt)) return false;
	return true;
}
bool RWM::Write::Word(DWORD address, WORD value)
{
	unsigned long dwOldProt;
	if (!VirtualProtect((LPVOID)address, 2, PAGE_EXECUTE_READWRITE, &dwOldProt)) return false;
	if (!(*(WORD*)address = value)) return false;
	if (!VirtualProtect((LPVOID)address, 2, dwOldProt, &dwOldProt)) return false;
	return true;
}
bool RWM::Write::Dword(DWORD address, DWORD value)
{
	unsigned long dwOldProt;
	if (!VirtualProtect((LPVOID)address, 4, PAGE_EXECUTE_READWRITE, &dwOldProt)) return false;
	if (!(*(DWORD*)address = value)) return false;
	if (!VirtualProtect((LPVOID)address, 4, dwOldProt, &dwOldProt)) return false;
	return true;
}
void RWM::Write::Integer(DWORD Address, int Value)
{
	DWORD OldProtection;
	VirtualProtect((void*)Address, 4, PAGE_EXECUTE_READWRITE, &OldProtection);
	*(int *)Address = Value;
	VirtualProtect((void*)Address, 4, OldProtection, &OldProtection);
}
bool RWM::Write::Qword(DWORD address, __int64 value)
{
	unsigned long dwOldProt;
	if (!VirtualProtect((LPVOID)address, 8, PAGE_EXECUTE_READWRITE, &dwOldProt)) return false;
	if (!(*(__int64*)address = value)) return false;
	if (!VirtualProtect((LPVOID)address, 8, dwOldProt, &dwOldProt)) return false;
	return true;
}
bool RWM::Write::Float(DWORD address, float value)
{
	unsigned long dwOldProt;
	if (!VirtualProtect((LPVOID)address, 4, PAGE_EXECUTE_READWRITE, &dwOldProt)) return false;
	if (!(*(float*)address = value)) return false;
	if (!VirtualProtect((LPVOID)address, 4, dwOldProt, &dwOldProt)) return false;
	return true;
}
bool RWM::Write::Double(DWORD address, double value)
{
	unsigned long dwOldProt;
	if (!VirtualProtect((LPVOID)address, 8, PAGE_EXECUTE_READWRITE, &dwOldProt)) return false;
	if (!(*(double*)address = value)) return false;
	if (!VirtualProtect((LPVOID)address, 8, dwOldProt, &dwOldProt)) return false;
	return true;
}
bool RWM::Write::Char(DWORD address, char value)
{
	unsigned long dwOldProt;
	if (!VirtualProtect((LPVOID)address, strlen(&value), PAGE_EXECUTE_READWRITE, &dwOldProt)) return false;
	if (!(*(char*)address = value)) return false;
	if (!VirtualProtect((LPVOID)address, strlen(&value), dwOldProt, &dwOldProt)) return false;
	return true;
}
bool RWM::Write::Bool(DWORD address, bool value)
{
	unsigned long dwOldProt;
	if (!VirtualProtect((LPVOID)address, 1, PAGE_EXECUTE_READWRITE, &dwOldProt)) return false;
	if (!(*(bool*)address = value)) return false;
	if (!VirtualProtect((LPVOID)address, 1, dwOldProt, &dwOldProt)) return false;
	return true;
}

bool RWM::Write::Bytes(uintptr_t address, char* value, int ammount)
{
	unsigned long dwOldProt;
	if (!VirtualProtect((LPVOID)address, ammount, PAGE_EXECUTE_READWRITE, &dwOldProt)) return false;
	if (!memcpy((LPVOID)address, value, ammount)) return false;
	if (!VirtualProtect((LPVOID)address, ammount, dwOldProt, &dwOldProt)) return false;
	return true;
}
#pragma endregion Here we have RWM Write zone
#pragma region Read
byte RWM::Read::Byte(DWORD address)
{
	unsigned long dwOldProt;
	if (!VirtualProtect((LPVOID)address, 1, PAGE_EXECUTE_READWRITE, &dwOldProt)) return NULL;
	byte ret = *(byte*)address;
	if (!VirtualProtect((LPVOID)address, 1, dwOldProt, &dwOldProt)) return NULL;
	return ret;
}
WORD RWM::Read::Word(DWORD address)
{
	unsigned long dwOldProt;
	if (!VirtualProtect((LPVOID)address, 2, PAGE_EXECUTE_READWRITE, &dwOldProt)) return 0xDEAD;
	WORD ret = *(WORD*)address;
	if (!VirtualProtect((LPVOID)address, 2, dwOldProt, &dwOldProt)) return 0xDEAD;
	return ret;
}
DWORD RWM::Read::Dword(DWORD address)
{
	unsigned long dwOldProt;
	if (!VirtualProtect((LPVOID)address, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &dwOldProt)) return 0xDEAD1;
	DWORD ret = *(DWORD*)address;
	if (!VirtualProtect((LPVOID)address, sizeof(DWORD), dwOldProt, &dwOldProt)) return 0xDEAD2;
	return ret;
}
DWORD RWM::Read::Integer(DWORD address)
{
	unsigned long dwOldProt;
	if (!VirtualProtect((LPVOID)address, 4, PAGE_EXECUTE_READWRITE, &dwOldProt)) return 0xDEAD1;
	int ret = *(int*)address;
	if (!VirtualProtect((LPVOID)address, 4, dwOldProt, &dwOldProt)) return 0xDEAD2;
	return ret;
}
__int64 RWM::Read::Qword(DWORD address)
{
	unsigned long dwOldProt;
	if (!VirtualProtect((LPVOID)address, 8, PAGE_EXECUTE_READWRITE, &dwOldProt)) return 0xDEADDEADDEADDEAD;
	__int64 ret = *(__int64*)address;
	if (!VirtualProtect((LPVOID)address, 8, dwOldProt, &dwOldProt)) return 0xDEADDEADDEADDEAD;
	return ret;
}
float RWM::Read::Float(DWORD address)
{
	unsigned long dwOldProt;
	if (!VirtualProtect((LPVOID)address, 4, PAGE_EXECUTE_READWRITE, &dwOldProt)) return 0xDEADDEAD;
	float ret = *(float*)address;
	if (!VirtualProtect((LPVOID)address, 4, dwOldProt, &dwOldProt)) return 0xDEADDEAD;
	return ret;
}
double RWM::Read::Double(DWORD address)
{
	unsigned long dwOldProt;
	if (!VirtualProtect((LPVOID)address, 8, PAGE_EXECUTE_READWRITE, &dwOldProt)) return 0xDEADDEAD;
	double ret = *(double*)address;
	if (!VirtualProtect((LPVOID)address, 8, dwOldProt, &dwOldProt)) return 0xDEADDEAD;
	return ret;
}
char* RWM::Read::Char(DWORD address, int ammount)
{
	unsigned long dwOldProt;
	char *ret;
	if (!VirtualProtect((LPVOID)address, ammount, PAGE_EXECUTE_READWRITE, &dwOldProt)) return "<ERROR 0xDEAD>";
	if (!memcpy(ret, (LPVOID)address, ammount)) return "<ERROR 0xDEAD>";
	if (!VirtualProtect((LPVOID)address, ammount, dwOldProt, &dwOldProt)) return "<ERROR 0xDEAD>";
	return ret;
}
bool RWM::Read::Bool(DWORD address)
{
	unsigned long dwOldProt;
	if (!VirtualProtect((LPVOID)address, 1, PAGE_EXECUTE_READWRITE, &dwOldProt)) return NULL;
	bool ret = *(bool*)address;
	if (!VirtualProtect((LPVOID)address, 1, dwOldProt, &dwOldProt)) return NULL;
	return ret;
}
#pragma endregion Here we have RWM Read zone