#include "SDK.h"
struct enemy
{
	DWORD EnemyPos;
}enemy[15];
void CVisuals::Run()
{
	if (!this->Update()) return;


}

bool CVisuals::Update()
{
	Offset CLocalPltrOffs[] = { 0x7C, 0x10C, 0 };
	this->Room = *(CRoom**)0x011B5FC0;
	this->pLocal = (MVEntity*)CalcPtr(0x11B5FBC, CLocalPltrOffs, 3);
	
	return this->Room && this->pLocal;
}

float CVisuals::Get2dDistance(float x1, float y1, float x2, float y2)
{
	float Dx = (x1 - x2);
	float Dy = (y1 - y2);
	return sqrt(Dx*Dx + Dy * Dy);
}

void CVisuals::UpdateFOV()
{
	Offset CameraFOVOffs[] = { 0x18 };
	float fov_addy = *(float*)CalcPtr(0x11B9150, CameraFOVOffs, 1);
	this->ScreenWidth = *(float*)0x011A2AB4;
	this->ScreenHeight = *(float*)0x011A2AB8;


	float ASPECT = ((float)this->ScreenWidth) / ((float)this->ScreenHeight);
	float fv0 = (Deg2Rad(fov_addy)) / 2.0f;
	float Wfov = tan(fv0);
	float fv1 = atan(Wfov * ASPECT);
	float Hfov = tan(fv1);
	this->FOV[0] = 1 / Hfov;
	this->FOV[1] = 1 / Wfov;
}

void CVisuals::AngleVectors()
{
	float angle;
	float sr, sp, sy, cr, cp, cy;

	angle = ReadRotation.x;

	angle = 6.28318530718 - angle;

	sy = sin(angle);
	cy = cos(angle);

	angle = ReadRotation.y;
	angle = 6.28318530718 - angle;

	sp = sin(angle);
	cp = cos(angle);

	angle = 0;
	sr = sin(angle);
	cr = cos(angle);

	vForward.x = cp * cy;
	vForward.y = -sp;
	vForward.z = cp * sy;

	vRight.x = (-1 * sr * sp * cy + -1 * cr * -sy);
	vRight.y = -1 * sr * cp;
	vRight.z = (-1 * sr * sp * sy + -1 * cr * cy);

	vUpward.x = (cr * sp * cy + -sr * -sy);
	vUpward.y = cr * cp;
	vUpward.z = (cr * sp * sy + -sr * cy);
}

bool CVisuals::WorldToScreen(Vector3 WorldLocation, Vector3 mypos, D3DXVECTOR2 & pos)
{
	this->AngleVectors();
	this->UpdateFOV();

	Vector3 vLocal, vTransForm;
	vLocal = WorldLocation.VectorDist(mypos);
	vTransForm.x = this->vRight.Dot(vLocal);
	vTransForm.y = this->vUpward.Dot(vLocal);
	vTransForm.z = this->vForward.Dot(vLocal);
	if (vTransForm.z < 0.01f)
	{
		pos.x = -1;
		pos.y = -1;
		return false;
	}
	pos.x = this->ScreenWidth / 2 + ((this->ScreenWidth / 2) / vTransForm.z * (this->FOV[0])) * vTransForm.x;
	pos.y = this->ScreenHeight / 2 - ((this->ScreenHeight / 2) / vTransForm.z * (this->FOV[1])) * vTransForm.y;
	return true;
}

bool CVisuals::IsEnemy(byte id)
{
	if (id > 15) return false;
	TeamID MyTeam = (TeamID)((this->pLocal->Room->Team >> 12) & 7);
	TeamID HisTeam = (TeamID)((this->Room->Players[id]->Room->Team >> 12) & 7);
	return (MyTeam == TEAM_EVERYONE || MyTeam == TEAM_OBS || MyTeam != HisTeam);
}
