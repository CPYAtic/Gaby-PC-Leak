#pragma once

#ifdef USE_XYZ
#undef USE_XYZ
#endif
#define DebugMsg(msg) MessageBox(NULL,msg,"[Debug]",MB_OK)
//#define USE_SYZ // define this if you want to use ( X; Y; Z ) instead of ( X; Z; Y )
typedef struct tRoomSettings
{
	byte time, weaponds, map, rules, players;
};
struct POINT3Df
{
#ifdef USE_XYZ
	float x, y, z;
#endif
#if not defined(USE_XYZ)
	float x, z, y;
#endif
};
struct POINT3D
{
#ifdef USE_XYZ
	double x, y, z;
#endif
#if not defined(USE_XYZ)
	double x, z, y;
#endif
};

typedef unsigned long DWORD;
typedef float FLOAT;
typedef char CHAR;

#define CHECK_VALID( _v ) 0
#define  Assert( _exp )										((void)0)
class Vector3
{
public:
	Vector3()
	{

	}

	Vector3(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}

	float Dot(Vector3& playerTo)
	{
		return (x * playerTo.x + y * playerTo.y + z * playerTo.z);
	}

	Vector3 VectorDist(Vector3& playerTo)
	{
		return Vector3(x - playerTo.x, y - playerTo.y, z - playerTo.z);
	}

	float Dist(Vector3 playerTo)
	{
		float Dx = (x - playerTo.x);
		float Dy = (y - playerTo.y);
		float Dz = (z - playerTo.z);

		return sqrt(Dx*Dx + Dy * Dy + Dz * Dz);
	}
	

	float DistToSqrt(const Vector3 &vOther) const
	{
		Vector3 delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.LengthSqr();
	}
	float x;
	float y;
	float z;

	void operator=(Vector3 a)
	{
		x = a.x;
		y = a.y;
		z = a.z;
	}
	bool IsZero()
	{
		CHECK_VALID(*this);
		if (this->x == 0.f || this->y == 0.f || this->z == 0.f)
			return true;

		return false;
	}
	inline float Length() const
	{
		return (float)sqrt(LengthSqr()); //todo replace with fastsqrt
	}
	__forceinline float LengthSqr(void) const
	{
		return (this->x*this->x + this->y*this->y + this->z*this->z);
	}
	Vector3 operator+(const Vector3 &v) const
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(const Vector3 &v) const
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator*(const Vector3 &v) const
	{
		return Vector3(x * v.x, y * v.y, z * v.z);
	}
	Vector3 operator/(const Vector3 &v) const
	{
		return Vector3(x / v.x, y / v.y, z / v.z);
	}
	Vector3 operator*(float fl) const
	{
		Vector3 res;
		res.x = x * fl;
		res.y = y * fl;
		res.z = z * fl;
		return res;
	}

	Vector3 operator/(float fl) const
	{
		Vector3 res;
		res.x = x / fl;
		res.y = y / fl;
		res.z = z / fl;
		return res;
	}

	Vector3 operator+=(const Vector3& v)
	{
		CHECK_VALID(*this);
		CHECK_VALID(v);
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	Vector3 operator-=(const Vector3& v)
	{
		CHECK_VALID(*this);
		CHECK_VALID(v);
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	bool operator!=(const Vector3& v)
	{
		CHECK_VALID(*this);
		CHECK_VALID(v);
		
		return this->x != v.x && this->y != v.y && this->z != v.z;
	}

	bool operator==(const Vector3& v)
	{
		return this->x == v.x && this->y == v.y && this->z == v.z;
	}

	Vector3 operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		z *= fl;
		CHECK_VALID(*this);
		return *this;
	}



	Vector3 operator/=(float fl)
	{
		Assert(fl != 0.0f);
		float oofl = 1.0f / fl;
		x *= oofl;
		y *= oofl;
		z *= oofl;
		CHECK_VALID(*this);
		return *this;
	}
	
};
class Vector4 : public Vector3
{
public:
	float w;
	Vector4()
	{

	}

	Vector4(float X, float Y, float Z, float W)
	{
		x = X;y = Y;z = Z;w = W;
		pointer = false;
	}
	Vector4(float X, float Y, float Z)
	{
		x = X;y = Y;z = Z;w = 1.0f;
		pointer = false;
	}
	Vector4(float* X, float* Y, float* Z, float* W)
	{
		px = X;py = Y;pz = Z;pw = W;
		x = *X;y = *Y;z = *Z;w = *W;
		pointer = true;
	}
	~Vector4() {}

	Vector4 operator= (const Vector4& v)
	{
		if (pointer)
		{
			*px = x = v.x;
			*py = y = v.y;
			*pz = z = v.z;
			*pw = w = v.w;
		}
		else
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = v.w;
		}
	}
	const Vector4 operator* (const float &scalar) const
	{
		return Vector4(x*scalar, y*scalar, z*scalar, w*scalar);
	}
	const Vector4 operator/ (const float &scalar) const
	{
		return Vector4(x/scalar, y/scalar, z/scalar, w/scalar);
	}
	const Vector4 operator+ (const Vector4 &v) const
	{
		Vector4 vec;
		vec.x = this->x + v.x;
		vec.y = this->y + v.y;
		vec.z = this->z + v.z;
		vec.w = this->w + v.w;
		return vec;
	}
	const Vector4 operator- (const Vector4 &v) const
	{
		Vector4 vec;
		vec.x = this->x - v.x;
		vec.y = this->y - v.y;
		vec.z = this->z - v.z;
		vec.w = this->w - v.w;
		return vec;
	}

	inline void show() { std::cout << this->x << " " << this->y << " " << this->z << " " << this->w << std::endl; }

private:


	// only to allow do that: m(0) = Vector4(1,1,1,1) //
	float* px;
	float* py;
	float* pz;
	float* pw;

	bool pointer;  // to check what constructor was called //
};

struct Login
{
	char Username[16];
	char Password[20];
};
class CChat
{
public:
	
