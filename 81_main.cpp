#include "SDK.h"
#include "RWM.h"

#define RAND(min, max) rand() % (max - min) + min;
#define X3_NOT_INITIALIZED -536805375;

typedef char (__thiscall* tSub_91CF90)(DWORD **, int, char);
typedef int(__stdcall* tX3Dispatch)(void*, uint32_t);
tX3Dispatch oX3Dispatch = nullptr;
tSub_91CF90 sub_91CF90 = nullptr;

#pragma region ----------------------------------API HOOKING----------------------------------

typedef BOOL(WINAPI* tPostMessageW)(HWND, UINT, WPARAM, LPARAM); 
typedef SC_HANDLE(WINAPI* tOpenSCManager)(LPCWSTR, LPCWSTR, DWORD);
typedef BOOL(WINAPI* tDuplicateHandle)(HANDLE, HANDLE, HANDLE, LPHANDLE, DWORD, BOOL, DWORD);
typedef void (WINAPI* tExitProcess)(UINT);
typedef BOOL(WINAPI* tTerminate)(HANDLE, UINT);
typedef NTSTATUS (NTAPI* tZwTerminateProcess)(HANDLE, NTSTATUS); 
typedef NTSTATUS (NTAPI* tZwSetInformationThread)(HANDLE, THREADINFOCLASS, PVOID, ULONG);
typedef NTSTATUS(NTAPI* tNtCreateSemaphore)(PHANDLE SemaphoreHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, ULONG InitialCount, ULONG MaximumCount);
typedef BOOL(WINAPI* tCreateProcessW)(LPCWSTR, LPWSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, BOOL, DWORD, LPVOID, LPCWSTR, LPSTARTUPINFOW, LPPROCESS_INFORMATION);
tPostMessageW oPostMessageW = nullptr;
tOpenSCManager oOpenSCManager = nullptr;
tDuplicateHandle oDuplicateHandle = nullptr;
tExitProcess oExitProcess = nullptr;
tTerminate oTerminateProcess = nullptr;
tZwTerminateProcess oZwTerminateProcess = nullptr; 
tZwSetInformationThread oZwSetInformationThread = nullptr;
tTerminate oTerminateThread = nullptr;
tNtCreateSemaphore oNtCreateSemaphore = nullptr;
tCreateProcessW oCreateProcessW = nullptr;

