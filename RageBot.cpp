#include "Cheat.h"
#include <vector>
#define PI 3.14159265358979323846f
CAntiAim* AntiAim = new CAntiAim;
float m_bestfov = 360.0f;
float m_bestdist = 8192.f;
float m_bestthreat = 0.f;
Vector m_bestpoint = Vector(0, 0, 0);
CBaseEntity* m_bestent = nullptr;
bool m_target = false;
static int ticks = 0;
int ticksMax = 16;
void VectorAngles3(Vector forward, Vector &angles)
{
	float tmp, yaw, pitch;

	if (forward[2] == 0 && forward[0] == 0)
	{
		yaw = 0;

		if (forward[2] > 0)
			pitch = 90;
		else
			pitch = 270;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / PI);

		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / PI);

		if (pitch < 0)
			pitch += 360;
	}

	if (pitch > 180)
		pitch -= 360;
	else if (pitch < -180)
		pitch += 360;

	if (yaw > 180)
		yaw -= 360;
	else if (yaw < -180)
		yaw += 360;

	if (pitch > 89)
		pitch = 89;
	else if (pitch < -89)
		pitch = -89;

	if (yaw > 180)
		yaw = 180;
	else if (yaw < -180)
		yaw = -180;

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

void CRageBot::Run()
{
	DropTarget();

	if (Vars.Ragebot.HvH.AntiAim.Enabled) {
		
		AntiAim->Run();
		G::InAntiAim = true;
		
	}
	else {
		G::InAntiAim = false;
		G::SendPacket = true;
	}


	if (Vars.Ragebot.Hold && !G::PressedKeys[Vars.Ragebot.HoldKey]) {

		return;
	}

	if (!G::LocalPlayer->GetWeapon()->IsGun() || G::LocalPlayer->GetWeapon()->IsEmpty()) {

		return;
	}

	G::Aimbotting = false;

	if (G::BestTarget == -1 && !G::LocalPlayer->GetWeapon()->IsReloading())
		FindTarget();

	if (m_target)
		GoToTarget();

	


}

void CRageBot::FindTarget()
{
	m_bestfov = Vars.Ragebot.FOV;
	m_bestdist = 8192.f;
	m_bestthreat = 0.f;
	m_bestpoint = Vector(0, 0, 0);
	m_target = false;
	m_bestent = nullptr;

	for (int i = 0; i <= I::Globals->maxClients; i++)
	{
		if (!EntityIsValid(i))
			continue;

		CBaseEntity* Entity = I::ClientEntList->GetClientEntity(i);
		Vector hitboxpoint = Entity->GetBonePosition(Vars.Ragebot.Hitbox);
		float fov = M::GetFov(G::UserCmd->viewangles, M::CalcAngle(G::LocalPlayer->GetEyePosition(), hitboxpoint));

		if (Vars.Ragebot.TargetMethod == 1) // Distance
		{
			float dist = M::VectorDistance(G::LocalPlayer->GetOrigin(), Entity->GetOrigin());
			if (dist < m_bestdist && fov < m_bestfov)
			{
				m_bestdist = dist;
				G::BestTarget = i;
				m_bestent = Entity;

				if (Vars.Ragebot.HitScanAll) {
					m_target = BestAimPointAll(Entity, m_bestpoint);
					continue;
				}
				m_target = BestAimPointHitbox(Entity, m_bestpoint);
			}
			continue;
		}

		else if (Vars.Ragebot.TargetMethod == 0)
		{
			if (fov < m_bestfov)
			{
				m_bestfov = fov;
				G::BestTarget = i;
				m_bestent = Entity;
				if (Vars.Ragebot.HitScanAll && BestAimPointAll(Entity, m_bestpoint))
				{
					m_target = true;
					continue;
				}
				if (BestAimPointHitbox(Entity, m_bestpoint))
					m_target = true;
			}
			continue;
		}

		else if (Vars.Ragebot.TargetMethod == 2)
		{
			float dist = M::VectorDistance(G::LocalPlayer->GetOrigin(), Entity->GetOrigin());
			float health = (float)Entity->GetHealth();

			float threat = health / dist;

			if (Entity->IsTargetingLocal())
				threat += 100;

			if (threat > m_bestthreat && fov < m_bestfov)
			{
				m_bestthreat = threat;
				G::BestTarget = i;
				m_bestent = Entity;
				if (Vars.Ragebot.HitScanAll && BestAimPointAll(Entity, m_bestpoint))
				{
					m_target = true;
					continue;
				}
				BestAimPointHitbox(Entity, m_bestpoint);
				m_target = true;
			}
		}
	}
	return;
}

