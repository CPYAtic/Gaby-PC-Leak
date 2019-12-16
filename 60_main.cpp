#include <Windows.h>
#include "Hooks.h"
#include "CSetAim.h"
#include "CPacket.h"


int prew = 0;

#pragma region Cheat

void Cheat()
{
	for (;;)
	{
		if (ua)
		{
			DWORD UaMeleeOff[] = { 0x7c, 0x10c, 0x28, 0x4c, 0x20c };
			DWORD UaRifleOff[] = { 0x7c, 0x10c, 0x28, 0x50, 0x20c };
			DWORD UaShotgunOff[] = { 0x7c, 0x10c, 0x28, 0x54, 0x20c };
			DWORD UaSniperOff[] = { 0x7c, 0x10c, 0x28, 0x58, 0x20c };
			DWORD UaGatlingOff[] = { 0x7c, 0x10c, 0x28, 0x5C, 0x20c };
			DWORD UaBazookaOff[] = { 0x7c, 0x10c, 0x28, 0x60, 0x20c };
			DWORD UaGrenadeOff[] = { 0x7c, 0x10c, 0x28, 0x64, 0x20c };
			DWORD MeleeAmmo = Func.GetDMA(WeaponBase, UaMeleeOff, 5);
			DWORD RifleAmmo = Func.GetDMA(WeaponBase, UaRifleOff, 5);
			DWORD ShotgunAmmo = Func.GetDMA(WeaponBase, UaShotgunOff, 5);
			DWORD SniperAmmo = Func.GetDMA(WeaponBase, UaSniperOff, 5);
			DWORD GatlingAmmo = Func.GetDMA(WeaponBase, UaGatlingOff, 5);
			DWORD BazookaAmmo = Func.GetDMA(WeaponBase, UaBazookaOff, 5);
			DWORD GrenadeAmmo = Func.GetDMA(WeaponBase, UaGrenadeOff, 5);
			if (ValidatePointer(MeleeAmmo))
				*(int*)MeleeAmmo = *(int*)AmmoXOR ^ 1337;
			if (ValidatePointer(RifleAmmo))
				*(int*)RifleAmmo = *(int*)AmmoXOR ^ 1337;
			if (ValidatePointer(ShotgunAmmo))
				*(int*)ShotgunAmmo = *(int*)AmmoXOR ^ 1337;
			if (ValidatePointer(SniperAmmo))
				*(int*)SniperAmmo = *(int*)AmmoXOR ^ 1337;
			if (ValidatePointer(GatlingAmmo))
				*(int*)GatlingAmmo = *(int*)AmmoXOR ^ 1337;
			if (ValidatePointer(BazookaAmmo))
				*(int*)BazookaAmmo = *(int*)AmmoXOR ^ 1337;
			if (ValidatePointer(GrenadeAmmo))
				*(int*)GrenadeAmmo = *(int*)AmmoXOR ^ 1337;
		}
		if (uar)
		{
			DWORD UarMeleeOff[] = { 0x7c, 0x10c, 0x28, 0x4c, 0x210 };
			DWORD UarRifleOff[] = { 0x7c, 0x10c, 0x28, 0x50, 0x210 };
			DWORD UarShotgunOff[] = { 0x7c, 0x10c, 0x28, 0x54, 0x210 };
			DWORD UarSniperOff[] = { 0x7c, 0x10c, 0x28, 0x58, 0x210 };
			DWORD UarGatlingOff[] = { 0x7c, 0x10c, 0x28, 0x5C, 0x210 };
			DWORD UarBazookaOff[] = { 0x7c, 0x10c, 0x28, 0x60, 0x210 };;
			DWORD UarGrenadeOff[] = { 0x7c, 0x10c, 0x28, 0x64, 0x210 };
			DWORD MeleeReloadAmmo = Func.GetDMA(WeaponBase, UarMeleeOff, 5);
			DWORD RifleReloadAmmo = Func.GetDMA(WeaponBase, UarRifleOff, 5);
			DWORD ShotgunReloadAmmo = Func.GetDMA(WeaponBase, UarShotgunOff, 5);
			DWORD SniperReloadAmmo = Func.GetDMA(WeaponBase, UarSniperOff, 5);
			DWORD GatlingReloadAmmo = Func.GetDMA(WeaponBase, UarGatlingOff, 5);
			DWORD BazookaReloadAmmo = Func.GetDMA(WeaponBase, UarBazookaOff, 5);
			DWORD GrenadeReloadAmmo = Func.GetDMA(WeaponBase, UarGrenadeOff, 5);
			if (ValidatePointer(MeleeReloadAmmo))
				*(int*)MeleeReloadAmmo = *(int*)AmmoXOR ^ 1337;
			if (ValidatePointer(RifleReloadAmmo))
				*(int*)RifleReloadAmmo = *(int*)AmmoXOR ^ 1337;
			if (ValidatePointer(ShotgunReloadAmmo))
				*(int*)ShotgunReloadAmmo = *(int*)AmmoXOR ^ 1337;
			if (ValidatePointer(SniperReloadAmmo))
				*(int*)SniperReloadAmmo = *(int*)AmmoXOR ^ 1337;
			if (ValidatePointer(GatlingReloadAmmo))
				*(int*)GatlingReloadAmmo = *(int*)AmmoXOR ^ 1337;
			if (ValidatePointer(BazookaReloadAmmo))
				*(int*)BazookaReloadAmmo = *(int*)AmmoXOR ^ 1337;
			if (ValidatePointer(GrenadeReloadAmmo))
				*(int*)GrenadeReloadAmmo = *(int*)AmmoXOR ^ 1337;
		}
		
		if (isFiringRate)
		{
			DWORD FiringOffs[] = { 0x6c };
			DWORD FiringRate = Func.GetDMA(FiringBase, FiringOffs, 1);
			if (ValidatePointer(FiringRate))
				Write.Integer(FiringRate, 0);
		}
		if (isMeleeRange)
		{
			DWORD MeleeRangeOffs[] = { 0x7C, 0x10C, 0x4, 0x124 };
			DWORD MeleeRange = Func.GetDMA(WeaponBase, MeleeRangeOffs, 4);
			if (ValidatePointer(MeleeRange))
				Write.Integer(MeleeRange, 999999999);
		}
		if (bypass_censore)
		{
			Write.Bytes(0x005874D9, "\x90\x90\x90\x90\x90", 5); //censore chat  on
			Write.Bytes(0x0064FC48, "\xEB", 1); //Censore title in room edit
			Write.Bytes(0x005D0CA8, "\xEB", 1); //Censore title in create room
			Write.Bytes(0x0064FE42, "\xEB", 1); //Censore password in room edit
			Write.Bytes(0x005D0E95, "\xEB", 1); //Censore password in create room
		}
		else
		{
			Write.Bytes(0x005874D9, "\xE8\xE2\x86\x05\x00", 5); //censore chat off
			Write.Bytes(0x0064FC48, "\x74", 1); //Censore title in room edit off
			Write.Bytes(0x005D0CA8, "\x74", 1); //Censore title in create room off
			Write.Bytes(0x0064FE42, "\x74", 1); //Censore password in room edit off
			Write.Bytes(0x005D0E95, "\x74", 1); //Censore password in create room off
		}
		if (bypass_penalty)
		{
			//Write.Bytes(0x0058168E, "\xE9\x87\x01\x00\x00\x90", 6);
			Write.Bytes(0x0058168E, "\x33\xC0\x90", 3);
		}
		else
		{
			Write.Bytes(0x0058168E, "\x83\xC0\x01", 3);
		}
		
		Sleep(1);
	}
}
#pragma endregion Here is our cheat codenz
#pragma region PrintInfo
void PlayerInfos()
{
	std::cout << "#1 %s" << *(char*)PlayerName << "X: %f" << *(float*)PlayerX << "Y: %f" <<*(float*)PlayerY <<"Z: %f" <<*(float*)PlayerZ <<std::endl;
	std::cout << "#2 %s" << *(char*)EnemyName << "X: %f" << *(float*)EnemyX << "Y: %f" << *(float*)EnemyY << "Z: %f" << *(float*)EnemyZ << std::endl;
	std::cout << "#3 %s" << *(char*)Enemy2Name << "X: %f" << *(float*)Enemy2X << "Y: %f" << *(float*)Enemy2Y << "Z: %f" << *(float*)Enemy2Z << std::endl;
	std::cout << "#4 %s" << *(char*)Enemy3Name << "X: %f" << *(float*)Enemy3X << "Y: %f" << *(float*)Enemy3Y << "Z: %f" << *(float*)Enemy3Z << std::endl;
	std::cout << "#5 %s" << *(char*)Enemy4Name << "X: %f" << *(float*)Enemy4X << "Y: %f" << *(float*)Enemy4Y << "Z: %f" << *(float*)Enemy4Z << std::endl;
	std::cout << "#6 %s" << *(char*)Enemy5Name << "X: %f" << *(float*)Enemy5X << "Y: %f" << *(float*)Enemy5Y << "Z: %f" << *(float*)Enemy5Z << std::endl;
}
#pragma endregion


BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwAttached, LPVOID lpvReserved)
{
	if (dwAttached == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Cheat, NULL, NULL, NULL);
		oSendLobbyChatMessage = (tSendLobbyChatMessage)DetourFunction((PBYTE)dwSendChatMessage, (PBYTE)hkSendChatMessage, 5); //Hooking chat messages
		Func.DetourFunction((void*)WRITECOORDS, Wallwalk, 32); //wallwalk	
		//Func.DetourFunction((void*)0x00E2E2DA, HkSend, 5); //Printing Send() packets
		
	}
	return FALSE;
}

