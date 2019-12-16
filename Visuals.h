#pragma once

class CVisuals
{
public:
	void Run();
	bool Update();
	bool w2sworking = false;
	Vector3* targetFrom;
	bool IsEnemy(byte id);
	CRoom * Room = nullptr;
	CCamera * Camera = nullptr;
	MVEntity* pLocal = nullptr;
private:
	
	Vector3 EyeAngle;
	Vector3 ReadRotation;
	float FOV[2] = { 0, 0 };
	float ScreenWidth, ScreenHeight;
	Vector3 vRight, vUpward, vForward;
	float Get2dDistance(float x1, float y1, float x2, float y2);
	void UpdateFOV();
	void AngleVectors();
	bool WorldToScreen(Vector3 WorldLocation, Vector3 mypos, D3DXVECTOR2& pos);
	

};
extern CVisuals * cVisuals;