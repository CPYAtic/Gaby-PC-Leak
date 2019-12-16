#include <iostream>
#include <Windows.h>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "Console.h"
std::unique_ptr<CLog> Log = std::make_unique<CLog>();
std::unique_ptr<CLog> ChatLog = std::make_unique<CLog>();
std::string getTimeStamp2()
{
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	std::time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "[%d-%m-%Y %H:%M:%S]", timeinfo);
	return std::string(buffer);
}
void CLog::Initialize(const std::string& Path)
{
	File.open(Path, std::ofstream::out | std::ofstream::app);

	if (!File.is_open())
		std::runtime_error("Could not open file: " + Path);
}

void CLog::Write(const std::string& Text)
{
	time_t rawtime = std::time(0);
	tm time;
	localtime_s(&time, &rawtime);
	char buffer[80];

	strftime(buffer, sizeof(buffer), "[%d-%m-%Y %H:%M:%S]", &time);
	const std::string Output = std::string(buffer) + " " + Text;

	WriteMutex.lock_shared();
	File << Output << std::endl;
	WriteMutex.unlock_shared();
}


void Console::Print(std::string msg)
{
	printf("\x1B[35m%s\033[0m ", getTimeStamp2().c_str());
	printf("\x1B[31m[SERVER] %s\033[0m", msg.c_str());
}

void Console::PrintErr(std::string msg)
{
	printf("\x1B[35m%s\033[0m ", getTimeStamp2().c_str());
	printf("\x1B[32m[SERVER-ERROR] %s\033[0m", msg.c_str());
}

void Console::PrintWarn(std::string msg)
{
	printf("\x1B[35m%s\033[0m ", getTimeStamp2().c_str());
	printf("\x1B[33m[SERVER-WARNING] %s\033[0m", msg.c_str());
}
void Console::PrintPackets(const char* Info, char* buff, int len, int pName)
{
	printf("%s [PNAME: %d] %s [SIZE: %d]", getTimeStamp2().c_str(), pName, Info, len);

	for (int i = 0; i < len; i++)
		printf(" %02X", static_cast<byte>(buff[i]));

	printf("\n");
}

