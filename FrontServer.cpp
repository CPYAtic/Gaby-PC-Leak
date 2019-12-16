#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <ctime>
#include "Console.h"
#include "Peer.h"

using namespace sql;

void Peer::FrontServer_Communicate()
{
	if (!FrontServer_SendConfirmMessage())
	{
		Console::PrintWarn(0, "Communication with %s failed!\n", IPv4);
		return;
	}

	char buff[1024];
	int len = 1024;

	if (AwaitData(buff, len))
	{
		if (len == 124 && memcmp(buff + 2, "\xF0\x81", 2) == 0)
		{
			char usrn[64], pswd[64];
			AuthInfo authInfo;

			ZeroMemory(usrn, sizeof(usrn));
			ZeroMemory(pswd, sizeof(pswd));

			strcpy_s(usrn, buff + 56);
			strcpy_s(pswd, buff + 12);

			ZeroMemory(&authInfo, sizeof(authInfo));
			
			AuthType type = FrontServer_RequestAuthInfo(usrn, pswd, authInfo);

			if (type == Confirmed)
			{
				if (!FrontServer_SendAuthInfo(authInfo))
				{
					Console::PrintWarn(0, "Communication with %s failed!\n", IPv4);
					return;
				}

				if (AwaitData(buff, len))
				{
					if (len == 8 && memcmp(buff + 2, "\x20\x00", 2) == 0)
					{
						if (!FrontServer_SendServerInfo(Confirmed))
						{
							Console::PrintWarn(0, "Communication with %s failed!\n", IPv4);
							return;
						}
						else
						{
							Console::Print(Console::Lime, 0, "%s has succesfully authorized as %s with (id : %s && pass : %s)!\n", IPv4, authInfo.nickName, usrn, pswd);
							Console::PrintWarn(0, "Terminating connection with %s!\n", IPv4);
							return;
						}
					}
				}
			}
			else if (type == Banned)
			{
				if (!FrontServer_SendServerInfo(Banned))
				{
					Console::PrintWarn(0, "Communication with %s failed!\n", IPv4);
					return;
				}
				else
				{
					Console::Print(Console::Lime, 0, "%s has tried to authorize on a banned account as %s!\n", IPv4, authInfo.nickName);
					Console::PrintWarn(0, "Terminating connection with %s!\n", IPv4);
					return;
				}
			}
			else if (type == Unknown)
			{
				if (!FrontServer_SendServerInfo(Unknown))
				{
					Console::PrintWarn(0, "Communication with %s failed!\n", IPv4);
					return;
				}
				else
				{
					Console::Print(Console::Lime, 0, "%s has failed to authorize!\n", IPv4);
					Console::PrintWarn(0, "Terminating connection with %s!\n", IPv4);
					return;
				}
			}
		}
		else return;
	}
}

bool Peer::FrontServer_SendConfirmMessage()
{
	struct ConfirmMessage
	{
		unsigned short id = 0;
		unsigned short pType = 0x0040;
		unsigned long Unknown1 = 0x74226470;
		unsigned long Unknown2 = 0;
		__time32_t TimeStamp = 0;
	};

	ConfirmMessage msg;
	msg.id = rand() % 0xF;
	msg.Unknown1 += rand() % 0xF;
	msg.TimeStamp = (__time32_t)std::time(0);

	if (int sent = send(Socket, (char*)&msg, sizeof(msg), 0))
	{
		if (sent == sizeof(msg))
		{
			return true;
		}
		else if (sent == SOCKET_ERROR)
		{
			Console::PrintWarn(0, "Connection with %s was aborted : %d\n", IPv4, WSAGetLastError());
			return false;
		}
		else
		{
			Console::PrintWarn(0, "Failed to send confirm message to %s : %d\n", IPv4, WSAGetLastError());
			Console::Print(Console::Yellow, 0, "Retrying...\n");

			if (int sent = send(Socket, (char*)&msg, sizeof(msg), 0))
			{
				if (sent == sizeof(msg))
				{
					return true;
				}
				else if (sent == SOCKET_ERROR)
				{
					Console::PrintWarn(0, "Connection with %s was aborted : %d\n", IPv4, WSAGetLastError());
					return false;
				}
				else
				{
					Console::PrintWarn(0, "Repeatedly failed to send confirm message to %s : %d\n", IPv4, WSAGetLastError());
					return false;
				}
			}
		}
	}

	return false;
}

bool Peer::FrontServer_SendAuthInfo(AuthInfo authInfo)
{
	if (int sent = send(Socket, (char*)&authInfo, sizeof(authInfo), 0))
	{
		if (sent == sizeof(authInfo))
		{
			return true;
		}
		else if (sent == SOCKET_ERROR)
		{
			Console::PrintWarn(0, "Connection with %s was aborted : %d\n", IPv4, WSAGetLastError());
			return false;
		}
		else
		{
			Console::PrintWarn(0, "Failed to send auth info to %s : %d\n", IPv4, WSAGetLastError());
			Console::Print(Console::Yellow, 0, "Retrying...\n");

			if (int sent = send(Socket, (char*)&authInfo, sizeof(authInfo), 0))
			{
				if (sent == sizeof(authInfo))
				{
					return true;
				}
				else if (sent == SOCKET_ERROR)
				{
					Console::PrintWarn(0, "Connection with %s was aborted : %d\n", IPv4, WSAGetLastError());
					return false;
				}
				else
				{
					Console::PrintWarn(0, "Repeatedly failed to send auth info to %s : %d\n", IPv4, WSAGetLastError());
					return false;
				}
			}
		}
	}
	return false;
}

