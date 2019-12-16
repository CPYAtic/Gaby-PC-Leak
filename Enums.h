#pragma once
enum PlayerState : int
{
	STATE_STOP = 0,
	STATE_FREEFALL = 1,
	STATE_JUMPING = 2,
	STATE_LANDING = 3,
	STATE_MOVING = 4
};
enum TeamID : int
{
	TEAM_EVERYONE = 0,
	TEAM_RED = 1,
	TEAM_BLUE = 2,
	TEAM_OBS = 4,
	TEAM_ZOMBIEIDK = 5
};
enum ServerID : int
{
	SERVER_EU = 1,
	SERVER_NA = 2
};
enum PlayerStatus : int
{
	STATUS_EXIT = 0,
	STATUS_WAITING = 8,
	STATUS_SHOPPING = 5,
	STATUS_INVENTORY = 6,
	STATUS_GACHAPON = 7,
	STATUS_READY = 9,
	STATUS_LOADING = 10,
	STATUS_LOADED = 11,
	STATUS_NORMAL = 12,
	STATUS_DYING = 13
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
	GAME_STATE_TUTORIAL_LOAD = 19,
	GAME_STATE_AGORA_LOAD = 20,
	GAME_STATE_ROOM_2ND = 21,
	GAME_STATE_CLAN_MATCHING = 22,
	GAME_STATE_MOD_LOADING = 23,
	GAME_STATE_MOD_LOADED = 24,
	GAME_STATE_MOD_PLAYING = 25,
	GAME_STATE_TUTORIAL_PLAYING = 26,
	GAME_STATE_AGORA_PLAYING = 27,
	GAME_STATE_MOD_STOP = 28,
	GAME_STATE_MOD_END = 29
};
enum MoveType : int
{
	NOMOVE = 0,
	FORWARD = 1,
	BACKWARD = 256,


};
enum WeaponState : int
{
	EQUIP_STATE = 0,
	FIRE_STATE = 1,
	FIRE_READY_STATE = 2
};
enum WeaponType : int
{
	MELEE = 0,
	RIFLE = 1,
	SHOTGUN = 2,
	SNIPER = 3,
	MG = 4,
	BAZOOKA = 5,
	GRENADE = 6
};
enum HitType : int
{
	DirectShot = 0,
	LegsArms = 1,
	Headshot = 2,
	Normal = 4
};
enum Weaponds : byte
{
	All = 0xDD,
	Select = 0xE5,
	MeleeOnly = 0xC1,
	RifleOnly = 0xC5,
	ShotgunOnly = 0xC9,
	SniperOnly = 0xCD,
	GatlingOnly = 0xD1,
	BazookaOnly = 0xD5,
	GrenadeOnly = 0xD9
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
enum Mode : byte
{
	TDM = 0,
	FFA = 1,
	ItemMatch = 2,
	CTB = 3,
	CloseCombat = 4,
	Enlimination = 5,
	SIM = 6,
	ZombieMode = 7,
	ArmsRace = 8,
	Scrimage = 9,
	BombBattle = 10,
	SniperMode = 11,
	SquareMode = 12,
	BossBattle = 13,
	AIMode = 14
};
enum Map : byte
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
	HauseTop = 14,
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

enum Bones : DWORD
{
	HEAD = 0xC,
	MIDDLE = 0x10,
	RIGHT_ARM = 0x14,
	LEFT_ARM = 0x1C,
	RIGHT_FOOT = 0x24,
	RIGHT_FOOT2 = 0x28,
	LEFT_FOOT = 0x2C,
	LEFT_FOOT2 = 0x30
};