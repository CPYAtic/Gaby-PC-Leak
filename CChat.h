#include <Windows.h>
#include "SDK.h"
/*////////////////////////////////////////////////PARAMS////////////////////////////////////////////////*/
typedef void(__thiscall* tPrintChatMessage)(uint32_t, LPCSTR, LPCSTR, uint8_t, bool, bool, uint8_t);
//Params: thispointer (class instance), lpText, lpName, uiType, bShow, bRegGameMode, uiColor
typedef int8_t(__thiscall* tProcessChatMessage)(uint32_t, LPCSTR, int32_t);
//Params: thispointer (class instance), lpText, iLen
typedef int8_t(__cdecl* tSendLobbyChatMessage)(uint8_t, uint8_t, LPCSTR);
//Params: uiMode, unknown?, lpText
typedef int8_t(__cdecl* tSendGameChatMessage)(uint8_t, uint8_t, LPCSTR);
//Params: uiMode, unknown?, lpText
typedef DWORD(__stdcall* tGetInstance)();
//Params : none, this is just a function to get instance
typedef uint8_t(__stdcall* tCreateDlg)(LPCSTR, uint8_t);
//Params : lpText(Dialog name), unknown(0)
typedef int8_t(__cdecl* tCreateGameDlg)(LPCSTR, uint8_t);
//Params : lpText(Dialog name), unknown(0)
typedef uint8_t(__thiscall* tCloseDlg)(uint32_t, LPCSTR);
//Params : instance,lpText(Dialog name)
typedef uint8_t(__thiscall* tGetMapInfo)(uint32_t, uint8_t);
//Params : instance, (int)infoname.cdb
/*////////////////////////////////////////////////PARAMS////////////////////////////////////////////////*/

/*/////////////////////////////////////////Functions - Address/////////////////////////////////////////*/
tGetInstance GetInstance = reinterpret_cast<tGetInstance>(0x005A6220); //GetInstance
tPrintChatMessage oPrintChatMessage = reinterpret_cast<tPrintChatMessage>(0x00581AC0); //PrintChatMessage
tProcessChatMessage oProcessChatMessage = reinterpret_cast<tProcessChatMessage>(0x005813D0); //ProcessChatMessage
tSendLobbyChatMessage oSendLobbyChatMessage = reinterpret_cast<tSendLobbyChatMessage>(0x00B389F0); //SendChatMessage(Lobby)
tSendGameChatMessage oSendGameChatMessage = reinterpret_cast<tSendGameChatMessage>(0x00B37910); //SendChatMessage(InGame)
tCreateDlg oCreateDlg = reinterpret_cast<tCreateDlg>(0x005C1A50);
tCreateGameDlg oCreateGameDlg = reinterpret_cast<tCreateGameDlg>(0x005C1AA0);
tCloseDlg oCloseDlg = reinterpret_cast<tCloseDlg>(0x00E76250);
/*/////////////////////////////////////////Functions - Address/////////////////////////////////////////*/

class CChat
{
public:
	enum Type : uint8_t
	{
		NORMAL = 0,
		HELP = 1,
		WHISPER = 2,
		TEAM = 5,
		INFO = 10,
		GM = 11,
		TIP = 12
	};

	enum Color : uint8_t
	{
		NONE = 0,
		PINK = 1,
		YELLOW = 2,
		GREEN = 4
	};
	static void __stdcall CChat::PrintChatMessage(LPCSTR lpText, LPCSTR lpName, uint8_t uiType, bool bShow, bool bRegGameMode, uint8_t uiColor);
	static void __stdcall CChat::ProcessChatMessage(LPCSTR lpText, int8_t iLen);
	static void __stdcall CChat::SendGameChatMessage(uint8_t uiMode, uint8_t unknown, LPCSTR lpText);
	static void __stdcall CChat::SendLobbyChatMessage(uint8_t uiMode, uint8_t unknown, LPCSTR lpText);
	static void __stdcall CChat::CreateDlg(LPCSTR lpText,uint8_t unknown);
	static void __stdcall CChat::CreateGameDlg(LPCSTR lpText, uint8_t unknown);
	static void __stdcall CChat::CloseDlg(LPCSTR lpText);
	

};


void __stdcall CChat::PrintChatMessage(LPCSTR lpText, LPCSTR lpName, uint8_t uiType, bool bShow, bool bRegGameMode, uint8_t uiColor)
{
	uint32_t thisptr = *reinterpret_cast<uint32_t*>(0x011B95A8 + 0x108); //This can also be used, see GetInstance function to see what I mean
	//uint32_t thisptr = *reinterpret_cast<uint32_t*>(0x11B95CC);
	oPrintChatMessage(thisptr, lpText, lpName, uiType, bShow, bRegGameMode, uiColor);
}
void __stdcall CChat::ProcessChatMessage(LPCSTR lpText, int8_t iLen)
{
	uint32_t thisptr = *reinterpret_cast<uint32_t*>(0x011B95A8 + 0x108);
	//uint32_t thisptr = *reinterpret_cast<uint32_t*>(0x11B95CC);
	oProcessChatMessage(thisptr, lpText, iLen);
}
void __stdcall CChat::SendGameChatMessage(uint8_t uiMode, uint8_t unknown, LPCSTR lpText)
{
	oSendGameChatMessage(uiMode, unknown, lpText);
}
void __stdcall CChat::SendLobbyChatMessage(uint8_t uiMode, uint8_t unknown, LPCSTR lpText)
{
	oSendLobbyChatMessage(uiMode, unknown, lpText);
}
void __stdcall CChat::CreateDlg(LPCSTR lpText, uint8_t unknown)
{
	oCreateDlg(lpText, unknown);
}
void __stdcall CChat::CreateGameDlg(LPCSTR lpText, uint8_t unknown)
{
	oCreateGameDlg(lpText, unknown);
}
void __stdcall CChat::CloseDlg(LPCSTR lpText)
{
	//uint32_t thisptr = *reinterpret_cast<uint32_t*>(0x011B95A8 + 0x108);
	uint32_t thisptr = *reinterpret_cast<uint32_t*>(0x11B95CC);
	oCloseDlg(thisptr, lpText);
}