void CRageBot::GoToTarget()
{
	bool auto_shoot = false;
	bool can_shoot = true;
	bool reloading = false;

	if (G::LocalPlayer->GetWeapon()->GetNextPrimaryAttack() - (G::LocalPlayer->GetTickBase() * I::Globals->interval_per_tick) > 0)
		can_shoot = false;

	m_bestpoint = m_bestent->GetPredicted(m_bestpoint);

	QAngle aim_angle = M::CalcAngle(G::LocalPlayer->GetEyePosition(), m_bestpoint);

	aim_angle -= G::LocalPlayer->GetPunch() * 2.f;

	CFixMove fixMove;
	if (Vars.Ragebot.Silent)
		fixMove.Start();

	if (can_shoot) {
		if (Vars.Ragebot.Aimstep)
		{
			G::Aimbotting = true;

			QAngle angNextAngle;
			bool bFinished = Aimstep(G::LastAngle, aim_angle, angNextAngle, Vars.Ragebot.AimstepAmount);
			G::UserCmd->viewangles = angNextAngle;
			G::LastAngle = angNextAngle;
			if (bFinished) auto_shoot = true;
		}
		else
		{
			G::Aimbotting = true;
			G::UserCmd->viewangles = aim_angle;
			auto_shoot = true;
		}
	}

	if (Vars.Ragebot.Silent)
		fixMove.End();

	if (Vars.Ragebot.AutoStop)
	{
		G::UserCmd->forwardmove = 0;
		G::UserCmd->sidemove = 0;
		G::UserCmd->upmove = 0;
		G::UserCmd->buttons = 0;
		
	}

	//if (Vars.Ragebot.AutoCrouch)
		//G::UserCmd->buttons |= IN_DUCK;

	float hitchance = 75.f + (Vars.Ragebot.HitChanceAmt / 4);

	if (auto_shoot && can_shoot && Vars.Ragebot.AutoFire && (!Vars.Ragebot.HitChance || (1.0f - G::LocalPlayer->GetWeapon()->GetAccuracyPenalty()) * 100.f >= hitchance))
		G::UserCmd->buttons |= IN_ATTACK;
}

void CRageBot::DropTarget()
{
	if (!EntityIsValid(G::BestTarget) || Vars.Ragebot.HitScanAll || Vars.Ragebot.HitScanHitbox)
		G::BestTarget = -1;
}

bool CRageBot::Aimstep(QAngle src, QAngle dst, QAngle& out, int steps)
{
	QAngle delta_angle = (dst - src).Normalized();;
	bool x_finished = false;
	bool y_finished = false;

	if (delta_angle.x > steps)
		src.x += steps;

	else if (delta_angle.x < -steps)
		src.x -= steps;

	else
	{
		x_finished = true;
		src.x = dst.x;
	}

	if (delta_angle.y > steps)
		src.y += steps;

	else if (delta_angle.y < -steps)
		src.y -= steps;

	else
	{
		y_finished = true;
		src.y = dst.y;
	}
	src.Normalized();
	out = src;
	return x_finished && y_finished;
}

