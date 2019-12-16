#pragma once
struct POINT2D
{
	float x, y;
	POINT2D()
	{
		this->x = 0;
		this->y = 0;
	}
	POINT2D(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	~POINT2D()
	{

	}
};
class CAimbot
{
public:
	
	void Run();
	bool Update();
	char* TargetName;
	float distfov = 0;
	int GetCurrentWeapon();
	float Timer = 0.0f;
	Vector3 oldPos;
	Vector3 Velocity[15];
	Vector3 GetVelocity(Vector3 currentPos);
	std::string GetCurrentWeaponStr(int weapIndex);
	CRoom* Room = nullptr;
	MVEntity* pLocal = nullptr;
	CCamera * Camera = nullptr;
	//CCamera * Camera = nullptr;
	Vector3 ReadRotation;
	bool bTriggerbotCanFire = false;
	
	void AngleNormalize(Vector3& vAngles);
	void NormalizeAngle(Vector3& vAngles);
	void AngleClamp(Vector3& vAngles);
	Vector3 SmoothAngle(Vector3 src, Vector3 dst, float Percent);
	Vector3 CurveAngle(Vector3 src, Vector3 dst, float CurveX, float CurveY, bool bUseRandom, float Timer);
	Vector3 CalculateSmoothForFactorRAD(Vector3 src, Vector3 dst, float factor, bool bUseRandom);
	inline void SinCos(float radians, float *sine, float *cosine);
	void AngleVectors(Vector3 &angles, Vector3 *forward);
	MVEntity* FindClosestTarget();
	Vector3 GetBone(MVEntity* player);
	Vector3 GetClosestBone(MVEntity* player);
	void UpdateTime(float DeltaTime);
	void ResetTime(Vector3 newTime);
	Vector3 m_TimeToTarget;
	Vector3 m_CurAimTime;
	int LockedTarget;
	float deltashit;
	MVEntity* pTarget = nullptr;
private:
	bool IsEnemy(MVEntity* player);
	
	float GetDistance(Vector3 me, Vector3 enemy);
	float GetFOVFraction(Vector3 myAngles, Vector3 targetAngles);
	float GetFOV(Vector3 myAngles, Vector3 targetAngles, float Distance);
	float GetFovReal(Vector3 myAngles, Vector3 targetAngles, float Distance);
	static float CalcDist2D(POINT2D* from, POINT2D* to);
	Vector3 CalcAngle(Vector3 src, Vector3 dst);
	Vector3 CalcAngle2(Vector3 src, Vector3 dst);
	void CerpAngle(Vector3 &from, Vector3 &to, float StepX, float StepY);
	Vector3 GetPredicted(Vector3 Position, Vector3 Velocity);
	inline float RandFloat(float M, float N)
	{
		return (float)(M + (rand() / (RAND_MAX / (N - M))));
	}
	bool ApplyAngles(Vector3 angles);
	std::chrono::high_resolution_clock m_Timer;
	std::chrono::high_resolution_clock::time_point m_LastTime;

};
extern CAimbot * cAimbot;