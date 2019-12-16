#pragma once

struct _User
{
	long long	ID = 0;
	char		LoginID[32];
	char		Password[32];
	char		Email[128];
	char		Nickname[16];
	long		Level = 0;
	long		Exp = 0;
	long		VIP = 0;
	long		MicroPoints = 18000;
	long		RockTokens = 0;
	long		Coins = 0;
	long		Coupons = 0;
	long		BatteryCapacity = 1000;
	long		Battery = 0;

	long		RegisterDate = 0;

	long		SinglePlayer = 3;
	long		Playtime = 0;

	long		LastLogin = 0;
	long		ChannelID = 0;
	long long	ConnID = -1;

	char		Character = 0;
	long		RoomID = -1;

	bool		Online = false;
	bool		RecievedDaily = false;

	bool		Muted = false;
	long		MuteExpire = 0;

	long		ServerState = 0;

	long		Wins = 0;
	long		Loses = 0;
	long		Draws = 0;

	long		Deaths = 0;
	long		Assists = 0;

	struct Kills
	{
		long	Total = 0;
		long	Melee = 0;
		long	Rifle = 0;
		long	Shotgun = 0;
		long	Sniper = 0;
		long	Gatling = 0;
		long	Bazooka = 0;
		long	Grenade = 0;
		long	Zombie = 0;
		long	Infected = 0;

	}			Kills;

	long		Invasion = 0;
	long		Killstreak = 0;
	long		Headshots = 0;

	bool		DoTutorial = true;

	struct Characters
	{
		bool	Naomi = true;
		bool	Knox = true;
		bool	Pandora = false;
		bool	Chip = false;
		bool	Kai = false;
		bool	Simon = false;
		bool	Sophitia = false;
	}			Characters;

	struct Clan
	{
		long	ID = 0;

		long	Contribution = 0;

		long	Wins = 0;
		long	Loses = 0;
		long	Draws = 0;

		long	Kills = 0;
		long	Deaths = 0;
		long	Assists = 0;
	}			Clan;

	enum Grade : char
	{
		Normal = 0,
		Banned = 1,
		Mod = 2,
		TMod = 3,
		GM = 4,
		Tester = 5
	}			Grade = Normal;

	short		InventorySize = 100;
	short		InventoryCount = 0;

	long		Inventory[1000];

	struct Achievements
	{
		struct Battle
		{
			char	HardKnockLife = 0;
			short	WeAreTheChampions = 0;
			short	Assasin = 0;
			char	SuperJumper = 0;
			char	EnamelEater = 0;
			char	ShieldMania = 0;
			char	DoubleKillMaster = 0;
			char	MultiKillMaster = 0;
			char	GoodShotMaster = 0;

			bool	Complete = false;
		}		Battle;

		struct Melee
		{
			short	Expertise = 0;
			char	SwipeAttack = 0;
			char	SwingAttack = 0;
			char	JumpJumpJump = 0;

			bool	Complete = false;
		}		Melee;

		struct Rifle
		{
			short	Expertise = 0;
			char	AccurateShooter = 0;
			char	PinpointPrecision = 0;
			char	FlyLikeAnEagle = 0;

			bool	Complete = false;
		}		Rifle;

		struct Shotgun
		{
			short	Expertise = 0;
			char	OneHitWonder = 0;
			char	OutOfRange = 0;
			char	Suprise = 0;

			bool	Complete = false;
		}		Shotgun;

		struct Sniper
		{
			short	Expertise = 0;
			char	WatchYourHead = 0;
			char	JumpersBeware = 0;
			char	Bulseye = 0;

			bool	Complete = false;
		}		Sniper;

		struct Gatling
		{
			short	Expertise = 0;
			char	TooWarm = 0;
			char	SpinningTheBarrels = 0;
			char	HeavyFlight = 0;

			bool	Complete = false;
		}		Gatling;

