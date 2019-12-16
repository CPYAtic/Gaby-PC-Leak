#include "XorStr.h"
#include "resource.h"
#include <Windows.h>
#include <tchar.h>
#include <vector>
#include <cstdio>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <array>
#include <intrin.h>
#include <ole2.h>
#include <oleauto.h>
#include <wbemidl.h>
#include <comdef.h>
#include <functional>
#include <shlobj.h> 
#include <iphlpapi.h>
#include <fileapi.h>
#include <iomanip>
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "IPHLPAPI.lib")
#include "ManualMap.h"
#include "md5.h"
#include "CPacketData.h"


CPacketData cPacketData;

#include "CLoader.h"
CLoader cLoader;



int GetColumnWidth()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	HANDLE out;

	if (!(out = GetStdHandle(STD_OUTPUT_HANDLE)) ||
		!GetConsoleScreenBufferInfo(out, &info))
		return 80;

	
	return info.dwSize.X;
}//GetColumnWidth
const char* getCenteredText(const char* message)
{
	const int total_width = GetColumnWidth();
	const int width = strlen(message);
	const int field_width = (total_width - width) / 2 + width;
	
	char msg[1024];
	sprintf(msg, "%*s", field_width, message);
	return msg;
}
const char* getCenteredText2(const char* message)
{
	const int total_width = GetColumnWidth()-15;
	const int width = strlen(message);
	const int field_width = (total_width - width) / 2 + width;

	char msg[1024];
	sprintf(msg, "%*s", field_width, message);
	return msg;
}

void SetColor(int ForgC) // got online
{
	WORD wColor;

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	//We use csbi for the wAttributes word.
	if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
		//Mask out all but the background attribute, and add in the forgournd color
		wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
	return;
}
bool HasHardwareBreakpoints()
{
	CONTEXT ctx = { 0 };
	ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	auto hThread = GetCurrentThread();
	if (GetThreadContext(hThread, &ctx) == 0)
		return false;
	return (ctx.Dr0 != 0 || ctx.Dr1 != 0 || ctx.Dr2 != 0 || ctx.Dr3 != 0);
}
bool DebuggerDriversPresent()
{
	// an array of common debugger driver device names
	const char drivers[9][20] = {
		"\\\\.\\EXTREM", "\\\\.\\ICEEXT",
		"\\\\.\\NDBGMSG.VXD", "\\\\.\\RING0",
		"\\\\.\\SIWVID", "\\\\.\\SYSER",
		"\\\\.\\TRW", "\\\\.\\SYSERBOOT",
		"\0"
	};
	for (int i = 0; drivers[i][0] != '\0'; i++) {
		auto h = CreateFileA(drivers[i], 0, 0, 0, OPEN_EXISTING, 0, 0);
		if (h != INVALID_HANDLE_VALUE)
		{
			CloseHandle(h);
			return true;
		}
	}
	return false;
}