	void __stdcall PrintChatMessage(LPCSTR lpText, LPCSTR lpName, uint8_t uiType, bool bShow, bool bRegGameMode, uint8_t uiColor)
	{
		using tPrintChatMessage = void(__thiscall*)(uint32_t, LPCSTR, LPCSTR, uint8_t, bool, bool, uint8_t);
		tPrintChatMessage oPrintChatMessage = reinterpret_cast<tPrintChatMessage>(0x00581AC0);
		uint32_t thisptr = *reinterpret_cast<uint32_t*>(0x011B95A8 + 0x108);
		oPrintChatMessage(thisptr, lpText, lpName, uiType, bShow, bRegGameMode, uiColor);
	}
	void PrintMsg(uint8_t uiType, uint8_t uiColor,const char* msg, ...)
	{
		char buff[1024], uff[1024 + 16];
		va_list vl;
		va_start(vl, msg);

		if (vsnprintf_s(buff, 1024, msg, vl) == -1)
			return;

		PrintChatMessage(buff, "", uiType, true, false, uiColor);

		va_end(vl);
	}
	void __stdcall ProcessChatMessage(LPCSTR lpText, int8_t iLen)
	{
		using tProcessChatMessage = int8_t(__thiscall*)(uint32_t, LPCSTR, int8_t);
		tProcessChatMessage oProcessChatMessage = reinterpret_cast<tProcessChatMessage>(0x005813D0);
		uint32_t thisptr = *reinterpret_cast<uint32_t*>(0x011B95A8 + 0x108);
		oProcessChatMessage(thisptr, lpText, iLen);
	}
	void SendGameChatMessage(uint8_t uiMode, LPCSTR toPlayer, LPCSTR lpText)
	{
		using tSendGameChatMessage = int8_t(__cdecl*)(uint8_t, LPCSTR, LPCSTR);
		tSendGameChatMessage oSendGameChatMessage = reinterpret_cast<tSendGameChatMessage>(0x00B37910);
		oSendGameChatMessage(uiMode, toPlayer, lpText);
	}
	void SendLobbyChatMessage(uint8_t uiMode, LPCSTR toPlayer, LPCSTR lpText)
	{
		using tSendLobbyChatMessage = int8_t(__cdecl*)(uint8_t, LPCSTR, LPCSTR);
		tSendLobbyChatMessage oSendLobbyChatMessage = reinterpret_cast<tSendLobbyChatMessage>(0x00B389F0);
		oSendLobbyChatMessage(uiMode, toPlayer, lpText);
	}
	void CreateDlg(LPCSTR lpText, uint8_t unknown)
	{
		using tCreateDlg = uint8_t(__stdcall*)(LPCSTR, uint8_t);
		tCreateDlg oCreateDlg = reinterpret_cast<tCreateDlg>(0x005C1A50);
		oCreateDlg(lpText, unknown);
	}
	void CreateGameDlg(LPCSTR lpText, uint8_t unknown)
	{
		using tCreateGameDlg = int8_t(__cdecl*)(LPCSTR, uint8_t);
		tCreateGameDlg oCreateGameDlg = reinterpret_cast<tCreateGameDlg>(0x005C1AA0);
		oCreateGameDlg(lpText, unknown);
	}
	void CloseDlg(LPCSTR lpText)
	{
		using tCloseDlg = uint8_t(__thiscall*)(uint32_t, LPCSTR);
		tCloseDlg oCloseDlg = reinterpret_cast<tCloseDlg>(0x00E76250);
		uint32_t thisptr = *reinterpret_cast<uint32_t*>(0x11B95CC);
		oCloseDlg(thisptr, lpText);
	}