bool Peer::FrontServer_SendServerInfo(AuthType type)
{
	enum ServerStatus
	{
		Normal = 0,
		Busy = 0x55555500,
		Full = 0xAAAAAA00,
		Offline = 0xFFFFFF00
	};

	struct ServerInfo
	{
		WORD id = 0;
		WORD pName = 0x8040;
		AuthType type = Unknown;
		BYTE unknown = 0;
		BYTE servercount = 2;
		DWORD Server1 = 1;
		DWORD Server2 = 2;
	};

	ServerInfo sInfo;
	sInfo.id = rand() % 0xF;
	sInfo.type = type;
	sInfo.Server1 = 1 | Normal;
	sInfo.Server2 = 2 | Normal;

	if (int sent = send(Socket, (char*)&sInfo, sizeof(sInfo), 0))
	{
		if (sent == sizeof(sInfo))
		{
			return true;
		}
		else if (sent == SOCKET_ERROR)
		{
			Console::PrintWarn(0, "Connection with %s was aborted : %d\n", IPv4, WSAGetLastError());
			return false;
		}
		else
		{
			Console::PrintWarn(0, "Failed to send server info to %s : %d\n", IPv4, WSAGetLastError());
			Console::Print(Console::Yellow, 0, "Retrying...\n");

			if (int sent = send(Socket, (char*)&sInfo, sizeof(sInfo), 0))
			{
				if (sent == sizeof(sInfo))
				{
					return true;
				}
				else if (sent == SOCKET_ERROR)
				{
					Console::PrintWarn(0, "Connection with %s was aborted : %d\n", IPv4, WSAGetLastError());
					return false;
				}
				else
				{
					Console::PrintWarn(0, "Repeatedly failed to send server info to %s : %d\n", IPv4, WSAGetLastError());
					return false;
				}
			}
		}
	}
	return false;
}

Peer::AuthType Peer::FrontServer_RequestAuthInfo(char* usrn, char* pwsd, AuthInfo& authInfo)
{
	authInfo.id += rand() % 0xF;
	authInfo.pName = 0x8160;
	authInfo.idk1 = 0x80;
	authInfo.idk2 = 0x05;
	authInfo.unknown2 = -1;
	authInfo.unknown3 = -1;
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
	if (db.open("MegaVoltsDB.db"))
	{
		Table Users(db.getHandle(), "Users", def_Users);
		if (Users.exists())
		{
			if (Users.open())
			{
				for (int i = 0; i < Users.recordCount(); i++)
				{
					if (strcmp(Users.getRecord(i)->getValue(0)->toString().c_str(), usrn) == 0) //id check
					{
						if (strcmp(Users.getRecord(i)->getValue(1)->toString().c_str(), pwsd) == 0) //pwd check
						{
							if (!Users.getRecord(i)->getValue(4)->asInteger()) // ban check
							{
								ZeroMemory(authInfo.nickName, sizeof(authInfo.nickName));
								ZeroMemory(authInfo.clanName, sizeof(authInfo.clanName));
								sprintf_s(authInfo.nickName, "%s", Users.getRecord(i)->getValue(2)->toString().c_str());
								sprintf_s(authInfo.clanName, "%s", Users.getRecord(i)->getValue(13)->toString().c_str());
								authInfo.AuthorizeType = Peer::AuthorizeType::Success;
								authInfo.Grade = (Peer::PlayerGrade)Users.getRecord(i)->getValue(3)->asInteger();
								authInfo.level = Users.getRecord(i)->getValue(5)->asInteger() + 1;
								authInfo.exp = Users.getRecord(i)->getValue(6)->asInteger();
								authInfo.kills = Users.getRecord(i)->getValue(7)->asInteger();
								authInfo.deaths = Users.getRecord(i)->getValue(8)->asInteger();
								authInfo.assists = Users.getRecord(i)->getValue(9)->asInteger();
								authInfo.wins = Users.getRecord(i)->getValue(10)->asInteger();
								authInfo.loses = Users.getRecord(i)->getValue(11)->asInteger();
								authInfo.draws = Users.getRecord(i)->getValue(12)->asInteger();
								authInfo.clanIcon1 = Users.getRecord(i)->getValue(13)->asINT();
								authInfo.clanIcon2 = Users.getRecord(i)->getValue(14)->asINT();
								break;
							}
							else
							{
								if (i == Users.recordCount()) break;
								authInfo.AuthorizeType = Peer::AuthorizeType::Blocked;
							}
						}
						else
						{
							if (i == Users.recordCount()) break;
							authInfo.AuthorizeType = Peer::AuthorizeType::Wrong;
						}
					}
					else
					{
						if (i == Users.recordCount()) break;
						authInfo.AuthorizeType = Peer::AuthorizeType::Wrong;
					}
				}
			}
		}
	}
	db.close();

	if (authInfo.AuthorizeType == AuthorizeType::Blocked)
		return Peer::AuthType::Banned;
	else if (authInfo.AuthorizeType == AuthorizeType::Success)
		return Peer::AuthType::Confirmed;
	else
		return Peer::AuthType::Unknown;
}