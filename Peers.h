#pragma once
#include <vector>
enum WeaponRestriction : byte
{
	MeleeOnly = 0,
	RifleOnly = 1,
	ShotgunOnly = 2,
	SniperOnly = 3,
	GatlingOnly = 4,
	BazookaOnly = 5,
	GrenadeOnly = 6,
	All = 7,
	WeaponSelect = 9
};

enum ModeIndex : byte
{
	TeamDeathMatch = 0,
	FreeForAll = 1,
	ItemMatch = 2,
	CaptureTheBattery = 3,
	CloseCombat = 4,
	Elimination = 5,
	SuperItemMatch = 6,
	ZombieMode = 7,
	ArmsRace = 8,
	Scrimage = 9,
	BombBattle = 10,
	SniperMode = 11,
	SquareMode = 12,
	BossBattle = 13,
	AIMode = 14
};
enum MapIndex : byte
{
	Random = 0,
	Chess = 1,
	ToyFleet = 2,
	TrackersFactory = 3,
	Beach = 4,
	BattleMine = 5,
	ToyGarden = 6,
	Neighbothood = 7,
	MagicPaperLand = 8,
	HobbyShop = 9,
	Academy = 10,
	TheStudio = 11,
	//12 doesn't exsist
	PVCFactory = 13,
	HouseTop = 14,
	WildWest = 15,
	RumpusRoom = 16,
	Cargo = 17,
	PVCFacoryNight = 18,
	ForgottenJunkYard = 19,
	JunkYard = 20,
	GothicCastle = 21,
	Bitmap = 22,
	RockBand = 23,
	//24, 25, 26 doesn't exsist
	ModelShip = 27,
	AcademyInvasion = 28,
	Foosball = 29,
	Bitmap2 = 30,
	ToyGarden2 = 31,
	RockBandS = 32,
	RockBandW = 33,
	CastleSiege = 34,
	TempleRuins = 35,
	BitmapPlant = 36,
	TheAftermath = 37,
	//38, 39, 40, 41, 42 doesn't exsist
	SinglePlayerEasy = 43,
	SinglePlayerHard = 44,
	BossMap = 45,
	SquareMap = 46,
	SquareMap2 = 47
};

enum Players : byte
{
	Players2 = 2,
	Players4 = 4,
	Players6 = 6,
	Players8 = 8,
	Players10 = 10,
	Players12 = 12,
	Players14 = 14,
	Players16 = 16,
};

enum Time : byte
{
	Mins10 = 0x40,
	Mins15 = 0xE0,
	Mins20 = 0x80,
	Mins25 = 0x20,
	Mins30 = 0xC0,

	Mins2 = 0x40,
	Mins3 = 0x60,
	Mins4 = 0x80,
	Mins5 = 0xA0,

	Unlimited = 0x00
};

enum Kills : byte
{
	//FFA, TDM, ItemMatch, SIM, SniperMode, CloseCombat
	Kills10 = 0x21, //A.I only
	Kills20 = 0x22,
	Kills40 = 0x24, //FFA only
	Kills30 = 0x23,
	Kills50 = 0x25,
	Kills80 = 0x28,
	Kills100 = 0x2A,
	Kills130 = 0x2D,
	Kills150 = 0x2F,

	//Arms Race
	Points20 = 0x34,

	//Enlimination, ZombieMode, BombBattle
	Rounds3 = 0x23,
	Rounds5 = 0x25,
	Rounds7 = 0x27,
	Rounds9 = 0x29,

	//Square
	NoGoals = 0x21,

	//Scrimmage
	HP10000 = 0x21,
	HP20000 = 0x22,
	HP30000 = 0x23,

	//BossBattle
	BossCleare = 0x21,

	//CTB
	Points3 = 0x23,
	Points5 = 0x25,
	Points7 = 0x27,
	Points9 = 0x29
};

enum GameState : int
{
	GAME_STATE_NONE = 0,
	GAME_STATE_NONE2 = 1,
	GAME_STATE_INTRO = 2,
	GAME_STATE_LOGIN = 3,
	GAME_STATE_DISCONNECT = 4,
	GAME_STATE_SELECT_SERVER = 5,
	GAME_STATE_ROOM_JOIN = 6,
	GAME_STATE_CHANNEL_JOIN = 7,
	GAME_STATE_ROOM_CREATE = 8,
	GAME_STATE_FOLLOW_SERVER = 9,
	GAME_STATE_AVATAR_SELECT = 10,
	GAME_STATE_AVATAR_CREATE = 11,   //this check is used in other mv games to show namechanger when new account at beginning
	GAME_STATE_PLAYERID_CREATE = 12,
	GAME_STATE_LOBBY_AGORA = 13,
	GAME_STATE_VENDOR = 14,
	GAME_STATE_INVENTORY = 15,
	GAME_STATE_GACHAPON = 16,
	GAME_STATE_PLAYER_CREATE = 17,
	GAME_STATE_TUTORIAL = 18,
	GAME_STATE_TUTORIAL_LOAD = 19, //singlewave load
	GAME_STATE_AGORA_LOAD = 20,
	GAME_STATE_ROOM_2ND = 21,
	GAME_STATE_CLAN_MATCHING = 22,
	GAME_STATE_MOD_LOADING = 23,
	GAME_STATE_MOD_LOADED = 24,
	GAME_STATE_MOD_PLAYING = 25,
	GAME_STATE_TUTORIAL_PLAYING = 26, //singlewave play
	GAME_STATE_AGORA_PLAYING = 27,
	GAME_STATE_MOD_STOP = 28,
	GAME_STATE_MOD_END = 29
};

