#include "SDK.h"

#pragma region ////////////////////////FUNCTIONS////////////////////////

bool IsValidReadPtr(Pointer addr)
{
	if (!addr)
		return false;

	MEMORY_BASIC_INFORMATION MBI{ 0 };
	if (!VirtualQuery(addr, &MBI, sizeof(MEMORY_BASIC_INFORMATION)))
		return false;

	if (MBI.State == MEM_COMMIT && !(MBI.Protect & PAGE_NOACCESS))
		return true;
	return false;
}
bool IsValidReadPtr2(LPCVOID addr)
{
	if (!addr)
		return false;

	MEMORY_BASIC_INFORMATION MBI{ 0 };
	if (!VirtualQuery(addr, &MBI, sizeof(MEMORY_BASIC_INFORMATION)))
		return false;

	if (MBI.State == MEM_COMMIT && !(MBI.Protect & PAGE_NOACCESS))
		return true;
	return false;
}

bool IsValidWritePtr(Pointer addr)
{
	if (!addr)
		return false;

	MEMORY_BASIC_INFORMATION MBI{ 0 };
	if (!VirtualQuery(addr, &MBI, sizeof(MEMORY_BASIC_INFORMATION)))
		return false;

	if (MBI.State == MEM_COMMIT && (MBI.Protect & MEM_WRITE))
		return true;
	return false;
}

Address Detour(Pointer src, Pointer hkFunct, Size len)
{
	if (len < 5 || len > 250) return DEAD;

	Address dwOldProt;
	Byte* funct = new Byte[len + 5];
	if (funct == nullptr) return DEAD;

	VirtualProtect((Pointer)src, len, PAGE_EXECUTE_READWRITE, &dwOldProt);

	memcpy(&funct[0], src, len);
	*(Byte*)(&funct[len]) = 0xE9;
	*(Address*)(&funct[len] + 1) = (Address)src - (Address)&funct[len];

	memset((Byte*)src, 0x90, len);

	*(Byte*)src = 0xE9;
	Address addr = (Address)hkFunct - (Address)src - 5;
	*(Address*)((Address)src + 1) = addr;

	VirtualProtect((Pointer)src, len, dwOldProt, &dwOldProt);

	VirtualProtect((Pointer)&funct[0], len + 5, PAGE_EXECUTE_READWRITE, &dwOldProt);
	return (Address)&funct[0];
}

void ShowAddress(Address addy, LPCSTR name)
{
	char szBuffer[1024];
	sprintf(szBuffer, "%s = %02X", name, addy);
	MessageBoxA(0, szBuffer, "Debug info", MB_OK);
}

Address CalcPtr(Address base, Offset offs[], Count level)
{
	Address Ptr = base;
	for (Count i = 0; i < level; ++i)
	{
		if (Ptr < 0x00400000) return 0;
		Ptr = Read<Address>(Ptr) + offs[i];
	}
	return Ptr;
}
float random_float(float min, float max)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(min, max);

	return dis(gen);
}
int random_int(int min, int max)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);

	return dis(gen);
}
Address sCalcPtr(Address base, Offset offs[], Count level)
{
	Address Ptr = base;
	for (Count i = 0; i < level; ++i)
	{
		if (Ptr == 0) return 0;
		Ptr = sRead<Address>(Ptr) + offs[i];
	}
	if (!IsValidReadPtr((Pointer)Ptr)) return 0;
	return Ptr;
}

Address FindPattern(char* module, LPCSTR pattern, LPCSTR mask)
{
	MODULEINFO mInfo;
	HMODULE hMod = GetModuleHandleA(module);


	GetModuleInformation(GetCurrentProcess(), hMod, &mInfo, sizeof(MODULEINFO));

	char* mod = (char*)mInfo.lpBaseOfDll;
	Size len = strlen(mask);

	Count j = 0;
	for (Count i = 0; i < mInfo.SizeOfImage - len; i++)
	{
		if (mask[j] == '?' || pattern[j] == mod[i])
		{
			if (++j == len) return (Address)mod + i - len + 1;
		}
		else j = 0;
	}

	return DEAD;
}

#pragma endregion

#pragma region /////////////////////////MEMORY//////////////////////////

bool sCopyMemory(LPVOID destination, LPVOID source, size_t size)
{
	DWORD oldProtection;

	if (!VirtualProtect(destination, size, MEM_WRITE, &oldProtection))
		return false;

	memcpy(destination, source, size);

	if (!VirtualProtect(destination, size, oldProtection, &oldProtection))
		return false;

	return true;
}
bool WriteBytes(uintptr_t address, const char* value, int ammount)
{
	unsigned long dwOldProt;
	if (!VirtualProtect((LPVOID)address, ammount, PAGE_EXECUTE_READWRITE, &dwOldProt)) return false;
	if (!memcpy((LPVOID)address, value, ammount)) return false;
	if (!VirtualProtect((LPVOID)address, ammount, dwOldProt, &dwOldProt)) return false;
	return true;
}

#pragma endregion