	enum Type : uint8_t
	{
		NORMAL = 0,
		HELP = 1,
		WHISPER = 2,
		TEAM = 5,
		TEAM2 = 8,
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
};
extern CChat* cChat;
struct RoomSettings
{
	DWORD Unknown[26];
	DWORD Score;
	DWORD Unknown1[2];
	BOOL Item;
	byte Spacing[7];
	DWORD Time;
	DWORD Unknown2[13];
	DWORD RedTeamScore;
	DWORD BlueTeamScore;
	// MORE?
};
struct Functions
{
	void* Unknown[215];
};
struct WeaponStats
{
	DWORD unk1[120];//0x0
	int DAMAGE_HEAD;//0x1E0
	DWORD unk2;//0x1E4
	int DAMAGE_HEAD_XOR;//0x1E8
	int DAMAGE_UPPER;//0x1EC
	DWORD unk3;//0x1F0
	int DAMAGE_UPPER_XOR;//0x1F4
	int DAMAGE_UNDER;//0x1F8
	DWORD unk4;//0x1FC
	int DAMAGE_UNDER_XOR;//0x200
	DWORD unk5[0xB4]; // 0x204
	int BulletSpeed; //0x2B8;
	DWORD unk6;  // 0x2BC;
	int BulletSpeed_XOR; // 0x2C0;
	DWORD unk7[0x24]; // 0x2C4
	int AttackRange;//0x2E8
	DWORD unk8;//0x2EC
	int AttackRange_XOR;//0x2F0
	DWORD unk9[0xB4];//0x2F4
	int ReloadTime;//0x3A8
	DWORD unk10;//0x3AC
	int ReloadTime_XOR;//0x3B0
	DWORD unk11[0x54];//0x3B4
	int Ammo;//0x408
	DWORD unk12;//0x40C
	int Ammo_XOR;//0x410
	int ReloadAmmo;//0x414
	DWORD unk13;//0x418
	int ReloadAmmo_XOR;//0x41C
	int ChangeTime;//0x420
	DWORD unk14;//0x424
	int ChangeTime_XOR;//0x428
	DWORD unk15[0x18];//0x42C
	int Speedshot;//0x444
	DWORD unk16;//0x448
	int Speedshot_XOR;//0x44c
};
struct WeaponClass
{
	DWORD Unknown[0x3A0];
	WeaponStats* Stats;//0x3A0
};
struct CWeapond
{
	//DWORD unk1[0x20];
	DWORD64 unk1;//0x00
	DWORD64 unk2;//0x08
	DWORD64 unk3;//0x10
	DWORD64 unk4;//0x18
	WeaponClass* Class;//0x20
	//DWORD unk2[0x108]; //0x24
	DWORD64 unk5;//0x24
	DWORD64 unk6;//0x28
	DWORD64 unk7;//0x34
	DWORD64 unk8;//0x3C
	DWORD64 unk9;//0x44
	DWORD64 unk10;//0x4C
	DWORD64 unk11;//0x54
	DWORD64 unk12;//0x5C
	DWORD64 unk13;//0x64
	DWORD64 unk14;//0x6C
	DWORD64 unk15;//0x74
	DWORD64 unk16;//0x7C
	DWORD64 unk17;//0x84
	DWORD64 unk18;//0x8C
	DWORD64 unk19;//0x94
	DWORD64 unk20;//0x9C
	DWORD64 unk21;//0xA4
	DWORD64 unk22;//0xAC
	DWORD64 unk23;//0xB4
	DWORD64 unk24;//0xBC
	DWORD64 unk25;//0xC4
	DWORD64 unk26;//0xCC
	DWORD64 unk27;//0xD4
	DWORD64 unk28;//0xDC
	DWORD64 unk29;//0xE4
	DWORD64 unk30;//0xEC
	DWORD64 unk31;//0xF4
	DWORD64 unk32;//0xFC
	DWORD64 unk33;//0x104
	DWORD64 unk34;//0x10C
	DWORD64 unk35;//0x114
	DWORD64 unk36;//0x11C
	DWORD unk37;//0x120
	DWORD SniperNoscope; //0x12C;
	//Vector3 WeaponAngles; //0x1F0
	//DWORD unk3[0xDC]; //0x130
	DWORD64 unk38;//0x130
	DWORD64 unk39;//0x138
	DWORD64 unk40;//0x140
	DWORD64 unk41;//0x148
	DWORD64 unk42;//0x150
	DWORD64 unk43;//0x158
	DWORD64 unk44;//0x160
	DWORD64 unk45;//0x168
	DWORD64 unk46;//0x170
	DWORD64 unk47;//0x178
	DWORD64 unk48;//0x180
	DWORD64 unk49;//0x188
	DWORD64 unk50;//0x190
	DWORD64 unk51;//0x198
	DWORD64 unk52;//0x1A0
	DWORD64 unk53;//0x1A8
	DWORD64 unk54;//0x1B0
	DWORD64 unk55;//0x1B8
	DWORD64 unk56;//0x1C0
	DWORD64 unk57;//0x1C8
	DWORD64 unk58;//0x1D0
	DWORD64 unk59;//0x1D8
	DWORD64 unk60;//0x1E0
	DWORD64 unk61;//0x1E8
	DWORD64 unk62;//0x1F0
	DWORD64 unk63;//0x1F8
	DWORD64 unk64;//0x200
	DWORD unk65; //0x208
	DWORD Ammo; // 0x20C
	DWORD ReloadAmmo; //0x210
	DWORD64 unk66;//0x214
	DWORD64 unk67;//0x21C
	DWORD64 unk68;//0x224
	DWORD64 unk69;//0x22C
	DWORD64 unk70;//0x234
	DWORD64 unk71;//0x23C
	DWORD64 unk72;//0x244
	DWORD64 unk73;//0x24C
	DWORD64 unk74;//0x254
	DWORD64 unk75;//0x25C
	DWORD64 unk76;//0x264
	DWORD64 unk77;//0x26C
	DWORD64 unk78;//0x274
	DWORD64 unk79;//0x27C
	DWORD64 unk80;//0x284
	DWORD64 unk81;//0x28C
	DWORD64 unk82;//0x294
	DWORD64 unk83;//0x29C
	DWORD64 unk84;//0x2A4
	DWORD64 unk85;//0x2AC
	DWORD64 unk86;//0x2B4
	DWORD64 unk87;//0x2BC
	//DWORD unk4[0xB0]; //0x214
	DWORD IsActive;//0x2C4
	DWORD64 unk88;//0x2C8
	DWORD64 unk89;//0x2D0
	DWORD64 unk90;//0x2D8
	DWORD64 unk91;//0x2E0
	DWORD64 unk92;//0x2E8
	DWORD64 unk93;//0x2F0
	DWORD unk94; //0x300
	//DWORD unk5[0x3C];//0x2C8
	DWORD GatlingHeat;//0x304

};
struct WeapondInfo
{
	//DWORD Unknown[0x4C];
	DWORD64 unk1;
	DWORD64 unk2;
	DWORD64 unk3;
	DWORD64 unk4;
	DWORD64 unk5;
	DWORD64 unk6;
	DWORD64 unk7;
	DWORD64 unk8;
	DWORD64 unk9;
	DWORD unk10;
	CWeapond* Weapon[7];
	//CWeapond* Melee;
	//CWeapond* Rifle;
	//CWeapond* Shotgun;
	//CWeapond* Sniper;
	//CWeapond* Gatling;
	//CWeapond* Bazooka;
	//CWeapond* Grenade;
};
struct Plyr
{
	DWORD YES;//0x0000
	DWORD unk1;//0x0004
	DWORD shit;//0x0008
	DWORD unk[0x10C];//0x000C
	Vector3 Coords;//0x0118
};
struct Bone
{
public:
	DWORD Unknown[8];
	Vector3 Location;
	Vector3* GetTransform(int index)
	{
		return (Vector3*)(((DWORD*)&this->pos) + (0x37 * index));
	}
private:
	byte _unknown[125];
	Vector3 pos;
};
class BoneName
{
	CHAR name[28];
};
class OutlineStuff
{
	DWORD outline;
};
class CBone
{
	byte _empty[136];
	Vector3 position;
public:
	Vector3 * GetTransform(int index)
	{
		return (Vector3*)(((DWORD*)&this->position) + (0x37 * index));
	}
};
enum BoneID
{
	Bip01_Head = 0,
	Bip01_Spine= 1,
	Bip01_R_UpperArm = 2,
	Bip01_R_ForeArm = 3,
	Bip01_L_UpperArm = 4,
	Bip01_L_ForeArm = 5,
	Bip01_R_Thig = 6,
	Bip01_R_Calf = 7,
	Bip01_L_Thig = 8,
	Bip01_L_Calf = 9
};
struct Coordinates
{
	DWORD Unknown[3];//0
	CBone* Bone[38];//c
	/*CBone* Head;//c
	CBone* Middle;//10
	CBone* RightArm;//14
	DWORD idk1;//18
	CBone* LeftArm;//1C
	DWORD idk2;//20
	CBone* RightFoot;//24
	CBone* idk3;//28
	CBone* LeftFoot;//2c
	CBone* idk4;//0x30
	DWORD Unknown3[28];//0x34*/
	FLOAT Rotation;//A4
	FLOAT Pitch;//A8
	DWORD Unknown4[0x360];//AC
	OutlineStuff* cOutline; // 0x40c
	DWORD Unknown5[0x22C];
	Plyr* Player;//638
};
struct PlayerInfo
{
	DWORD Unknown[52];
	Vector3 UnitVelocity;//0xD0
	DWORD Unknown2[18];
	Coordinates* Coords;//0x124
	//DWORD Unknown[73];
	//Coordinates* Coords;
};
struct ServerInfo
{
	DWORD serialID;
};
struct PlayerRoomInfo
{
	ServerInfo* Server; // SEND ID
	//BYTE Unknown[0x3];
	union
	{
		DWORD Team;
		DWORD Level;
	};
	DWORD RoomStatus;
	DWORD HP1; //0xC
	DWORD notHP2; // !HP2
	DWORD HP2; //0x14
	DWORD unk5[67];
	int   MeleeRange; //0x124
	DWORD Unknown1[644]; //0xb34
	FLOAT Yaw; //0xb38
	DWORD Unknown2[31];
	BYTE MeleeKills;
	BYTE RifeKills;
	BYTE ShotgunKills;
	BYTE SniperKills;
	BYTE GatlingKills;
	BYTE BazookaKills;
	BYTE GrenadeKills;
	BYTE Unknown3;
	BYTE Kills;
	BYTE Deaths;
	BYTE Unknown4; // MISSION?
	BYTE Assists;
};
struct MovementInfo
{
	DWORD Unknown[0x10];
	//FLOAT Speed;
	Vector3 Coords;
	// MORE?
};
struct ShitInfo
{
	DWORD Unknown[12];
	DWORD Ping;
	// MORE?
};
enum Characters : byte
{
	Naomi = 0x00,
	Kai = 0x01,
	Pandora = 0x02,
	CHIP = 0x03,
	Knox = 0x04
};
struct MVEntity
{
	Functions* FunctionList; //0x0
	PlayerRoomInfo* Room; //0x4
	DWORD Unknown; //0x8
	ShitInfo* PlayerShit; //0xC
	MovementInfo* Movement; //0x10
	DWORD Unknown2[5];//0x14
	WeapondInfo* Weaponds;//0x28
	DWORD Unknown3[8];//0x48
	PlayerInfo* Player;//0x4c
	DWORD Unknown4[5];//0x50
	CHAR Name[40];//0x64
	
