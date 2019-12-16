#include <Windows.h>
#include "SDK.h"

typedef int(__stdcall *tSend) (SOCKET, const char*, int, int);
typedef int(__stdcall *tRecv) (SOCKET, char*, int, int);
tSend oSend = reinterpret_cast<tSend>(0x00F81B50);
tRecv oRecv = reinterpret_cast<tRecv>(0x00F81B54);

template< class charT >
charT toupper(charT ch, const std::locale& loc) {
	return std::use_facet<std::ctype<charT>>(loc).toupper(ch);
}
class CPacket
{
public:

	static void __stdcall CPacket::Send(SOCKET s, const char* buf, int len, int flags);
	static void __stdcall CPacket::Recv(SOCKET s, char* buf, int len, int flags);
	
};
DWORD t1, t2, t3, t4, t5, t6, t7, t8;
DWORD jmp04 = 0x00E2E2DF;
std::string echo;
RWM::Read reaad;
std::string to_hex(int to_convert)
{
	std::string result;
	std::stringstream ss;
	ss << std::hex << to_convert;
	ss >> result;
	return result;
}
std::string toUpper(const std::string& s)
{
	std::string result;

	std::locale loc;
	for (unsigned int i = 0; i < s.length(); ++i)
	{
		result += toupper(s.at(i), loc);
	}
	return result;
}
bool edittxt(char* path, LPCSTR text, int flags = 1)
{
	std::ofstream outfile;
	if (flags == 0)
		outfile.open(path, std::ofstream::trunc);
	else if (flags == 1)
		outfile.open(path, std::ofstream::app);
	else
	{
		outfile.close();
		return false;
	}
	if (!outfile)
		return false;
	if (!(outfile << text << std::endl))
		return false;
	outfile.close();
	return true;
}
std::string readtxt(char* path, int line = -1)
{
	std::string thisline;
	std::ifstream infile(path);
	std::string ret = "";
	int i = 0;
	while (std::getline(infile, thisline))
	{
		if (line < 0)
			ret += thisline + "\n";
		else
		{
			if (i == line)
				return thisline + "\n";
		}
		i++;
	}
	return ret;
}
int allLines(char* path)
{
	std::string thisline;
	std::ifstream infile(path);
	int i = 0;
	while (std::getline(infile, thisline))
		i++;
	return i;
}
void print(int a1, int a2, int a3, int a4)
{
	echo = "SOCKET: " + std::to_string(a1);
	edittxt("C:/hooked_send.txt", echo.c_str());
	echo = "SIZE: " + std::to_string(a4);
	edittxt("C:/hooked_send.txt", echo.c_str());
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", timeinfo);
	std::string str(buffer);
	echo = "TIME: " + str;
	edittxt("C:/hooked_send.txt", echo.c_str());
	echo = "DATA(hex) 0x" + toUpper(to_hex(a3)) + ":\n";
	char szBuffer[1024];
	int j = 0;
	for (int i = 0; i < a4; i++)
	{
		sprintf(szBuffer, "%02x", (reaad.Byte(a3) + i));
		echo += toUpper(szBuffer) + " ";
		j++;
		if (j == 16)
		{
			j = 0;
			echo += "\n";
		}
	}
	edittxt("C:/hooked_send.txt", echo.c_str());
	edittxt("C:/hooked_send.txt", "---------------------------------------------------------------------------------");
}
void __declspec(naked) HkSend()
{
	__asm
	{
		mov eax, [edi + 0x3C]
			mov[t1], eax
			mov[t2], ecx
			mov[t3], edx
			mov[t4], ebx
			mov[t5], esp
			mov[t6], ebp
			mov[t7], esi
			mov[t8], edi
			//2 avoid rash
	}
	if (t7 > 0)
	{
		print(t1, t3, t2, t7);
	}
	__asm
	{
		mov eax, [t1]
			mov ecx, [t2]
			mov edx, [t3]
			mov ebx, [t4]
			mov esp, [t5]
			mov ebp, [t6]
			mov esi, [t7]
			mov edi, [t8]
			push edx
			push esi
			jmp[jmp04]
	}
}