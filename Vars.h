#pragma once
struct Triggerbot
{
	bool Enabled;
	int Mode;
	int Key;
	bool Delay;
	int DelayAmmount;
};
struct ClosestBone
{
	bool Enabled;
	bool Arms;
	bool Legs;
	bool Body;
	bool Head;
};
struct Weapon
{
	bool Enabled;
	int Mode;
	int Key;
	bool VisualCheck;
	int Bone;
	ClosestBone closestBone;
	
	float FOV;
	bool AimHeight;
	float HeightAmt;
	bool Smooth;
	int SmoothType;
	bool RandomisedSmooth;
	float Speed;
	float MinTimeToTarget, MaxTimeToTarget;
	float TimeToTarget;
	bool RandomTime;
	float CurveX, CurveY;
	float CurveTimer;
	bool VelocityPredict;
	bool ProjectilePredict;
	Triggerbot Trigger;
	

};
struct WeaponInfo
{
	bool Enabled;
	bool Ammo;
	bool ReloadAmmo;
	bool Accuaracy;
	float AccuaracyVal;
	bool Wallshot;
	bool MeleeRange;
	int  MeleeRangeVal;
	bool ReloadSpeed;
	int  ReloadSpeedVal;
	bool SwapSpeed;
	int  SwapSpeedVal;
	bool Speedshot;
	int  SpeedshotVal;
	bool MultiplyDamage;
	int MultiplyDamageVal;
	bool SniperNoScope;
	bool GatlingHeat;
	bool FiringRate;
};
struct Variables
{

	struct cAimbot
	{
		Weapon Weapon[7];
	}Aimbot;
	struct cVisuals
	{
		bool Enabled;
		struct
		{
			struct 
			{
				bool Enabled;
				int type;
				float VisiblePlayers[3] = { 1.0f, 0.0f, 0.0f };
				float HiddenPlayers[3] = { 1.0f, 0.0f, 0.0f };
			}Box;
			struct
			{
				bool Enabled;
				int type;
			}Healthbar;
			struct
			{
				bool Name;
				bool HP;
				bool Weapon;
				bool Velocity;
				float VisiblePlayers[3] = { 1.0f, 0.0f, 0.0f };
				float HiddenPlayers[3] = { 1.0f, 0.0f, 0.0f };
			}Info;
			struct
			{
				bool Enabled;
				int type;
				float VisiblePlayers[3] = { 1.0f, 0.0f, 0.0f };
				float HiddenPlayers[3] = { 1.0f, 0.0f, 0.0f };
				
			}Snaplines;
			struct
			{
				bool DrawFOV;
				bool DrawFOVDistance;
				bool DrawLineToTarget;
				float FOVColor[3] = { 0.0f, 1.0f, 0.0f };
				float VisiblePlayers[3] = { 1.0f, 0.0f, 0.0f };
				float HiddenPlayers[3] = { 1.0f, 0.0f, 0.0f };

			}AimInfo;
			struct 
			{
				bool Enabled;
				int type;
			}Crosshair;
			struct
			{
				bool Enabled;
				float VisiblePlayers[3] = { 1.0f, 0.0f, 0.0f };
				float HiddenPlayers[3] = { 1.0f, 0.0f, 0.0f };
			}Skeleton;
			struct
			{
				bool Enabled;
				float Color[3] = { 1.0f, 0.0f, 0.0f };
				float Length;
			}Tracers;
			struct
			{
				bool Enabled;
				int type = 0;
			}Outline;
		}ESP;

	}Visuals;
	struct cMisc
	{
		
		WeaponInfo WeaponInfo[7];
		struct
		{
			int uiMode;
			char toPlayer;
			char Text;
			bool inGame;
		}SendMSG;
		struct
		{
			LPCSTR Text;
			int Length;
		}ProcessMSG;
		struct
		{
			LPCSTR Text;
			LPCSTR Name;
			int Type = 0;
			bool bShow = true;
			bool bRegGameMode = false;
			int Color = 0;
		}PrintMSG;
		struct
		{
			LPCSTR Dialog;
			bool inGame;
		}Dialogs;
		struct
		{
			char* playerName = new char[40];
			int respawnKey;
		}PlayerFuncs;
		bool AutoStart;
		int AutoStartDelay = 0;
		bool AutoReady;
		int AutoReadyDelay = 0;
		bool AutoSkipResults;
		int AutoSkipResultsDelay = 0;
		bool AntiAFK;
		bool AutoFire;
		bool AutoKick;
		bool PlayerList;
		bool RoomInfo;
		bool RoomRape;
		bool FarmGifts;
		bool AutoSelectWeapon;
		int AutoSelectWeaponType = 0;
		int RoomRapeType;
		int RoomRapeWeapon;
		struct
		{
			bool Enabled, Randomised;
			int min = 0, max = 0, ping =0;
		}Ping;
		bool AutoInvite;
		int AutoInviteDelay;
		int AutoInvitieKey;
		std::vector<std::string> AutoInvitePlayers;
	}Misc;
	struct cInfo
	{

	}Info;
	struct cSettings
	{
		int StartupConfig = 0;
	}Settings;
	
};
extern Variables Vars;
