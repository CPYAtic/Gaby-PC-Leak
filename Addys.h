#include <Windows.h>
#include "SDK.h"
#include "CMem.h"

RWM::Write Write;
RWM::Read Read;
RWM::Funcs Func;
/*//////////////////////////////Base Addresses//////////////////////////////*/
DWORD WeaponBase = 0x011B5FBC;
DWORD PlayerBase = 0x011B9150;
DWORD BossBase = 0x011B5FC0;
DWORD AmmoXOR = 0x01182A74;
DWORD FiringBase = 0x011B7D50;
#pragma region EnemyBases
DWORD EnemyBase = 0x011B9E50;
DWORD EnemyBase2 = EnemyBase + 4;
DWORD EnemyBase3 = EnemyBase2 + 4;
DWORD EnemyBase4 = EnemyBase3 + 4;
DWORD EnemyBase5 = EnemyBase4 + 4;
DWORD EnemyBase6 = EnemyBase5 + 4;
DWORD EnemyBase7 = EnemyBase6 + 4;
DWORD EnemyBase8 = EnemyBase7 + 4;
DWORD EnemyBase9 = EnemyBase8 + 4;
DWORD EnemyBase10 = EnemyBase9 + 4;
DWORD EnemyBase11 = EnemyBase10 + 4;
DWORD EnemyBase12 = EnemyBase11 + 4;
DWORD EnemyBase13 = EnemyBase12 + 4;
DWORD EnemyBase14 = EnemyBase13 + 4;
DWORD EnemyBase15 = EnemyBase14 + 4;
#pragma endregion
#pragma region Names
DWORD PlayerNameOffs[] = { 0x54, 0x58, 0x14, 0x64 };
DWORD EnemyNameOffs[] = { 0x58, 0x58, 0x14, 0x64 };
DWORD EnemyNameOffs2[] = { 0x5c, 0x58, 0x14, 0x64 };
DWORD EnemyNameOffs3[] = { 0x60, 0x58, 0x14, 0x64 };
DWORD EnemyNameOffs4[] = { 0x64, 0x58, 0x14, 0x64 };
DWORD EnemyNameOffs5[] = { 0x68, 0x58, 0x14, 0x64 };
DWORD EnemyNameOffs6[] = { 0x6c, 0x58, 0x14, 0x64 };
DWORD EnemyNameOffs7[] = { 0x70, 0x58, 0x14, 0x64 };
DWORD EnemyNameOffs8[] = { 0x74, 0x58, 0x14, 0x64 };
DWORD EnemyNameOffs9[] = { 0x78, 0x58, 0x14, 0x64 };
DWORD EnemyNameOffs10[] = { 0x7c, 0x58, 0x14, 0x64 };
DWORD EnemyNameOffs11[] = { 0x80, 0x58, 0x14, 0x64 };
DWORD EnemyNameOffs12[] = { 0x84, 0x58, 0x14, 0x64 };
DWORD EnemyNameOffs13[] = { 0x88, 0x58, 0x14, 0x64 };
DWORD EnemyNameOffs14[] = { 0x8c, 0x58, 0x14, 0x64 };
DWORD EnemyNameOffs15[] = { 0x90, 0x58, 0x14, 0x64 };
#pragma endregion
/*//////////////////////////////Base Addresses//////////////////////////////*/

/*/////////////////////////////////Offsets/////////////////////////////////*/
DWORD HealthOffs[] = { 0x40c, 0x10c, 0x4, 0xc };
DWORD HealthXorOffs[] = { 0x40c, 0x10c, 0x4, 0x14 };
DWORD JumpOffs[] = { 0x7c, 0x11c, 0x4 };
DWORD xoff[] = { 0x7C, 0x124, 0x638, 0x108 };
DWORD yoff[] = { 0x7C, 0x10C, 0x4, 0xB38 };
DWORD poff[] = { 0x7C, 0x124, 0xA8 };
DWORD MynameOffs[] = { 0x54, 0x58, 0x14, 0x64 };


DWORD PlayerYOffs[] = { 0x44c, 0x124, 0x638, 0x118 };
DWORD PlayerXOffs[] = { 0x44c, 0x124, 0x638, 0x108 };
DWORD PlayerZOffs[] = { 0x44c, 0x124, 0x638, 0x110 };
DWORD EnemyYOffs[] = { 0x4c, 0x124, 0x638, 0x118 };
DWORD EnemyXOffs[] = { 0x4c, 0x124, 0x638, 0x108 };
DWORD EnemyZOffs[] = { 0x4c, 0x124, 0x638, 0x110 };
DWORD PingOffs[] = { 0x3c, 0x10c, 0xc, 0x30 };
/*/////////////////////////////////Offsets/////////////////////////////////*/



