#include "SDK.h"


void CAimbot::Run()
{

	
	if (!this->Update()) return;
	if (GetCurrentWeapon() == -1) return;
	if (cEngine->GetServerState() != GameState::GAME_STATE_MOD_PLAYING) return;
	if (cEngine->GetStatus(pLocal) != PlayerStatus::STATUS_NORMAL) return;


	float deltaTime = 1.0f / 60;
	deltashit = deltaTime;
	
	
	if (Vars.Aimbot.Weapon[GetCurrentWeapon()].Enabled) // Aimbot
	{
		static MVEntity* pOldTarget = FindClosestTarget();
		pTarget = FindClosestTarget();
		Vector3 Target = Vars.Aimbot.Weapon[GetCurrentWeapon()].closestBone.Enabled ? GetClosestBone(pTarget) : GetBone(pTarget);
		if ((pTarget != pOldTarget) || Target.IsZero() || (Vars.Aimbot.Weapon[GetCurrentWeapon()].Mode && (!GetAsyncKeyState(Vars.Aimbot.Weapon[GetCurrentWeapon()].Key))))
		{
			pOldTarget = pTarget;
			if (Vars.Aimbot.Weapon[GetCurrentWeapon()].RandomTime)
				ResetTime(Vector3(random_float(Vars.Aimbot.Weapon[GetCurrentWeapon()].MinTimeToTarget, Vars.Aimbot.Weapon[GetCurrentWeapon()].MaxTimeToTarget), random_float(Vars.Aimbot.Weapon[GetCurrentWeapon()].MinTimeToTarget, Vars.Aimbot.Weapon[GetCurrentWeapon()].MaxTimeToTarget), random_float(Vars.Aimbot.Weapon[GetCurrentWeapon()].MinTimeToTarget, Vars.Aimbot.Weapon[GetCurrentWeapon()].MaxTimeToTarget)));
			else
				ResetTime(Vector3(Vars.Aimbot.Weapon[GetCurrentWeapon()].TimeToTarget, Vars.Aimbot.Weapon[GetCurrentWeapon()].TimeToTarget, Vars.Aimbot.Weapon[GetCurrentWeapon()].TimeToTarget));
			
		}
		else
		{
			pTarget = FindClosestTarget();
			UpdateTime(deltaTime);
			if (!this->ApplyAngles(CalcAngle2(Camera->MatrixPtr->WorldPos2, Target))) return;
		}	
	
	}
	if (Vars.Misc.WeaponInfo[cAimbot->GetCurrentWeapon()].Enabled)
	{
		if (Vars.Misc.WeaponInfo[WeaponType::MELEE].MeleeRange)
			pLocal->Room->MeleeRange = Vars.Misc.WeaponInfo[WeaponType::MELEE].MeleeRangeVal;
		else
			pLocal->Room->MeleeRange = 0;

		if (Vars.Misc.WeaponInfo[cAimbot->GetCurrentWeapon()].Ammo)
			pLocal->Weaponds->Weapon[cAimbot->GetCurrentWeapon()]->Ammo = 99 ^ *(int*)0x01182A74; //xor addy

		if (Vars.Misc.WeaponInfo[cAimbot->GetCurrentWeapon()].ReloadAmmo)
			pLocal->Weaponds->Weapon[cAimbot->GetCurrentWeapon()]->ReloadAmmo = 1337 ^ *(int*)0x01182A74; //xor addy

		if (Vars.Misc.WeaponInfo[cAimbot->GetCurrentWeapon()].FiringRate)
		{
			Offset firingOffs[] = { 0x6C };
			sWrite(CalcPtr(0x011B7D50, firingOffs, 1), 0);
		}
		if (Vars.Misc.WeaponInfo[SNIPER].SniperNoScope)
			pLocal->Weaponds->Weapon[SNIPER]->SniperNoscope = 1;

		if (Vars.Misc.WeaponInfo[MG].GatlingHeat)
			pLocal->Weaponds->Weapon[MG]->GatlingHeat = 0;

	}
}
bool CAimbot::Update()
{
	Offset CLocalPltrOffs[] = { 0x7C, 0x10C, 0 };
	this->Room = *(CRoom**)0x011B5FC0;
	this->pLocal = (MVEntity*)CalcPtr(0x11B5FBC, CLocalPltrOffs, 3);
	this->Camera = *(CCamera**)0x011B9150;
	


	return this->Room && this->pLocal && this->Camera;
}


