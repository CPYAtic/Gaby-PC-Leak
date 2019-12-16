#pragma once
#include "RWM.h"

template <class T> inline T RWM::Read(uint32_t addr)
{
	return *(T*)addr;
}
template <class T> inline T RWM::Read(uint32_t addr, T& out)
{
	out = *(T*)addr;
	return out;
}

bool RWM::IsValidReadPtr(LPVOID addr)
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
bool RWM::IsValidWritePtr(LPVOID addr)
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

uint32_t RWM::Detour(LPVOID source, LPVOID hkFunc, size_t len)
{
	if (len < 5 || len > 250)
		return 0;

	DWORD oldProtection;
	auto func = new uint8_t[len + 5];

	if (func == nullptr)
		return 0;

	VirtualProtect(source, len, PAGE_EXECUTE_READWRITE, &oldProtection);

	memcpy(&func[0], source, len);
	*reinterpret_cast<uint8_t*>	(&func[len]) = 0xE9;
	*reinterpret_cast<uint32_t*>(&func[len] + 1) = (uint32_t)source - (uint32_t)&func[len];

	memset((uint8_t*)source, 0x90, len);

	*reinterpret_cast<uint8_t*>(source) = 0xE9;
	*reinterpret_cast<uint32_t*>((uint32_t)source + 1) = (uint32_t)hkFunc - (uint32_t)source - 5;

	VirtualProtect(source, len, oldProtection, &oldProtection);
	VirtualProtect((LPVOID)&func[0], len + 5, PAGE_EXECUTE_READWRITE, &oldProtection); // FOR WHAT THIS SHOULD BE CHANGE HERE??! TF

	return (uint32_t)&func[0];
}

uint32_t RWM::CalcPtr(uint32_t base, std::vector<uint16_t> offsets)
{
	auto ptr = base;

	if (ptr == 0)
		return 0;

	for (auto const &offset : offsets)
	{
		ptr = Read<uint32_t>(ptr);

		if (!ptr)
			return 0;

		ptr += offset;
	}

	return ptr;
}

uint32_t RWM::sCalcPtr(uint32_t base, std::vector<uint16_t> offsets)
{
	auto ptr = base;

	for (auto const &offset : offsets)
	{
		if (!ptr)
			return 0;

		ptr = Read<uint32_t>(ptr) + offset;
	}

	if (!IsValidReadPtr((LPVOID)ptr))
		return 0;

	return ptr;
}

uint32_t RWM::FindPattern(char* module, LPCSTR pattern, LPCSTR mask)
{
	MODULEINFO mInfo;
	HMODULE hMod = GetModuleHandleA(module);
	GetModuleInformation(GetCurrentProcess(), hMod, &mInfo, sizeof(MODULEINFO));

	char* mod = (char*)mInfo.lpBaseOfDll;
	size_t len = strlen(mask);

	uint32_t j = 0;
	for (uint32_t i = 0; i < mInfo.SizeOfImage - len; i++)
	{
		if (mask[j] == '?' || pattern[j] == mod[i])
		{
			if (++j == len)
				return (uint32_t)mod + i - len + 1;
		}
		else
			j = 0;
	}

	return 0;
}

bool RWM::sCopyMemory(LPVOID destination, LPVOID source, size_t size)
{
	DWORD oldProtection;

	if (!VirtualProtect(destination, size, MEM_WRITE, &oldProtection))
		return false;

	memcpy(destination, source, size);

	if (!VirtualProtect(destination, size, oldProtection, &oldProtection))
		return false;

	return true;
}

uint32_t RWM::DetourWINAPI(std::string module, std::string functionName, LPVOID hkFunc, size_t lenght)
{
	if (module.empty())
	{
		//SetLastError();
		return 0;
	}
	if (functionName.empty())
	{
		//SetLastError();
		return 0;
	}
	if (hkFunc == NULL)
	{
		//SetLastError();
		return 0;
	}
	if (lenght < 5)
	{
		//SetLastError();
		return 0;
	}

	HMODULE hModule = GetModuleHandleA(module.c_str());

	if (!hModule)
	{
		//SetLastError();
		return 0;
	}

	FARPROC function = GetProcAddress(hModule, functionName.c_str());

	if (!function)
	{
		//SetLastError();
		return 0;
	}

	auto address = Detour(function, hkFunc, lenght);

	if (address == 0)
	{
		//SetLastError();
		return 0;
	}

	return address;
}