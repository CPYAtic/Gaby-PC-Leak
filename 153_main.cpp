#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <stdio.h>
#include <ctime>
#pragma comment(lib, "Ws2_32.lib")
#include "Console.h"
enum EncryptionUsed : int
{
	CNONE = 0,
	CEncryptWithNULLKey = 1,
	CEncrypt = 2,
	CEncryptLargeWithNULLKey = 3,
	CEncryptLarge = 4
};
HMODULE CCryptographyModule = LoadLibraryW(L"CCryptography.dll");
HANDLE __stdcall CreateHandle(DWORD dwKey)
{
	using tCreateHandle = HANDLE(__stdcall*)(DWORD);
	tCreateHandle oCreateHandle = reinterpret_cast<tCreateHandle>(GetProcAddress(CCryptographyModule, "CreateHandle"));
	return oCreateHandle(dwKey);
}
bool __stdcall DestroyHandle(HANDLE hCCryptography)
{
	using tDestroyHandle = bool(__stdcall*)(HANDLE);
	tDestroyHandle oDestroyHandle = reinterpret_cast<tDestroyHandle>(GetProcAddress(CCryptographyModule, "DestroyHandle"));
	return oDestroyHandle(hCCryptography);
}
bool __stdcall ChangeKey(HANDLE hCCryptography, DWORD dwKey)
{
	using tChangeKey = bool(__stdcall*)(HANDLE, DWORD);
	tChangeKey oChangeKey = reinterpret_cast<tChangeKey>(GetProcAddress(CCryptographyModule, "ChangeKey"));
	return oChangeKey(hCCryptography, dwKey);
}
DWORD __stdcall GetKey(HANDLE hCCryptography)
{
	using tGetKey = DWORD(__stdcall*)(HANDLE);
	tGetKey oGetKey = reinterpret_cast<tGetKey>(GetProcAddress(CCryptographyModule, "GetKey"));
	return oGetKey(hCCryptography);
}
bool __stdcall Encrypt(HANDLE hCCryptography, const void* src, void* dst, int size)
{
	using tEncrypt = bool(__stdcall*)(HANDLE, const void*, void*, int);
	tEncrypt oEncrypt = reinterpret_cast<tEncrypt>(GetProcAddress(CCryptographyModule, "Encrypt"));
	return oEncrypt(hCCryptography, src, dst, size);
}
bool __stdcall Decrypt(HANDLE hCCryptography, const void* src, void* dst, int size)
{
	using tDecrypt = bool(__stdcall*)(HANDLE, const void*, void*, int);
	tDecrypt oDecrypt = reinterpret_cast<tDecrypt>(GetProcAddress(CCryptographyModule, "Decrypt"));
	return oDecrypt(hCCryptography, src, dst, size);
}
bool __stdcall EncryptLarge(HANDLE hCCryptography, const void* src, void* dst, int size)
{
	using tEncryptLarge = bool(__stdcall*)(HANDLE, const void*, void*, int);
	tEncryptLarge oEncryptLarge = reinterpret_cast<tEncryptLarge>(GetProcAddress(CCryptographyModule, "EncryptLarge"));
	return oEncryptLarge(hCCryptography, src, dst, size);
}
bool __stdcall DecryptLarge(HANDLE hCCryptography, const void* src, void* dst, int size)
{
	using tDecryptLarge = bool(__stdcall*)(HANDLE, const void*, void*, int);
	tDecryptLarge oDecryptLarge = reinterpret_cast<tDecryptLarge>(GetProcAddress(CCryptographyModule, "DecryptLarge"));
	return oDecryptLarge(hCCryptography, src, dst, size);
}
HANDLE ClientHandleNull = CreateHandle(NULL);
HANDLE ClientHandle = NULL;

SOCKET FrontSock = -1, MainSock = -1, CastSock = -1;
struct sockaddr_in FrontSV,MainSV,CastSV;

