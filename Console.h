#pragma once

struct Console
{
	enum CColor
	{
		Normal = 7,
		Blue = 9,
		Lime = 10,
		Aqua = 11,
		Red = 12,
		Fuschia = 13,
		Yellow = 14,
		Light = 15
	};

	static std::string GetTimeStamp();
	static void Print(CColor color, int smooth, const char text[512], ...);
	static void PrintErr(int smooth, const char text[512], ...);
	static void PrintWarn(int smooth, const char text[512], ...);

	static void Separator(int size, int smooth = 1, CColor color = Normal, char c = '-');

	static char* ToLowerCase(char* buff);
	static char* ToUpperCase(char* buff);
};