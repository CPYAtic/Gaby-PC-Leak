#include <Windows.h>
#include "SDK.h"
typedef int(__cdecl* tCreateRoom)(int, DWORD, DWORD*, LPCSTR, LPCSTR);

typedef struct tRoomSettings
{
	byte time, weaponds, map, rules, players;
};

tCreateRoom oCreateRoom = reinterpret_cast<tCreateRoom>(0x00B21310);

class CRoom
{
public:
	enum Weaponds : byte
	{
		All = 0xDD,
		Select = 0xE5,
		MeleeOnly = 0xC1,
		RifleOnly = 0xC5,
		ShotgunOnly = 0xC9,
		SniperOnly = 0xCD,
		GatlingOnly = 0xD1,
		BazookaOnly = 0xD5,
		GrenadeOnly = 0xD9
	};
	enum Kills : byte
	{
		//FFA, TDM, ItemMatch, SIM, SniperMode, CloseCombat
		Kills10 = 0x21, //A.I only
		Kills20 = 0x22,
		Kills40 = 0x24, //FFA only
		Kills30 = 0x23,
		Kills50 = 0x25,
		Kills80 = 0x28,
		Kills100 = 0x2A,
		Kills130 = 0x2D,
		Kills150 = 0x2F,

		//Arms Race
		Points20 = 0x34,

		//Enlimination, ZombieMode, BombBattle
		Rounds3 = 0x23,
		Rounds5 = 0x25,
		Rounds7 = 0x27,
		Rounds9 = 0x29,

		//Square
		NoGoals = 0x21,

		//Scrimmage
		HP10000 = 0x21,
		HP20000 = 0x22,
		HP30000 = 0x23,

		//BossBattle
		BossCleare = 0x21,

		//CTB
		Points3 = 0x23,
		Points5 = 0x25,
		Points7 = 0x27,
		Points9 = 0x29
	};
	enum Time : byte
	{
		Mins10 = 0x40,
		Mins15 = 0xE0,
		Mins20 = 0x80,
		Mins25 = 0x20,
		Mins30 = 0xC0,

		Mins2 = 0x40,
		Mins3 = 0x60,
		Mins4 = 0x80,
		Mins5 = 0xA0,

		Unlimited = 0x00
	};
	enum Mode : byte
	{
		TDM = 0,
		FFA = 1,
		ItemMatch = 2,
		CTB = 3,
		CloseCombat = 4,
		Enlimination = 5,
		SIM = 6,
		ZombieMode = 7,
		ArmsRace = 8,
		Scrimage = 9,
		BombBattle = 10,
		SniperMode = 11,
		SquareMode = 12,
		BossBattle = 13,
		AIMode = 14
	};
	enum Map : byte
	{
		Random = 0,
		Chess = 1,
		ToyFleet = 2,
		TrackersFactory = 3,
		Beach = 4,
		BattleMine = 5,
		ToyGarden = 6,
		Neighbothood = 7,
		MagicPaperLand = 8,
		HobbyShop = 9,
		Academy = 10,
		TheStudio = 11,
		//12 doesn't exsist
		PVCFactory = 13,
		HauseTop = 14,
		WildWest = 15,
		RumpusRoom = 16,
		Cargo = 17,
		PVCFacoryNight = 18,
		ForgottenJunkYard = 19,
		JunkYard = 20,
		GothicCastle = 21,
		Bitmap = 22,
		RockBand = 23,
		//24, 25, 26 doesn't exsist
		ModelShip = 27,
		AcademyInvasion = 28,
		Foosball = 29,
		Bitmap2 = 30,
		ToyGarden2 = 31,
		RockBandS = 32,
		RockBandW = 33,
		CastleSiege = 34,
		TempleRuins = 35,
		BitmapPlant = 36,
		TheAftermath = 37,
		//38, 39, 40, 41, 42 doesn't exsist
		SinglePlayerEasy = 43,
		SinglePlayerHard = 44,
		BossMap = 45,
		SquareMap = 46,
		SquareMap2 = 47
	};
	enum Players : byte
	{
		Players2 = 2,
		Players4 = 4,
		Players6 = 6,
		Players8 = 8,
		Players10 = 10,
		Players12 = 12,
		Players14 = 14,
		Players16 = 16,
	};

	static void __stdcall CRoom::CreateRoom(int p1, DWORD mode, DWORD* p3, LPCSTR title, LPCSTR password, int time);

};
void __stdcall CRoom::CreateRoom(int p1, DWORD mode, DWORD* p3, LPCSTR title, LPCSTR password, int time)
{
	//cuz mv is ugly
	RWM::Write write;
	RWM::Read read;
	//write.Byte((DWORD)p3, read.Byte((DWORD)p3 + 5));
	if (mode == CRoom::Enlimination || mode == CRoom::ZombieMode || mode == CRoom::BombBattle)
	{
		write.Byte((DWORD)p3 + 1, read.Byte((DWORD)p3 + 1) - 0x41);
	}
	else if (mode == CRoom::BossBattle)
	{
		write.Byte((DWORD)p3 + 1, 0x5C);
	}
	else if (mode == CRoom::ArmsRace)
	{
		write.Byte((DWORD)p3 + 1, 0x1D);
	}
	else
	{
		std::string echo;
		switch (time)
		{
		case CRoom::Mins5:
			write.Byte((DWORD)p3 + 1, read.Byte((DWORD)p3 + 1) - 1);
			break;
		case CRoom::Mins10:
			write.Byte((DWORD)p3 + 1, read.Byte((DWORD)p3 + 1));
			break;
		case CRoom::Mins15:
			write.Byte((DWORD)p3 + 1, read.Byte((DWORD)p3 + 1));
			break;
		case CRoom::Mins20:
			write.Byte((DWORD)p3 + 1, read.Byte((DWORD)p3 + 1) + 1);
			break;
		case CRoom::Mins25:
			write.Byte((DWORD)p3 + 1, read.Byte((DWORD)p3 + 1) + 2);
			break;
		case CRoom::Mins30:
			write.Byte((DWORD)p3 + 1, read.Byte((DWORD)p3 + 1) + 2);
			break;

		default:
			write.Byte((DWORD)p3 + 1, read.Byte((DWORD)p3 + 1));
			break;
		}
	}
	oCreateRoom(p1, mode, p3, title, password);
}