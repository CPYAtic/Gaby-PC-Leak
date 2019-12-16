#include <Windows.h>
#include "SDK.h"
#include "Addys.h"
/*////////////////////////////////////////////////PARAMS////////////////////////////////////////////////*/
typedef uint8_t(__thiscall* tSetModId)(uint32_t, DWORD);
typedef uint8_t(__thiscall* tSetMapId)(DWORD, DWORD*);
typedef uint8_t(__cdecl* tKickHook)(int);
typedef int(__cdecl* tSendMail)(LPCSTR, const char*);
typedef int(__cdecl* tSendGift)(LPCSTR, LPCSTR, int);
typedef int(__cdecl* tSendGift2)(LPCSTR, int);
typedef int(__cdecl* tChangeNickname)(LPCSTR);
typedef int(__cdecl* tKickPlayer)(LPCSTR);
typedef int(__cdecl* tSendSkill)(LPCSTR);

/*////////////////////////////////////////////////PARAMS////////////////////////////////////////////////*/

/*/////////////////////////////////////////Functions - Address/////////////////////////////////////////*/
tKickPlayer oKickPlayer = reinterpret_cast<tKickPlayer>(0x00B21110);
tSendSkill oSendSkill = reinterpret_cast<tSendSkill>(0x00AFCC90);
tChangeNickname oChangeNickname = reinterpret_cast<tChangeNickname>(0x00B0B780);
tSetModId oSetModId = reinterpret_cast<tSetModId>(0x0064BE70);
tSetMapId oSetMapId = reinterpret_cast<tSetMapId>(0x0064B6B0);
tKickHook oKickHook = reinterpret_cast<tKickHook>(0x00AFC2E0);
tSendMail oSendMail = reinterpret_cast<tSendMail>(0x00B17BE0);
tSendGift oSendGift = reinterpret_cast<tSendGift>(0x00B0B120);
tSendGift2 oSendGift2 = reinterpret_cast<tSendGift2>(0x00B0B120);
/*/////////////////////////////////////////Functions - Address/////////////////////////////////////////*/
class CPlayer
{
public:
	static void CPlayer::SetRoomMod(DWORD Value);
	static void CPlayer::SetRoomMap(DWORD Value);
	static void CPlayer::SetSpeed(float Value);
	static void CPlayer::SetAFK(byte Value);
	static void CPlayer::FakeLagg(float Value);
	static void CPlayer::SetY(float Value);
	static void CPlayer::SetX(float Value);
	static void CPlayer::SetZ(float Value);
	static void CPlayer::SetAmmo(int Value);
	static void CPlayer::SetReloadAmmo(int Value);
	static void CPlayer::SetHealth(int Value);
	static void CPlayer::SetPing(int Value);
	static void CPlayer::SetJump(float Value);
	static void CPlayer::SetFiring(int Value);
	static void CPlayer::SetMeleeRange(int Value);
	static char* CPlayer::GetHealth(void);
	static char* CPlayer::GetSpeed(void);
	
	static void CPlayer::SendPaperMessage(LPCSTR Player, const char *msg);
	static void CPlayer::SendPaperGift(LPCSTR Player, LPCSTR GiftMessage, int Itemid);
	static void CPlayer::SendGift(LPCSTR Player, int Itemid);
	static void CPlayer::ChangeName(LPCSTR Nickname);
	static void CPlayer::CreateRoom(DWORD mode, DWORD map, LPCSTR roomname, LPCSTR roompass);
	static void CPlayer::Kick(LPCSTR Nickname);
	static void CPlayer::Skill(LPCSTR Nickname);

	static char* CPlayer::GetGrade(void);
	static char* CPlayer::GetName(void);
	static char* CPlayer::GetExp(void);
	static void CPlayer::NametagsOn();
	static void CPlayer::NametagsOff();
	static void CPlayer::Respawn();
	static void CPlayer::ResetGameMatch();
	static void CPlayer::TeamChange();
	static void CPlayer::SetFarmAI();
	static void CPlayer::SetFarmFFA();
	static void CPlayer::ShowRoomSettings();
	static void CPlayer::RandomModSpam();
	static void CPlayer::RandomMapSpam();

	static void CPlayer::SetRifleTriggerOn();
	static void CPlayer::SetRifleTriggerOff();
	static void CPlayer::SetShotgunTriggerOn();
	static void CPlayer::SetShotgunTriggerOff();
	static void CPlayer::SetSniperTriggerOn();
	static void CPlayer::SetSniperTriggerOff();
};


void CPlayer::ChangeName(LPCSTR Nickname)
{
	oChangeNickname(Nickname);
}
void CPlayer::Skill(LPCSTR Nickname)
{
	oSendSkill(Nickname);
}
void CPlayer::Kick(LPCSTR Nickname)
{
	oKickPlayer(Nickname);
}
void CPlayer::SendPaperMessage(LPCSTR Player, const char *msg)
{
	oSendMail(Player, msg);
}
void CPlayer::SendPaperGift(LPCSTR Player, LPCSTR GiftMessage, int Itemid)
{
	oSendGift(Player, GiftMessage, Itemid);
}
void CPlayer::SendGift(LPCSTR Player, int Itemid)
{
	oSendGift2(Player, Itemid);
}
void CPlayer::SetY(float Value)
{
	Write.Float(PlayerY, Value);
}
void CPlayer::SetX(float Value)
{
	Write.Float(PlayerX, Value);
}
void CPlayer::SetZ(float Value)
{
	Write.Float(PlayerZ, Value);
}
void CPlayer::SetPing(int Value)
{
	Write.Integer(PingMS, Value);
}