	// MORE?
};
struct CRoom
{
	DWORD Unknown[2]; // SEND ID? ROOM ID?
	DWORD Unknown1[19]; // FUNCTIONS?
	MVEntity* Players[26];
	//MVEntity* Observers[10];
	DWORD Unknown2[5];
	RoomSettings* Settings;
	DWORD PlayerCount;//D4
	DWORD ObserversCount;
	DWORD Unknown3[0xC];
	DWORD MaxPlayers;
	BOOL bUnknown;
	BOOL HasPassword;
	BOOL Open;
	BOOL ObserverMode;
	BOOL TeamBalance;
	BOOL bUnknown1;
	BOOL bUnknown2;
	BOOL TeamBalanceDisabled;
	DWORD Unknown4;
	CHAR Title[40]; // ACTUAL MAXLENGTH IS 30 (+ 1 NULL?)
	CHAR Password[12]; // ACTUAL MAXLENGTH IS 8 (+ 1 NULL?)
	DWORD Unknown5[4];
	DWORD MapIndex;
	// MORE UNKNOWNS (size 0x230)
};
struct Matrix
{
	char pad_0x0000[0x20]; //0x0000
	Vector3 WorldPos; //0x0020 
	char pad_0x002C[0x38]; //0x002C
	float matrix_13; //0x0064 
	float matrix_12; //0x0068 
	float matrix_11; //0x006C 
	float matrix_23; //0x0070 
	float matrix_22; //0x0074 
	float matrix_21; //0x0078 
	float matrix_33; //0x007C 
	float matrix_32; //0x0080 
	float matrix_31; //0x0084 
	Vector3 WorldPos2; //0x0088 
	char pad_0x0094[0x18]; //0x0094
	Vector4 matrix1; //0x00AC 
	Vector4 matrix2; //0x00BC 
	Vector4 matrix3; //0x00CC // world dir
	Vector4 matrix4; //0x00DC 
	char pad_0x00EC[0x10]; //0x00EC
	float Realnear; //0x00FC 
	float Realfar; //0x0100 
	char pad_0x0104[0xC]; //0x0104
	Vector4 Viewport; //0x0110 
}; //Size=0x011C
struct CCamera
{
	char pad_0x0000[0xC]; //0x0000
	float RotationX; //0x000C 
	float RotationY; //0x0010 
	char pad_0x0014[0x4]; //0x0014
	float FOV; //0x0018 
	char pad_0x001C[0x4]; //0x001C
	float RealFar; //0x0020 
	float ZoomDistance; //0x0024 
	char pad_0x0028[0x58]; //0x0028
	Vector3 WorldPos; //0x0080 
	char pad_0x008C[0x2C]; //0x008C
	Matrix* MatrixPtr; //0x00B8 
	char pad_0x00BC[0x36C]; //0x00BC
	Vector3 CrosshairColor; // 0x0428
	Vector3 HasTargetCrosshairColor;
	Vector3 SniperCrosshairColor;
}; 
enum Item : unsigned long
{
	MisteryCapsule = 0x003D0D07,
	Ammo = 0x003D76C4,
	Health = 0x003D0CE8,
	Shield = 0x003D10D0,
	BronzeEgg = 0x00496960,
	SilverEgg = 0x0049695F,
	GoldEgg = 0x0049695E,
	Lantern = 0x003D76C5
};
class CEngine
{
public:
	CRoom * Room = nullptr;
	MVEntity* pLocal = nullptr;
	CCamera* Camera = nullptr;
	typedef int(WINAPI * tSend)(SOCKET, char*, Size, int);
	tSend Send;
	clock_t FarmTime = 2 * CLOCKS_PER_SEC;
	int FarmIndexPlayer = 0;
	int playerStatus = 0;
	int playersStatus = 0;
	int gameState4Ready = 0;
	int gameState4Skip = 0;
	int gameState4Farm = 0;
	int CurrentWeapon = 0;
	bool OverWritedGayShitOfMicroVolts = false, OverWritedBackGayShitOfMicrovolts = false;
	
	void delay(int seconds)
	{
		clock_t endwait;
		endwait = clock() + seconds * CLOCKS_PER_SEC;
		while (clock() < endwait) {}
	}
	void delayMs(int ms)
	{
		clock_t endwait;
		endwait = clock() + ms;
		while (clock() < endwait) {}
	}
	bool Update()
	{
		Offset CLocalPltrOffs[] = { 0x7C, 0x10C, 0 };
		this->Room = *(CRoom**)0x011B5FC0;
		this->pLocal = (MVEntity*)CalcPtr(0x11B5FBC, CLocalPltrOffs, 3);
		this->Camera = *(CCamera**)0x011B9150;
		return this->Room && this->pLocal && this->Camera;
	}
	void Fire()
	{
		sWrite<int>(0x011B20F7, 1);
	}
	