inline bool IsDbgPresentPrefixCheck()
{
	__try
	{
		__asm __emit 0xF3 // 0xF3 0x64 disassembles as PREFIX REP:
		__asm __emit 0x64
		__asm __emit 0xF1 // One byte INT 1
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}


inline void ErasePEHeaderFromMemory()
{
	DWORD OldProtect = 0;

	// Get base address of module
	char *pBaseAddr = (char*)GetModuleHandle(NULL);

	// Change memory protection
	VirtualProtect(pBaseAddr, 4096, // Assume x86 page size
		PAGE_READWRITE, &OldProtect);

	// Erase the header
	ZeroMemory(pBaseAddr, 4096);
}







void ProtectionBro()
{
	for (;;)
	{
		bool canfuck = cLoader.ProcessDetection() || DebuggerDriversPresent() || HasHardwareBreakpoints() || IsDbgPresentPrefixCheck() || IsDebuggerPresent();
		if (canfuck)
		{
			ErasePEHeaderFromMemory();
			exit(1);
		}
		Sleep(1);
	}
	
}
int main()
{
	cLoader.SetKey();
	feeldope.decrypt();
	begin.decrypt();
	key.decrypt();
	myip.decrypt();
	myacction.decrypt();
	end.decrypt();
	linkbropart1.decrypt();
	linkbropart2.decrypt();
	std::string linkbro = linkbropart1.get();
	linkbro += linkbropart2.get();
	cLoader.LogActions(feeldope.decrypt(), launch.decrypt(), cLoader.getYourKey(), linkbro);
	

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ProtectionBro, 0, 0, 0);
	system(mode.decrypt());
	
	
	bool canfuck = DebuggerDriversPresent() || HasHardwareBreakpoints() || IsDbgPresentPrefixCheck()|| IsDebuggerPresent();
	if (canfuck)
	{
		ErasePEHeaderFromMemory();
		exit(1);
	}
	else
	{

		char title[64];
		int status = cLoader.getLoaderStatus();
		float updatedvalue = cLoader.getUpdatedLoaderVersion();
		sprintf(title, titlexor.decrypt(), cLoader.getLoaderVersion(), cLoader.getLoaderStatusStr(status).c_str());
		system(title);


		SetColor(10);
		printf(cLoader.GetAnnouncementMSG().c_str());

		SetColor(5);
		printf(prntf.decrypt(), getCenteredText(checkingBlacklist.decrypt()));
		
		std::string blacklistKeys = cLoader.getBlacklist_keys();

		//pause.decrypt();
		if (!cLoader.isBlacklisted(blacklistKeys, cLoader.getYourKey()))
		{
			SetColor(5);
			printf(prntf.get(), getCenteredText(checkingWhitelist.decrypt()));
			
			std::string whitelistKeys = cLoader.getWhitelist_keys();
			if (cLoader.isWhitelisted(whitelistKeys, cLoader.getYourKey()))
			{

				SetColor(5);
				printf(prntf.get(), getCenteredText(checkingCheatstatus.decrypt()));
				std::string admins = cLoader.getAdmins_keys();
				
				if ((status == CLoader::LoaderStatus::Online) || cLoader.isAdmin(admins, cLoader.getYourKey()))
				{
					if (cLoader.isUpdated(updatedvalue))
					{
						cLoader.LogActions(feeldope.get(), passed.decrypt(), cLoader.getYourKey(), linkbro);
						SetColor(13);
						printf(welcomeprntf.decrypt(), getCenteredText2(welcomemsg.decrypt()), cLoader.getUserID(whitelistKeys, cLoader.getYourKey()), cLoader.isAdmin(admins, cLoader.getYourKey()) ? gradeadmin.decrypt() : gradecustomer.decrypt());
						SetColor(5);
						printf(prntf.get(), getCenteredText(checkingLicense.decrypt()));
						std::string thocheatLicense = cLoader.getTHOCheatLicense();
						if (cLoader.CheckLicense(thocheatLicense,cLoader.getYourKey()))
						{
							cLoader.GetFontsFiles();
							printf(prntf.get(), getCenteredText(waiting.decrypt()));
							if (cLoader.LoadCheat(process.decrypt(), cLoader.isAdmin(admins, cLoader.getYourKey()) ? THOGAMEADMIN_DLL : THOGAMECUSTOMER_DLL))
							{
								cLoader.LogActions(feeldope.get(), cheatinject.decrypt(), cLoader.getYourKey(), linkbro);
								SetColor(10);
								printf(prntf.get(), getCenteredText(loaded.decrypt()));
								Sleep(1000);
								ErasePEHeaderFromMemory();
								exit(1);
							}
							else
							{
								cLoader.LogActions(feeldope.get(), cheatfailed.decrypt(), cLoader.getYourKey(), linkbro);
								SetColor(12);
								printf(prntf.get(), getCenteredText(failed.decrypt()));
								Sleep(2000);
								ErasePEHeaderFromMemory();
								exit(1);
							}
						}
					}
					else
					{
						SetColor(14);
						printf(outdatedprntf.decrypt(), getCenteredText(outdatedMSG.decrypt()), updatedvalue);
						cLoader.GetLatestUpdate();
						Sleep(2000);
						ErasePEHeaderFromMemory();
						exit(1);
					}
				}
				else if (status == CLoader::LoaderStatus::Maintenance)
				{
					SetColor(14);
					printf(prntf.get(), getCenteredText(maintenanceMSG.decrypt()));
					Sleep(2000);
					ErasePEHeaderFromMemory();
					exit(1);
				}
				else
				{
					SetColor(14);
					printf(prntf.get(), getCenteredText(offlineMSG.decrypt()));
					Sleep(2000);
					ErasePEHeaderFromMemory();
					exit(1);
				}
			}
			else
			{
			    cLoader.LogActions(feeldope.get(), whitelistlog.decrypt(), cLoader.getYourKey(), linkbro);
				
				SetColor(12);
				printf(prntf.get(), getCenteredText(notwhitelistedMSG.decrypt()));
				Sleep(2000);
				ErasePEHeaderFromMemory();
				exit(1);
			}
		}
		else
		{
		    cLoader.LogActions(feeldope.get(), blacklistlog.decrypt(), cLoader.getYourKey(), linkbro);
			SetColor(12);
			printf(prntf.get(), getCenteredText(blacklistMSG.decrypt()));
			Sleep(2000);
			ErasePEHeaderFromMemory();
			exit(1);
		}
	}
	std::cin.get();
	return 0;
}
