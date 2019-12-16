#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <ctime>
#include "Console.h"
#include "Peer.h"
#define Minutes(x) x * 60;
#define Hours(x) x * Minutes(60);
#define Days(x) x * Hours(24);
int RockTockens = 500000000;
int MicroPoints = 500000000;

using namespace sql;
enum Characters : int
{
	Naomi = 0,
	Kai = 1,
	Pandora = 2,
	CHIP = 3,
	Knox = 4
};
DWORD GetCurrency(int RT, int MP)
{
	DWORD shitmp = MP & 0x7FFFFFFF;
	DWORD shitrt = (RT & 0x3FFFFFFF) << 31;

	return shitmp + shitrt;
}
DWORD Assemble(DWORD value)
{
	return (((value >> 15) & 0x7F) << 25);
}
DWORD Disassemble(DWORD value)
{
	return ((value >> 25) & 0x7F) << 15;
}
DWORD GetCharacter(BYTE Character)
{
	return (Character & 0x1F) << 7;
}
DWORD GetLevel(int Level)
{
	return ((Level + 1) & 0x7F) << 15;;
}
DWORD GetMaxBattery(int maxBattery)
{
	return maxBattery & 0x1FFF;
}
DWORD GetSingleWaveAttempts(int attempts)
{
	return (attempts & 0x1FF) << 13;
}
DWORD GetMaxInventory(int maxInventory)
{
	return (maxInventory & 0x3FF) << 22;
}
DWORD GetTutorialFinished(bool Finished)
{
	return Finished & 0x3FFFFF;
}
DWORD GetBattery(int Battery)
{
	return (Battery & 0x3FFF) << (0x27 & 0x1F);
}
DWORD GetCoins(int Coins)
{
	return (Coins & 0x7F) << (0x20 & 0x1F);
}
DWORD GetSecondPlayerInfoIDK(int idk)
{
	return (idk & 0x7FF) << (0x35 & 0x1F);
}
DWORD GetHighestKillStreak(int kills)
{
	return (kills & 0x000000FF) << (0x38 & 0x1F);
}
DWORD GetHeadshotKills(int kills)
{
	return kills << 0x1B;
}
DWORD GetHeadshotsIDK(int idk = 2)
{
	return idk & 0x07FFFFFF;
}
void Peer::MainServer_Communicate()
{
	if (!MainServer_SendConfirmMessage())
	{
		Console::PrintWarn(0, "Communication with %s failed!\n", IPv4);
		return;
	}

	char buff[1024];
	int len = 1024;

	while ((len = 1024) && AwaitData(buff, len))
	{

		if (len == 24 && memcmp(buff + 2, "\x60\x80", 2) == 0 && !verified)
		{
			SEND_ID = *(unsigned short*)buff;
			bool ret = MainServer_SendAccountInfo();
			ret &= MainServer_SendInventoryInfo();

			if (!ret) Console::PrintErr(0, "Failed to send details to %s\n", IPv4);
			//else Console::Print(Console::Light, 0, "Sent details and inventory to %s\n", IPv4);

			verified = true;

		}
		/*else if ((memcmp(buff + 3, "\x40", 1) == 0) && (memcmp(buff + 6, "\x00", 1) == 0) /*&& (*(unsigned char*)(len - 8) == *(unsigned char*)buff[7]))
		{
			bool ret = Send((char*)"\x05\x00\x6C\x80\x0B\x4F\x00\x03\x47\x61\x62\x79\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x61\x61\x61", 27, "msg back");
			if (!ret) Console::PrintErr(0, "Failed to send details to %s\n", IPv4);

		}*/
		/*else if (len >= 9)
		{

			Console::PrintErr(0, "%2X", *(unsigned char*)(buff + 7));

			//bool ret = Send((char*)"\x05\x00\x6C\x80\x0B\x4F\x01\x09\x47\x61\x62\x79\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x61\x61\x61", 27, "msg back");
		}*/
		else if (len == 8 && memcmp(buff + 2, "\x20\x40", 2) == 0)
		{
			//Console::Print(Console::Light, 0, "Recieved a push from %s\n", IPv4);
			SEND_ID = *(unsigned short*)buff;

			enum PushType : unsigned char
			{
				Character = 18,
				RoomRequest = 35,
				PlayerState = 39,
				EnterLobby = 40
			};

			struct Push
			{
				unsigned short id;
				unsigned short pName;
				unsigned char bUnknown1;
				PushType bType;
				unsigned short data;
			} *push = (Push*)buff;

			if (push->bType == Character)
			{


				if (!MainServer_AnswerPush(push->bType, push->data >> 8)) Console::PrintErr(0, "Failed to answer a push from %s\n", IPv4);
				else Console::Print(Console::Light, 0, "Answered the push from %s\n", IPv4);
			}
		}
		else
		{
			Console::PrintWarn(0, "Recieved an unknown packet from %s ; Size = %d , Name = %02X %02X, %02X\n", IPv4, len, (unsigned char)buff[2], (unsigned char)buff[3]);
		}
	}

	Console::PrintWarn(0, "Exiting the thread associated with %s\n", IPv4);
	return;
}
bool Peer::MainServer_SendConfirmMessage()
{
	struct ConfirmMessage
	{
		unsigned short id = 0;
		unsigned short pType = 0x0040;
		unsigned long Unknown[3] = { 0x0100647D, 0xBD145BC0, 0x000000A2 };
	};

	ConfirmMessage msg;
	msg.id = rand() % 0xFFFF;

	return Send((char*)&msg, sizeof(msg), "Confirm Message");
}
bool Peer::MainServer_SendAccountInfo()
{
	struct Details
	{
		unsigned short id;					// 0 : Connection ID
		unsigned short pName = 0x8420;		// 2 : Packet's name
		unsigned long Unknown1 = 0x16700;	// 4 : Unknown
		uint64_t dioramaInfo;			// 8 : dioramaInfo
		uint32_t Kills;				// 16 (0x10) : Kills
		uint32_t Deaths;				// 20 (0x14) : Deaths
		uint32_t Assists;				// 24 (0x18) : Assists
		uint32_t Wins;					// 28 (0x1C) : Total Wins
		uint32_t Loses;				// 32 (0x20) : Total Loses
		uint32_t Draws;				// 36 (0x24) : Total Draws
		struct KillInfo
		{
			uint32_t Melee;
			uint32_t Rifle;
			uint32_t Shotgun;
			uint32_t Sniper;
			uint32_t Gatling;
			uint32_t Bazooka;
			uint32_t Grenade;
		} WeapondKills;						// 40 (0x28) : Kills by each weapond

		struct idkk1
		{
			uint32_t TotalHeadshots;
			uint32_t HighestKillStreak;
			uint32_t idk3;
			uint32_t playtime;
			uint32_t idk5;
			uint32_t idk6;
			uint32_t idk7;
		}idk1;

		struct idkk2
		{
			uint32_t idk;
			uint32_t idk2;
			uint32_t idk3;
			uint32_t idk4;
			uint32_t idk5;
			uint32_t idk6;
		}idk2;

		struct ZombieInfo
		{
			uint32_t ZombieKills;
			uint32_t InfectedKills;
			uint32_t idk3;
		}ZombieInfo;

		//unsigned long Unknown3[7];
		//unsigned long Unknown4[6];			// 96 (0x60) : Unknown (usualy 0's)
		//unsigned long Unknown5[3];			// 120 (0x78) : Unknown

		char Nickname[16];					// 132 (0x84) : Nickname
		struct idkk4
		{
			uint32_t idk;
			uint32_t idk2;
			uint32_t idk3;
			uint32_t PlayerInfo; // level , playergrade, last character selected, character he owns
			uint32_t SecondPlayerInfo; // coins & battery
		}idk4;
		//unsigned long Unknown6[5];			// 148 (0x94) : Unknown

		unsigned long Exp;					// 168 (0xA8) : Experience points
		DWORD MP;					// 172 (0xAC) : Micro Point
		DWORD RT;					// 176 (0xB0) : Potentualy Rock Tockens
		DWORD MaxInventoryAndTutorial;
		DWORD SingleWaveAttemptsAndMaxBattery;
		unsigned long Unknown7[7];
		char Clanname[16];
		WORD clanId;
		//DWORD contributeWinLoseAndDraw;
		uint32_t contributePoint;
		uint32_t clanWin;
		uint32_t clanLose;
		uint32_t clanKill;
		uint32_t clanDeath;
		uint32_t clanAssist;
	};


	Details details;
	ZeroMemory(&details, sizeof(details));

	details.id = SEND_ID;
	details.pName = 0x8420;

	Field def_Users[] =
	{
		Field(FIELD_KEY),
		Field("Username", type_text),
		Field("Password", type_text),
		Field("Nickname", type_text),
		Field("Grade", type_int),
		Field("Banned", type_int),
		Field("Level", type_int),
		Field("EXP", type_int),
		Field("Kills", type_int),
		Field("Deaths", type_int),
		Field("Assists", type_int),
		Field("Wins", type_int),
		Field("Loses", type_int),
		Field("Draws", type_int),
		Field("Clanname", type_text),
		Field("ClanIcon1", type_int),
		Field("ClanIcon2", type_int),
		Field("MeleeKills", type_int),
		Field("RifleKills", type_int),
		Field("ShotgunKills", type_int),
		Field("SniperKills", type_int),
		Field("GatlingKills", type_int),
		Field("BazookaKills", type_int),
		Field("GrenadeKills", type_int),
		Field("Headshots", type_int),
		Field("HighestKillstreak", type_int),
		Field("Playtime", type_int),
		Field("ZombieKills", type_int),
		Field("InfectedKills", type_int),
		Field("MP", type_int),
		Field("RT", type_int),
		Field("COINS", type_int),
		Field("Battery", type_int),
		Field("MaxBattery", type_int),
		Field("MaxInventory", type_int),
		Field("bTutorialFinished", type_int),
		Field("SingleWaveAttempts", type_int),
		Field("LastCharacterUsed", type_int),
		Field(DEFINITION_END),
	};
	Database db;
	details.Unknown1 = 0x0001677F;
	details.idk1.idk3 = 0;
	details.idk1.idk5 = 0x00011620;
	details.idk1.idk6 = 0x2FFFFBDE;
	details.idk1.idk7 = 0x00003E90;
	details.idk4.idk = 0x2693CE39;
	details.idk4.idk2 = 0;
	details.idk4.idk3 = 0x0001003A;

	if (db.open("MegaVoltsDB.db"))
	{
		Table Users(db.getHandle(), "Users", def_Users);
		if (Users.exists())
		{
			if (Users.open())
			{
				for (int i = 0; i < Users.recordCount(); i++)
				{
					ZeroMemory(details.Nickname, sizeof(details.Nickname));
					ZeroMemory(details.Clanname, sizeof(details.Clanname));
					strcpy_s(details.Nickname, Users.getRecord(i)->getValue(2)->toString().c_str());
					strcpy_s(details.Clanname, Users.getRecord(i)->getValue(13)->toString().c_str());

					
					details.Kills = Users.getRecord(i)->getValue(7)->asInteger();
					details.Deaths = Users.getRecord(i)->getValue(8)->asInteger();
					details.Assists = Users.getRecord(i)->getValue(9)->asInteger();
					details.Wins = Users.getRecord(i)->getValue(10)->asInteger();
					details.Loses = Users.getRecord(i)->getValue(11)->asInteger();
					details.Draws = Users.getRecord(i)->getValue(12)->asInteger();
					details.WeapondKills.Melee = Users.getRecord(i)->getValue(16)->asInteger();
					details.WeapondKills.Rifle = Users.getRecord(i)->getValue(17)->asInteger();
					details.WeapondKills.Shotgun = Users.getRecord(i)->getValue(18)->asInteger();
					details.WeapondKills.Sniper = Users.getRecord(i)->getValue(19)->asInteger();
					details.WeapondKills.Gatling = Users.getRecord(i)->getValue(20)->asInteger();
					details.WeapondKills.Bazooka = Users.getRecord(i)->getValue(21)->asInteger();
					details.WeapondKills.Grenade = Users.getRecord(i)->getValue(22)->asInteger();
					
					details.idk1.TotalHeadshots = GetHeadshotKills(Users.getRecord(i)->getValue(23)->asInteger()) + GetHeadshotsIDK(2);
					details.idk1.HighestKillStreak = GetHighestKillStreak(Users.getRecord(i)->getValue(24)->asInteger()) + (Users.getRecord(i)->getValue(23)->asInteger() >> 5);;

					
					details.idk1.playtime = Users.getRecord(i)->getValue(25)->asInteger();
					details.ZombieInfo.ZombieKills = 66;
					details.ZombieInfo.InfectedKills = Users.getRecord(i)->getValue(27)->asInteger();


					details.idk4.PlayerInfo = Assemble(GetLevel(Users.getRecord(i)->getValue(5)->asInteger()) + GetCharacter(Users.getRecord(i)->getValue(35)->asInteger()) + 2) + 0x3E00 + (Peer::PlayerGrade)Users.getRecord(i)->getValue(3)->asInteger();//0x60003E02; // 0x3E00 characters he has ,
					details.Exp = Users.getRecord(i)->getValue(6)->asInteger();
					details.MP = GetCurrency(Users.getRecord(i)->getValue(29)->asInteger(), Users.getRecord(i)->getValue(28)->asInteger());//0x800008D4;
					details.RT = Users.getRecord(i)->getValue(29)->asInteger() / 2;
					details.idk4.SecondPlayerInfo = GetSecondPlayerInfoIDK(240) + GetCoins(Users.getRecord(i)->getValue(30)->asInteger()) + GetBattery(Users.getRecord(i)->getValue(31)->asInteger());
					details.MaxInventoryAndTutorial = GetMaxInventory(Users.getRecord(i)->getValue(33)->asInteger()) + GetTutorialFinished(Users.getRecord(i)->getValue(34)->asInteger());//0x19000001;
					MainServer_MaxInventory = Users.getRecord(i)->getValue(33)->asInteger();
					details.SingleWaveAttemptsAndMaxBattery = GetSingleWaveAttempts(10) + GetMaxBattery(Users.getRecord(i)->getValue(32)->asInteger());//0x000063E8; //-1 single wave attempts means unlimited
				}
			}
		}
	}
	return Send((char*)&details, sizeof(details), "AccountInfo");
}
bool Peer::MainServer_SendInventoryInfo()
{
	struct ItemInfo
	{
		uint32_t itemid = 0;
		unsigned long itemType = 0; // 0:Normal 1: Unused 2: Destroyed 3+: Expired?
		unsigned long somestaticPointer = 0;
		__time32_t timestamp = (__time32_t)std::time(0);
		unsigned long itemDurability = 0;
		bool isSealed = 0;
		uint32_t giftCount = 0;
		uint32_t dunno2 = 0;
		uint32_t dunno3 = 0;
		uint32_t dunno4 = 0;
	};
	struct InventoryInfo
	{
		unsigned short id;                    // 0 : Connection ID
		unsigned short pName = 0x9560;        // 2 : Packet's name
		uint32_t idk1;
		std::vector<ItemInfo> items;
	};

	InventoryInfo Inventory;
	ZeroMemory(&Inventory, sizeof(Inventory));

	Inventory.id = rand() % 0xF;
	Inventory.pName = 0x9560;
	Inventory.idk1 = 0x2225136B; //some pointer or smth, dynamic. any long hex value is fine
	Inventory.items.reserve(MainServer_MaxInventory);

	
	return Send((char*)&Inventory, sizeof(Inventory), "InventoryInfo");
}
bool Peer::MainServer_AnswerPush(unsigned char pType, unsigned char data)
{
	struct Push
	{
		unsigned short id;
		unsigned short pName = 0x8020;
		unsigned char bUnknown1 = 0x80;
		unsigned char bType;
		unsigned char bUnknown2 = 1;
		unsigned char data;
	} push;

	push.id = SEND_ID;
	push.bType = pType;
	push.data = data;

	return Send((char*)&push, sizeof(push), "Push Answer");
}
bool Peer::MainServer_AnswerChat(const char* nickname, const char* message, int size)
{
	return true;
}