	void Run()
	{
		if (!this->Update()) return;
		
		if (Vars.Misc.AutoSelectWeapon)
			AutoSelectWeap(Vars.Misc.AutoSelectWeaponType);

		if (Vars.Misc.AutoFire)
			Fire();

		if (Vars.Misc.AntiAFK)
			AntiAFK();

		if (Vars.Misc.AutoStart)
			AutoStart(Vars.Misc.AutoStartDelay);

		if (Vars.Misc.AutoReady)
			AutoReady(Vars.Misc.AutoReadyDelay);

		if (Vars.Misc.FarmGifts)
			FarmGifts();

		if (Vars.Misc.AutoSkipResults)
		{
			WriteBytes(0x00BA6212, "\x90\x90\x90\x90\x90", 5);
			AutoSkipResults(Vars.Misc.AutoSkipResultsDelay);
		}
		else
		{
			WriteBytes(0x00BA6212, "\xE8\xE9\x74\xF7\xFF", 5);
		}
	}
	void SendMailboxMessage(const char* playername, const char* text)
	{
		typedef int(__cdecl* tSendMail)(const char*, const char*);
		tSendMail oSendMail = reinterpret_cast<tSendMail>(0x00B17BE0);
		oSendMail(playername, text);
	}
	void SpawnItem(MVEntity* player, Item item)
	{
		typedef int(__cdecl* tSpawnItem)(CRoom*, MVEntity*, DWORD unknown, DWORD unknown2, Item itemID);
		tSpawnItem oSpawnItem = reinterpret_cast<tSpawnItem>(0x00E12190);
		oSpawnItem(this->Room, player, 0, 0x00010000, item);
	}

	void FarmGifts()
	{
		if (GetServerState() == GameState::GAME_STATE_MOD_PLAYING)
		{
			if (PlayersInMatch() >= 4)
			{
				if (clock() - FarmTime >= 60 * CLOCKS_PER_SEC)
				{
					
					for (int times = 0; times < PlayersInMatch(); times++)
						SpawnItem(Room->Players[FarmIndexPlayer], Item::GoldEgg);

					FarmTime = clock();
					FarmIndexPlayer++;
					if (FarmIndexPlayer == Room->PlayerCount)
						FarmIndexPlayer = 0;
					if(!Room->Players[FarmIndexPlayer])
						FarmIndexPlayer = 0;
				}
			}
		}
		else
		{
			FarmIndexPlayer = 0;
			FarmTime = 2 * CLOCKS_PER_SEC;
		}
			
	}

	int GetServerState()
	{
		return *(int*)0x11C5060;
	}
	int GetServerRegion()
	{
		Offset sv[1] = { 0x1054 };
		int server = *reinterpret_cast<int*>(sCalcPtr(0x11B5FC8, sv, 1));
		return server;
	}
	std::string GetServerRegionStr()
	{
		switch (GetServerRegion())
		{
		case 0:
			return std::string("None");
			break;
		case 1:
			return std::string("Europe");
			break;
		case 2:
			return std::string("North America");
			break;
		}
	}
	enum Characters : int
	{
		Naomi = 0,
		Kai = 1,
		Pandora = 2,
		CHIP = 3,
		Knox = 4
	};
	DWORD GetCharacter(int Character)
	{
		return (Character >> 7) & 0x1F;
	}
	std::string GetCharacterStr(int value)
	{
		switch (GetCharacter(value))
		{
		case 0:
			return std::string("Naomi");
			break;
		case 1:
			return std::string("Kai");
			break;
		case 2:
			return std::string("Pandora");
			break;
		case 3:
			return std::string("CHIP");
			break;
		case 4:
			return std::string("Knox");
			break;
		}
	}
	bool IsEnemy(byte id)
	{
		if (id > 15) return false;
		int MyTeam = *(int*)((this->pLocal->Room->Team >> 12) & 7);
		int HisTeam = *(int*)((this->Room->Players[id]->Room->Team >> 12) & 7);
		return (MyTeam == 0 || MyTeam == 4 || MyTeam != HisTeam);
	}
	int GetWidth()
	{
		return *(int*)0x011A2AB4;;
	}
	int GetHeight()
	{
		return *(int*)0x011A2AB8;
	}
	bool WorldToScreen(Matrix* matrix, Vector3* from, D3DXVECTOR2* to)
	{
		using tWorldToScreen = bool(__thiscall*)(Matrix* matrix, Vector3* playerpos, float* x, float* y, float idk);
		tWorldToScreen oWorldToScreen = reinterpret_cast<tWorldToScreen>(0x00CEBCB0);
		bool ret = oWorldToScreen(matrix, from, &to->x, &to->y, 0.0000099999997);
		if (ret)
		{
			to->x *= GetWidth(); //multiply with width
			to->y = 1.0 - to->y;
			to->y *= GetHeight(); //multiply with height
			return true;
		}
		return false;
	}
	void OutlinePlayers(MVEntity* player)
	{
		/*using tSendOutlineInfo = int(__thiscall*)(Address, DWORD);
		tSendOutlineInfo oSendOutlineInfo = reinterpret_cast<tSendOutlineInfo>(0x0046C240);
		Offset outlineOffs[] = { 0x0 };
		Address outlineBase = sCalcPtr(0x011B4350, outlineOffs, 1);
		oSendOutlineInfo(outlineBase, player->Player->Coords->cOutline->outline);*/
	}
	bool isVisible(MVEntity* player, float Distance)
	{
		Vector3 WorldDirection = { Camera->MatrixPtr->matrix_13, Camera->MatrixPtr->matrix_23, Camera->MatrixPtr->matrix_33 };
		using tCheckUnitInSight = bool(__thiscall*)(void* thisptr, Vector3* worldPos, Vector3* worldDirection, float* rotation, PlayerInfo* player, float Distance);
		tCheckUnitInSight oCheckUnitInSight = reinterpret_cast<tCheckUnitInSight>(0x007B0C00);

		return oCheckUnitInSight(*(void**)0x11B5FB4, &Camera->MatrixPtr->WorldPos2, &WorldDirection, &Camera->RotationX, player->Player, Distance);
	}

