#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <ctime>

#include "Console.h"

#include "Front.h"

void Peer::Communicate()
{
	if (!SendConfirmMessage())
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
			AuthType type = RequestAuthInfo(usrn, pswd, authInfo);

			if (type == Confirmed)
			{
				if (!SendAuthInfo(authInfo))
				{
					Console::PrintWarn(0, "Communication with %s failed!\n", IPv4);
					return;
				}

				if (AwaitData(buff, len))
				{
					if (len == 8 && memcmp(buff + 2, "\x20\x00", 2) == 0)
					{
						if (!SendServerInfo(Confirmed))
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
				if (!SendServerInfo(Banned))
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
				if (!SendServerInfo(Unknown))
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

void Peer::ComThread(Peer* _this)
{
	_this->Communicate();
}

bool Peer::AwaitData(char* data, int& len)
{
	clock_t timer = clock();

	ZeroMemory(data, len);
	
	do
	{
		int size = recv(Socket, data, len, 0);
		if (size > 0)
		{
			len = size;
			return true;
		}
		else if (size == SOCKET_ERROR)
		{
			Console::PrintWarn(0, "Connection with %s was aborted : %d\n", IPv4, WSAGetLastError());
			return false;
		}
	}
	while (clock() - timer <= 3000);

	Console::Print(Console::Red, 0, "Connection with %s timed out.\n", IPv4);
	return false;
}

bool Peer::SendConfirmMessage()
{
	struct ConfirmMessage
	{
		unsigned short id		= 0;
		unsigned short pType	= 0x0040;
		unsigned long Unknown1	= 0x74226470;
		unsigned long Unknown2	= 0;
		__time32_t TimeStamp		= 0;
	};

	ConfirmMessage msg;
	msg.id			= rand() % 0xF;
	msg.Unknown1	+= rand() % 0xF;
	msg.TimeStamp	= (__time32_t)time(0);

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

bool Peer::SendAuthInfo(AuthInfo authInfo)
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

bool Peer::SendServerInfo(AuthType type)
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
		BYTE servercount = 1;
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

Peer::AuthType Peer::RequestAuthInfo(char* usrn, char* pwsd, AuthInfo& authInfo)
{
	authInfo.id += rand() % 0xF;
	authInfo.pName = 0x8160;
	authInfo.idk1 = 0x80;
	authInfo.idk2 = 0x05;
	authInfo.AuthorizeType = Success;
	authInfo.Grade = Player;
	authInfo.unknown2 = -1;
	authInfo.unknown3 = -1;
	authInfo.level = 105 + 1;
	authInfo.exp = 0;
	authInfo.kills = 0;
	authInfo.deaths = 0;
	authInfo.assists = 0;
	authInfo.wins = 0;
	authInfo.loses = 0;
	authInfo.draws = 0;
	authInfo.clanIcon1 = 0xFF;
	authInfo.clanIcon2 = 0xFF;

	ZeroMemory(authInfo.nickName, sizeof(authInfo.nickName));
	ZeroMemory(authInfo.clanName, sizeof(authInfo.clanName));
	sprintf_s(authInfo.nickName, usrn);
	//sprintf_s(authInfo.clanName, "%s", "feel-dope");

	return Confirmed;
}