bool AwaitData(SOCKET sock,char* data, int& len)
{
	if (len < 4) return false;
	unsigned long size = 4, i = 0;
	unsigned long* pName = (unsigned long*)&data[0];
	do
	{
		int length = recv(sock, data + i++, 1, MSG_WAITALL);

		if (length > 0)
		{
			if (i == 4)
			{
				Decrypt(ClientHandleNull, pName, pName, 4);
				size = ((*pName) >> 18) & 2047;
			}
			if (i >= size || i >= len)
			{
				EncryptionUsed encryption = (EncryptionUsed)(((*pName) >> 29) & 7);
				if (encryption == EncryptionUsed::CEncrypt)
					Decrypt(ClientHandle, data + 4, data + 4, size - 4);
				else if (encryption == EncryptionUsed::CEncryptLarge)
					DecryptLarge(ClientHandle, data + 4, data + 4, size - 4);
				if (encryption == EncryptionUsed::CEncryptWithNULLKey)
					Decrypt(ClientHandleNull, data + 4, data + 4, size - 4);
				else if (encryption == EncryptionUsed::CEncryptLargeWithNULLKey)
					DecryptLarge(ClientHandleNull, data + 4, data + 4, size - 4);

				len = i;
				return true;
			}
		}
		else if (length == 0)
		{
			len = i;
			if (ClientHandle)
				DestroyHandle(ClientHandle);

			ClientHandle = NULL;
			return false;
		}
		else if (length == SOCKET_ERROR)
		{
			if (ClientHandle)
				DestroyHandle(ClientHandle);

			ClientHandle = NULL;
			return false;
		}
	} while (true);

	return false;
}
DWORD playerID = 0;
bool SendData(SOCKET sock, char* data, int len)
{
	Console::PrintPackets("SENT", data, len);

	unsigned long* pName = (unsigned long*)&data[0];
	unsigned long size = ((*pName) >> 18) & 2047;
	EncryptionUsed encryption = (EncryptionUsed)(((*pName) >> 29) & 7);

	if (encryption == EncryptionUsed::CEncrypt)
		Encrypt(ClientHandle, data + 4, data + 4, size - 4);
	else if (encryption == EncryptionUsed::CEncryptLarge)
		EncryptLarge(ClientHandle, data + 4, data + 4, size - 4);
	if (encryption == EncryptionUsed::CEncryptWithNULLKey)
		Encrypt(ClientHandleNull, data + 4, data + 4, size - 4);
	else if (encryption == EncryptionUsed::CEncryptLargeWithNULLKey)
		EncryptLarge(ClientHandleNull, data + 4, data + 4, size - 4);

	Encrypt(ClientHandleNull, data, data, 4);
	if (int sent = send(sock, data, len, 0))
	{

		if (sent == len)
		{
			return true;
		}
		else if (sent == SOCKET_ERROR)
			return false;
		else
		{
			if (int sent = send(sock, data, len, 0))
			{
				if (sent == len)
				{
					
					return true;
				}
				else if (sent == SOCKET_ERROR)
					return false;
				else
					return false;
			}
		}
	}
}
enum Regions
{
	EU,
	NA,
	SA
};
Regions server = Regions::EU;
int MeleeConfig = 7043, RifleConfig = 12301, ShotgunConfig = 24290, SniperConfig = 15223, GatlingConfig = 539, ZookaConfig = 5442, GrenadeConfig = 3817;
int meleeDone = 0, rifleDone = 0, shotgunDone = 0, sniperDone = 0, gatlingDone = 0, zookaDone = 0, grenadeDone = 0;
int deathsDone = 0, deathsConfig = 43443;
int assistsDone = 0, assistsConfig = 8992;
int headshotsDone = 0, headshotsConfig = 4882;
int KillDeathConfig = 50;
int timeMinConfig = 28.2;
const char* roomTitle = "Laggggggggggggggggg";
const char* roomPass = "xaxa6161";
const char* GetServer(Regions reg)
{
	if (reg == Regions::NA)
		return "207.148.11.92";
	else if (reg == Regions::SA)
		return "18.231.71.225";
	else 
		return "185.212.200.90";
}
DWORD GetCIDShit(Regions reg)
{
	if (reg == Regions::NA)
		return 0x20000;
	else if (reg == Regions::SA)
		return 0x30000;
	else
		return 0x10000;
}
bool SetupConnFront(Regions reg)
{
	if (FrontSock == -1)
		FrontSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	bind(FrontSock, (sockaddr*)&FrontSV, sizeof(FrontSV));
	FrontSV.sin_addr.s_addr = inet_addr(GetServer(reg));
	FrontSV.sin_family = AF_INET;
	FrontSV.sin_port = htons(13000);
	return connect(FrontSock, (struct sockaddr *)&FrontSV, sizeof(FrontSV)) == 0;
}
bool SetupConnMain(Regions reg)
{
	if (MainSock == -1)
		MainSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	bind(MainSock, (sockaddr*)&MainSV, sizeof(MainSV));
	MainSV.sin_addr.s_addr = inet_addr(GetServer(reg));
	MainSV.sin_family = AF_INET;
	MainSV.sin_port = htons(13005);
	return connect(MainSock, (struct sockaddr *)&MainSV, sizeof(MainSV)) == 0;
}
bool loginSuccess = false, connectedMain = false, roomCreated = false, matchRunning = false, keepKilling = false;
enum WeaponType : int
{
	MELEE,
	RIFLE,
	SHOTGUN,
	SNIPER,
	GATLING,
	ZOOKA,
	GRENADE
};
WeaponType weaponStart = WeaponType::MELEE;
int killsdone = 0;
int killsmultiply = 71;
int deathsSet = 0, assistSet = 0, headshotsSet = 0;
void HostKill(WeaponType weap, int kills, int multiply)
{
	killsdone = kills;
	killsmultiply = multiply;
	for (int i = 0; i < kills; i++)
	{
		Sleep(2500);
		char kill[] = "\x00\x00\x30\x40\x81\x15\x00\x00\x69\x69\x69\x69";
		DWORD cID = playerID | GetCIDShit(server);
		memcpy(kill + 8, &cID, 4);
		SendData(MainSock, kill, 12);
		printf("[INFO] %d kills done\n", i + 1);
	}
	if (weap == WeaponType::MELEE)
		meleeDone += kills * multiply;
	else if (weap == WeaponType::RIFLE)
		rifleDone += kills * multiply;
	else if (weap == WeaponType::SHOTGUN)
		shotgunDone += kills * multiply;
	else if (weap == WeaponType::SNIPER)
		sniperDone += kills * multiply;
	else if (weap == WeaponType::GATLING)
		gatlingDone += kills * multiply;
	else if (weap == WeaponType::ZOOKA)
		zookaDone += kills * multiply;
	else if (weap == WeaponType::GRENADE)
		grenadeDone += kills * multiply;

	keepKilling = false;
}
void KillThread()
{
	while (true)
	{
		if (keepKilling && matchRunning)
		{
			if (deathsDone == deathsConfig)
				deathsSet = 0;
			else if ((deathsConfig - deathsDone) <= KillDeathConfig)
				deathsSet = deathsConfig - deathsDone;
			else if ((deathsConfig - deathsDone) <= KillDeathConfig * 71)
				deathsSet = (deathsConfig - deathsDone / 71);
			else if (deathsDone < deathsConfig)
				deathsSet = KillDeathConfig;

			if (assistsDone == assistsConfig)
				assistSet = 0;
			else if ((assistsConfig - assistsDone) <= KillDeathConfig)
				assistSet = assistsConfig - assistsDone;
			else if ((assistsConfig - assistsDone) <= KillDeathConfig * 71)
				assistSet = (assistsConfig - assistsDone / 71);
			else if (assistsDone < assistsConfig)
				assistSet = KillDeathConfig;

			if (headshotsDone == headshotsConfig)
				headshotsSet = 0;
			else if ((headshotsConfig - headshotsDone) <= KillDeathConfig)
				headshotsSet = deathsConfig - headshotsDone;
			else if ((headshotsConfig - headshotsDone) <= KillDeathConfig * 71)
				headshotsSet = (headshotsConfig - headshotsDone / 71);
			else if (headshotsDone < headshotsConfig)
				headshotsSet = KillDeathConfig;

			if (weaponStart == WeaponType::MELEE)
			{
			
				if (meleeDone == MeleeConfig)
					weaponStart = WeaponType::RIFLE;
				else if ((MeleeConfig - meleeDone) <= KillDeathConfig)
					HostKill(WeaponType::MELEE, MeleeConfig - meleeDone, 1);
				else if ((MeleeConfig - meleeDone) <= KillDeathConfig * 71)
					HostKill(WeaponType::MELEE, (MeleeConfig - meleeDone) / 71, 71);
				else if (meleeDone < MeleeConfig)
					HostKill(WeaponType::MELEE, KillDeathConfig, 71);
				else
					weaponStart = WeaponType::RIFLE;

				printf("[INFO] Melee Kills : %d/%d\n", meleeDone , MeleeConfig);
			}
			else if (weaponStart == WeaponType::RIFLE)
			{
				if (rifleDone == RifleConfig)
					weaponStart = WeaponType::SHOTGUN;
				else if ((RifleConfig - rifleDone) <= KillDeathConfig)
					HostKill(WeaponType::RIFLE, RifleConfig - rifleDone, 1);
				else if ((RifleConfig - rifleDone) <= KillDeathConfig * 71)
					HostKill(WeaponType::RIFLE, (RifleConfig - rifleDone) / 71, 71);
				else if (rifleDone < RifleConfig)
					HostKill(WeaponType::RIFLE, KillDeathConfig, 71);
				else
					weaponStart = WeaponType::SHOTGUN;

				printf("[INFO] Rifle Kills : %d/%d\n", rifleDone , RifleConfig);
			}
			else if (weaponStart == WeaponType::SHOTGUN)
			{
				if (shotgunDone == ShotgunConfig)
					weaponStart = WeaponType::SNIPER;
				else if ((ShotgunConfig - shotgunDone) <= KillDeathConfig)
					HostKill(WeaponType::SHOTGUN, ShotgunConfig - shotgunDone, 1);
				else if ((ShotgunConfig - shotgunDone) <= KillDeathConfig * 71)
					HostKill(WeaponType::SHOTGUN, (ShotgunConfig - shotgunDone) / 71, 71);
				else if (shotgunDone < ShotgunConfig)
					HostKill(WeaponType::SHOTGUN, KillDeathConfig, 71);
				else
					weaponStart = WeaponType::SNIPER;

				printf("[INFO] Shotgun Kills : %d/%d\n", shotgunDone , ShotgunConfig);
			}
			else if (weaponStart == WeaponType::SNIPER)
			{
				if (sniperDone == SniperConfig)
					weaponStart = WeaponType::GATLING;
				else if ((SniperConfig - sniperDone) <= KillDeathConfig)
					HostKill(WeaponType::SNIPER, SniperConfig - sniperDone, 1);
				else if ((SniperConfig - sniperDone) <= KillDeathConfig * 71)
					HostKill(WeaponType::SNIPER, (SniperConfig - sniperDone) / 71, 71);
				else if (sniperDone < SniperConfig)
					HostKill(WeaponType::SNIPER, KillDeathConfig, 71);
				weaponStart = WeaponType::GATLING;

				printf("[INFO] Sniper Kills : %d/%d\n", sniperDone , SniperConfig);
			}
			else if (weaponStart == WeaponType::GATLING)
			{
				if (gatlingDone == GatlingConfig)
					weaponStart = WeaponType::ZOOKA;
				else if ((GatlingConfig - gatlingDone) <= KillDeathConfig)
					HostKill(WeaponType::GATLING, GatlingConfig - gatlingDone, 1);
				else if ((GatlingConfig - gatlingDone) <= KillDeathConfig * 71)
					HostKill(WeaponType::GATLING, (GatlingConfig - gatlingDone) / 71, 71);
				else if (gatlingDone < GatlingConfig)
					HostKill(WeaponType::GATLING, KillDeathConfig, 71);
				else
					weaponStart = WeaponType::ZOOKA;

				printf("[INFO] Gatling Kills : %d/%d\n", gatlingDone , GatlingConfig);
			}
			else if (weaponStart == WeaponType::ZOOKA)
			{
				if (zookaDone == ZookaConfig)
					weaponStart = WeaponType::GRENADE;
				else if ((ZookaConfig - zookaDone) <= KillDeathConfig)
					HostKill(WeaponType::ZOOKA, ZookaConfig - zookaDone, 1);
				else if ((ZookaConfig - zookaDone) <= KillDeathConfig * 71)
					HostKill(WeaponType::ZOOKA, (ZookaConfig - zookaDone) / 71, 71);
				else if (zookaDone < ZookaConfig)
					HostKill(WeaponType::ZOOKA, KillDeathConfig, 71);
				weaponStart = WeaponType::GRENADE;

				printf("[INFO] Bazooka Kills : %d/%d\n", zookaDone , ZookaConfig);
			}
			else if (weaponStart == WeaponType::GRENADE)
			{
				if (grenadeDone == GrenadeConfig)
					keepKilling = false;
				else if ((GrenadeConfig - grenadeDone) <= KillDeathConfig)
					HostKill(WeaponType::GRENADE, GrenadeConfig - grenadeDone, 1);
				else if ((GrenadeConfig - grenadeDone) <= KillDeathConfig * 71)
					HostKill(WeaponType::GRENADE, (GrenadeConfig - grenadeDone) / 71, 71);
				else if (grenadeDone < GrenadeConfig)
					HostKill(WeaponType::GRENADE, KillDeathConfig, 71);
				else
					keepKilling = false;

				printf("[INFO] Grenade Kills : %d/%d\n", grenadeDone , GrenadeConfig);
			}
		}
	}
}
void EndMatch(int loopTime, int redpoints, int bluepoints, int melee, int rifle, int shotgun, int sniper, int gatling, int zooka, int grenade, int deaths, int headshots, int assists, DWORD cID)
{
	if (!loopTime)
		loopTime = 1;
	struct PlayerStats
	{
		byte Melee = 0, Rifle = 0, Shotgun = 0, Sniper = 0, Gatling = 0, Bazooka = 0, Grenade = 0;
		byte idk = 0;
		byte kills = 0, deaths = 0, headshots = 0, assists = 0;
		DWORD idk2 = 0;
		DWORD connID = 0;
	};
	struct cEndMatch
	{
		unsigned long pHead = 0x40000000;
		unsigned short pName = 0x3F85;
		byte idk = 0;
		byte Players = 1;
		byte RedPoints = 0;
		byte BluePoints = 0;
		byte Players2 = 1;
		byte Winner = 0;

	}end;
	end.pHead = end.pHead & 0xE003FFFF | ((12 + 20 * loopTime) & 2047) << 18;
	char packet[2048];
	end.Players = loopTime;
	end.Players2 = loopTime;
	memcpy(packet, (char*)&end, sizeof(end));
	PlayerStats player[72];
	for (int i = 0; i < loopTime; i++)
	{
		player[i].Melee = melee;
		player[i].Rifle = rifle;
		player[i].Shotgun = shotgun;
		player[i].Sniper = sniper;
		player[i].Gatling = gatling;
		player[i].Bazooka = zooka;
		player[i].Grenade = grenade;
		player[i].kills = melee + rifle + shotgun + sniper + gatling + zooka + grenade;
		player[i].deaths = deaths;
		player[i].headshots = headshots;
		player[i].assists = assists;
		player[i].connID = cID | GetCIDShit(server);
	}
	memcpy(packet + 12, (char*)&player, 20 * loopTime);
	SendData(MainSock, packet, 12 + 20 * loopTime);

}
void EndMatchAlgorithm()
{
	if (weaponStart == WeaponType::MELEE)
		EndMatch(killsmultiply, 0, killsdone, killsdone, 0, 0, 0, 0, 0, 0, deathsSet, headshotsSet, assistsDone, playerID);
	else if (weaponStart == WeaponType::RIFLE)
		EndMatch(killsmultiply, 0, killsdone, 0, killsdone, 0, 0, 0, 0, 0, deathsSet, headshotsSet, assistsDone, playerID);
	else if (weaponStart == WeaponType::SHOTGUN)
		EndMatch(killsmultiply, 0, killsdone, 0, 0, killsdone, 0, 0, 0, 0, deathsSet, headshotsSet, assistsDone, playerID);
	else if (weaponStart == WeaponType::SNIPER)
		EndMatch(killsmultiply, 0, killsdone, 0, 0, 0, killsdone, 0, 0, 0, deathsSet, headshotsSet, assistsDone, playerID);
	else if (weaponStart == WeaponType::GATLING)
		EndMatch(killsmultiply, 0, killsdone, 0, 0, 0, 0, killsdone, 0, 0, deathsSet, headshotsSet, assistsDone, playerID);
	else if (weaponStart == WeaponType::ZOOKA)
		EndMatch(killsmultiply, 0, killsdone, 0, 0, 0, 0, 0, killsdone, 0, deathsSet, headshotsSet, assistsDone, playerID);
	else if (weaponStart == WeaponType::GRENADE)
		EndMatch(killsmultiply, 0, killsdone, 0, 0, 0, 0, 0, 0, killsdone, deathsSet, headshotsSet, assistsDone, playerID);
}