	bool raycastPlayer(MVEntity* player, float Distance)
	{
		Vector3 WorldDirection = { Camera->MatrixPtr->matrix_13, Camera->MatrixPtr->matrix_23, Camera->MatrixPtr->matrix_33 };
		using tWorldDirMath = double(__fastcall*)(Vector3* worldDir);
		tWorldDirMath oWorldDirMath = reinterpret_cast<tWorldDirMath>(0x004697B0);
		Vector3 WorldDirMath = WorldDirection;
		oWorldDirMath(&WorldDirMath);


		using tRayCastPlayer = bool(__thiscall*)(void* thisptr, Vector3* worldPos, Vector3* worldDirection, float* rotation, PlayerInfo* player, float Distance);
		tRayCastPlayer oRayCastPlayer = reinterpret_cast<tRayCastPlayer>(0x007B09B0);

		return oRayCastPlayer(*(void**)0x11B6DC4, &Camera->MatrixPtr->WorldPos2, &WorldDirection, &Camera->RotationX, player->Player, Distance);
	}
	void Respawn()
	{
		DWORD RespawnAddy = 0x00B35310;
		__asm call RespawnAddy
	}
	void StartMatch()
	{
		DWORD StartMatch = 0x00B1AF50;
		__asm call StartMatch
	}
	void ExitRoom()
	{
		DWORD rqMatch = 0x00B23670;
		__asm call rqMatch;
	}
	void ExitMatch()
	{
		typedef char(__cdecl* tEnd)(int, int);
		tEnd EndGame = (tEnd)0x00B1C1A0;
		EndGame(0, 0);
	}
	void TeamChange()
	{
		DWORD TeamChange = 0x00B39910;
		__asm call TeamChange
	}
	void CreateRoom(int p1, DWORD mode, DWORD* p3, LPCSTR title, LPCSTR password, int time)
	{
		typedef int(__cdecl* tCreateRoom)(int, DWORD, DWORD*, LPCSTR, LPCSTR);
		tCreateRoom oCreateRoom = reinterpret_cast<tCreateRoom>(0x00B21310);
		//write.Byte((DWORD)p3, read.Byte((DWORD)p3 + 5));
		if (mode == Enlimination || mode == ZombieMode || mode == BombBattle)
		{
			sWrite<Byte>((DWORD)p3 + 1, sRead<Byte>((DWORD)p3 + 1) - 0x41);
		}
		else if (mode == BossBattle)
		{
			sWrite<Byte>((DWORD)p3 + 1, 0x5C);
		}
		else if (mode == ArmsRace)
		{
			sWrite<Byte>((DWORD)p3 + 1, 0x1D);
		}
		else
		{
			std::string echo;
			switch (time)
			{
			case Mins5:
				sWrite<Byte>((DWORD)p3 + 1, sRead<Byte>((DWORD)p3 + 1) - 1);
				break;
			case Mins10:
				sWrite<Byte>((DWORD)p3 + 1, sRead<Byte>((DWORD)p3 + 1));
				break;
			case Mins15:
				sWrite<Byte>((DWORD)p3 + 1, sRead<Byte>((DWORD)p3 + 1));
				break;
			case Mins20:
				sWrite<Byte>((DWORD)p3 + 1, sRead<Byte>((DWORD)p3 + 1) + 1);
				break;
			case Mins25:
				sWrite<Byte>((DWORD)p3 + 1, sRead<Byte>((DWORD)p3 + 1) + 2);
				break;
			case Mins30:
				sWrite<Byte>((DWORD)p3 + 1, sRead<Byte>((DWORD)p3 + 1) + 2);
				break;

			default:
				sWrite<Byte>((DWORD)p3 + 1, sRead<Byte>((DWORD)p3 + 1));
				break;
			}
		}
		oCreateRoom(p1, mode, p3, title, password);
	}
	void SwitchScreen(GameState state)
	{
		DWORD GameState = (DWORD)state;
		DWORD StateAddy = 0x011C5058;
		DWORD SwitchScreen = 0x00BA53F0;
		
		__asm
		{
			push GameState
			mov ecx, StateAddy
			call SwitchScreen
		}
	}
	
	bool PlayersReady()
	{
		for (int i = 1; i < this->Room->PlayerCount; i++)
		{
			if (GetStatus(this->Room->Players[i]) != PlayerStatus::STATUS_READY)
				return false;
		}

		return true;
	}
	int PlayersInMatch()
	{
		int playersLoaded = 0;
		for (int i = 0; i < this->Room->PlayerCount; i++)
		{
			if ((GetStatus(this->Room->Players[i]) == PlayerStatus::STATUS_DYING) || (GetStatus(this->Room->Players[i]) == PlayerStatus::STATUS_NORMAL))
				playersLoaded++;
		}
		return playersLoaded;
	}
	PlayerStatus GetStatus(MVEntity* Player)
	{
		return  (PlayerStatus)(Player->Room->RoomStatus & 15);
	}
	
