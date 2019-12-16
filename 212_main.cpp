#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <stdio.h>
#include <ctime>
#include "Console.h"
#include "Peer.h"
#include "Server.h"

int main()
{
	srand((unsigned long)std::time(0));

	Server* server = new Server();
	if (server == nullptr)
	{
		Console::PrintErr(0, "Failed to start the server - not enough memory.\n");
		Console::Print(Console::Yellow, 0, "Retrying in ");

		Console::Print(Console::Yellow, 250, "3...",false);
		Console::Print(Console::Yellow, 250, "2...",false);
		Console::Print(Console::Yellow, 250, "1...\n",false);

		if (server == nullptr)
		{
			server = new Server();

			if (server == nullptr)
			{
				Console::PrintErr(0, "Repeatedly failed to start the server - not enough memory.\n");
				Console::Print(Console::Red, 0, "Please try again later.\n");
				std::cin.get();
				return 0;
			}
		}
	}

	char input[1024];
	while (true)
	{
		gets_s(input);

		if (strcmp(Console::ToLowerCase(input), "!exit") == 0)
		{
			Console::Print(Console::Aqua, 0, "Executing command `shutdown`.\n");

			// main/cast server here will send a dc message to every peer
			Sleep(200);

			Console::PrintWarn(0, "Server will shut down in ");
			Console::Print(Console::Yellow, 250, "3...",false);
			Console::Print(Console::Yellow, 250, "2...",false);
			Console::Print(Console::Yellow, 250, "1...\n",false);

			server->~Server();

			Console::Print(Console::Lime, 10, "Server is now offline. Have a nice day! :)\n");
			break;
		}
		else
		{
			Console::PrintErr(0, "Unknown command `%s`.\n", input);
		}
		Sleep(10);
	}


	std::cin.get();
	return 0;
}