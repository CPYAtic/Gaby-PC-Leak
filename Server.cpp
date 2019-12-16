#include <iostream>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <iomanip>
#include <sstream>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

UINT __cdecl DataThreadFunc(LPVOID pParam);
std::string timestamp()
{
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	std::stringstream ssTp;
	ssTp << std::put_time(&tm, "[%d-%m-%Y %H-%M-%S]");
	return ssTp.str();
}
struct Client
{
	HANDLE Thread = 0;
	SOCKET sock = 0;
};

int main()
{
	WSADATA wsa;
	SOCKET s, newsock;
	sockaddr_in server, client;
	std::vector<Client*> Clients;

	//printf("Initializing Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		//printf("Failed. Error Code : %d", WSAGetLastError());
		std::cin.get();
		return 0;
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(13001);

	//printf("Creating a socket...\n");
	printf("%s Initializing connections\n", timestamp().c_str());
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		//printf("Could not create socket : %d", WSAGetLastError());
		std::cin.get();
		return 0;
	}

	//printf("Binding...\n");
	printf("%s Binding server's port\n", timestamp().c_str());
	if (bind(s, (sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		//printf("Bind failed with error code : %d", WSAGetLastError());
		std::cin.get();
		return 0;
	}

	//printf("Press [INSERT] to exit safely..\n");
	//printf("----------------------\n");

	while (!GetAsyncKeyState(VK_INSERT))
	{
		//printf("Listening...\n");
		printf("%s Waiting for users\n", timestamp().c_str());
		listen(s, 100);

		char* message = nullptr;
		int c = sizeof(sockaddr_in);

		if ((newsock = accept(s, (sockaddr *)&client, &c)) != INVALID_SOCKET)
		{
			//puts("Connection accepted!");

			socklen_t len = 0;
			sockaddr_storage addr;
			char ipstr[INET6_ADDRSTRLEN];
			int port = 0;

			len = sizeof(addr);
			getpeername(newsock, (sockaddr*)&addr, &len);

			if (addr.ss_family == AF_INET)
			{
				sockaddr_in *s = (sockaddr_in*)&addr;
				port = ntohs(s->sin_port);
				inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof(ipstr));
			}

			printf("Peer IPv4: %s\n", ipstr);
			HANDLE thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)DataThreadFunc, (LPVOID)newsock, 0, 0);

			if (thread == 0)
			{
				printf("Failed to create a thread : %d\n", GetLastError());
				printf("Terminating this connection. Please try again!\n");
				closesocket(newsock);
			}
			else
			{
				Client* client = new Client;
				client->sock = newsock;
				client->Thread = thread;
				Clients.push_back(client);
			}
		}
		else
		{
			printf("Accept failed with error code : %d\n", WSAGetLastError());
			std::cin.get();
			return 0;
		}

		newsock = INVALID_SOCKET;
	}

	printf("Terminating...\n");
	for (auto i : Clients)
	{
		closesocket(i->sock);
		TerminateThread(i->Thread, 0);
		delete i;
	}

	Clients.clear();
	Sleep(2000);
	return 0;
}
void SendData(SOCKET s, const char *buf, int len, int flags)
{
	if (send(s, buf, len, flags) != len)
	{
		printf_s("Failed to send response to %04X: %d\n", s, WSAGetLastError());
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf_s("Sent data (%d) to %04X: ", len, s);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
		for (int i = 0; i < len; ++i)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
			printf_s("%02X ", (unsigned char)buf[i]);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
		}
		printf("\n");
	}
}
struct SAuthResult
{
	DWORD id;
	DWORD cancerID[3];
	int Level;
	int XP;
	int TOTAL_KILLS;
	int DEATHS;
	int ASSISTS;
	int WINS;
	int LOSSES;
	int DRAWS;
	char name[16];
	WORD clanFrontIconID;
	WORD clanBackIconID;
	char clanName[20];
};
UINT __cdecl DataThreadFunc(LPVOID pParam)
{
	SOCKET sock = reinterpret_cast<SOCKET>(pParam);

	char data[4096];
	int size = 0;


	SendData(sock, "\x06\x00\x40\x00\x7B\x64\x22\x74\x5F\x2E\x69\x95\xD3\x53\x77\x5B", 16, 0);
	printf("%s Server connection initialized\n", timestamp().c_str());

	ZeroMemory(data, sizeof(data));
	while (size = recv(sock, data, sizeof(data), 0))
	{
		if (size == SOCKET_ERROR)
		{
			printf_s("Connection %04X failed with error code : %d\n", sock, WSAGetLastError());
			return 0;
		}
		
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf_s("Recieved data (%d) from %04X: ", size, sock);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
		for (int i = 0; i < size; ++i)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
			printf_s("%02X ", (unsigned char)data[i]);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
		}
		printf("\n");

		if (size == 124)
		{
			SAuthResult* AuthResult = new SAuthResult();
			
			AuthResult->Level = 105;
			AuthResult->XP = 1000000;
			AuthResult->TOTAL_KILLS = 1337;
			AuthResult->DEATHS = 0;
			AuthResult->ASSISTS = 0;
			AuthResult->WINS = 1337;
			AuthResult->LOSSES = 0;
			AuthResult->DRAWS = 0;
			memcpy(&AuthResult->name, "[GM]Gaby", 14);
            #pragma region Info
			AuthResult->id = 0x81600007;
			AuthResult->cancerID[0] = 0x02010597; AuthResult->cancerID[1] = 0x0599183f; AuthResult->cancerID[2] = 0x71854038;
			AuthResult->clanFrontIconID = 0x24;
			AuthResult->clanBackIconID = 0x00;
			memcpy(&AuthResult->clanName, "feel-dope", 14);
			printf("%s LOGIN_PLAYER_INFO - Initialized\n", timestamp().c_str());
			printf("LEVEL : %d\nXP : %d\nNAME : %s\n", AuthResult->Level, AuthResult->XP, AuthResult->name);
			SendData(sock, (char*)AuthResult, 88, 0);
			printf("%s LOGIN_SERVER_INFO - Initialized \n", timestamp().c_str());

            #pragma endregion

		}
		if (size == 8)
		{
			SendData(sock, "\x02\x00\x40\x80\xD3\x05\xFB\x02\x01\x00\x00\x00\x02\x00\x00\x00", 16, 0);
			
			printf("%s LOGIN_AUTHORIZE - Initialized \n", timestamp().c_str());
			
		}







	}
	return 0;
}