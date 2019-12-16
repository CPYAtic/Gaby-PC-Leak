#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <fstream>
#include <ctime>

#include "Console.h"

#include "Peers.h"
#define Minutes(x) x * 60;
#define Hours(x) x * Minutes(60);
#define Days(x) x * Hours(24);
int RockTockens = 500000000;
int MicroPoints = 500000000;

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
enum Characters : unsigned char
{
	Naomi = 0x00,
	Kai = 0x01,
	Pandora = 0x02,
	CHIP = 0x03,
	Knox = 0x04
};
void Peer::Communicate()
{
	if (!SendConfirmMessage())
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
			id = *(unsigned short*)buff;
			bool ret = SendDetails();
			ret &= SendInventory();

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
			
			//bool ret = Send((char*)"\x0A\x00\x60\x80\xC3\x15\x00\x01\x50\x00\x14\x00\x01\x00\x00\x00\x50\x1B\x02\x01\xC8\xD3\x58\x5C", 24, "Item");
		}*/
		else if (len == 12 && memcmp(buff + 2, "\x30\x40", 2) != 6969)
		{
			Console::Print(Console::Light, 0, "Received a push from %s (item buy)\n", IPv4);
			id = *(unsigned short*)buff;
			bool ret = BuyItem();

			//bool ret = Send((char*)"\x0A\x00\x60\x80\xC3\x15\x00\x01\ x50\x00\x14\x00\x01\x00\x00\x00\x50\x1B\x02\x01\xC8\xD3\x58\x5C", 24, "Item");
		}

		else if (len == 8 && memcmp(buff + 2, "\x20\x40", 2) != 6969)
		{
			Console::Print(Console::Light, 0, "Received a push from %s (Response)\n", IPv4);
			id = *(unsigned short*)buff;

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
				

				if (!AnswerPush(push->bType, push->data >> 8)) Console::PrintErr(0, "Failed to answer a push from %s\n", IPv4);
				//else Console::Print(Console::Light, 0, "Answered the push from %s\n", IPv4);
			}
		}
		else
		{
			//Console::PrintWarn(0, "Recieved an unknown packet from %s ; Size = %d , Name = %02X %02X, %02X\n", IPv4, len, (unsigned char)buff[2], (unsigned char)buff[3]);
		}
	}

	Console::PrintWarn(0, "Exiting the thread associated with %s\n", IPv4);
	return;
}

void Peer::ComThread(Peer* _this)
{
	_this->Communicate();
}

bool Peer::AwaitData(char* data, int& len)
{
	clock_t timer = clock();

	ZeroMemory(data, len);

	int size = 0;
	unsigned long count = 1, i = 0;
	do
	{
		int length = recv(Socket, data + size, 4, MSG_WAITALL);

		if (length == 4)
		{
			if (size == 0) count = (unsigned long)(data[2] >> 4);

			size += length;
			if (++i == count || size >= len)
			{
				len = size;
				return true;
			}
		}
		else if (length == 0)
		{
			len = size;
			Console::PrintWarn(0, "Connection with %s was closed by the peer.\n", IPv4);
			return false;
		}
		else if (length == SOCKET_ERROR)
		{
			Console::PrintWarn(0, "Connection with %s was aborted : %d\n", IPv4, WSAGetLastError());
			return false;
		}
	} while (clock() - timer <= 108000); // 30 minutes

	Console::Print(Console::Red, 0, "Connection with %s timed out.\n", IPv4);
	return false;
}

bool Peer::SendConfirmMessage()
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

bool Peer::BuyItem()
{
	struct BuyItem1
	{
		unsigned short id = 0;
		unsigned short pType = 0x8060;
		unsigned long Unknown[1] = { 0x010015C3 };
		unsigned long itemID = 0x00140050;
		BYTE itemType = 0x01; //itemtype
		unsigned long Unknown1[2] = { 0x01021B50, 0x5C58D3C8 };
	};

	BuyItem1 msg1;
	msg1.id = rand() % 0xFFFF;

	return Send((char*)&msg1, sizeof(msg1), "Item");
}


