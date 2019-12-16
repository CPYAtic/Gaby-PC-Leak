#include "CAim.h"
#include "SDK.h"
UINT_PTR GetDMA(UINT_PTR BaseAddress, DWORD * dwOffsets, UINT PointerLevel);
double Dist3D(POS_3D * a, POS_3D * b);
bool GetAngles(POS_3D * own, POS_3D * target, float & pitch, float & yaw);

UINT_PTR GetDMA(UINT_PTR BaseAddress, DWORD * dwOffsets, UINT PointerLevel)
{
	//An address being != 0 is no safe check, VirtualQuery would be better
	if (!BaseAddress)
		return 0;

	UINT_PTR Buffer = *reinterpret_cast<UINT_PTR*>(BaseAddress);

	for (UINT i = 0; i != PointerLevel - 1 && Buffer; ++i, ++dwOffsets)
		Buffer = *reinterpret_cast<UINT_PTR*>(Buffer + *dwOffsets);

	if (!Buffer)
		return 0;

	return (Buffer + *dwOffsets);
}

UINT_PTR FirstPlayerbase = 0x011B9E50;
DWORD of_pHP[] = { 0x4c, 0x10c, 0x4, 0xc };
DWORD of_pHPXOR[] = { 0x4c, 0x10c, 0x4, 0x14 };
DWORD of_pTeamFlag[] = { 0x12, 0x34, 0x56 };
DWORD of_pPos[] = { 0x7C, 0x124, 0x638, 0x108 };
DWORD of_pPosY[] = { 0x4c, 0x124, 0x638, 0x118};
DWORD of_pPosX[] = { 0x4c, 0x124, 0x638, 0x108 };
DWORD of_pPosZ[] = { 0x4c, 0x124, 0x638, 0x110 };

UINT_PTR OwnRankBase = 0x011B5FBC;
DWORD of_Rank[] = { 0x7c, 0x10c, 0x1c, 0xc };
DWORD of_Pitch[] = { 0x7C, 0x124, 0xA8 };
DWORD of_Yaw[] = { 0x7C, 0x10C, 0x4, 0xB38 };

DWORD PitchAngle = GetDMA(OwnRankBase, of_Pitch, 3);
DWORD YawAngle = GetDMA(OwnRankBase, of_Yaw, 4);

UINT_PTR OwnRank = GetDMA(OwnRankBase, of_Rank, 4);

DWORD RoomPlayerCountBase = 0x011B5FC0;
DWORD of_RoomPlayerCount[] = { 0xD4 };
DWORD RoomPlayerCount = GetDMA(RoomPlayerCountBase, of_RoomPlayerCount, 1);
PlayerInfo PlayerData[16];

double Dist3D(POS_3D * a, POS_3D * b)
{
	double dx = a->x - b->x;
	double dy = a->y - b->y;
	double dz = a->z - b->z;
	return sqrt(dx * dx + dy * dy + dz * dz);
}

bool GetAngles(POS_3D * own, POS_3D * target, float & pitch, float & yaw)
{
	double dx = own->x - target->x;
	double dy = own->y - target->y;
	POS_3D delta[3] = { target->x - own->x, target->z - own->z, target->y - own->y };
	double hyp = sqrt(dx * dx + dy * dy);
	
	//convert the delta vector to angles, google
}




bool PlayerInfo::GetPlayerInfo(UINT_PTR PlayerBase)
{
	/*UINT_PTR InfoAddress = GetDMA(PlayerBase, of_pTeamFlag, 3);
	if (!InfoAddress)
		return false;
		
	IsEnemy = true; // add your own check for the IsEnemy/team flag, maybe a global team flag is needed

	UINT_PTR HP_addy = GetDMA(PlayerBase, of_pHP, 4);
	UINT_PTR HPXOR_addy = GetDMA(PlayerBase, of_pHPXOR, 4);
	int Health = *(int*)HP_addy ^ *(int*)HPXOR_addy;
	if (!HP_addy)
	{
		IsAlive = false;
		return false;
	}
	if (Health != 0)
	{
		IsAlive = true; // do the xor shit here
	}
	*/

	UINT_PTR PosAddress = GetDMA(PlayerBase, of_pPos, 4);
	if (!PosAddress)
		return false;

	Pos = reinterpret_cast<POS_3D*>(PosAddress); // just store a pointer to the xzy coordinates
}

bool Aimbot()
{
	//read own team first

	PlayerInfo * pOwnInfo = nullptr;

	UINT closest = MAX_PLAYER_COUNT; // invalid value
	double closest_dist = 0.0f;
	for (UINT i = 0; i != *reinterpret_cast<DWORD*>(RoomPlayerCount); ++i)
	{
		if (PlayerData[i].GetPlayerInfo(FirstPlayerbase + i * 4))
			if (i == *reinterpret_cast<DWORD*>(OwnRank))
				pOwnInfo = &PlayerData[i];
	};

	if (!pOwnInfo)
		return false;

	for (UINT i = 0; i != *reinterpret_cast<DWORD*>(RoomPlayerCount); ++i)
	{
		//if (PlayerData[i].IsEnemy && PlayerData[i].IsAlive)
		
			double dist = Dist3D(PlayerData[i].Pos, pOwnInfo->Pos);
			if (dist < closest_dist || !closest_dist)
			{
				closest_dist = dist;
				closest = i;
			}
		
	}

	float Pitch = 0.0f;
	float Yaw = 0.0f;


	CalcAngle(pOwnInfo->Pos, PlayerData[closest].Pos, Pitch, Yaw);
	if (aimbut)
	{
		if (ValidatePointer(PitchAngle))
			Write.Float(PitchAngle, Pitch);
		if (ValidatePointer(YawAngle))
			Write.Float(YawAngle, Yaw);
	}
	//write angles

	return true;
}