Vector3 CAimbot::GetPredicted(Vector3 Position, Vector3 Velocity)
{
	return Position + (Velocity * interval_per_tick);
}

float euclideanDist(Vector3 src, Vector3 dst)
{
	return pow(src.x - dst.x, 2) + pow(src.y - dst.y, 2);
}
Vector3 CAimbot::SmoothAngle(Vector3 src, Vector3 dst, float SmoothPercent)
{
	if (SmoothPercent < 0.f)
	{
		SmoothPercent = 0.f;
	}
	Vector3 delta = dst - src;
	NormalizeAngle(delta);
	
	Vector3 result = src + (Vars.Aimbot.Weapon[GetCurrentWeapon()].SmoothType ?  (delta * (m_CurAimTime / m_TimeToTarget)) : delta /  100 * SmoothPercent);
	
	NormalizeAngle(result);
	return result;
}
template<typename T>
static T CubicInterpolate(T const& p1, T const& p2, T const& p3, T const& p4, float t)
{
	return p1 * (1 - t) * (1 - t) * (1 - t) +
		p2 * 3 * t * (1 - t) * (1 - t) +
		p3 * 3 * t * t * (1 - t) +
		p4 * t * t * t;
}
Vector3 CAimbot::CurveAngle(Vector3 src, Vector3 dst, float CurveX, float CurveY, bool bUseRandom, float Timer)
{
	if (CurveX < 0.f)
	{
		CurveX = 0.f;
	}
	if (CurveY < 0.f)
	{
		CurveY = 0.f;
	}
	Vector3 delta = src - dst;
	NormalizeAngle(delta);

	float ValPt1 = bUseRandom ? (random_float(0.1f, 0.4f)) : CurveX;
	Vector3 point1 = src + (delta * ValPt1);
	NormalizeAngle(point1);

	float ValPt2 = bUseRandom ? (random_float(0.8f, 1.3f)) : CurveY;
	Vector3 point2 = src + (delta * ValPt2);
	NormalizeAngle(point2);
	return CubicInterpolate(src, point1, point2, dst, Timer);
}

Vector3 CAimbot::CalculateSmoothForFactorRAD(Vector3 src, Vector3 dst, float factor, bool bUseRandom)
{
	if (factor < 0.f)
	{
		factor = 0.f;
	}
	Vector3 delta = dst - src;
	NormalizeAngle(delta);
	
	Vector3 result = src + delta * (delta * factor);

	NormalizeAngle(result);
	return result;
}

void inline CAimbot::SinCos(float radians, float * sine, float * cosine)
{
	*sine = sin(radians);
	*cosine = cos(radians);
}