	void AutoStart(int secondsDelay)
	{
		GameState state = *(GameState*)GetServerState();
		switch (GetServerState())
		{

		case GameState::GAME_STATE_ROOM_2ND:
			if (GetStatus(pLocal) == STATUS_READY)
			{
				if (secondsDelay)
					delay(secondsDelay);

				if (PlayersReady())
					StartMatch();
			}
			

			break;
		}
	}
	void AutoReady(int secondsDelay)
	{
		if (gameState4Ready != GetServerState())
		{
			gameState4Ready = GetServerState();
			if (gameState4Ready == GameState::GAME_STATE_ROOM_2ND)
			{
				if (GetStatus(pLocal) == STATUS_WAITING)
				{
					if (secondsDelay)
						delay(secondsDelay);

					SetStatus(PlayerStatus::STATUS_READY);
				}
			}
		}
		/*
		GameState state = *(GameState*)GetServerState();
		switch (GetServerState())
		{
		case GameState::GAME_STATE_ROOM_2ND:
			if (GetStatus(pLocal) == STATUS_WAITING)
			{
				if (secondsDelay)
					delay(secondsDelay);

				SetStatus(PlayerStatus::STATUS_READY);
			}
			break;
		}*/
	}
	void MoveMouse(int x, int y)
	{
		INPUT Inputs[3] = { 0 };

		Inputs[0].type = INPUT_MOUSE;
		Inputs[0].mi.dx = x; // desired X coordinate
		Inputs[0].mi.dy = y; // desired Y coordinate
		Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

		Inputs[1].type = INPUT_MOUSE;
		Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

		Inputs[2].type = INPUT_MOUSE;
		Inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;

		SendInput(3, Inputs, sizeof(INPUT));
	}
	void GoBackRoom()
	{
		DWORD yes = 0x011C5058;
		DWORD goback = 0x00BA55C0;
		__asm
		{
			mov ecx, yes
			call goback
		}
	}
	void AutoSkipResults(int secondsDelay)
	{
		if (gameState4Skip != GetServerState())
		{
			gameState4Skip = GetServerState();
			if (gameState4Skip == GameState::GAME_STATE_MOD_END)
			{
				if (secondsDelay)
					delay(secondsDelay);

				StartMatch();
			}
		}

		/*GameState state = *(GameState*)GetServerState();
		switch (GetServerState())
		{
		case GameState::GAME_STATE_MOD_END:
			if (secondsDelay)
				delay(secondsDelay);
			
			GoBackRoom();

			break;
		}*/
	}
	void AntiAFK()
	{
		sWrite<byte>(0x0043BF53, 1);
		sWrite<byte>(0x00462879, 1);		
	}
	void SetStatus(int status)
	{
		using tSetPlayerStatus = int(__cdecl*)(int);
		tSetPlayerStatus oSetPlayerStatus = reinterpret_cast<tSetPlayerStatus>(0x00B39B80);
		oSetPlayerStatus(status);
	}
	void SendRifleHit(int Weapon, MVEntity* Player, HitType hitType)
	{
		using tSendHit = int(__cdecl*)(Vector3, Vector3, DWORD, Vector3, HitType);
		tSendHit oSendHit = reinterpret_cast<tSendHit>(0x00B30560);
		Offset idkcoordOffs[] = { 0x30, 0xC };
		Vector3* idk = reinterpret_cast<Vector3*>(sCalcPtr(0x11B5FB4, idkcoordOffs, 2));
		Vector3 shit = { 0.f,0.f,0.f };
		oSendHit(shit, shit, Player->Room->Server->serialID, shit, hitType);
	}
	void SendSniperHit(int Weapon, MVEntity* Player, HitType hitType, bool isScope)
	{
		using tSendHit = int(__cdecl*)(Vector3, Vector3, DWORD, Vector3, HitType, int unk, bool isScope);
		tSendHit oSendHit = reinterpret_cast<tSendHit>(0x00B31C80);
		Offset idkcoordOffs[] = { 0x30, 0xC };
		Vector3* idk = reinterpret_cast<Vector3*>(sCalcPtr(0x11B5FB4, idkcoordOffs, 2));
		Vector3 shit = { 0.f,0.f,0.f };
		//oSendHit(&this->pLocal->Weaponds->Weapon[Weapon]->WeaponAngles, idk, (DWORD)Player->Room->Server, &Player->Movement->Coords, hitType, 1, isScope);
		oSendHit(shit, shit, Player->Room->Server->serialID, shit, hitType, 1, isScope);
	}
	void readFile(const char* filename, std::vector<std::string>& lines)
	{
		lines.clear();
		std::ifstream file(filename);
		std::string s;
		while (std::getline(file, s))
			lines.push_back(s);
	}
	void AutoQuit(std::vector<std::string> players)
	{
		for (int i = 0; i < Room->PlayerCount + Room->ObserversCount; i++)
		{
			for (int p = 0; p < players.size(); p++)
			{
				if (Room->Players[i])
				{
					if (strcmp(Room->Players[i]->Name, players[p].c_str()) == 0)
					{
						ExitRoom();
					}
				}
			}
		}
	}
	void AutoInvite(std::vector<std::string> players, int delay)
	{
		if (GetServerState() == GAME_STATE_ROOM_2ND)
		{
			for (int p = 0; p < players.size();)
			{
				static clock_t oldtime = clock();
				for (clock_t now = clock(); now - oldtime > delay;)
				{
					Invite(players[p].c_str());
					p++;
					if (p == players.size())
						p = 0;
				}
			}
		}
	}
	int GetCurrentWeapon()
	{


		DWORD IsMelee = { 0x11B9FC4 };
		DWORD IsRifle = { IsMelee + 0x4c };
		DWORD IsShotgun = { IsMelee + 0x98 };
		DWORD IsSniper = { IsMelee + 0xE4 };
		DWORD IsGatling = { IsMelee + 0x130 };
		DWORD IsBazooka = { IsMelee + 0x17C };
		DWORD IsGrenade = { IsMelee + 0x1C8 };

		if (*(int*)IsMelee)
			return WeaponType::MELEE;
		else if (*(int*)IsRifle)
			return WeaponType::RIFLE;
		else if (*(int*)IsShotgun)
			return WeaponType::SHOTGUN;
		else if (*(int*)IsSniper)
			return WeaponType::SNIPER;
		else if (*(int*)IsGatling)
			return WeaponType::MG;
		else if (*(int*)IsBazooka)
			return WeaponType::BAZOOKA;
		else if (*(int*)IsGrenade)
			return WeaponType::GRENADE;

		return -1;
	}
	void AutoSelectWeap(int Weapon)
	{
		if ((GetServerState() == GAME_STATE_MOD_PLAYING ) && (GetStatus(this->pLocal) == PlayerStatus::STATUS_NORMAL))
		{
			if (GetCurrentWeapon() != Weapon)
			{
				if (Weapon == WeaponType::MELEE)
				{
					sWrite<byte>(0x11B20DF, 1);
					sWrite<byte>(0x11B20DF, 0);
				}
				else if (Weapon == WeaponType::RIFLE)
				{
					sWrite<byte>(0x11B20E0, 1);
					sWrite<byte>(0x11B20E0, 0);
				}
				else if (Weapon == WeaponType::SHOTGUN)
				{
					sWrite<byte>(0x11B20E1, 1);
					sWrite<byte>(0x11B20E1, 0);
				}
				else if (Weapon == WeaponType::SNIPER)
				{
					sWrite<byte>(0x11B20E2, 1);
					sWrite<byte>(0x11B20E2, 0);
				}
				else if (Weapon == WeaponType::MG)
				{
					sWrite<byte>(0x11B20E3, 1);
					sWrite<byte>(0x11B20E3, 0);
				}
				else if (Weapon == WeaponType::BAZOOKA)
				{
					sWrite<byte>(0x11B20E4, 1);
					sWrite<byte>(0x11B20E4, 0);
				}
				else if (Weapon == WeaponType::GRENADE)
				{
					sWrite<byte>(0x11B20E5, 1);
					sWrite<byte>(0x11B20E5, 0);
				}
					
			}
		}
	}
	void RoomRape(int Weapon, int hitType)
	{
		if (GetServerState() == GAME_STATE_MOD_PLAYING)
		{
			for (int i = 0; i < this->Room->PlayerCount; i++)
			{
				if (this->Room->Players[i])
				{
					if (this->pLocal != this->Room->Players[i] && IsEnemy(i) && GetStatus(this->pLocal) == PlayerStatus::STATUS_NORMAL && GetStatus(this->Room->Players[i]) == PlayerStatus::STATUS_NORMAL)
					{
						if (Weapon == WeaponType::RIFLE)
							SendRifleHit(Weapon, this->Room->Players[i], (HitType)hitType);
						else if (Weapon == WeaponType::SNIPER)
							SendSniperHit(Weapon, this->Room->Players[i], (HitType)hitType, true);
					}
				}
			}
		}
	}
	void HostKill(MVEntity* Killer, MVEntity* Victim, HitType killtype, WeaponType weapon)
	{
		struct KillStruct
		{
			DWORD wpn;//0
			DWORD killerID;//4
			DWORD victimID;//8
			byte unk[0x1c];//c
			DWORD kilType;
		};
		using tHostKill = int(__cdecl*)(KillStruct);
		tHostKill oHostKill = reinterpret_cast<tHostKill>(0x00AF08B0);
		KillStruct yes;
		yes.wpn = 0x0000448A + (0x01000000 * weapon);
		yes.killerID = Killer->Room->Server->serialID;
		yes.victimID = Victim->Room->Server->serialID;
		yes.kilType = (DWORD)killtype;
		oHostKill(yes);
	}
	void ChangeHost(char* Name)
	{
		using tChangeHost = int(__cdecl*)(unsigned int);
		tChangeHost oChangeHost = reinterpret_cast<tChangeHost>(0x00B23F60);
		for (int i = 0; i < this->Room->PlayerCount; i++)
		{
			if (!strcmp(this->Room->Players[i]->Name, Name))
			{
				oChangeHost(i);
				return;
			}
		}
	}
	void ChangeHost(int index)
	{
		using tChangeHost = int(__cdecl*)(unsigned int);
		tChangeHost oChangeHost = reinterpret_cast<tChangeHost>(0x00B23F60);
		oChangeHost(index);
	}
	void Kick(char* Name)
	{
		using tKick = char(__cdecl*)(MVEntity*);
		tKick oKick = reinterpret_cast<tKick>(0x00B23890);
		for (int i = 0; i < this->Room->PlayerCount; i++)
		{
			if (strcmp(this->Room->Players[i]->Name, Name) == 0)
			{
				oKick(this->Room->Players[i]);
				return;
			}
		}
	}
	void Invite(const char* Name)
	{
		using tInvite = char(__cdecl*)(const char* Name);
		tInvite oInvite = reinterpret_cast<tInvite>(0x00B3B480);
		oInvite(Name);
		//std::string invitemsg = "Invited player '" + std::string(Name) + "'";
		//cChat->PrintChatMessage(invitemsg.c_str(), "feel-dope", CChat::Type::GM, true, false, CChat::Color::GREEN);

	}
	void Kick(MVEntity* Player)
	{
		using tKick = char(__cdecl*)(MVEntity*);
		tKick oKick = reinterpret_cast<tKick>(0x00B23890);
		oKick(Player);
		std::string kickMSG = "Kicked player '" + std::string(Player->Name) + "' from room.";
		cChat->PrintChatMessage(kickMSG.c_str(), "Worker", CChat::Type::INFO, true, false, CChat::Color::YELLOW);

	}
	void addPlayerToWhitelist(const char* Name, int selectedCollumn)
	{
		if (whitelist[selectedCollumn].empty())
		{
			whitelist[selectedCollumn] = Name;
		}
	}
	void removePlayerFromWhitelist(int selectedCollumn)
	{
		if (!whitelist[selectedCollumn].empty())
		{
			whitelist[selectedCollumn].clear();
		}
	}
	int getWhitelistSize()
	{
		size_t i = 0;
		while (!whitelist[i].empty())
			++i;

		return i;
	}
	void AutoKickNonWhitelistedMembers()
	{
		for (int i = 0; i < this->Room->PlayerCount; i++)
		{
			for (int j = 0; i < getWhitelistSize(); j++)
			{
				if (strcmp(this->Room->Players[i]->Name, whitelist[j].c_str()) == 0)
				{
					if (!strcmp(pLocal->Name, this->Room->Players[i]->Name))
						Kick(this->Room->Players[i]->Name);

					return;
				}
			}
		}
	}

