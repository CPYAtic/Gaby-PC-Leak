#pragma once
#include "easySQLite/SqlCommon.h"
#include "easySQLite/SqlField.h"
#include "easySQLite/SqlDatabase.h"
#include "easySQLite/SqlTable.h"
#include "UnitTest++/src/UnitTest++.h"
class Peer
{
public:
	enum AuthType : WORD
	{
		Unknown = 0x5B0,
		Banned = 0x5A0,
		Confirmed = 0x5E0
	};
	enum AuthorizeType : BYTE
	{
		Wrong = 0x00,
		Success = 0x01,
		DataError = 0x04,
		Busy = 0x05,
		DontExist = 0x0D,
		TIME_EXPIRE_2 = 0x18,
		Blocked = 0x2A,
		SHUTDOWN = 0x50

	};
	enum PlayerGrade : BYTE
	{
		Player = 0x02,
		MOD = 0x03,
		TESTER = 0x04,
		GM = 0x09
	};
	struct AuthInfo
	{
		WORD id = 0;
		WORD pName = 0x8160;
		byte idk1 = 0x80;
		byte idk2 = 0x05;
		AuthorizeType AuthorizeType = DataError;
		PlayerGrade Grade = Player;
		DWORD unknown2 = -1;
		DWORD unknown3 = -1;
		DWORD level = 1;
		DWORD exp = 0;
		DWORD kills = 0;
		DWORD deaths = 0;
		DWORD assists = 0;
		DWORD wins = 0;
		DWORD loses = 0;
		DWORD draws = 0;
		char nickName[16];
		WORD clanIcon1 = 0;
		WORD clanIcon2 = 0;
		char clanName[20];
	};

	bool AwaitData(char* data, int& len);
	void FrontServer_Communicate();
	void CastServer_Communicate();
	void MainServer_Communicate();
	WORD SEND_ID = 0;
	bool detailsSent = false;
	bool verified = false;
	static void ComThread(Peer* _this);
	bool Send(char* buff, int len, const char* lpName);

	bool MainServer_SendConfirmMessage();
	bool MainServer_SendAccountInfo();
	bool MainServer_SendInventoryInfo();
	int  MainServer_MaxInventory = 100;
	bool MainServer_AnswerPush(unsigned char pType, unsigned char data);
	bool MainServer_AnswerChat(const char* nickname, const char* message, int size);

	bool CastServer_verified = false;
	bool CastServer_SendConfirmMessage();
	bool CastServer_AcceptVerifyInfo();
	bool CastServer_AnswerPush(WORD connID);

	bool FrontServer_SendConfirmMessage();
	bool FrontServer_SendAuthInfo(AuthInfo authInfo);
	bool FrontServer_SendServerInfo(AuthType type);
	AuthType FrontServer_RequestAuthInfo(char* usrn, char* pswd, AuthInfo& authInfo);


	bool Initialized = false;

	char IPv4[INET6_ADDRSTRLEN];

	HANDLE Thread = nullptr;
	SOCKET Socket = NULL;

	Peer(SOCKET sock, char IPv4[INET6_ADDRSTRLEN])
	{
		srand((unsigned long)std::time(0));
		Console::Print(Console::Lime, 0, "Connected peer %s\n", IPv4);

		this->Socket = sock;
		memcpy(this->IPv4, IPv4, strlen(IPv4) + 1);

		Thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ComThread, this, 0, 0);
		if (Thread == INVALID_HANDLE_VALUE || Thread == 0)
		{
			Console::PrintErr(0, "Failed to create a thread : %d\n", GetLastError());
			Console::Print(Console::Yellow, 0, "Retrying...\n");
			Sleep(100);

			Thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ComThread, this, 0, 0);
			if (Thread == INVALID_HANDLE_VALUE || Thread == 0)
			{
				Console::PrintErr(0, "Failed repeatedly. Terminating this connection. \n");
				closesocket(sock);
			}
			else
			{
				Console::Print(Console::Lime, 0, "Thread created successfully!\n");
				Initialized = true;
			}
		}
		else
		{
			Initialized = true;
		}
	}
	~Peer()
	{
		if (Initialized)
		{
			closesocket(Socket);
			TerminateThread(Thread, 0);
		}
	}
};
