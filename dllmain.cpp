#include "SDK.h"
#include "Console.h"
CEngine* cEngine = new CEngine();
typedef int(__cdecl* tMeleeHit)(Vector3, Vector3, DWORD, Vector3, int, int);
tMeleeHit oMeleeHit = nullptr;
typedef int(__cdecl* tRifleHit)(Vector3, Vector3, DWORD, Vector3, int, int);
tRifleHit oRifleHit = nullptr;
typedef int(__cdecl* tSniperHit)(Vector3, Vector3, DWORD, Vector3, int, int, int);
tSniperHit oSniperHit = nullptr;
typedef bool(__thiscall * tRaycast)(char *_this, int a2, int a3, int a4, int a5, float a6);
tRaycast oRaycast = nullptr;
std::string timestamp()
{
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	std::stringstream ssTp;
	ssTp << std::put_time(&tm, "%H-%M-%S : ");
	return ssTp.str();
}
std::string center(std::string input, int width = 0) {
	return std::string((width - input.length()) / 2, ' ') + input;
}
void centerify_output(std::string str, int num_cols) {
	// Calculate left padding
	int padding_left = (num_cols / 2) - (str.size() / 2);

	// Put padding spaces
	for (int i = 0; i < padding_left; ++i) std::cout << ' ';

	// Print the message
	std::cout << str;
}
void ConsoleLog(Console::CColor color, const char* text, ...)
{
	char buff[1024], buff2[1024 + 16];

	va_list vl;
	va_start(vl, text);

	if (vsnprintf_s(buff, 1024, text, vl) == -1)
		return;

	Console::Print(Console::CColor::Fuschia, 0, timestamp().c_str());
	Console::Print(color, 0, buff);
	va_end(vl);
}

std::string GetLocalPlayerName()
{
	char yes[40];
	sprintf_s(yes, "%s", cEngine->GetLocalPlayer()->Name);
	return yes;
}
std::string GetTargetNameBySerialID(DWORD serialID)
{
	char name[40];
	for (int i = 1; i < cEngine->GetRoom()->PlayerCount; i++)
	{
		if (cEngine->GetRoom()->Players[i]->Room->Server->serialID == serialID)
		{
			sprintf_s(name, "%s", cEngine->GetRoom()->Players[i]->Name);
			break;
		}
	}
	return name;
}
enum WeaponType : int
{
	MELEE = 0,
	RIFLE = 1,
	SHOTGUN = 2,
	SNIPER = 3,
	MG = 4,
	BAZOOKA = 5,
	GRENADE = 6
};
int GetCurrentWeapon()
{


	DWORD IsMelee = { 0x11B9FC4 };
	DWORD IsRifle = { IsMelee + 0x4c };
	DWORD IsShotgun = { IsMelee + 0x98 };
	DWORD IsSniper = { IsMelee + 0xE4 };
	DWORD IsGatling = { IsMelee + 0x130 };
	DWORD IsBazooka = { IsMelee + 0x17C };
	DWORD IsGrenade = { IsMelee + 0x1C8 };

	if (*(int*)IsMelee)
		return WeaponType::MELEE;
	else if (*(int*)IsRifle)
		return WeaponType::RIFLE;
	else if (*(int*)IsShotgun)
		return WeaponType::SHOTGUN;
	else if (*(int*)IsSniper)
		return WeaponType::SNIPER;
	else if (*(int*)IsGatling)
		return WeaponType::MG;
	else if (*(int*)IsBazooka)
		return WeaponType::BAZOOKA;
	else if (*(int*)IsGrenade)
		return WeaponType::GRENADE;

	return -1;
}
std::string localplayername;
clock_t RaycastingTime;
bool bStartoffTime = true;
bool __fastcall hkRaycast(char *_this, int a2, int a3, int a4, int a5, float a6)
{
	auto ret = oRaycast(_this, a2, a3, a4, a5, a6);
	/*if (thisptr == *(void**)0x11B6DC4)
	{
		if (GetCurrentWeapon() == WeaponType::SNIPER)
		{
			if (ret)
			{
				if (bStartoffTime)
				{
					RaycastingTime = clock();
					bStartoffTime = false;
				}
			}
			else bStartoffTime;
		}
	}*/
	return ret;
}
int __cdecl hkMeleeHit(Vector3 idk1, Vector3 idk2, DWORD SerialID, Vector3 idk3, int hitType, int MeleeShootType)
{
	auto ret = oMeleeHit(idk1, idk2, SerialID, idk3, hitType, MeleeShootType);
	if (cEngine->GetLocalPlayer()->Room->MeleeRange > 0)
	{
		ConsoleLog(Console::CColor::Red, "[AntiCheat] ");
		Console::Print(Console::CColor::Aqua, 0, "[%s]", localplayername.c_str());
		Console::Print(Console::CColor::Normal, 0, " hitted ");
		Console::Print(Console::CColor::Aqua, 0, "[%d]", GetTargetNameBySerialID(SerialID).c_str());
		Console::Print(Console::CColor::Normal, 0, " with ");
		Console::Print(Console::CColor::Aqua, 0, "[MELEE] ");
		Console::Print(Console::CColor::Red, 0, " [RANGE: %d]\n", cEngine->GetLocalPlayer()->Room->MeleeRange);
	}
	return ret;
}

