#pragma once

#ifndef AIM_H
#define AIM_H

#include <Windows.h>
#include <math.h>

#define MAX_PLAYER_COUNT 16
float PI = 3.14159265F;
float RAD = 57.29578F;
struct POS_3D
{
	double x;
	double z;
	double y;
};

struct PlayerInfo
{
	bool GetPlayerInfo(UINT_PTR PlayerBase);

	bool IsAlive;
	POS_3D * Pos;
	bool IsEnemy;
};

bool Aimbot();

#endif

void CalcAngle(POS_3D * src, POS_3D * dst, float & pitch, float & yaw)
{
	double delta[3] = { (src->x - dst->x), (src->y - dst->y), (src->z - dst->z) };
	double hyp = sqrt(delta[0] * delta[0] + delta[1] * delta[1]);
	pitch = (float)(asinf(delta[2] / hyp) ); //pitch
	yaw = (float)(((atanf(delta[1] / delta[0]) * -1) - PI)); //yaw
	if (delta[0] >= 0.0) 
	{ 
		yaw += PI; 
	}
}