#pragma once

#define DEAD 0xDEADCAFE
#define Randomize() srand(time(0))
#define RAND(min, max) (rand() % (max - min) + min)
#define MEM_WRITE (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)

// IsValidReadPtr/WritePtr by Broihon
bool IsValidReadPtr(Pointer addr);
bool IsValidWritePtr(Pointer addr);
template <class T> inline bool Write(Address addr, T value)
{
	*(T*)addr = value;
	return true;
}

template <class T> inline T Read(Address addr, T& out)
{
	out = *(T*)addr;
	return out;
}

template <class T> inline T Read(Address addr)
{
	return *(T*)addr;
}

template <class T> bool sWrite(Address addr, T value)
{
	if (IsValidWritePtr((Pointer)addr)) return Write<T>(addr, value);

	Protection oldProt;
	if (!VirtualProtect((Pointer)addr, sizeof(T), MEM_WRITE, &oldProt)) return false;
	bool ret = Write<T>(addr, value);
	if (!VirtualProtect((Pointer)addr, sizeof(T), oldProt, &oldProt)) return false;
	return ret;
}

template <class T> T sRead(Address addr, T& out)
{
	if (IsValidReadPtr((Pointer)addr)) return Read<T>(addr, out);

	Protection oldProt;
	if (!VirtualProtect((Pointer)addr, sizeof(T), MEM_WRITE, &oldProt)) return DEAD;
	Read<T>(addr, out);
	if (!VirtualProtect((Pointer)addr, sizeof(T), oldProt, &oldProt)) return DEAD;
	return out;
}

template <class T> T sRead(Address addr)
{
	T out;
	if (IsValidReadPtr((Pointer)addr)) return Read<T>(addr);

	Protection oldProt;
	if (!VirtualProtect((Pointer)addr, sizeof(T), MEM_WRITE, &oldProt)) return DEAD;
	Read<T>(addr, out);
	if (!VirtualProtect((Pointer)addr, sizeof(T), oldProt, &oldProt)) return DEAD;
	return out;
}



bool sCopyMemory(Pointer dest, Pointer source, Size size); 

Address CalcPtr(Address base, Offset offs[], Count level);
Address sCalcPtr(Address base, Offset offs[], Count level);
Address FindPattern(char* module, LPCSTR pattern, LPCSTR mask);
Address Detour(Pointer src, Pointer hkFunct, Size len);
void ShowAddress(Address addy, LPCSTR name);