static clock_t roomStartedTime = clock();
clock_t roomTimeNow = clock();
static clock_t roomSendPingTime = clock();

void PingThread()
{
	while (true)
	{
		if (connectedMain && roomCreated)
		{
			Sleep(5000);
			char ping[] = "\x00\x01\x30\x40\xD2\x11\x00\x03\x00\xC8\x00\x00";
			SendData(MainSock, ping, 12);	
		}
	}
}
void RoomThread()
{
	
	while (true)
	{
		if (roomCreated && !matchRunning)
		{
			char startmatch[] = "\x40\x06\x20\x40\xC2\x1A\x26\x00";
			SendData(MainSock, startmatch, 8);
			char startmatch2[] = "\x40\x06\x20\x40\xC2\x1A\x29\x00";
			SendData(MainSock, startmatch2, 8);
			matchRunning = true;
			roomStartedTime = clock();
			printf("[INFO] Match started\n");
			keepKilling = true;
		}
		else if (matchRunning)
		{
			int time = timeMinConfig * 60000 + 2000;
			Sleep(time); // 2 extra seconds just incase
			//EndMatchAlgorithm();
			EndMatch(20, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, playerID);
			matchRunning = false;
			printf("[INFO] Match ended\n");
		}
	}
}
void TitleThread()
{
	char title[32] = "FDP BotsManager";
	
	while (true)
	{
		if (matchRunning && connectedMain)
			sprintf(title, "Remaining time %d seconds", ((timeMinConfig * 60000 + 2000) - (clock() - roomStartedTime))/1000);
		else if(!matchRunning && connectedMain)
			sprintf(title, "Match not running");

		SetConsoleTitle(title);
		Sleep(1);
	}
}
int main()
{
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)TitleThread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)PingThread, 0, 0, 0);
	//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)RoomThread, 0, 0, 0);
	//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)KillThread, 0, 0, 0);
	

	DWORD PacketName = 0;
	uint64_t authKey = 0;
	WSADATA wsa;
	
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		printf("[ERROR] Couldn't initialize wsa\n");

	


		//Console::PrintErr(0, "Couldn't initialize wsa\n");
	
	char username[68] = "";
	char password[40] = "";

	
	printf("Enter your login id: ");
	std::cin >> username;
	printf("Enter your login password: ");
	std::cin >> password;
	Console::Print(Console::CColor::Light, 0, "\n");
	printf("\n");
	if (CCryptographyModule == NULL)
		printf("[ERROR] Couldn't find CCryptography.dll\n");
		//Console::PrintErr(0, "Couldn't find CCryptography.dll\n");

	srand(time(NULL));
	
	if (!SetupConnFront(Regions::EU))
		printf("[ERROR] Couldn't connect to Front Server\n");
	else
		printf("[INFO] Connected to Front Server\n");

	while (true)
	{
		char buff[0xC0000];
		int len = 0xC0000;

		while ((len = 0xC0000) && AwaitData(FrontSock, buff, len) && (!loginSuccess))
		{
			memcpy(&PacketName, buff + 4, 2);
			PacketName = PacketName >> 4;
			Console::PrintPackets("RECV FRONT", buff, len, PacketName);
			if (len == 16 && PacketName == 1604)
			{
				DWORD Key = NULL;
				memcpy(&Key, buff + 8, 4);
				ClientHandle = CreateHandle(Key);

				char login[] = "\x00\x00\xF0\x81\x84\x05\x34\x00\x6F\xA8\xDD\x07\x31\x32\x33\x34\x35\x36\x37\x38\x39\x30\x00\x5D\x43\x57\xD5\x8C\x16\x18\xE3\x93\x1F\xA5\x3B\x8B\x2F\xB2\xDF\xA1\x9A\x34\x97\x1F\x3A\x0C\x05\xB7\x98\x11\xBB\x00\x65\xD9\xD1\x5C\x43\x4F\x4D\x45\x44\x49\x53\x43\x4F\x52\x44\x39\x37\x38\x34\x00\x6E\x50\x48\xA7\xE5\xEB\x76\xE2\x5F\x7A\x79\x15\x69\xA5\xC0\x08\xC8\x26\x87\x6A\xD2\x5C\x13\xAF\xED\x63\xA8\xCA\x73\x13\x9E\x9D\x89\x04\x73\x57\x33\x19\x4F\x87\x1F\x98\xD9\x3E\x7D\x5B\xD3\xEA\x70\x18\x49\x00";
				memcpy(login + 8, buff + 8, 4);
				memcpy(login + 12, password, strlen(password) + 1);
				memcpy(login + 52, buff + 12, 4);
				memcpy(login + 56, username, strlen(username) + 1);
				SendData(FrontSock, login, 124);
			}
			else if (len == 16 && PacketName == 89)
			{
				char loggouthim[] = "\x00\x00\x40\x80\x41\x06\x00\x00\x81\x3A\xB0\x71\x5C\x1F\x6F\x54";
				memcpy(loggouthim + 8, buff + 8, 8);
				SendData(FrontSock, loggouthim, 16);
				printf("[INFO] Someone was already on the account, but you logged him out fuck him.\n");
			}
			else if (len == 88)
			{
				//Console::Print(Console::CColor::Lime, 0, "[INFO] Successfully logged in account name [%s].\n", std::string(buff + 48, 16).c_str());
				printf("[INFO] Successfully logged in account name [%s].\n", std::string(buff + 48, 16).c_str());
				char answer[] = "\x00\x00\x20\x00\xC5\x05\x00\x02";
				SendData(FrontSock, answer, 8);
				memcpy(&authKey, buff + 8, 8);
				loginSuccess = true;
			}
			else if (len == 8 && memcmp(buff + 6, "\x2A", 1) == 0)
			{
				printf("[ERROR] Account is banned\n");
			}
			else if (len == 8 && memcmp(buff + 6, "\x00", 1) == 0)
			{
				printf("[ERROR] ID/PW is wrong\n");
			}
		}
		if (loginSuccess && !connectedMain)
		{
			//Console::PrintErr(0, "Couldn't connect to Main Server\n");
			if (!SetupConnMain(server))
				printf("[ERROR] Couldn't connect to Main Server\n");	
			else
			{
				connectedMain = true;
				//Console::Print(Console::CColor::Lime,0, "[INFO] Connected to Main Server\n");
				printf("[INFO] Connected to Main Server\n");
			}
		}

		while ((len = 0xC0000) && AwaitData(MainSock, buff, len) && (loginSuccess))
		{
			memcpy(&PacketName, buff + 4, 2);
			PacketName = PacketName >> 4;
			if (len != 1036)
				Console::PrintPackets("RECV MAIN", buff, len, PacketName);

			if (len == 16 && PacketName >= 1600)
			{
				DWORD Key = NULL;
				memcpy(&Key, buff + 8, 4);
				ChangeKey(ClientHandle, Key);
				char answer[] = "\xD0\x04\x60\x80\x0F\x11\x25\x00\x32\x40\x21\x3A\x97\x4C\x1F\x0E\xA9\xFE\x15\x9A\x01\x01\x01\x00";
				memcpy(answer+8, &authKey, 8);
				memcpy(&playerID, buff + 12, 4);
				SendData(MainSock, answer, 24);
			}
			else if (len == 1036 && PacketName == 324)
			{
				char hb[] = "\x90\x01\x30\x70\x41\x14\x34\x05\x00\x04\x00\x00\x4E\x00\x00\x00\xE0\x03\x00\x00\x04\x00\x00\x00\x52\xE9\xCF\x2B\x3F\x49\x00\x1A\xC4\x91\x5C\xEC\xB1\xBC\xF0\x0F\x20\x55\x36\x1A\x03\xCB\x18\xCF\xDA\x5D\x57\xFC\x46\x80\xE5\x6F\x22\xBE\xB5\x1F\xF1\xE5\x91\xDD\x85\xDA\x46\x39\x89\xBA\xDB\x62\xB2\x84\x25\xD0\x79\x95\xEC\xF5\x12\x0D\xB2\x34\x55\xF3\x53\x73\x75\xB4\x82\x49\x1E\x60\xAC\x23\x87\x15\x3A\xC2\x11\x01\xF6\x5B\xE8\xCE\x40\x81\xE8\x0E\x59\xF5\x84\x89\x25\xCA\xFC\x67\x78\xB1\xA5\xAD\x5A\xDA\x3A\x7E\x59\x8E\xD3\xD5\x62\xD1\x7B\x09\x71\xE4\x51\x2D\x0C\xA5\xD4\x5D\x0B\x2C\x29\x9C\xF2\xAB\x20\x85\xB6\x08\x6C\x3F\xD4\xB1\xA4\x31\x5B\x3B\x31\xFD\x71\xFD\x91\x93\xD5\x37\x1F\x2E\x49\x8E\x74\x0B\xEB\x0B\x8F\xE9\x64\xF6\x97\x37\x92\x71\x3A\xC5\xC9\x85\x9C\x49\x8B\x37\x33\x91\x8B\x39\x36\x43\xD4\xA5\xF5\xAB\x3B\xBA\x78\xDA\xE5\x04\x32\x12\x41\xF2\x28\x08\x51\x59\x55\xE2\x38\x84\x38\xD8\x53\x95\x7E\xE4\xF6\x96\x0E\x02\x4C\xA4\x0E\xB0\x80\xC5\x7A\x8B\x31\xD5\xBC\x0E\xC2\x84\x6E\x6F\x78\x49\x6E\x4B\xA5\xFB\xB5\xF0\x26\x1D\xFF\xA6\xDE\xBF\x18\xDA\xFA\x1E\x79\xA8\x01\x87\xC5\xC8\x14\x2F\xA0\x1F\x55\xD1\x69\xBD\xD7\xFB\xBA\x34\xF0\x13\x45\xE5\x02\x97\xCC\x50\xEA\x6D\x4D\x42\x11\x50\xFF\x76\xC4\x99\xA6\x83\x97\xF9\x0A\x1C\x64\x9B\xB1\xED\xE7\x65\xCA\xC8\x89\x96\xBA\x2F\x39\xD9\x20\xD6\x6F\xDA\x90\x77\xB4\x9A\xFF\xAE\x55\xFB\xFC\x83\x20\x57\xC1\x45\x27\x20\x4A\x55\x00\xC8\x57\xED\x3F\xA6\xB8\x9A\xF9\xDA\x89\xA5\xAB\x06\xB2\x6A\x63\x78\xEB\xBC\x74\xA4\xF8\xD9\x16\x7A\x66\x78\xC0\x79\xB1\x30\xD7\x70\x12\xA1\xDE\x89\xF9\x0F\xB5\x4E\xE6\xCB\xF1\xBF\xA4\xB0\xDF\x60\x00\x7B\xD2\xF1\xE4\x07\xB0\x2B\xF7\x7F\x90\x14\x36\x12\xAF\x98\x01\xE5\x9F\x2D\xF0\xAB\x54\x6D\xBF\xAE\x2F\x68\x75\xA0\x75\x1A\x37\x35\x8C\x64\x8F\x1D\x1D\x2E\x7B\x2B\x13\x3C\x40\xC2\x74\x07\xB1\xB5\xA2\x30\xF0\xFA\x0B\x0A\xB2\xFC\x44\x66\x3C\x89\xBA\xBA\xB5\xA9\x69\x44\x23\xCD\x5C\x72\x10\xBE\xC5\x91\x87\xF3\x8C\x0A\x6B\xCB\x15\xD0\xE2\xFD\x56\x1A\xC9\xAA\x34\x46\x43\xDC\xD6\x90\x43\x3D\x89\xE0\xE0\xE2\x87\x6D\x3E\x89\xB9\xD5\xC7\xA3\x7A\x81\x68\x6D\x8D\x33\x74\x90\xF7\x11\x39\xD6\xED\x5B\xFD\x78\x89\xF0\x58\x36\x8F\x89\x0B\xDE\xC1\x8B\x5C\xCA\xD0\x4F\xA4\xED\xD7\x6F\xDC\xCD\x8A\x2A\xD5\xDA\xA5\x3C\x43\x3C\xD8\x5B\xED\xA3\xA5\xC4\x9F\x54\x8C\xD8\x57\xD6\x56\xB7\x27\xA2\x5A\x0B\xD0\xFA\xF6\xCD\xB6\x07\x14\x92\x97\x14\x29\xF6\xE2\x1E\xAE\x7A\xEC\x5E\x62\xAB\xF5\x5D\x85\xDF\x75\xE5\x59\x43\x01\x47\xAB\x40\x85\xD2\xA9\x5E\xB2\x5B\x10\x14\x89\x81\x66\x0B\xB3\xFB\xDD\x40\x78\x88\x19\xCF\xC9\x9C\x7E\xEC\x73\x54\x01\x4A\x90\xF8\x6B\xD7\x2E\x12\xED\xEA\xC5\x6E\xC3\x1C\xDB\xE0\x19\x65\x90\xD3\x4E\xCF\x0D\xDF\xC4\xD8\x99\x3E\x94\x4D\x84\x87\x96\x02\x25\xDE\x83\xAC\xE2\x1C\xE7\x26\x6F\xD5\xA1\x31\xD7\x4C\x94\xA1\x3F\xFE\x77\x4F\x12\x66\xDB\x71\xCF\x68\xA0\xC8\x65\x64\x32\xB3\x62\x74\x96\x76\xA6\xB8\x29\x6C\x66\xBC\x14\x6A\x41\x45\x6B\x5D\x71\xAB\xE0\x8D\xD9\xEF\xA3\x7E\xDF\xB4\x9E\xE8\x49\x00\x7A\x34\x8F\xC6\x39\x04\x2B\xCB\x30\x56\xE7\x87\xA9\x94\x06\x99\x6F\xFE\xBB\xC8\xCF\x93\xD8\x74\x70\x43\x0C\xCA\x6B\xFE\x48\xAB\x70\xAA\xAF\x12\x8E\x8B\x01\x31\x5E\x29\x18\x94\x5D\xC5\x7C\x9C\x9D\x91\xA8\x92\xE4\xA3\x49\xD8\x23\x08\xA1\x7A\xE9\xFD\x3F\x9B\xFE\xF2\x9C\xC2\x25\x2A\xED\x2E\x1A\x78\x36\xF6\xCD\xE7\xD8\xBE\x79\x4C\xE2\x1F\x05\x18\x35\xBA\xFD\x86\xBB\x30\x96\xCB\xEB\xAF\xAC\x84\x99\x4E\xA9\x4A\x66\x10\x34\x5D\xE7\xF2\x5D\x7D\x83\x2A\x52\xA7\xC9\x2A\x9E\x8F\x9D\x31\xFA\xD9\xBA\xEB\x52\x6E\xFC\x09\xE1\xD9\x71\x7E\x69\xA5\x76\xFF\x0B\x44\x8F\x85\x5D\xCE\x45\xCF\x1C\xF7\xCC\x08\xF6\x38\xC3\xF9\x33\x36\x46\xF5\x58\x95\x8C\x2D\x97\xC9\xB1\xA2\xDB\x3F\x1B\x67\xE3\x9F\xFA\xD2\xE0\x1B\x18\x00\x47\xD0\x3E\x78\x20\xD4\x29\x07\x55\xFB\xF6\xBE\x13\x60\x77\x89\x90\x3D\x79\xE9\x36\xC0\x87\xA6\x9D\x21\x76\x9B\xA5\xA0\xFB\x8A\x76\x0D\x84\x6B\x4C\x31\xEE\xC7\xAA\x1E\x0A\xE3\xB2\xF0\xCC\x35\x79\x9A\x06\xE8\xE2\x5E\x83\xC8\xFA\x42\x70\x93\x85\x71\x11\x24\x7D\xFB\xE7\x94\xEC\xC5\x0A\x48\x4F\x85\xB3\x73\x82\x3E\x73\x5F\x23\x48\x1D\x2B\xEC\xED\xF3\x77\x16\xFA\x08\x10\x7A\x97\x1E\xDB\xE2\xF2\x0F\x2D\x47\x96\xD9\xE0\xEE\x45\x6E\xEF\x14\x7F\x0B\x26\x79\x53\x67\xEA\xC4\x22\xA1\x2A\xFC\x48\x2A\xE4\xBA\x91\xD9\x49\xB5\xCC\xCF\x18\x5C\xCA\xF6\x31\xF5\x21\x97\x5B\xBB\xEF";
				SendData(MainSock, hb, 1036);
				printf("[INFO] Sent XignCode Heartbeat\n");
				//Console::Print(Console::CColor::Lime, 0, "[INFO] Sent Xigncode Heartbeat\n");
			}
			else if (len == 264 && PacketName == 1652)
			{
				memcpy(&deathsDone, buff + 20, sizeof(int));
				memcpy(&assistsDone, buff + 24, sizeof(int));
				memcpy(&meleeDone, buff + 40, sizeof(int));
				memcpy(&rifleDone, buff + 44, sizeof(int));
				memcpy(&shotgunDone, buff + 48, sizeof(int));
				memcpy(&sniperDone, buff + 52, sizeof(int));
				memcpy(&gatlingDone, buff + 56, sizeof(int));
				memcpy(&zookaDone, buff + 60, sizeof(int));
				memcpy(&grenadeDone, buff + 64, sizeof(int));
				memcpy(&headshotsDone, buff + 68, sizeof(int));
				headshotsDone = (headshotsDone - (2 & 0x07FFFFFF)) >> 0x1B;

				char crtRoom[] = "\xC0\x01\xF0\x40\x8E\x22\x05\x00\xFE\x8E\x85\xA3\x4C\x61\x67\x20\x73\x65\x72\x76\x65\x72\x2E\x2E\x2E\x2E\x2E\x2E\x2E\x2E\x2E\x2E\x2E\x2E\x2E\x00\x00\x00\x00\x00\x00\x00\x00\x00\x33\x32\x33\x32\x33\x32\x00\x35\x35\x22\x20\x66\x6F\x6E\x74\x00";
				//memcpy(crtRoom + 12, roomTitle, 30);
				//memcpy(crtRoom + 44, roomPass, 8);
				SendData(MainSock, crtRoom, 60);
				printf("[INFO] Created room\n");
				roomCreated = true;
			}
			else if (len == 8 && PacketName == 294)
			{
				printf("[ERROR] You got logged out.\n");
			}
		}

		Sleep(1);
	}
	WSACleanup();

	std::cin.get();
	return 0;
}