bool CRageBot::GetHitbox(CBaseEntity* target, Hitbox* hitbox)
{
	matrix3x4a_t matrix[MAXSTUDIOBONES];

	if (!target->SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0))
		return false;

	studiohdr_t *hdr = I::ModelInfo->GetStudioModel(target->GetModel());

	if (!hdr) {

		return false;
	}
	mstudiohitboxset_t *hitboxSet = hdr->pHitboxSet(target->GetHitboxSet());
	mstudiobbox_t *untransformedBox = hitboxSet->pHitbox(hitbox->hitbox);

	Vector bbmin = untransformedBox->bbmin;
	Vector bbmax = untransformedBox->bbmax;
	if (untransformedBox->radius != -1.f)
	{
		bbmin -= Vector(untransformedBox->radius*.8, untransformedBox->radius*.8, untransformedBox->radius*.8);
		bbmax += Vector(untransformedBox->radius*.8, untransformedBox->radius*.8, untransformedBox->radius*.8);
	}

	Vector points[] = { ((bbmin + bbmax) * .5f),
		Vector(bbmin.x, bbmin.y, bbmin.z),
		Vector(bbmin.x, bbmax.y, bbmin.z),
		Vector(bbmax.x, bbmax.y, bbmin.z),
		Vector(bbmax.x, bbmin.y, bbmin.z),
		Vector(bbmax.x, bbmax.y, bbmax.z),
		Vector(bbmin.x, bbmax.y, bbmax.z),
		Vector(bbmin.x, bbmin.y, bbmax.z),
		Vector(bbmax.x, bbmin.y, bbmax.z) };

	for (int index = 1; index < 9; index++) // Skip first point (center of hitbox)
	{
		M::VectorTransform(points[index], matrix[untransformedBox->bone], hitbox->points[index]);
	}
	return true;
}

bool CRageBot::GetBestPoint(CBaseEntity* target, Hitbox* hitbox, BestPoint *point)
{
	/*
	if( hitbox->hitbox == HITBOX_HEAD )
	{
	Vector high = ( ( hitbox->points[ 3 ] + hitbox->points[ 5 ] ) * .5f );
	float pitch = target->GetEyeAngles().x;
	if( ( pitch > 0.f ) && ( pitch <= 89.f ) )
	{
	Vector height		= ( ( ( high - hitbox->points[ 0 ] ) / 3 ) * 4 );
	Vector new_height	= ( hitbox->points[ 0 ] + ( height * ( pitch / 89.f ) ) );
	hitbox->points[ 0 ] = new_height;
	point->flags |= FL_HIGH;
	}
	else if( ( pitch < 292.5f ) && ( pitch >= 271.f ) )
	{
	hitbox->points[ 0 ] -= Vector( 0.f, 0.f, 1.f );
	point->flags |= FL_LOW;
	}
	}
	*/
	for (int index = 0; index <= 8; ++index)
	{
		int temp_damage = AutoWall->GetDamage(hitbox->points[index]);
		if ((point->dmg < temp_damage)) // update best point across all if a higher damage point is found
		{
			point->dmg = temp_damage;
			point->point = hitbox->points[index];
			point->index = index;
		}
	}
	return (point->dmg > Vars.Ragebot.AutoWallDmg);
}

int hitboxes[8] =
{
	HITBOX_PELVIS,
	HITBOX_CHEST,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_CALF,
	HITBOX_LEFT_CALF,
	HITBOX_HEAD
};


bool CRageBot::BestAimPointAll(CBaseEntity* target, Vector& best_point)
{
	int highest_damage = 0;
	for (int i = 0; i < 8; i++)
	{
		BestPoint aim_point;
		Hitbox hitbox(hitboxes[i]);
		GetHitbox(target, &hitbox);

		if (!GetBestPoint(target, &hitbox, &aim_point)) {
			continue;
		}

		if (aim_point.dmg > highest_damage) {
			highest_damage = aim_point.dmg;
			best_point = aim_point.point;
		}
	}
	return(highest_damage > Vars.Ragebot.AutoWallDmg);
}

bool CRageBot::BestAimPointHitbox(CBaseEntity* target, Vector& best_point)
{
	BestPoint aim_point;

	int hitboxnum = 0;

	switch (Vars.Ragebot.Hitbox) {
	case 0:
		hitboxnum = 3;
		break;
	case 1:
		hitboxnum = 6;
		break;
	case 2:
		break;
	case 3:
		hitboxnum = 4;
		break;
	case 4:
		hitboxnum = 7;
		break;
	case 5:
		hitboxnum = 1;
		break;
	case 6:
		hitboxnum = 0;
		break;
	default:
		hitboxnum = 0;
		break;
	}

	Hitbox target_hitbox(hitboxnum);
	GetHitbox(target, &target_hitbox);

	if (!GetBestPoint(target, &target_hitbox, &aim_point))
		return false;

	if (aim_point.dmg > Vars.Ragebot.AutoWallDmg) {
		best_point = aim_point.point;
		return true;
	}
	return false;
}