void CPlayer::FakeLagg(float Value)
{		
	Write.Float(dwFakelagg, Value);
}
void CPlayer::SetRoomMod(DWORD Value)
{
	uint32_t thisptr = *reinterpret_cast<uint32_t*>(0x11B95CC);
	oSetModId(thisptr,Value);
}
void CPlayer::SetRoomMap(DWORD map)
{
	uint32_t thisptr = *reinterpret_cast<uint32_t*>(0x11B95CC);
	oSetMapId(thisptr, &map);
}
void CPlayer::SetSpeed(float Value)
{
	Write.Float(Speed, Value);
}
void CPlayer::SetHealth(int Value)
{
	*(int*)Health = *(int*)HealthXOR ^ Value; //Some address are encrypted with xor.
}
void CPlayer::SetJump(float Value)
{
	Write.Float(Jump, Value);
}
char* CPlayer::GetHealth(void)
{

	char szBuffer[1024];
	sprintf(szBuffer, "User Health: %i", (*(int*)Health) ^ (*(int*)HealthXOR));
	return szBuffer;
}
char* CPlayer::GetSpeed(void)
{

	char szBuffer[1024];
	sprintf(szBuffer, "Succesfully set speed to (%f).", *(float*)Speed);
	return szBuffer;
}



void CPlayer::SetRifleTriggerOn()
{
	Write.Bytes(0x0058A92C, "\xE9\x3F\x5A\x9F\x00\x90", 6);
	Write.Bytes(0x00F80370, "\x89\x48\x04\x90\x8B\x52\x08\x83\xF9\x00\x75\x0C\xC6\x05\xF7\x20\x1B\x01\x01\xE9\xAA\xA5\x60\xFF\xC6\x05\xF7\x20\x1B\x01\x00\xE9\x9E\xA5\x60\xFF", 36);
}
void CPlayer::SetRifleTriggerOff()
{
	Write.Bytes(0x0058A92C, "\x89\x48\x04\x8B\x52\x08", 6);
}
void CPlayer::SetShotgunTriggerOn()
{
	Write.Bytes(0x0058BBBC, "\xE9\xD3\x47\x9F\x00\x90", 6);
	Write.Bytes(0x00F80394, "\x89\x48\x04\x90\x8B\x52\x08\x83\xF9\x00\x75\x0C\xC6\x05\xF7\x20\x1B\x01\x01\xE9\x16\xB8\x60\xFF\xC6\x05\xF7\x20\x1B\x01\x00\xE9\x0A\xB8\x60\xFF", 36);
}
void CPlayer::SetShotgunTriggerOff()
{
	Write.Bytes(0x0058BBBC, "\x89\x48\x04\x8B\x52\x08", 6);
}
void CPlayer::SetSniperTriggerOn()
{
	Write.Bytes(0x0058C9E8, "\xE9\xCB\x39\x9F\x00\x90", 6);
	Write.Bytes(0x00F803B8, "\x89\x42\x04\x90\x8B\x49\x08\x83\xF8\x00\x75\x0C\xC6\x05\xF7\x20\x1B\x01\x01\xE9\x1E\xC6\x60\xFF\xC6\x05\xF7\x20\x1B\x01\x00\xE9\x12\xC6\x60\xFF", 36);
}
void CPlayer::SetSniperTriggerOff()
{
	Write.Bytes(0x0058C9E8, "\x89\x42\x04\x8B\x49\x08", 6);
}
void CPlayer::NametagsOn()
{
	Write.Bytes(0x009321AA, "\x0F\x85", 2);
}
void CPlayer::NametagsOff()
{
	Write.Bytes(0x009321AA, "\x0F\x84", 2);
}
void CPlayer::SetAFK(byte Value)
{
	Write.Byte(AFK1, Value);
	Write.Byte(AFK2, Value);
}
void CPlayer::Respawn()
{
	DWORD RespawnAddy = 0x00B35310;
	__asm call RespawnAddy
}
void CPlayer::ResetGameMatch()
{
	DWORD ResetAddy = 0x00B1AF50;
	__asm call ResetAddy
}
void CPlayer::TeamChange()
{
	DWORD TeamChange = 0x00B39910;
	__asm call TeamChange
}



void CPlayer::ShowRoomSettings()
{
	/*DWORD Instance = 0x011B95CC;
	__asm mov[ebp - 0x00000260], ecx
	__asm mov edx, [Instance]
	__asm mov[ebp - 0x00000260], edx
	__asm push 0x010070E8 //E_DLG_ROOM_SET
	__asm mov eax, [ebp - 0x00000260]
	__asm mov edx, [eax]
	__asm mov ecx, [ebp - 0x00000260]
	__asm mov eax, [edx + 0x08]
	__asm call eax*/
	CChat::CreateDlg("E_DLG_ROOM_SET", 0);
}

void CPlayer::RandomModSpam()
{
	DWORD RandomMod = 0x0064BA70;
	DWORD Instance = 0x011B95CC;
	__asm mov ecx, [Instance]
	__asm call RandomMod
}
void CPlayer::RandomMapSpam()
{
	DWORD RandomMap = 0x0064B7D0;
	DWORD Instance = 0x011B95CC;
	__asm mov ecx, [Instance]
		__asm call RandomMap
}