/*////////////////////////////////Variables////////////////////////////////*/
DWORD PingMS = Func.GetDMA(WeaponBase, PingOffs, 4);
DWORD ServerState = 0x011C5060;
DWORD dwSendChatMessage = 0x00B389F0;//Here we define again the address of SendChatMessage *lobby one*, for hook
DWORD dwGetTickCount = 0x00F81224;
DWORD WRITECOORDS = 0x007D5EB6;
DWORD dwSendPacket = 0x00E2DFF0;
DWORD dwRecvPacket = 0x00E2EA50;
DWORD dwKickHook = 0x00AFC2E0;
DWORD dwWallshot = 0x00FDA5C8;
DWORD dwGameInput = 0x005053F0; //Here we define again the address of GameInput
DWORD dwThreadId = Func.GetProcessID("MicroVolts.exe");
DWORD dwProcessChatMessage = 0x005813D0;
DWORD Speed = 0x01055E70;


DWORD Health = Func.GetDMA(PlayerBase, HealthOffs, 4);   //base, offsets, pointer level.
DWORD Name = Func.GetDMA(BossBase, MynameOffs, 4);
DWORD HealthXOR = Func.GetDMA(PlayerBase, HealthXorOffs, 4);
DWORD Jump = Func.GetDMA(WeaponBase, JumpOffs, 3);
DWORD AFK1 = 0x0043BF53;
DWORD AFK2 = 0x00462879;
DWORD dwFakelagg = 0x011A5538;
#pragma region EnemyY
DWORD EnemyY = Func.GetDMA(EnemyBase, EnemyYOffs, 4);
DWORD Enemy2Y = Func.GetDMA(EnemyBase2, EnemyYOffs, 4);
DWORD Enemy3Y = Func.GetDMA(EnemyBase3, EnemyYOffs, 4);
DWORD Enemy4Y = Func.GetDMA(EnemyBase4, EnemyYOffs, 4);
DWORD Enemy5Y = Func.GetDMA(EnemyBase5, EnemyYOffs, 4);
DWORD Enemy6Y = Func.GetDMA(EnemyBase6, EnemyYOffs, 4);
DWORD Enemy7Y = Func.GetDMA(EnemyBase7, EnemyYOffs, 4);
DWORD Enemy8Y = Func.GetDMA(EnemyBase8, EnemyYOffs, 4);
DWORD Enemy9Y = Func.GetDMA(EnemyBase9, EnemyYOffs, 4);
DWORD Enemy10Y = Func.GetDMA(EnemyBase10, EnemyYOffs, 4);
DWORD Enemy11Y = Func.GetDMA(EnemyBase11, EnemyYOffs, 4);
DWORD Enemy12Y = Func.GetDMA(EnemyBase12, EnemyYOffs, 4);
DWORD Enemy13Y = Func.GetDMA(EnemyBase13, EnemyYOffs, 4);
DWORD Enemy14Y = Func.GetDMA(EnemyBase14, EnemyYOffs, 4);
DWORD Enemy15Y = Func.GetDMA(EnemyBase15, EnemyYOffs, 4);
#pragma endregion
#pragma region EnemyX
DWORD EnemyX = Func.GetDMA(EnemyBase, EnemyXOffs, 4);
DWORD Enemy2X = Func.GetDMA(EnemyBase2, EnemyXOffs, 4);
DWORD Enemy3X = Func.GetDMA(EnemyBase3, EnemyXOffs, 4);
DWORD Enemy4X = Func.GetDMA(EnemyBase4, EnemyXOffs, 4);
DWORD Enemy5X = Func.GetDMA(EnemyBase5, EnemyXOffs, 4);
DWORD Enemy6X = Func.GetDMA(EnemyBase6, EnemyXOffs, 4);
DWORD Enemy7X = Func.GetDMA(EnemyBase7, EnemyXOffs, 4);
DWORD Enemy8X = Func.GetDMA(EnemyBase8, EnemyXOffs, 4);
DWORD Enemy9X = Func.GetDMA(EnemyBase9, EnemyXOffs, 4);
DWORD Enemy10X = Func.GetDMA(EnemyBase10, EnemyXOffs, 4);
DWORD Enemy11X = Func.GetDMA(EnemyBase11, EnemyXOffs, 4);
DWORD Enemy12X = Func.GetDMA(EnemyBase12, EnemyXOffs, 4);
DWORD Enemy13X = Func.GetDMA(EnemyBase13, EnemyXOffs, 4);
DWORD Enemy14X = Func.GetDMA(EnemyBase14, EnemyXOffs, 4);
DWORD Enemy15X = Func.GetDMA(EnemyBase15, EnemyXOffs, 4);
#pragma endregion
#pragma region EnemyZ
DWORD EnemyZ = Func.GetDMA(EnemyBase, EnemyZOffs, 4);
DWORD Enemy2Z = Func.GetDMA(EnemyBase2, EnemyZOffs, 4);
DWORD Enemy3Z = Func.GetDMA(EnemyBase3, EnemyZOffs, 4);
DWORD Enemy4Z = Func.GetDMA(EnemyBase4, EnemyZOffs, 4);
DWORD Enemy5Z = Func.GetDMA(EnemyBase5, EnemyZOffs, 4);
DWORD Enemy6Z = Func.GetDMA(EnemyBase6, EnemyZOffs, 4);
DWORD Enemy7Z = Func.GetDMA(EnemyBase7, EnemyZOffs, 4);
DWORD Enemy8Z = Func.GetDMA(EnemyBase8, EnemyZOffs, 4);
DWORD Enemy9Z = Func.GetDMA(EnemyBase9, EnemyZOffs, 4);
DWORD Enemy10Z = Func.GetDMA(EnemyBase10, EnemyZOffs, 4);
DWORD Enemy11Z = Func.GetDMA(EnemyBase11, EnemyZOffs, 4);
DWORD Enemy12Z = Func.GetDMA(EnemyBase12, EnemyZOffs, 4);
DWORD Enemy13Z = Func.GetDMA(EnemyBase13, EnemyZOffs, 4);
DWORD Enemy14Z = Func.GetDMA(EnemyBase14, EnemyZOffs, 4);
DWORD Enemy15Z = Func.GetDMA(EnemyBase15, EnemyZOffs, 4);
#pragma endregion
#pragma region EnemyName
DWORD EnemyName = Func.GetDMA(BossBase, EnemyNameOffs, 4);
DWORD Enemy2Name = Func.GetDMA(BossBase, EnemyNameOffs2, 4);
DWORD Enemy3Name = Func.GetDMA(BossBase, EnemyNameOffs3, 4);
DWORD Enemy4Name = Func.GetDMA(BossBase, EnemyNameOffs4, 4);
DWORD Enemy5Name = Func.GetDMA(BossBase, EnemyNameOffs5, 4);
DWORD Enemy6Name = Func.GetDMA(BossBase, EnemyNameOffs6, 4);
DWORD Enemy7Name = Func.GetDMA(BossBase, EnemyNameOffs7, 4);
DWORD Enemy8Name = Func.GetDMA(BossBase, EnemyNameOffs8, 4);
DWORD Enemy9Name = Func.GetDMA(BossBase, EnemyNameOffs9, 4);
DWORD Enemy10Name = Func.GetDMA(BossBase, EnemyNameOffs10, 4);
DWORD Enemy11Name = Func.GetDMA(BossBase, EnemyNameOffs11, 4);
DWORD Enemy12Name = Func.GetDMA(BossBase, EnemyNameOffs12, 4);
DWORD Enemy13Name = Func.GetDMA(BossBase, EnemyNameOffs13, 4);
DWORD Enemy14Name = Func.GetDMA(BossBase, EnemyNameOffs14, 4);
DWORD Enemy15Name = Func.GetDMA(BossBase, EnemyNameOffs15, 4);
#pragma endregion
#pragma region PlayerInfo
DWORD PlayerY = Func.GetDMA(PlayerBase, PlayerYOffs, 4);
DWORD PlayerX = Func.GetDMA(PlayerBase, PlayerXOffs, 4);
DWORD PlayerZ = Func.GetDMA(PlayerBase, PlayerZOffs, 4);
DWORD PlayerName = Func.GetDMA(BossBase, PlayerNameOffs, 4);
#pragma endregion

/*////////////////////////////////Variables////////////////////////////////*/

/*////////////////////////////////Booleans////////////////////////////////*/
bool killmessage = false;
bool chatcolor = false;
bool FarmPlayTime = false;
bool packets = false;
bool modespam = false;
bool mapspam = false;
bool roomspam = false;
bool ua = false;
bool uar = false;
bool FarmAI = false;
bool FarmFFA = false;
bool antiafk = false;
bool nametags = false;
bool TSniper = false;
bool TRifle = false;
bool TShotgun = false;
bool wallwalk = false;
bool flyhax = false;
bool elevator = false;
bool isFakeLagg = false;
bool jumphax = false;
bool isFiringRate = false;
bool isMeleeMode = false;
bool isMeleeRange = false;
bool isAim = false;
bool wallshot = false;
bool randomping = false;
bool aimbut = false;
bool speedhacking = false;
bool bypass_censore = false;
bool bypass_penalty = false;
bool chatspammer = false;
/*////////////////////////////////Booleans////////////////////////////////*/


float wspeed = 0.8;
float fspeed = 1;
float espeed = 0.9;