bool Peer::SendDetails()
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
		unsigned long Unknown7[8];			// 180 (0xB4) : Unknown

		union
		{
			char Clanname[16];				// 220 (0xDC) : Clan's name
			unsigned long Unknown8[4];		// 220 (0xDC) : Unknown	
		};

		unsigned long Unknown9[7];			// 236 (0xEC) : Unknown	*/
	};


	Details details;
	ZeroMemory(&details, sizeof(details));

	details.id = id;
	details.pName = 0x8420;

	strcpy_s(details.Nickname, botname);
	strcpy_s(details.Clanname, "feel-dope");

	// some static unknowns
	{
		details.Unknown1 = 0x0001677F;

		details.WeapondKills.Melee = 10000;
		details.WeapondKills.Rifle = 10000;
		details.WeapondKills.Shotgun = 10000;
		details.WeapondKills.Sniper = 40000;
		details.WeapondKills.Gatling = 10000;
		details.WeapondKills.Bazooka = 10000;
		details.WeapondKills.Grenade = 10000;
		details.Kills = details.WeapondKills.Melee + details.WeapondKills.Rifle + details.WeapondKills.Shotgun + details.WeapondKills.Sniper + details.WeapondKills.Gatling + details.WeapondKills.Bazooka + details.WeapondKills.Grenade;
		details.Deaths = 10000;
		details.Assists = 30000;
		details.Loses = 3000;
		details.Draws = 2000;
		details.Wins = 5000;

		details.idk1.TotalHeadshots = GetHeadshotKills(10000) + GetHeadshotsIDK(2);
		details.idk1.HighestKillStreak = GetHighestKillStreak(255) + (10000 >> 5);;

		details.idk1.idk3 = 0; 

		details.idk1.playtime = Days(1337);

		details.idk1.idk5 = 0x00011620;
		details.idk1.idk6 = 0x2FFFFBDE;
		details.idk1.idk7 = 0x00003E90;

		details.ZombieInfo.ZombieKills = 66; 

		details.ZombieInfo.InfectedKills = 10000;
		
		details.idk4.idk = 0x2693CE39;
		details.idk4.idk2 = 0;
		details.idk4.idk3 = 0x0001003A;


		details.idk4.PlayerInfo = Assemble(GetLevel(105) + GetCharacter(Naomi) + 2 ) + 0x3E00 + GM;//0x60003E02; // 0x3E00 characters he has ,
		details.idk4.SecondPlayerInfo = GetSecondPlayerInfoIDK(240) + GetCoins(100) + GetBattery(8000);
		details.Exp = 5000000;
		details.MP = GetCurrency(RockTockens, MicroPoints);//0x800008D4;
		details.RT = RockTockens / 2;
		details.MaxInventoryAndTutorial = GetMaxInventory(1000) + GetTutorialFinished(true);//0x19000001;
		details.SingleWaveAttemptsAndMaxBattery = GetSingleWaveAttempts(10) + GetMaxBattery(8000);//0x000063E8; //-1 single wave attempts means unlimited

		details.Unknown8[0] = 0x009E7018;
		details.Unknown8[1] = 0x0000019C;
		details.Unknown8[2] = 0x05A87874;
		details.Unknown8[3] = 0x004FFAA0;

		details.Unknown9[0] = 1;
		details.Unknown9[3] = 0x10002010;
		details.Unknown9[5] = 0x0B5761A0;
		details.Unknown9[6] = 0x043E205C;
		details.Unknown9[7] = 0x004FFB40;
	}
	
	return Send((char*)&details, sizeof(details), "Details");
}

bool Peer::SendInventory()
{
	std::ifstream invPacket("inventory.pct", std::ios::binary);

	if (invPacket.is_open())
	{
		do
		{
			char buff[256];
			invPacket.read(buff, sizeof(buff));

			unsigned int count = (unsigned int)invPacket.gcount();
			if (count < 1) break;

			if (!Send(buff, count, "Inventory")) return false;

		} while (invPacket);

		return true;
	}
	else
	{
		Console::PrintErr(0, "Invenotry packet file could not be found.\n");
		return false;
	}
}

bool Peer::AnswerPush(unsigned char pType, unsigned char data)
{
	struct Push
	{
		unsigned short id;
		unsigned short pName	= 0x8020;
		unsigned char bUnknown1 = 0x80;
		unsigned char bType;
		unsigned char bUnknown2 = 1;
		unsigned char data;
	} push;

	push.id = id;
	push.bType = pType;
	push.data = data;

	return Send((char*)&push, sizeof(push), "Push Answer");
}
bool Peer::AnswerChat(const char* nickname, const char* message, int size = 0)
{
	struct Push
	{
		unsigned short id;
		DWORD Instance;
		//unsigned short pName = 0x8020;
		//unsigned char bUnknown1 = 0x80;
		BYTE bType = 0x00;
		BYTE bSize;
		char nickname[16];
		char *msg;
	} push;

	push.id = rand() & 0xFFFF;
	push.Instance = 0x4F0B806C;//rand() & 0xFFFFFFFF;
	push.bType = 0x00;
	push.bSize = *(BYTE*)size;
	strcpy_s(push.nickname, nickname);
	push.msg = (char*)malloc(sizeof(char) * size);
	//push.msg = strdup(message);
	
	return Send((char*)&push, sizeof(push), "Push Answer");
}


bool Peer::Send(char* buff, int len, const char* lpName)
{
	if (len <= 0) return false;

	for (int i = 0, j = clock(); i < len && clock() - j < 60000;)
	{
		int sent = send(Socket, buff + i, len - i, 0);
		
		if (sent == len - i)
		{
			return true;
		}
		else if (sent < 0)
		{
			Console::PrintWarn(0, "Connection with %s encountered an error : %d\n", IPv4, WSAGetLastError());
			return false;
		}
		else if (sent == 0)
		{
			Console::PrintWarn(0, "Connection with %s was closed.\n", IPv4);
			return false;
		}
		else i += sent;		
	}

	Console::PrintWarn(0, "Failed to send %s to %s - reqest took too much effort required.\n", lpName, IPv4);
	return false;
}