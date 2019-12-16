#include <iostream>
#include <string>
#include <Windows.h>
#include <FileManager.h>
#include <cstddef>

#include "Structures.h"

int main()
{
	FileManager Users;
	FileManager Clans;
	FileManager Mail;
	FileManager Gifts;
	FileManager Chat;

	SetConsoleTitleA("MV-Dope Database");

	std::string input;

	if (Users.IsOpen() && Clans.IsOpen() && Mail.IsOpen() && Gifts.IsOpen() && Chat.IsOpen())
	{
		std::cout << "Wellcome to MV-DOPE Database Command Prompt" << std::endl;
		std::cout << "------------------------------" << std::endl;

	}
	else
	{
		std::cout << "Something went wrong. Please run me as admin." << std::endl;
		std::cout << "------------------------------" << std::endl;
		std::cin.get();

		return 1;
	}

	// count `Database` where `CellName`=`Value`
	// set `CellName`=`Value` in `Database` where `CellName`=`Value`
	// get `Cellname` from `Database` where `CellName`=`Value`
	// push back `Database` 
	// pop back `Database`
	// backup `Database` to <FilePath>

	std::string sDatabases   [] = { "Users", "Clans", "Mail", "Gifts", "Chat" };
	FileManager* dmDatabases [] = { &Users, &Clans, &Mail, &Gifts, &Chat };

	do
	{
		std::cout << "[FDB] ";
		std::getline(std::cin, input);

		if (input.substr(0, 7) == "count `")
		{
			bool found = false;
			for (int i = 0; i < 5; ++i)
			{

			}
		}


	} while (input != "exit");

	return 0;
}