void CAimbot::AngleVectors(Vector3 & angles, Vector3 * forward)
{
	float sp, sy, cp, cy;

	SinCos(angles.x, &sy, &cy); //yaw
	SinCos(angles.y, &sp, &cp); //pitch

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

bool CAimbot::IsEnemy(MVEntity* player)
{
	//if (id > 15) return false;
	TeamID MyTeam = (TeamID)((this->pLocal->Room->Team >> 12) & 7);
	TeamID HisTeam = (TeamID)((player->Room->Team >> 12) & 7);
	return (MyTeam == TEAM_EVERYONE || MyTeam == TEAM_OBS || MyTeam != HisTeam);
}

Vector3 CAimbot::GetBone(MVEntity* player)
{
	Vector3 Enemy = Vector3(0, 0, 0);

	float FOV = Vars.Aimbot.Weapon[GetCurrentWeapon()].FOV;
	if (player)
	{
		if (this->pLocal != player && IsEnemy(player) && cEngine->GetStatus(this->pLocal) == PlayerStatus::STATUS_NORMAL && cEngine->GetStatus(player) == PlayerStatus::STATUS_NORMAL)
		{
			if (Vars.Aimbot.Weapon[GetCurrentWeapon()].VisualCheck)
				if (!cEngine->isVisible(player, 10000.f)) return Vector3(0, 0, 0);

			Vector3 angles = CalcAngle2(this->Camera->MatrixPtr->WorldPos2, *player->Player->Coords->Bone[BoneID::Bip01_Head]->GetTransform(0));
			Vector3 myAngles = { this->pLocal->Room->Yaw + this->pLocal->Player->Coords->Rotation, this->pLocal->Player->Coords->Pitch,0.0f };

			distfov = GetFOVFraction(myAngles, angles);//GetFOV(myAngles, angles, distance3D);//GetFOVFraction(myAngles, angles);

			if (distfov < FOV)
			{
				FOV = distfov;
				Enemy = *player->Player->Coords->Bone[BoneID::Bip01_Head]->GetTransform(0);

				if (Vars.Aimbot.Weapon[GetCurrentWeapon()].AimHeight)
					Enemy.z -= Vars.Aimbot.Weapon[GetCurrentWeapon()].HeightAmt;
			}
			
		}
	}
	return Enemy;
}

MVEntity* CAimbot::FindClosestTarget()
{
	MVEntity* Enemy = nullptr;

	float FOV = Vars.Aimbot.Weapon[GetCurrentWeapon()].FOV;
	for (int i = 0; i < this->Room->PlayerCount; i++)
	{
		if (this->Room->Players[i])
		{
			if (this->pLocal != this->Room->Players[i] && IsEnemy(this->Room->Players[i]) && cEngine->GetStatus(this->pLocal) == PlayerStatus::STATUS_NORMAL && cEngine->GetStatus(this->Room->Players[i]) == PlayerStatus::STATUS_NORMAL)
			{
				if (Vars.Aimbot.Weapon[GetCurrentWeapon()].VisualCheck)
					if (!cEngine->isVisible(this->Room->Players[i], 10000.f)) continue;

				Vector3 angles = CalcAngle2(this->Camera->MatrixPtr->WorldPos2, *this->Room->Players[i]->Player->Coords->Bone[BoneID::Bip01_Head]->GetTransform(0));
				Vector3 myAngles = { this->pLocal->Room->Yaw + this->pLocal->Player->Coords->Rotation, this->pLocal->Player->Coords->Pitch,0.0f };

				distfov = GetFOVFraction(myAngles, angles);

				if (distfov < FOV)
				{
					FOV = distfov;
					Enemy = this->Room->Players[i];
				}

			}
		}
	}
	return Enemy;
}

Vector3 CAimbot::GetClosestBone(MVEntity* player)
{
	Vector3 ClosestBone = Vector3(0, 0, 0);
	float FOV = Vars.Aimbot.Weapon[GetCurrentWeapon()].FOV;

	if (player)
	{
		if (this->pLocal != player && IsEnemy(player) && cEngine->GetStatus(this->pLocal) == PlayerStatus::STATUS_NORMAL && cEngine->GetStatus(player) == PlayerStatus::STATUS_NORMAL)
		{
			if (Vars.Aimbot.Weapon[GetCurrentWeapon()].VisualCheck)
				if (!cEngine->isVisible(player, 10000.f)) return Vector3(0, 0, 0);

			Vector3 myAngles = { this->pLocal->Room->Yaw + this->pLocal->Player->Coords->Rotation, this->pLocal->Player->Coords->Pitch,0.0f };

			if (Vars.Aimbot.Weapon[GetCurrentWeapon()].closestBone.Head)
			{
				for (int head = 0; head <= 8; head++)
				{
					Vector3 BoneAngles = CalcAngle2(this->Camera->MatrixPtr->WorldPos2, *player->Player->Coords->Bone[BoneID::Bip01_Spine]->GetTransform(head));
					float distance = GetFOVFraction(myAngles, BoneAngles);
					if (FOV > distance)
					{
						FOV = distance;
						ClosestBone = *player->Player->Coords->Bone[BoneID::Bip01_Spine]->GetTransform(head);
					}
				}
			}
			

			if (Vars.Aimbot.Weapon[GetCurrentWeapon()].closestBone.Body)
			{
				for (int spine = 0; spine <= 7; spine++)
				{
					Vector3 BoneAngles = CalcAngle2(this->Camera->MatrixPtr->WorldPos2, *player->Player->Coords->Bone[BoneID::Bip01_Spine]->GetTransform(spine));
					float distance = GetFOVFraction(myAngles, BoneAngles);
					if (FOV > distance)
					{
						FOV = distance;
						ClosestBone = *player->Player->Coords->Bone[BoneID::Bip01_Spine]->GetTransform(spine);
					}
					
				}
			}
			if (Vars.Aimbot.Weapon[GetCurrentWeapon()].closestBone.Arms)
			{
				for (int larm = 0; larm <= 3; larm++)
				{
					Vector3 BoneAngles = CalcAngle2(this->Camera->MatrixPtr->WorldPos2, *player->Player->Coords->Bone[BoneID::Bip01_L_UpperArm]->GetTransform(larm));
					float distance = GetFOVFraction(myAngles, BoneAngles);
					if (FOV > distance)
					{
						FOV = distance;
						ClosestBone = *player->Player->Coords->Bone[BoneID::Bip01_L_UpperArm]->GetTransform(larm);
					}
					
				}
				for (int rarm = 0; rarm <= 3; rarm++)
				{
					Vector3 BoneAngles = CalcAngle2(this->Camera->MatrixPtr->WorldPos2, *player->Player->Coords->Bone[BoneID::Bip01_R_UpperArm]->GetTransform(rarm));
					float distance = GetFOVFraction(myAngles, BoneAngles);
					if (FOV > distance)
					{
						FOV = distance;
						ClosestBone = *player->Player->Coords->Bone[BoneID::Bip01_R_UpperArm]->GetTransform(rarm);
					}
					
				}
			}
			if (Vars.Aimbot.Weapon[GetCurrentWeapon()].closestBone.Legs)
			{
				for (int lfoot = 0; lfoot <= 3; lfoot++)
				{
					Vector3 BoneAngles = CalcAngle2(this->Camera->MatrixPtr->WorldPos2, *player->Player->Coords->Bone[BoneID::Bip01_L_Thig]->GetTransform(lfoot));
					float distance = GetFOVFraction(myAngles, BoneAngles);
					if (FOV > distance)
					{
						FOV = distance;
						ClosestBone = *player->Player->Coords->Bone[BoneID::Bip01_L_Thig]->GetTransform(lfoot);
					}
					
				}
				for (int rfoot = 0; rfoot <= 3; rfoot++)
				{
					Vector3 BoneAngles = CalcAngle2(this->Camera->MatrixPtr->WorldPos2, *player->Player->Coords->Bone[BoneID::Bip01_R_Thig]->GetTransform(rfoot));
					float distance = GetFOVFraction(myAngles, BoneAngles);
					if (FOV > distance)
					{
						FOV = distance;
						ClosestBone = *player->Player->Coords->Bone[BoneID::Bip01_R_Thig]->GetTransform(rfoot);
					}
					
				}
			}
			if (Vars.Aimbot.Weapon[GetCurrentWeapon()].AimHeight)
				ClosestBone.z -= Vars.Aimbot.Weapon[GetCurrentWeapon()].HeightAmt;
		}
	}
	return ClosestBone;
}

void CAimbot::UpdateTime(float DeltaTime)
{
	m_CurAimTime += Vector3(DeltaTime, DeltaTime, 0);
	if (m_CurAimTime.x > m_TimeToTarget.x)
		m_CurAimTime.x = m_TimeToTarget.x;

	if (m_CurAimTime.y > m_TimeToTarget.y)
		m_CurAimTime.y = m_TimeToTarget.y;
}

void CAimbot::ResetTime(Vector3 newTime)
{
	if (newTime.IsZero())
		m_TimeToTarget = Vector3(1.0f, 1.0f, 1.0f);
	else
		m_TimeToTarget = newTime;

	m_CurAimTime = Vector3(0, 0, 0);
}

float CAimbot::GetDistance(Vector3 me, Vector3 enemy)
{
	float Dx = me.x - enemy.x;
	float Dy = me.y - enemy.y;
	float Dz = me.z - enemy.z;

	return sqrt(Dx*Dx + Dy * Dy + Dz * Dz);
}

float CAimbot::GetFOV(Vector3 myAngles, Vector3 targetAngles, float Distance)
{
	auto yaw = sin(myAngles.x - targetAngles.x); //* Distance; 
	auto pitch = sin(myAngles.y - targetAngles.y); //* Distance;

	return sqrt(powf(pitch * M_RADPI, 2.0) + powf(yaw * M_RADPI, 2.0));
}
float CAimbot::GetFOVFraction(Vector3 myAngles, Vector3 targetAngles)
{
	Vector3 delta = targetAngles - myAngles;
	NormalizeAngle(delta);
	return sin(delta.Length() / 2.0f) * 180.0f;
}
float CAimbot::GetFovReal(Vector3 myAngles, Vector3 targetAngles, float Distance)
{
	Vector3 aimingAt;
	AngleVectors(myAngles, &aimingAt);
	aimingAt *= Distance;

	Vector3 aimAt;
	AngleVectors(targetAngles, &aimAt);
	aimAt *= Distance;

	return aimingAt.DistToSqrt(aimAt);
}



float CAimbot::CalcDist2D(POINT2D * from, POINT2D * to)
{
	if (from && to)
	{
		POINT2D delta;
		delta.x = to->x - from->x;
		delta.y = to->y - from->y;

		return sqrtf(delta.x * delta.x + delta.y * delta.y);
	}
	return 0;
}

Vector3 CAimbot::CalcAngle(Vector3 src, Vector3 dst)
{
	Vector3 delta = dst - src;

	float dist2D = sqrtf(delta.x * delta.x + delta.y * delta.y);
	Vector3 viewAngles;
	




	if (dist2D && delta.x && delta.y && delta.z)
	{
		viewAngles = { -atanf(delta.y / delta.x) * M_RADPI, atanf(delta.z / dist2D)  * M_RADPI , 0.f };
		if (delta.x < 0) viewAngles.x += 180;

		if (viewAngles.y < 0)
			viewAngles.y *= 1.253;
		else
			viewAngles.y *= 0.837;

		return viewAngles;
	}
	
}

Vector3 CAimbot::CalcAngle2(Vector3 src, Vector3 dst)
{

	Vector3 delta = dst - src;

	float dist2D = sqrtf(delta.x * delta.x + delta.y * delta.y);
	Vector3 viewAngles;

	


	if (dist2D && !delta.IsZero())
	{
		viewAngles = { -atanf(delta.y / delta.x), atanf(delta.z / dist2D), 0.f };
		if (delta.x < 0) viewAngles.x += M_PI;

		if (viewAngles.y < 0)
			viewAngles.y *= 1.253;
		else
			viewAngles.y *= 0.837;

		return viewAngles;
	}
}

void CAimbot::CerpAngle(Vector3 & from, Vector3 & to, float StepX, float StepY)
{
	float CubicStepX = (1 - cos(StepX*M_PI)) / 2;
	float CubicStepY = (1 - cos(StepY*M_PI)) / 2;
	Vector3 delta = to - from;
	NormalizeAngle(delta);
	to.x = (from.x + CubicStepX * delta.x);
	to.y = (from.y + CubicStepY * delta.y);
	NormalizeAngle(to);
}



int CAimbot::GetCurrentWeapon()
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

	/*if (this->pLocal->Weaponds->Weapon[WeaponType::MELEE]->IsActive)
		return  WeaponType::MELEE;
	else if (this->pLocal->Weaponds->Weapon[WeaponType::RIFLE]->IsActive)
		return WeaponType::RIFLE;
	else if (this->pLocal->Weaponds->Weapon[WeaponType::SHOTGUN]->IsActive)
		return WeaponType::SHOTGUN;
	else if (this->pLocal->Weaponds->Weapon[WeaponType::SNIPER]->IsActive)
		return WeaponType::SNIPER;
	else if (this->pLocal->Weaponds->Weapon[WeaponType::MG]->IsActive)
		return WeaponType::MG;
	else if (this->pLocal->Weaponds->Weapon[WeaponType::BAZOOKA]->IsActive)
		return WeaponType::BAZOOKA;
	else if (this->pLocal->Weaponds->Weapon[WeaponType::GRENADE]->IsActive)
		return WeaponType::GRENADE;*/
}