		struct Bazooka
		{
			short	Expertise = 0;
			char	DirectShots_Lucky = 0;
			char	RocketJumper = 0;
			char	TwoBirdsOneStone = 0;

			bool	Complete = false;
		}		Bazooka;

		struct Grenade
		{
			short	Expertise = 0;
			char	AngelAccuracy = 0;
			char	FlyingPills = 0;
			char	ExplosiveShowing = 0;

			bool	Complete = false;
		}		Grenade;
	} Achievements;

	long		Equipment[7][18];

	struct DailyMissions
	{
		long	Completion = 0;
		bool	Completed = false;
	}			Missions[3];

	long		LuckMetter = 0;

	long		Friends[50];
	long		Blocked[50];

	long		Reserved[695];
};

struct _Clan
{
	long long	ID = 0;
	char		Name[16];

	long		Author = 0;
	long		Leader[2] = { 0 };
	long		Created = 0;

	long		Wins = 0;
	long		Loses = 0;
	long		Draws = 0;

	long		Kills = 0;
	long		Deaths = 0;
	long		Assists = 0;
};

struct _Mail
{
	long long	ID = 0;
	long		Author = 0;
	long		Reciever = 0;

	long		Timestamp = 0;

	char		Message[1024];
};

struct _Gifts
{
	long long	ID = 0;
	long		Author = 0;
	long		Reciever = 0;

	long		Timestamp = 0;

	long		Item = 0;
};

struct _Chat
{
	long long	ID = 0;
	long		Author = 0;
	long		Reciever = 0;

	long		Type = 0;
	long		Color = -1;
	long		Timestamp = 0;

	long		ChannelID = 0;
	long		RoomID = 0;

	char		Message[512];
};