bool CRageBot::EntityIsValid(int i)
{
	CBaseEntity* Entity = I::ClientEntList->GetClientEntity(i);

	if (!Entity)
	{

		return false;
	}
	if (!Vars.Ragebot.FriendlyFire && !Entity->IsEnemy())
	{
		return false;
	}
	if (Entity == G::LocalPlayer)
	{

		return false;
	}

	if (Entity->GetDormant())
	{

		return false;
	}
	if (Entity->GetImmune())
	{

		return false;
	}
	if (Entity->GetHealth() <= 0)
	{

		return false;
	}


	return true;
}

void CRageBot::CFixMove::Start()
{
	m_oldangle = G::UserCmd->viewangles;
	m_oldforward = G::UserCmd->forwardmove;
	m_oldsidemove = G::UserCmd->sidemove;
}

void CRageBot::CFixMove::End()
{
	float yaw_delta = G::UserCmd->viewangles.y - m_oldangle.y;
	float f1;
	float f2;

	if (m_oldangle.y < 0.f)
		f1 = 360.0f + m_oldangle.y;
	else
		f1 = m_oldangle.y;

	if (G::UserCmd->viewangles.y < 0.0f)
		f2 = 360.0f + G::UserCmd->viewangles.y;
	else
		f2 = G::UserCmd->viewangles.y;

	if (f2 < f1)
		yaw_delta = abs(f2 - f1);
	else
		yaw_delta = 360.0f - abs(f1 - f2);
	yaw_delta = 360.0f - yaw_delta;

	G::UserCmd->forwardmove = cos(DEG2RAD(yaw_delta)) * m_oldforward + cos(DEG2RAD(yaw_delta + 90.f)) * m_oldsidemove;
	G::UserCmd->sidemove = sin(DEG2RAD(yaw_delta)) * m_oldforward + sin(DEG2RAD(yaw_delta + 90.f)) * m_oldsidemove;
}
void CRageBot::FakeLag_CreateMove(CUserCmd* cmd)
{
	if (!Vars.Ragebot.FakeLag.Enabled)
		return;

	CBaseEntity* localplayer = (CBaseEntity*)I::ClientEntList->GetClientEntity(I::Engine->GetLocalPlayer());
	if (!localplayer || !localplayer->GetAlive())
		return;

	if (localplayer->GetFlags() & FL_ONGROUND && Vars.Ragebot.FakeLag.adaptive)
		return;

	if (cmd->buttons & IN_ATTACK)
	{
		G::SendPacket = true;
		return;
	}

	if (ticks >= ticksMax)
	{
		G::SendPacket = true;
		ticks = 0;
	}
	else
	{
		if (Vars.Ragebot.FakeLag.adaptive)
		{
			int packetsToChoke;
			if (localplayer->GetVelocity().Length() > 0.f)
			{
				packetsToChoke = (int)((64.f / I::Globals->interval_per_tick) / localplayer->GetVelocity().Length()) + 1;
				if (packetsToChoke >= 15)
					packetsToChoke = 14;
				if (packetsToChoke < Vars.Ragebot.FakeLag.value)
					packetsToChoke = Vars.Ragebot.FakeLag.value;
			}
			else
				packetsToChoke = 0;

			G::SendPacket = ticks < 16 - packetsToChoke;
		}
		else
			G::SendPacket = ticks < 16 - Vars.Ragebot.FakeLag.value;
	}

	ticks++;
}
void CRageBot::FakeLag()
{
	if (Vars.Ragebot.FakeLag.Choke == 0 || Vars.Ragebot.FakeLag.Send == 0)
		return;

	int iFakeLagTickLimit = Vars.Ragebot.FakeLag.Choke + Vars.Ragebot.FakeLag.Send;

	if (iFakeLag > iFakeLagTickLimit) iFakeLag = 1;
	else iFakeLag = iFakeLag + 1;

	if (Vars.Ragebot.FakeLag.Send >= iFakeLag) G::SendPacket = TRUE;
	else G::SendPacket = FALSE;
}