BOOL WINAPI hkPostMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	void** ret = nullptr;
	__asm mov ret, ebp;
	if (Msg == WM_SYSCOMMAND)
	{
		//char title[256];
		//GetWindowTextA(hWnd, title, 256);
		if (wParam == SC_MINIMIZE)
		{
			//char out[256];
			//std::cout << "Call to PostMessageW(" << (void*)hWnd << " (" << title << "), WM_SYSCOMMAND, SC_MINIMIZE, ";
			//sprintf_s(out, "%08X) from %08X = FALSE", ret[1]);
			//std::cout << out << std::endl;
			return TRUE;
		}
	}
	else if (Msg == WM_SHOWWINDOW)
	{
		return TRUE;
	}
	return oPostMessageW(hWnd, Msg, wParam, lParam);
}
SC_HANDLE WINAPI hkOpenSCManager(LPCWSTR lpMachineName, LPCWSTR lpDatabaseName, DWORD dwDesiredAccess)
{
	/*
	void** ret = nullptr;
	__asm mov ret, ebp;

	char out[256];
	sprintf_s(out, "Call to OpenSCanager(%08X, %08X, %08X) from %08X", lpMachineName, lpDatabaseName, dwDesiredAccess, ret[1]);
	std::cout << out << std::endl;
	*/

	SetLastError(ERROR_ACCESS_DENIED);
	return 0;
}
BOOL WINAPI hkDuplicateHandle(HANDLE hSourceProcessHandle, HANDLE hSourceHandle, HANDLE hTargetProcessHandle,
LPHANDLE lpTargetHandle, DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwOptions)
{
	//void** ret = nullptr;
	//__asm mov [ret], ebp;

	if (hSourceProcessHandle == (HANDLE)-1 && hSourceHandle == (HANDLE)-1) return TRUE;

	BOOL result = oDuplicateHandle(hSourceProcessHandle, hSourceHandle, hTargetProcessHandle, lpTargetHandle, dwDesiredAccess,
		bInheritHandle, dwOptions);
	/*
	printf_s(out, "Call to DuplicateHandle(%04X, %04X, %04X, %08X, %04X, %i, %04X) from %08X = %i", hSourceProcessHandle, 
		hSourceHandle, hTargetProcessHandle, lpTargetHandle, dwDesiredAccess, bInheritHandle, dwOptions, ret[1], result);
	*/

	return result;
}
void WINAPI hkExitProcess(UINT uExitCode)
{
	int result = MessageBoxA(0, "Exit Process?", "kernel32.ExitProcess", MB_YESNO);
	if (result == IDYES)
	{
		return oExitProcess(uExitCode);
	}
	return;
}
BOOL WINAPI hkTerminateProcess(HANDLE hProcess, UINT uExitCode)
{
	//MessageBoxA(0, "kernel32.TerminateProcess", "kernel32.TerminateProcess", MB_OK);
	return oTerminateProcess(hProcess, uExitCode);
}
NTSTATUS NTAPI hkNtCreateSemaphore(PHANDLE SemaphoreHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, ULONG InitialCount, ULONG MaximumCount)
{
	if (ObjectAttributes && ObjectAttributes->ObjectName && ObjectAttributes->ObjectName->Buffer && !wcscmp(ObjectAttributes->ObjectName->Buffer, L"Global\\368457d19197f4eec4a257959dfdb062"))
		return oNtCreateSemaphore(SemaphoreHandle, DesiredAccess, NULL, InitialCount, MaximumCount);

	return oNtCreateSemaphore(SemaphoreHandle, DesiredAccess, ObjectAttributes, InitialCount, MaximumCount);
}
BOOL WINAPI hkCreateProcesW(LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{
	if (lpCommandLine && !_wcsicmp(lpCommandLine + wcslen(lpCommandLine) - 5, L".xem\""))
	{
		if (lpProcessInformation)
		{
			lpProcessInformation->dwProcessId = GetCurrentProcessId();
			lpProcessInformation->dwThreadId = GetCurrentThreadId();
			lpProcessInformation->hProcess = GetCurrentProcess();
			lpProcessInformation->hThread = GetCurrentThread();
		}

		return TRUE;
	}
	return oCreateProcessW(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
}
NTSTATUS NTAPI hkZwTerminateProcess(HANDLE ProcessHandle, NTSTATUS ExitStatus)
{
	//MessageBoxA(0, "ntdll.ZwTerminateProcess", "ntdll.ZwTerminateProcess", MB_OK);
	Sleep(-1);
	return 0;
	//return oZwTerminateProcess(ProcessHandle, ExitStatus);
}
NTSTATUS NTAPI hkZwSetInformationThread(HANDLE ThreadHandle, THREADINFOCLASS ThreadInformationClass, PVOID ThreadInformation,
	ULONG ThreadInformationLength)
{
	if (ThreadInformationClass == 0x11)
	{
		//MessageBoxA(0, "ntdll.ZwSetInformationThread", "Hook", MB_OK);
		return 0;
	}

	return oZwSetInformationThread(ThreadHandle, ThreadInformationClass, ThreadInformation, ThreadInformationLength);
}
BOOL WINAPI hkTerminateThread(HANDLE hThread, DWORD dwExitCode)
{
	//MessageBoxA(0, "kernel32.TerminateThread", "kernel32.TerminateThread", MB_OK);
	return oTerminateThread(hThread, dwExitCode);
}

#pragma endregion

enum ConsoleType
{
	Error = 12,
	Log = 11,
	Info = 10
};
void Push(LPCSTR lpText, ConsoleType Type)
{
	time_t now = time(0);
	tm tstruct = *localtime(&now);

	char buf[80];
	strftime(buf, sizeof(buf), "%X", &tstruct);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Type);
	std::cout << "[" << buf << "] ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	for (size_t i = 0; i < strlen(lpText); i++)
	{
		std::cout << lpText[i];
		if (lpText[i] != ' ')
			Sleep(13);
	}
	std::cout << std::endl;
}

char __fastcall hkSub_91CF90(DWORD **_this, DWORD edx, int a2, char a3)
{
	if (!IsValidReadPtr(_this))
	{
		//MessageBoxA(0, "Invalid thisptr", "Bugfix", MB_OK);
		return 1;
	}
	return sub_91CF90(_this, a2, a3);
}


unsigned __stdcall MainThread(void* pReserved)
{
	sWrite<bool>(0x0043BF53, true); //afk1
	sWrite<bool>(0x00462879, true); //afk2

	srand(time(0));
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
	MessageBeep(1000);
	char buff[256];

	using namespace std;

	sub_91CF90 = (tSub_91CF90)Detour((Pointer)0x0091CF90, hkSub_91CF90, 6);
		
	SetConsoleTitleA("XIGNCODE3 BYPASS");

	//Push("Bypassing the driver protection", Info);
	HMODULE hMod = GetModuleHandleA("Advapi32.dll");

	if (!hMod)
	{
		sprintf_s(buff, "Couldn't locate Advapi32.dll; #%08X", GetLastError());
		Push(buff, Error);
		Sleep(3000);
		FreeConsole();
	}

	FARPROC opnSCMgr = GetProcAddress(hMod, "OpenSCManagerW");

	if (!opnSCMgr)
	{
		sprintf_s(buff, "Couldn't locate OpenSCManagerW; #%08X", GetLastError());
		Push(buff, Error);
		Sleep(3000);
		FreeConsole();
	}

	//sprintf_s(buff, "Located OpenSCManager: %08X", opnSCMgr);
	//Push(buff, Log);

	oOpenSCManager = (tOpenSCManager)Detour(opnSCMgr, hkOpenSCManager, 5);

	if ((Value)oOpenSCManager == DEAD || oOpenSCManager == nullptr)
	{
		Push("Failed to bypass driver!", Error);
		Sleep(3000);
		FreeConsole();
	}

	Push("Driver protection bypassed", Info);
	
	//---------------------------------------------------------------------//

	//Push("Preventing handles from getting closed", Info);

	hMod = GetModuleHandleA("KERNELBASE.DLL");

	if (!hMod)
	{
		sprintf_s(buff, "Couldn't locate KERNELBASE.DLL; #%08X", GetLastError());
		Push(buff, Error);
		Sleep(3000);
		FreeConsole();
	}

	FARPROC dpHandle = GetProcAddress(hMod, "DuplicateHandle");

	if (!dpHandle)
	{
		sprintf_s(buff, "Couldn't locate DuplicateHandle; #%08X", GetLastError());
		Push(buff, Error);
		Sleep(3000);
		FreeConsole();
	}

	//sprintf_s(buff, "Located DuplicateHandle: %08X", dpHandle);
	//Push(buff, Log);

	oDuplicateHandle = (tDuplicateHandle)Detour(dpHandle, hkDuplicateHandle, 5);

	if ((Value)oDuplicateHandle == DEAD || oDuplicateHandle == nullptr)
	{
		Push("Failed to prevent handles from getting closed", Error);
		Sleep(3000);
		FreeConsole();
	}

	Push("Prevented handles from getting closed", Info);
	
	//---------------------------------------------------------------------//

	//Push("Preveting windows from getting minimized", Info);

	hMod = GetModuleHandleA("User32.dll");

	if (!hMod)
	{
		sprintf_s(buff, "Couldn't locate User32.dll; #%08X", GetLastError());
		Push(buff, Error);
		Sleep(3000);
		FreeConsole();
	}

	FARPROC pstMsgW = GetProcAddress(hMod, "PostMessageW");
	if (!pstMsgW)
	{
		sprintf_s(buff, "Couldn't locate PostMessageW; #%08X", GetLastError());
		Push(buff, Error);
		Sleep(3000);
		FreeConsole();
	}

	//sprintf_s(buff, "Located PostMessageW: %08X", pstMsgW);
	//Push(buff, Log);

	oPostMessageW = (tPostMessageW)Detour(pstMsgW, hkPostMessageW, 5);

	if ((Value)oPostMessageW == DEAD || oPostMessageW == nullptr)
	{
		Push("Failed to prevent windows from getting minimized", Error);
		Sleep(3000);
		FreeConsole();
	}

	Push("Prevented windows from getting minimized", Info);

	//---------------------------------------------------------------------//

	hMod = GetModuleHandleA("kernel32.dll");
	if (!hMod)
	{
		sprintf_s(buff, "Couldn't locate kernel32.dll; #%08X", GetLastError());
		Push(buff, Error);
		Sleep(3000);
		FreeConsole();
	}

	FARPROC idbgPrsnt = GetProcAddress(hMod, "IsDebuggerPresent");
	if (!idbgPrsnt)
	{
		sprintf_s(buff, "Couldn't locate IsDebuggerPresent; #%08X", GetLastError());
		Push(buff, Error);
		Sleep(3000);
		FreeConsole();
	}

	FARPROC extProc = GetProcAddress(hMod, "ExitProcess");
	if (!extProc)
	{
		sprintf_s(buff, "Couldn't locate ExitProcess; #%08X", GetLastError());
		Push(buff, Error);
		Sleep(3000);
		FreeConsole();
	}

	oExitProcess = (tExitProcess)Detour(extProc, hkExitProcess, 5);

	if ((Value)oExitProcess == DEAD || oExitProcess == nullptr)
	{
		Push("Failed to prevent client from unexpected exiting", Error);
		Sleep(3000);
		FreeConsole();
	}
	
	FARPROC trmProc = GetProcAddress(hMod, "TerminateProcess");
	if (!trmProc)
	{
		sprintf_s(buff, "Couldn't locate TerminateProcess; #%08X", GetLastError());
		Push(buff, Error);
		Sleep(3000);
		FreeConsole();
	}

	oTerminateProcess = (tTerminate)Detour(trmProc, hkTerminateProcess, 5);

	if ((Value)oTerminateProcess == DEAD || oTerminateProcess == nullptr)
	{
		Push("Failed to prevent client from terminating itself and other processes", Error);
		Sleep(3000);
		FreeConsole();
	}

	FARPROC trmThrd = GetProcAddress(hMod, "TerminateThread");
	if (!trmThrd)
	{
		sprintf_s(buff, "Couldn't locate TerminateThread; #%08X", GetLastError());
		Push(buff, Error);
		Sleep(3000);
		FreeConsole();
	}

	oTerminateThread = (tTerminate)Detour(trmThrd, hkTerminateThread, 5);

	if ((Value)oTerminateThread == DEAD || oTerminateThread == nullptr)
	{
		Push("Failed to prevent client from exiting (1)", Error);
		Sleep(3000);
		FreeConsole();
	}

	Push("Prevented process closing with kernel32 APIs", Info);

	//---------------------------------------------------------------------//

	hMod = GetModuleHandleA("ntdll.dll");
	if (!hMod)
	{
		sprintf_s(buff, "Couldn't locate ntdll.dll; #%08X", GetLastError());
		Push(buff, Error);
		Sleep(3000);
		FreeConsole();
	}

	FARPROC zwTrmProc = GetProcAddress(hMod, "ZwTerminateProcess");
	if (!zwTrmProc)
	{
		sprintf_s(buff, "Couldn't locate ZwTerminateProcess; #%08X", GetLastError());
		Push(buff, Error);
		Sleep(3000);
		FreeConsole();
	}

	oZwTerminateProcess = (tZwTerminateProcess)Detour(zwTrmProc, hkZwTerminateProcess, 5);

	if ((Value)oZwTerminateProcess == DEAD || oZwTerminateProcess == nullptr)
	{
		Push("Failed to prevent client from unexpected exiting (2)", Error);
		Sleep(3000);
		FreeConsole();
	}

	FARPROC zwStInfThrd = GetProcAddress(hMod, "ZwSetInformationThread");
	if (!zwStInfThrd)
	{
		sprintf_s(buff, "Couldn't locate ZwSetInformationThread; #%08X", GetLastError());
		Push(buff, Error);
		Sleep(3000);
		FreeConsole();
	}

	oZwSetInformationThread = (tZwSetInformationThread)Detour(zwStInfThrd, hkZwSetInformationThread, 5);

	if ((Value)oZwSetInformationThread == DEAD || oZwSetInformationThread == nullptr)
	{
		Push("Failed to bypass debugger detection", Error);
		Sleep(3000);
		FreeConsole();
	}

	Push("Bypassed debugger detection", Info);
	
	FARPROC semaforuPulii = GetProcAddress(hMod, "NtCreateSemaphore");

	if (!semaforuPulii)
	{
		sprintf_s(buff, "Couldn't locate NtCreateSemaphore; #%08X", GetLastError());
		Push(buff, Error);
		Sleep(3000);
		FreeConsole();
	}
	oNtCreateSemaphore = (tNtCreateSemaphore)Detour(semaforuPulii, hkNtCreateSemaphore, 5);

	hMod = GetModuleHandleA("kernel32.dll");
	if (!hMod)
	{
		sprintf_s(buff, "Couldn't locate kernel32.dll; #%08X", GetLastError());
		Push(buff, Error);
		Sleep(3000);
		FreeConsole();
	}

	FARPROC crtProcW = GetProcAddress(hMod, "CreateProcessW");

	if (!crtProcW)
	{
		sprintf_s(buff, "Couldn't locate CreateProcessW; #%08X", GetLastError());
		Push(buff, Error);
		Sleep(3000);
		FreeConsole();
	}
	oCreateProcessW = (tCreateProcessW)Detour(crtProcW, hkCreateProcesW, 5);
	Push("Bypassed multiclient detection", Info);
	//---------------------------------------------------------------------//

	Push("Bypass complete", Info);

	Sleep(7000);
	FreeConsole();

	return 1;
}
__declspec(dllexport) int32_t __stdcall x3_1(void* func, uint32_t type)
{
	if (oX3Dispatch) return oX3Dispatch(func, type);

	std::string Path;
	Path.resize(MAX_PATH);

	if (!GetModuleFileNameA(NULL, const_cast<LPSTR>(Path.data()), MAX_PATH))
	{
		MessageBoxA(NULL, "GetModuleFileNameA failed!", "Error", 0);
		return X3_NOT_INITIALIZED;
	}

	HMODULE hX3;
	if (!(hX3 = LoadLibraryW(L"XIGNCODE\\x3.original")))
	{
		MessageBoxA(NULL, "LoadLibraryW failed!", "Error", 0);
		return X3_NOT_INITIALIZED;
	}

	if (!(oX3Dispatch = reinterpret_cast<tX3Dispatch>(GetProcAddress(hX3, reinterpret_cast<LPCSTR>(1)))))
	{
		MessageBoxA(NULL, "GetProcAddress failed!", "Error", 0);
		return X3_NOT_INITIALIZED;
	}

	if (!_beginthreadex(nullptr, 0, &MainThread, nullptr, 0, nullptr))
	{
		char Buffer[512];
		RtlZeroMemory(Buffer, 512);
		strcpy_s(Buffer, "Thread creation failed\nError: ");
		strerror_s(Buffer + strlen(Buffer), 512 - strlen(Buffer), errno);

		MessageBoxA(NULL, Buffer, "Error", 0);
	}
	
	return oX3Dispatch(func, type);
}