const char* sUsers[] = {
	"ID",
	"LoginID",
	"Password",
	"Email",
	"Nickname",
	"Level",
	"Exp",
	"VIP",
	"MicroPoints",
	"RockTokens",
	"Coins",
	"Coupons",
	"BatteryCapacity",
	"Battery",
	"RegisterDate",
	"SinglePlayer",
	"Playtime",
	"LastLogin",
	"ChannelID",
	"ConnID",
	"Character",
	"RoomID",
	"Online",
	"RecievedDaily",
	"Muted",
	"MuteExpire",
	"ServerState",
	"Wins",
	"Loses",
	"Draws",
	"Deaths",
	"Assists",
	"Kills.Total",
	"Kills.Melee",
	"Kills.Rifle",
	"Kills.Shotgun",
	"Kills.Sniper",
	"Kills.Gatling",
	"Kills.Bazooka",
	"Kills.Grenade",
	"Kills.Zombie",
	"Kills.Infected",
	"Invasion",
	"Killstreak",
	"Headshots",
	"DoTutorial",
	"Characters.Naomi",
	"Characters.Knox",
	"Characters.Pandora",
	"Characters.Chip",
	"Characters.Kai",
	"Characters.Simon",
	"Characters.Sophitia",
	"Clan.ID",
	"Clan.Contribution",
	"Clan.Wins",
	"Clan.Loses",
	"Clan.Draws",
	"Clan.Kills",
	"Clan.Deaths",
	"Clan.Assists",
	"Grade",
	"InventorySize",
	"InventoryCount",
	"Achievements.Battle.HardKnockLife",
	"Achievements.Battle.WeAreTheChampions",
	"Achievements.Battle.SuperJumper",
	"Achievements.Battle.EnamelEater",
	"Achievements.Battle.ShieldMania",
	"Achievements.Battle.DoubleKillMaster",
	"Achievements.Battle.MultiKillMaster",
	"Achievements.Battle.GoodShotMaster",
	"Achievements.Battle.Complete",
	"Achievements.Melee.Expertise",
	"Achievements.Melee.SwipeAttack",
	"Achievements.Melee.SwingAttack",
	"Achievements.Melee.JumpJumpJump",
	"Achievements.Melee.Complete",
	"Achievements.Rifle.Expertise",
	"Achievements.Rifle.AccurateShooter",
	"Achievements.Rifle.PinpointPrecision",
	"Achievements.Rifle.FlyLikeAnEagle",
	"Achievements.Rifle.Complete",	
	"Achievements.Shotgun.Expertise",
	"Achievements.Shotgun.OneHitWonder",
	"Achievements.Shotgun.OutOfRange",
	"Achievements.Shotgun.Suprise",
	"Achievements.Shotgun.Complete",
	"Achievements.Sniper.Expertise",
	"Achievements.Sniper.WatchYourHead",
	"Achievements.Sniper.JumpersBeware",
	"Achievements.Sniper.Bulseye",
	"Achievements.Sniper.Complete",
	"Achievements.Gatling.Expertise",
	"Achievements.Gatling.TooWarm",
	"Achievements.Gatling.SpinningTheBarrels",
	"Achievements.Gatling.HeavyFlight",
	"Achievements.Gatling.Complete",
	"Achievements.Bazooka.Expertise",
	"Achievements.Bazooka.DirectShots_Lucky",
	"Achievements.Bazooka.RocketJumper",
	"Achievements.Bazooka.TwoBirdsOneStone",
	"Achievements.Bazooka.Complete",
	"Achievements.Grenade.Expertise",
	"Achievements.Grenade.AngelAccuracy",
	"Achievements.Grenade.FlyingPills",
	"Achievements.Grenade.ExplosiveShowing",
	"Achievements.Grenade.Complete",
	"Mission1.Completion",
	"Mission1.Completed",
	"Mission2.Completion",
	"Mission2.Completed",
	"Mission3.Completion",
	"Mission3.Completed",
	"LuckMetter"
};
const char* tUsers[] = {
	"INT64",
	"CHAR32",
	"CHAR32",
	"CHAR128",
	"CHAR16",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT64",
	"INT8",
	"INT32",
	"BOOL",
	"BOOL",
	"BOOL",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"BOOL",
	"BOOL",
	"BOOL",
	"BOOL",
	"BOOL",
	"BOOL",
	"BOOL",
	"BOOL",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT8",
	"INT16",
	"INT16",
	"INT8", // achievements
	"INT16",
	"INT16",
	"INT8",
	"INT8",
	"INT8",
	"INT8",
	"INT8",
	"INT8",
	"BOOL",
	"INT16", // melee
	"INT8",
	"INT8",
	"INT8",
	"BOOL",
	"INT16", // rifle
	"INT8",
	"INT8",
	"INT8",
	"BOOL",
	"INT16", // shotgun
	"INT8",
	"INT8",
	"INT8",
	"BOOL",
	"INT16", // sniper
	"INT8",
	"INT8",
	"INT8",
	"BOOL",
	"INT16", // gatling
	"INT8",
	"INT8",
	"INT8",
	"BOOL",
	"INT16", // bazooka
	"INT8",
	"INT8",
	"INT8",
	"BOOL",
	"INT16", // grenade
	"INT8",
	"INT8",
	"INT8",
	"BOOL",
	"INT32",
	"BOOL", 
	"INT32",
	"BOOL",
	"INT32",
	"BOOL",
	"INT32",
};
const long  oUsers[] = {
	offsetof(_User, ID),
	offsetof(_User, LoginID),
	offsetof(_User, Password),
	offsetof(_User, Email),
	offsetof(_User, Nickname),
	offsetof(_User, Level),
	offsetof(_User, Exp),
	offsetof(_User, VIP),
	offsetof(_User, MicroPoints),
	offsetof(_User, RockTokens),
	offsetof(_User, Coins),
	offsetof(_User, Coupons),
	offsetof(_User, BatteryCapacity),
	offsetof(_User, Battery),
	offsetof(_User, RegisterDate),
	offsetof(_User, SinglePlayer),
	offsetof(_User, Playtime),
	offsetof(_User, LastLogin),
	offsetof(_User, ChannelID),
	offsetof(_User, ConnID),
	offsetof(_User, Character),
	offsetof(_User, RoomID),
	offsetof(_User, Online),
	offsetof(_User, RecievedDaily),
	offsetof(_User, Muted),
	offsetof(_User, MuteExpire),
	offsetof(_User, ServerState),
	offsetof(_User, Wins),
	offsetof(_User, Loses),
	offsetof(_User, Draws),
	offsetof(_User, Deaths),
	offsetof(_User, Assists),
	offsetof(_User, Kills.Total),
	offsetof(_User, Kills.Melee),
	offsetof(_User, Kills.Rifle),
	offsetof(_User, Kills.Shotgun),
	offsetof(_User, Kills.Sniper),
	offsetof(_User, Kills.Gatling),
	offsetof(_User, Kills.Bazooka),
	offsetof(_User, Kills.Grenade),
	offsetof(_User, Kills.Zombie),
	offsetof(_User, Kills.Infected),
	offsetof(_User, Invasion),
	offsetof(_User, Killstreak),
	offsetof(_User, Headshots),
	offsetof(_User, DoTutorial),
	offsetof(_User, Characters.Naomi),
	offsetof(_User, Characters.Knox),
	offsetof(_User, Characters.Pandora),
	offsetof(_User, Characters.Chip),
	offsetof(_User, Characters.Kai),
	offsetof(_User, Characters.Simon),
	offsetof(_User, Characters.Sophitia),
	offsetof(_User, Clan.ID),
	offsetof(_User, Clan.Contribution),
	offsetof(_User, Clan.Wins),
	offsetof(_User, Clan.Loses),
	offsetof(_User, Clan.Draws),
	offsetof(_User, Clan.Kills),
	offsetof(_User, Clan.Deaths),
	offsetof(_User, Clan.Assists),
	offsetof(_User, Grade),
	offsetof(_User, InventorySize),
	offsetof(_User, InventoryCount),
	offsetof(_User, Achievements.Battle.HardKnockLife),
	offsetof(_User, Achievements.Battle.WeAreTheChampions),
	offsetof(_User, Achievements.Battle.SuperJumper),
	offsetof(_User, Achievements.Battle.EnamelEater),
	offsetof(_User, Achievements.Battle.ShieldMania),
	offsetof(_User, Achievements.Battle.DoubleKillMaster),
	offsetof(_User, Achievements.Battle.MultiKillMaster),
	offsetof(_User, Achievements.Battle.GoodShotMaster),
	offsetof(_User, Achievements.Battle.Complete),
	offsetof(_User, Achievements.Melee.Expertise),
	offsetof(_User, Achievements.Melee.SwipeAttack),
	offsetof(_User, Achievements.Melee.SwingAttack),
	offsetof(_User, Achievements.Melee.JumpJumpJump),
	offsetof(_User, Achievements.Melee.Complete),
	offsetof(_User, Achievements.Rifle.Expertise),
	offsetof(_User, Achievements.Rifle.AccurateShooter),
	offsetof(_User, Achievements.Rifle.PinpointPrecision),
	offsetof(_User, Achievements.Rifle.FlyLikeAnEagle),
	offsetof(_User, Achievements.Rifle.Complete),
	offsetof(_User, Achievements.Shotgun.Expertise),
	offsetof(_User, Achievements.Shotgun.OneHitWonder),
	offsetof(_User, Achievements.Shotgun.OutOfRange),
	offsetof(_User, Achievements.Shotgun.Suprise),
	offsetof(_User, Achievements.Shotgun.Complete),
	offsetof(_User, Achievements.Sniper.Expertise),
	offsetof(_User, Achievements.Sniper.WatchYourHead),
	offsetof(_User, Achievements.Sniper.JumpersBeware),
	offsetof(_User, Achievements.Sniper.Bulseye),
	offsetof(_User, Achievements.Sniper.Complete),
	offsetof(_User, Achievements.Gatling.Expertise),
	offsetof(_User, Achievements.Gatling.TooWarm),
	offsetof(_User, Achievements.Gatling.SpinningTheBarrels),
	offsetof(_User, Achievements.Gatling.HeavyFlight),
	offsetof(_User, Achievements.Gatling.Complete),
	offsetof(_User, Achievements.Bazooka.Expertise),
	offsetof(_User, Achievements.Bazooka.DirectShots_Lucky),
	offsetof(_User, Achievements.Bazooka.RocketJumper),
	offsetof(_User, Achievements.Bazooka.TwoBirdsOneStone),
	offsetof(_User, Achievements.Bazooka.Complete),
	offsetof(_User, Achievements.Grenade.Expertise),
	offsetof(_User, Achievements.Grenade.AngelAccuracy),
	offsetof(_User, Achievements.Grenade.FlyingPills),
	offsetof(_User, Achievements.Grenade.ExplosiveShowing),
	offsetof(_User, Achievements.Grenade.Complete),
	offsetof(_User, Missions[0].Completion),
	offsetof(_User, Missions[0].Completed),
	offsetof(_User, Missions[1].Completion),
	offsetof(_User, Missions[1].Completed),
	offsetof(_User, Missions[2].Completion),
	offsetof(_User, Missions[2].Completed),
	offsetof(_User, LuckMetter)
};