Vector3 CAimbot::GetVelocity(Vector3 currentPos)
{
	return Vector3(0, 0, 0);
}

std::string CAimbot::GetCurrentWeaponStr(int weapIndex)
{
	if (weapIndex == WeaponType::MELEE)
		return strenc("Melee");
	else if (weapIndex == WeaponType::RIFLE)
		return strenc("Rifle");
	else if (weapIndex == WeaponType::SHOTGUN)
		return strenc("Shotgun");
	else if (weapIndex == WeaponType::SNIPER)
		return strenc("Sniper");
	else if (weapIndex == WeaponType::MG)
		return strenc("Gatling");
	else if (weapIndex == WeaponType::BAZOOKA)
		return strenc("Bazooka");
	else if (weapIndex == WeaponType::GRENADE)
		return strenc("Grenade");
	else if (weapIndex == -1)
		return strenc("Invalid Weapon (-1)");
}

void CAimbot::AngleNormalize(Vector3& vAngles)
{
	/*if (vAngles.y > M_PI / 2 && vAngles.y <= M_PI)
		vAngles.y = M_PI / 2;


	while (vAngles.x > M_PI)
		vAngles.x -= M_2PI;
	while (vAngles.x < -M_PI)
		vAngles.x += M_2PI;

	while (vAngles.y > M_PI)
		vAngles.y -= M_2PI;
	while (vAngles.y < -(M_PI/2))
		vAngles.y += -(M_2PI/2);*/
	if (vAngles.x > Deg2Rad(89.0f) && vAngles.x <= Deg2Rad(180.0f))
	{
		vAngles.x = Deg2Rad(89.0f);
	}
	if (vAngles.x > Deg2Rad(180.f))
	{
		vAngles.x -= Deg2Rad(360.f);
	}
	if (vAngles.x < Deg2Rad(-89.0f))
	{
		vAngles.x = Deg2Rad(-89.0f);
	}
	if (vAngles.y > Deg2Rad(180.f))
	{
		vAngles.y -= Deg2Rad(360.f);
	}
	if (vAngles.y < Deg2Rad(-180.f))
	{
		vAngles.y += Deg2Rad(360.f);
	}

}
void CAimbot::NormalizeAngle(Vector3& vAngles)
{
	if (vAngles.x <= -M_PI)
		vAngles.x += M_2PI;

	if (vAngles.x > M_PI)
		vAngles.x -= M_2PI;

	if (vAngles.x <= -M_2PI)
		vAngles.x += M_PI;

	if (vAngles.x > M_2PI)
		vAngles.x -= M_PI;
}
void CAimbot::AngleClamp(Vector3& vAngles)
{
	/*if (vAngles.y > M_PI)
		vAngles.y = M_PI;
	else if (vAngles.y < -M_PI)
		vAngles.y = -M_PI;

	if (vAngles.x > 1.55334)
		vAngles.x = 1.55334;
	else if (vAngles.x < -1.55334)
		vAngles.x += -1.55334;

	vAngles.z = 0;*/
}

bool CAimbot::ApplyAngles(Vector3 angles)
{
	if (Vars.Aimbot.Weapon[GetCurrentWeapon()].Smooth)
	{
		//float smooth = Vars.Aimbot.Weapon[GetCurrentWeapon()].Speed;
		Vector3 src = { this->pLocal->Room->Yaw , this->pLocal->Player->Coords->Pitch,  0.0f };
		Vector3 dst = { angles.x - this->pLocal->Player->Coords->Rotation, angles.y, 0.0f };

		this->pLocal->Room->Yaw = SmoothAngle(src, dst, Vars.Aimbot.Weapon[GetCurrentWeapon()].Speed).x;
		this->pLocal->Player->Coords->Pitch = SmoothAngle(src, dst, Vars.Aimbot.Weapon[GetCurrentWeapon()].Speed).y;
	}
	else
	{
		this->pLocal->Room->Yaw = angles.x - this->pLocal->Player->Coords->Rotation;
		this->pLocal->Player->Coords->Pitch = angles.y;
	}

	return true;
}

