#pragma once

#define DEAD 0xDEADCAFE
#define Randomize() srand(time(0))
#define RAND(min, max) (rand() % (max - min) + min)
#define MEM_WRITE (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)

// IsValidReadPtr/WritePtr by Broihon
bool IsValidReadPtr(Pointer addr);
bool IsValidWritePtr(Pointer addr);
bool IsValidReadPtr2(LPCVOID addr);



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

bool sCopyMemory(LPVOID dest, LPVOID source, size_t size);
bool WriteBytes(uintptr_t address, const char* value, int ammount);
float random_float(float min, float max);
int random_int(int min, int max);
Address CalcPtr(Address base, Offset offs[], Count level);
Address sCalcPtr(Address base, Offset offs[], Count level);
Address FindPattern(char* module, LPCSTR pattern, LPCSTR mask);
Address Detour(Pointer src, Pointer hkFunct, Size len);
void ShowAddress(Address addy, LPCSTR name);



typedef struct
{
	const INT len;
	PBYTE function;
} datareturn;


template<typename t>
datareturn DetourDelete(t& dst)
{
	BYTE *jmp = (BYTE *)dst;
	datareturn* dt = (datareturn*)(jmp - sizeof(datareturn));
	auto len = dt->len;
	auto src = dt->function;
	DWORD dwback;
	VirtualProtect(src, len, PAGE_READWRITE, &dwback);
	memcpy(dt->function, jmp, dt->len);
	jmp -= sizeof(datareturn);
	datareturn retdt = { len, src };


	VirtualProtect(src, len, dwback, &dwback);
	free(jmp);
	dst = NULL;
	return retdt;
}


template<typename t>
t DetourCreate(PBYTE src, t dst, const INT len)
{
	BYTE *jmp = (BYTE *)malloc(sizeof(datareturn) + len + 5);
	datareturn dt = { len, src };


	memcpy(jmp, &dt, sizeof(datareturn));
	jmp += sizeof(datareturn);
	DWORD dwback;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &dwback);
	memcpy(jmp, src, len);
	jmp += len;
	jmp[0] = 0xE9;
	*(DWORD *)(jmp + 1) = (DWORD)(src + len - jmp) - 5;
	src[0] = 0xE9;
	*(DWORD *)(src + 1) = (DWORD)((PBYTE)dst - src) - 5;
	for (INT i = 5; i < len; i++) src[i] = 0x90;
	VirtualProtect(src, len, dwback, &dwback);


	return(t)(jmp - len);
}