int __cdecl hkRifleHit(Vector3 idk1, Vector3 idk2, DWORD SerialID, Vector3 idk3, int hitType, int unknown)
{
	auto ret = oRifleHit(idk1, idk2, SerialID, idk3, hitType, unknown);
	//float ReactionTime = clock() - RaycastingTime;
	ConsoleLog(Console::CColor::Red, "[AntiCheat] ");
	Console::Print(Console::CColor::Aqua, 0, "[%s]", localplayername.c_str());
	Console::Print(Console::CColor::Normal, 0, " hitted ");
	Console::Print(Console::CColor::Aqua, 0, "[%s]", GetTargetNameBySerialID(SerialID).c_str());
	Console::Print(Console::CColor::Normal, 0, " with ");
	Console::Print(Console::CColor::Aqua, 0, "[Rifle]\n");
	//Console::Print(Console::CColor::Red, 0, " [Reaction Time: %.2f]", ReactionTime);
	return ret;
}

int __cdecl hkSniperHit(Vector3 idk1, Vector3 idk2, DWORD SerialID, Vector3 idk3, int hitType, int unknown, int bSniperScope)
{
    auto ret = oSniperHit(idk1, idk2, SerialID, idk3, hitType, unknown, bSniperScope);
	//float ReactionTime = clock() - RaycastingTime;
	ConsoleLog(Console::CColor::Red, "[AntiCheat] ");
	Console::Print(Console::CColor::Aqua, 0, "[%s]", localplayername.c_str());
	Console::Print(Console::CColor::Normal, 0, " hitted ");
	Console::Print(Console::CColor::Aqua, 0, "[%s]", GetTargetNameBySerialID(SerialID).c_str());
	Console::Print(Console::CColor::Normal, 0, " with ");
	Console::Print(Console::CColor::Aqua, 0, "[SNIPER]\n");
	//Console::Print(Console::CColor::Red, 0, " [Reaction Time: %.2f]", ReactionTime);
	return ret;
}
int serverstate = 0;
int InitHooks()
{
	AllocConsole();

	FILE *conin, *conout;
	SetConsoleTitleA("FDP AntiCheat");

	freopen_s(&conin, "conin$", "r", stdin);
	freopen_s(&conout, "conout$", "w", stdout);
	freopen_s(&conout, "conout$", "w", stderr);
	//oRaycast = (tRaycast)RWM::Detour((LPVOID)0x007B09B0, hkRaycast, 9);
	oMeleeHit = (tMeleeHit)RWM::Detour((LPVOID)0x00B32F10, hkMeleeHit, 5);
	oRifleHit = (tRifleHit)RWM::Detour((LPVOID)0x00B30560, hkRifleHit, 5);
	oSniperHit = (tSniperHit)RWM::Detour((LPVOID)0x00B31C80, hkSniperHit, 5);
	ConsoleLog(Console::CColor::Red, "[AntiCheat] ");
	Console::Print(Console::CColor::Lime, 0, "Loaded successfully.\n");
	while (true)
	{
		cEngine->Run();
		if (cEngine->GetLocalPlayer())
		{
			char yes[40];
			sprintf_s(yes, "%s", cEngine->GetLocalPlayer()->Name);
			localplayername = yes;
		}
		
		if (serverstate != cEngine->GetServerState())
		{
			serverstate = cEngine->GetServerState();
			if (serverstate == GameState::GAME_STATE_LOBBY_AGORA)
			{
				ConsoleLog(Console::CColor::Red, "[AntiCheat] ");
				Console::Print(Console::CColor::Aqua, 0, "[%s]", localplayername.c_str());
				Console::Print(Console::CColor::Normal, 0, " joined lobby.\n");
			}
			else if(serverstate == GameState::GAME_STATE_ROOM_2ND)
			{
				ConsoleLog(Console::CColor::Red, "[AntiCheat] ");
				Console::Print(Console::CColor::Aqua, 0, "[%s]", localplayername.c_str());
				Console::Print(Console::CColor::Normal, 0, " joined room number ");
				Console::Print(Console::CColor::Aqua, 0, "[%d].\n", cEngine->GetRoom()->RoomNumber + 1);
			}
		}

		Sleep(1);
	}
}
BOOL WINAPI DllMain(HMODULE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)InitHooks, 0, 0, 0);
		return 1;
	}
	return 0;
}