const char* sClans[] = {
	"ID",
	"Name",
	"Author",
	"Leader1",
	"Leader2",
	"Created",
	"Wins",
	"Loses",
	"Draws",
	"Kills",
	"Deaths",
	"Assists"
};
const char* tClans[] = {
	"INT64",
	"CHAR16",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32"
};
const long  oClans[] = {
	offsetof(_Clan, ID),
	offsetof(_Clan, Name),
	offsetof(_Clan, Author),
	offsetof(_Clan, Leader[0]),
	offsetof(_Clan, Leader[1]),
	offsetof(_Clan, Created),
	offsetof(_Clan, Wins),
	offsetof(_Clan, Loses),
	offsetof(_Clan, Draws),
	offsetof(_Clan, Kills),
	offsetof(_Clan, Deaths),
	offsetof(_Clan, Assists)
};

const char* sMail[] = {
	"ID",
	"Author",
	"Reciever",
	"Timestamp",
	"Message"
};
const char* tMail[] = {
	"INT64",
	"INT32",
	"INT32",
	"INT32",
	"CHAR1024"
};
const long  oMail[] = {
	offsetof(_Mail, ID),
	offsetof(_Mail, Author),
	offsetof(_Mail, Reciever),
	offsetof(_Mail, Timestamp),
	offsetof(_Mail, Message)
};

const char* sGifts[] = {
	"ID",
	"Author",
	"Reciever",
	"Timestamp",
	"Item"
};
const char* tGifts[] = {
	"INT64",
	"INT32",
	"INT32",
	"INT32",
	"INT32"
};
const long  oGifts[] = {
	offsetof(_Gifts, ID),
	offsetof(_Gifts, Author),
	offsetof(_Gifts, Reciever),
	offsetof(_Gifts, Timestamp),
	offsetof(_Gifts, Item)
};

const char* sChat[] = {
	"ID",
	"Author",
	"Reciever",
	"Type",
	"Color",
	"Timestamp",
	"ChannelID",
	"RoomID",
	"Message"
};
const char* tChat[] = {
	"INT64",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"INT32",
	"CHAR512"
};
const long  oChat[] = {
	offsetof(_Chat, ID),
	offsetof(_Chat, Author),
	offsetof(_Chat, Reciever),
	offsetof(_Chat, Type),
	offsetof(_Chat, Color),
	offsetof(_Chat, Timestamp),
	offsetof(_Chat, ChannelID),
	offsetof(_Chat, RoomID),
	offsetof(_Chat, Message)
};