void CRageBot::EdgeJump(CBaseEntity* pLocal, CUserCmd* pUserCmd)
{
	//if ( ( m_MoveType( pLocal ) == MOVETYPE_LADDER ) )
	//    return;

	if (!(pLocal->GetFlags() & FL_ONGROUND))
		return;

	Vector Start, End;
	Start = pLocal->GetOrigin();

	VectorCopy(pLocal->GetOrigin(), Start);
	Start = pLocal->GetOrigin();
	VectorCopy(Start, End);
	constexpr int ticks = 1;
	End = Start + pLocal->GetVelocity() * I::Globals->interval_per_tick * ticks;

	End.z -= 64;

	Ray_t ray;
	ray.Init(Start, End);

	trace_t trace;

	CTraceFilter filter;
	filter.pSkip = (CBaseEntity*)I::ClientEntList->GetClientEntity(I::Engine->GetLocalPlayer());
	

	I::EngineTrace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, &filter, &trace);
	if (trace.fraction == 1.0f)
	{
		pUserCmd->buttons |= IN_JUMP;
	}
}
void CRageBot::EdgeStop(CBaseEntity* pLocal, CUserCmd* pUserCmd)
{
	//if ( ( m_MoveType( pLocal ) == MOVETYPE_LADDER ) )
	//    return;

	if (!(pLocal->GetFlags() & FL_ONGROUND))
		return;

	Vector Start, End;
	Start = pLocal->GetOrigin();

	VectorCopy(pLocal->GetOrigin(), Start);
	Start = pLocal->GetOrigin();
	VectorCopy(Start, End);
	constexpr int ticks = 1;
	End = Start + pLocal->GetVelocity() * I::Globals->interval_per_tick * ticks;

	End.z -= 64;

	Ray_t ray;
	ray.Init(Start, End);

	trace_t trace;

	CTraceFilter filter;
	filter.pSkip = (CBaseEntity*)I::ClientEntList->GetClientEntity(I::Engine->GetLocalPlayer());

	I::EngineTrace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, &filter, &trace);
	if (trace.fraction == 2.0f)
	{
		pUserCmd->forwardmove = 0;
		pUserCmd->sidemove = 0;
	}
}

void CRageBot::FakeStand(CUserCmd* m_pCmd, bool& bSendPacket)
{
	if (GetAsyncKeyState(VK_CONTROL))
	{
		static bool bDuck = false;

		if (bDuck)
		{
			bDuck = false;
			m_pCmd->buttons &= ~IN_DUCK;
			bSendPacket = true;
		}
		else
		{
			if (m_pCmd->buttons & IN_DUCK)
			{
				bSendPacket = false;
				bDuck = true;
				static bool bFakeCrouch = true;
				bFakeCrouch = !bFakeCrouch;
			}
		}
	}
}

void CRageBot::FakeDuck(CUserCmd* m_pCmd, bool& bSendPacket)
{
	if (GetAsyncKeyState(VK_CONTROL))
	{
		static bool bDuck = false;

		if (bDuck)
		{
			bDuck = false;
			m_pCmd->buttons &= ~IN_DUCK;
			bSendPacket = false;
		}
		else
		{
			if (m_pCmd->buttons & IN_DUCK)
			{
				bSendPacket = true;
				bDuck = true;
				static bool bFakeCrouch = true;
				bFakeCrouch = !bFakeCrouch;
			}
		}
	}
}

void CRageBot::AutoCrouch(CBaseEntity* player, CUserCmd* cmd)
{
	if (!Vars.Ragebot.AutoCrouch)
		return;

	if (!player)
		return;

	cmd->buttons |= IN_DUCK;
}

void CRageBot::AutoSlow(CBaseEntity* player, float& forward, float& sideMove, float& bestDamage, CBaseCombatWeapon* active_weapon, CUserCmd* cmd)
{
	if (!Vars.Ragebot.AutoWall)
		return;

	if (!Vars.Ragebot.AutoSlow)
		return;

	if (!player)
		return;

	float nextPrimaryAttack = active_weapon->GetNextPrimaryAttack();

	if (nextPrimaryAttack > I::Globals->curtime)
		return;

	if (bestDamage > Vars.Ragebot.AutoSlowDmg)
	{
		forward *= 0.2f;
		sideMove *= 0.16f;
		cmd->upmove = 0;
	}
}