	int GetServerTime() 
	{
		DWORD GetServerTime = 0x004D86B0;
		DWORD CalcServerTime = 0x00E0B230;
		int serverTime;
		__asm
		{
			call GetServerTime
			mov ecx,eax
			call CalcServerTime
			mov serverTime,eax
		}
		return serverTime;
	}
	int GetFPS()
	{
		return *(int*)0x11B4440;
	}
	
	void Draw_Nametags(float x, float y, int box_size, char* Text, bool isEnemy, bool isTeam, MVEntity* player)
	{
		using tNametags = int(__thiscall*)(void* thisptr, float x, float y, int box_size, char* Text, bool isTeamRED, bool isTeamBLUE, MVEntity* player, float viewProjSqrt);
		tNametags oNametags = reinterpret_cast<tNametags>(0x005AED90);
		oNametags((void*)0x011B9CE8, floor(x), floor(y), box_size, Text, isEnemy, isTeam, player, floor(player->Movement->Coords.Dist(Camera->MatrixPtr->WorldPos2)));
		
	}
	
	void Draw_Text(int x, int y, int color, char* text, int textSizeAlloc)
	{
		
		using tText = int(__thiscall*)(void*, int, int, int color, char* , int);
		tText oText = reinterpret_cast<tText>(0x00432A40);
		DWORD render = 0x011A5D20;
		oText(&render, x, y, color, text, textSizeAlloc);
	}
	char* GetNationIndex()
	{
		typedef char*(*tGetNationIndex)();
		tGetNationIndex GetNationIndex = (tGetNationIndex)0x0055EBA0;
		return GetNationIndex();
	}
private:
	HWND gameHWND;
	std::string whitelist[];
	
	
};
extern CEngine* cEngine;



