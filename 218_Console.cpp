#include <iostream>
#include <Windows.h>
#include <ctime>
#include "Console.h"

unsigned long next = 0;
unsigned long count = 0;

struct PrintParams
{
	unsigned long id = 0;
	Console::CColor color = Console::Normal;
	int smooth = 0;
	char* text = nullptr;
};

void cPrint(int smooth, const char* buff)
{
	for (unsigned long i = 0; i < strlen(buff); ++i)
	{
		printf("%c", buff[i]);
		if (smooth) Sleep(smooth);
	}
}

void __stdcall PrintThread(PrintParams* params)
{
	if (params == nullptr) return;
	if (params->text == nullptr) return;

	while (next != params->id) Sleep(10);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE || hConsole == NULL)
	{
		cPrint(params->smooth, "[ERROR] Failed to obtain output handle.\n");
		return;
	}

	if (!SetConsoleTextAttribute(hConsole, params->color))
	{
		cPrint(params->smooth, "[ERROR] Failed to set console's color.\n");
	}

	cPrint(params->smooth, params->text);

	if (!SetConsoleTextAttribute(hConsole, Console::Light))
	{
		cPrint(params->smooth, "[ERROR] Failed to restore console's color.\n");
		return;
	}

	delete[] params->text;
	delete params;
	++next;
}

std::string Console::GetTimeStamp()
{
	const auto now = std::time(nullptr);
	char cstr[256]{};
	return std::strftime(cstr, sizeof(cstr), "[%d/%m/%Y %H:%M:%S] ", std::localtime(&now)) ? cstr : "";
}

void Console::Print(CColor color, int smooth, const char text[512], ...)
{
	char buff[1024];

	va_list vl;
	va_start(vl, text);

	if (vsnprintf_s(buff, 1024, text, vl) == -1)
	{
		PrintErr(smooth, "Failed to print data (vsnprintf_s failed).\n");
		return;
	}

	char* newtext = new char[strlen(buff) + 1];
	if (newtext == nullptr)
	{
		PrintErr(smooth, "Failed to allocate data for text to print.\n");
		Print(Yellow, smooth, "Retrying...");

		newtext = new char[strlen(buff) + 1];
		if (newtext == nullptr)
		{
			PrintErr(smooth, "Repeatedly failed to allocate data for text.\n");
			return;
		}
		else
		{
			Print(Lime, smooth, "Successfully allocated memory for text.\n");
		}
	}
	memcpy(newtext, buff, strlen(buff) + 1);

	PrintParams* params = new PrintParams;
	if (params == nullptr)
	{
		PrintErr(smooth, "Failed to allocate data for text for print paramaters.\n");
		Print(Yellow, smooth, "Retrying...");

		params = new PrintParams;
		if (params == nullptr)
		{
			PrintErr(smooth, "Repeatedly failed to allocate data for print paramaters.\n");
			return;
		}
		else
		{
			Print(Lime, smooth, "Successfully allocated memory for print paramaters.\n");
		}
	}

	params->text = newtext;
	params->color = color;
	params->smooth = smooth;
	params->id = count++;

	HANDLE hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)PrintThread, params, 0, 0);
	if (hThread == nullptr)
	{
		PrintErr(smooth, "Failed to start the printing thread.\n");
		Print(Yellow, smooth, "Retrying...\n");

		hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)PrintThread, params, 0, 0);
		if (hThread == nullptr)
		{
			PrintErr(smooth, "Repeatedly failed to start the printing thread.\n");
		}
		else
		{
			Print(Lime, smooth, "Successfully started the printing thread.\n");
		}
	}

	va_end(vl);
}

void Console::PrintErr(int smooth, const char text[512], ...)
{
	char buff[1024], buff2[1024 + 16];

	va_list vl;
	va_start(vl, text);

	if (vsnprintf_s(buff, 1024, text, vl) == -1)
		return;

	if (sprintf_s(buff2, "[ERROR] %s", buff) == -1)
		return;

	Print(Red, smooth, buff2);
	va_end(vl);
}

void Console::PrintWarn(int smooth, const char text[512], ...)
{
	char buff[1024], buff2[1024 + 16];

	va_list vl;
	va_start(vl, text);

	if (vsnprintf_s(buff, 1024, text, vl) == -1)
		return;

	va_end(vl);

	if (sprintf_s(buff2, "[WARNING] %s", buff) == -1)
		return;

	Print(Yellow, smooth, buff2);
}

void Console::Separator(int size, int smooth, CColor color, char c)
{
	char buff[256];

	for (int i = 0; i < min(size, 254); ++i) buff[i] = c;

	buff[min(size, 254) - 1] = '\n';
	buff[min(size, 254)] = 0;

	Print(color, smooth, buff);
}


char* Console::ToLowerCase(char* buff)
{
	if (buff == nullptr)
	{
		Console::PrintErr(0, "Failed to cast c-string to lower case : buffer is nullptr.\n");
		return nullptr;
	}

	for (unsigned long i = 0; i < strlen(buff); ++i) if (buff[i] >= 'A' && buff[i] <= 'Z') buff[i] += 32;

	return buff;
}

char* Console::ToUpperCase(char* buff)
{
	if (buff == nullptr)
	{
		Console::PrintErr(0, "Failed to cast c-string to lower case : buffer is nullptr.\n");
		return nullptr;
	}
	for (unsigned long i = 0; i < strlen(buff); ++i) if (buff[i] >= 'a' && buff[i] <= 'z') buff[i] -= 32;

	return buff;
}