enum PlayerGrade : BYTE
{
	Player = 0x02,
	MOD = 0x03,
	TESTER = 0x04,
	GM = 0x09,
};
struct RoomInformations
{
	int RoomNumber = 0;
	byte HostSessionByte = 0;
	int HostMS = 2;
	MapIndex MapID;
	ModeIndex ModeID;
	int PlayersCount = 1;
	int MaxPlayers;
	bool bPlaying = false;
	bool RoomNotObserverLocked = true;
	bool RoomPasswordLocked = false;
	int TimeLimit;
	int ScoreLimit;
	WeaponRestriction restriction;
	char Title[32];
	char Password[16];
	bool RoomExists;
};
class Peer
{
	WORD id = 0;

	bool detailsSent = false;
	bool verified = false;
	int ItemsOwned = 7;

	bool AwaitData(char* data, int& len);
	bool SendData(char* data, int len);
	bool SendConfirmMessage();

	void Communicate();
	static void ComThread(Peer* _this);
public:
	struct ItemInfo_t
	{
		DWORD ID;
		DWORD Entity;
		DWORD Type = 1;
		DWORD Durability;
		DWORD Energy = 0;
		bool HasSeal = false;
		DWORD SealLevel = 0;
		DWORD Timestamp;
		DWORD TimestampExpireDate;
	};
	bool Initialized = false;
	HANDLE ClientHandle = NULL;
	char IPv4[INET6_ADDRSTRLEN];
	int ServerNumberID = 0;
	HANDLE Thread = nullptr;
	SOCKET Socket = NULL;
	bool isChat = false;
	char Nickname[16] = "";
	int level = 0;
	int PingMS = 0;
	std::vector<std::vector<ItemInfo_t>> CharactersEquipement;
	BYTE CurrentCharacter = 0;
	DWORD First4Bytes = 0;
	WORD ConnID = 0x0000;
	WORD ClanIcon1 = 0;
	WORD ClanIcon2 = 0;
	DWORD SessionID = 0x0;
	BYTE RoomMapID = 0x0;
	BYTE SessionByte = 0x0;
	PlayerGrade Grade = Player;
	GameState PlayerGameState = GAME_STATE_SELECT_SERVER;
	RoomInformations MyRoom;
	DWORD ItemEntity = 1;
	DWORD pNameID = 0;
	DWORD SentpNameID = 0;

	bool SendLobbyMessage(const char* nickname, const char* message, BYTE bType);
	bool UserList();
	bool RoomList(RoomInformations channelRooms[255]);
	bool RoomUserList();
	bool SendDetails();
	bool SendInvFull(std::vector<ItemInfo_t> items, bool firstSession);
	bool SendInvConfirm();
	bool SendEquipConfirm();
	bool SendEquipInfo(BYTE Character, std::vector<ItemInfo_t> items);
	bool ConfirmRoomJoin();
	bool AnswerPush(unsigned char pType, unsigned char data);
	bool AnswerRoomCreateRequest();
	bool LeaveRoomRequest(BYTE data);
	bool LeaveMatchRequest(DWORD SessionID);
	bool RoomStart(BYTE idk1, BYTE idk2, DWORD SessionID, BYTE mapid);
	bool RoomStart2();
	bool PlayerSpawn(DWORD SessionID);
	bool PlayerSpawn2();
	bool SingleWaveSuccess();
	bool SingleWaveFail();
	bool UpdateInventory(std::vector<ItemInfo_t> Items);
	bool UpdateInventoryManual(DWORD SessionID, DWORD ItemId);
	bool ClearInventory();
	bool GachaponData();
	bool SendMPRTInfoGachapon();
	bool SendNotice(const char msg[512]);
	bool RoomJoin(WORD roomnumber);
	bool EndMatch();
	bool ChangeMapSend(BYTE mapid, BYTE modeid);
	bool CapsuleSpinReq();;

	Peer(SOCKET sock, char IPv4[INET6_ADDRSTRLEN])
	{
		srand((unsigned long)std::time(nullptr));

		this->Socket = sock;
		memcpy(this->IPv4, IPv4, strlen(IPv4) + 1);
		Thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ComThread, this, 0, 0);
		if (Thread == INVALID_HANDLE_VALUE || Thread == 0)
		{
			Sleep(100);

			Thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ComThread, this, 0, 0);
			if (Thread == INVALID_HANDLE_VALUE || Thread == 0)
				closesocket(sock);
			else 
				Initialized = true;
		}
		else Initialized = true;	
	}
	~Peer()
	{
		if (Initialized)
		{
			closesocket(Socket);
			TerminateThread(Thread, 0);
		}
	}
};
