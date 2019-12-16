#include "hacks.h";
#include <sstream>
#include "Drawing.h"

#include<process.h>
#include"Sprites.h"
#include"Images.h"
#include<vector>
#include<string>


#include"Input.h"


bool IsMouseInRegion(int x1, int y1, int x2, int y2)
{
	POINT cPos;
	GetCursorPos(&cPos);

	if (cPos.x > x1 && cPos.x <x1 + x2 && cPos.y > y1 && cPos.y < y1 + y2)
		return true;

	return false;
}

#include<time.h>

char* FPS(int en)
{
	static int	 FPScounter = 0;
	static float FPSfLastTickCount = 0.0f;
	static float FPSfCurrentTickCount;
	static char  cfps[6] = "";

	if (!en)
	{
		FPSfCurrentTickCount = clock() * 0.001f;
		FPScounter++;

		if ((FPSfCurrentTickCount - FPSfLastTickCount) > 1.0f)
		{
			FPSfLastTickCount = FPSfCurrentTickCount;
			sprintf(cfps, "%d FPS", FPScounter);
			FPScounter = 0;
		}
	}
	return cfps;
}


Sprites* sprites;
Drawing* drawer;

LPD3DXMESH mesh;


struct _VERTEX
{
	D3DXVECTOR3 pos;     // vertex position
	D3DXVECTOR3 norm;    // vertex normal
	float tu;            // texture coordinates
	float tv;
} VERTEX, *LPVERTEX;
#define FVF_VERTEX    D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1
LPD3DXMESH CreateMappedSphere(LPDIRECT3DDEVICE9 pDev, float fRad, UINT slices, UINT stacks)
{
	// create the sphere
	LPD3DXMESH mesh;
	if (FAILED(D3DXCreateSphere(pDev, fRad, slices, stacks, &mesh, NULL)))
		return NULL;

	// create a copy of the mesh with texture coordinates,
	// since the D3DX function doesn't include them
	LPD3DXMESH texMesh;
	if (FAILED(mesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, FVF_VERTEX, pDev, &texMesh)))
		// failed, return un-textured mesh
		return mesh;

	// finished with the original mesh, release it
	mesh->Release();

	// lock the vertex buffer
	_VERTEX* pVerts;
	if (SUCCEEDED(texMesh->LockVertexBuffer(0, (LPVOID*)&pVerts)))
	{

		// get vertex count
		int numVerts = texMesh->GetNumVertices();

		// loop through the vertices
		for (int i = 0; i < numVerts; i++)
		{

			// calculate texture coordinates
			pVerts->tu = asinf(pVerts->norm.x) / D3DX_PI + 0.5f;
			pVerts->tv = asinf(pVerts->norm.y) / D3DX_PI + 0.5f;

			// go to next vertex
			pVerts++;
		}

		// unlock the vertex buffer
		texMesh->UnlockVertexBuffer();
	}

	// return pointer to caller
	return texMesh;
}

bool esp = true;

void Hacks::InitializeMenuItems(IDirect3DDevice9 *d3dDevice, D3DVIEWPORT9 view)
{
	std::function<void(byte*)> var = std::bind(&Hacks::KeyboardInput, this, std::placeholders::_1);
	Input::Listen(var);
	Input::Begin();


	Input::screenWidth = view.Width;
	Input::screenHeight = view.Height;

	sprites = new Sprites(d3dDevice);
	sprites->AddMemImage(Menu, "Menu", 250, 40);
	sprites->AddMemImage(Close, "x1", 18, 18);
	sprites->AddMemImage(Multy, "Multy", 800, 800);
	sprites->AddMemImage(Normal, "Normal", 800, 800);
	sprites->AddMemImage(Troll, "lol", 800, 800);

	drawer = new Drawing(d3dDevice);
	drawer->LineSettings(false, 1);
	drawer->AddFont("MS Reference Sans Serif", 16, true, false);
	drawer->AddFont("Comic Sans MS", 26, true, false);
	drawer->AddFont("Visitor TT2", 12, false, false);
	drawer->AddFont("Fixedsys", 24, false, false);
	drawer->AddFont("Wingdings", 20, false, false);

	mesh = CreateMappedSphere(d3dDevice, 10, 30, 30);

	D3DXCreateBox(d3dDevice, 30, 30, 30, &mesh, 0);

	Scene = physics->getScene(0);
}

DWORD red = D3DCOLOR_RGBA(240, 20, 100, 140);
DWORD linecolor = D3DCOLOR_RGBA(0, 200, 0, 155);
DWORD white = D3DCOLOR_RGBA(255, 255, 255, 140);


class Body_Part
{
	byte _empty[136];
	NxVec3 position;
public:
	NxVec3* GetTransform(int index)
	{
		return (NxVec3*)(((DWORD*)&this->position) + (0x37 * index));
	}
};


struct Body
{
	int _empty[3];//c
	Body_Part* Head;

	Body_Part* Midle;

	Body_Part* Right_Arm;
	Body_Part* idk1;
	Body_Part* Left_Arm;

	Body_Part* idk2;
	Body_Part* Right_Foot;
	Body_Part* idk3;
	Body_Part* Left_Foot;

	byte idk4[116];
	NxVec3 writeRotation;

	/*Body_Part* Right_Foot;
	Body_Part* Right_Foot_Down;
	Body_Part* Left_Foot;
	Body_Part* Left_Foot_Down;*/
};

struct Information
{
	unsigned char __Idk;//Example 4
	unsigned char Team;//5
	unsigned char __idk2;//6
	unsigned char __idk3;//7

	unsigned char State;//8
	unsigned char __Idk4[3];//9
	int Health;//c
	int __Idk7;//10
	int HealthKey;//14
};

struct Info
{
	char _empty[5];
	unsigned char Team;
	unsigned char _empty2[2];
	unsigned char State;
	unsigned char _empty3[3];
	int Health;//c
	char _empty4[4];
	int HealthKey;//14
};


struct Informations
{
	int _empty;
	Info* info;
	char _empty2[92];
	char Name;
	char _empty4[63];
};

struct Player
{
	byte _empty[0x10c];
	Informations* infos;
	byte _empty2[0x14];
	Body* body;
};

class Character
{
	Player* player;


	bool getBit(unsigned char byte, int position)
	{
		return (byte >> position) & 0x1;
	}

public:

	Character(Player* pointer, NxShape* shap)
	{
		player = pointer;
		shape = shap;
		infos = player->infos;
		body = player->body;
		Name = &player->infos->Name;
		Team = getBit(player->infos->info->Team, 4);
		Health = (player->infos->info->Health) ^ (player->infos->info->HealthKey);
		IsAlive = (!getBit(player->infos->info->State, 0)) && getBit(player->infos->info->State, 2) && !getBit(player->infos->info->State, 1) && getBit(player->infos->info->State, 3);
	}

	NxShape* shape;
	void UpdateVisibility(NxScene* Scene, NxVec3 camera)
	{
		NxVec3 dir = *body->Midle->GetTransform(2) - camera;
		dir.normalize();
		NxRay ray;
		NxRaycastHit hit;
		ray.orig = camera;
		ray.dir = dir;

		NxShape* out_hape = Scene->raycastClosestShape(ray, NxShapesType::NX_ALL_SHAPES, hit);

		if (out_hape != shape)
			IsVisible = false;
		else
			IsVisible = true;
	}

	bool IsAlive = false;
	bool IsVisible = false;
	bool Team;
	int Health;
	Body* body;
	Informations* infos;
	char* Name;
};

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

		return sqrt(Dx*Dx + Dy*Dy + Dz*Dz);
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

	void operator=(Position3 a)
	{
		x = a.X;
		y = a.Y;
		z = a.Z;
	}

};

class Entity
{

	bool getBit(unsigned char byte, int position)
	{
		return (byte >> position) & 0x1;
	}

	float ex = 0;
	int cnt = 0;

public:

	bool IsVisible = false;
	bool Team = false;
	bool Alive = false;

	int Hp = 0;

	Position3* position;
	Position3* down_position;
	Information* info;
	char* Name;

	void Update(DWORD Positionadress, DWORD InfoAdress, DWORD NameAddres, DWORD DownPositionAddres)
	{

		Name = (char*)NameAddres;
		position = (Position3*)Positionadress;
		down_position = (Position3*)DownPositionAddres;
		info = (Information*)InfoAdress;

		Team = getBit(info->Team, 4);
		Hp = ((info->Health) ^ (info->HealthKey));

		Alive = true;
		Alive = (!getBit(info->State, 0)) && getBit(info->State, 2) && !getBit(info->State, 1) && getBit(info->State, 3);

		if (Hp < 1)
			Alive = false;
		else
			if (strlen(Name) < 3)
				Alive = false;

	}

	int sbit(int bitfield, int n, int value)
	{
		if (value)
			bitfield |= 1 << n;
		else
			bitfield &= ~0 ^ (1 << n);

		return bitfield;
	}

	void Respawn()
	{

		if (!IsBadWritePtr((void*)info, sizeof(Information)))
			info->State = sbit(info->State, 0, 1);
	}
};


void Hacks::BeginScene()
{
	Input::Update();
	switch (Input::mse.pin)
	{
	case 1://left click
		break;
	case 2://right click
		break;
	case 3://middle
		break;
	}

}




void Hacks::DrawMenu(IDirect3DDevice9 *d3dDevice)
{
	if (!esp)
		return;
	int actors_count = Scene->getNbActors();

	//Scene->setGroupCollisionFlag(0, 1, false);
	//Scene->setGroupCollisionFlag(1, 0, false);
	//Scene->setGravity(NxVec3(0, 1, 0));

	std::vector<Character> chars;

	NxShape* Shape_me;
	NxActor** Actors = Scene->getActors();

	bool foundMe = false;
	for (size_t i = 0; i < actors_count; i++)
	{
		NxActor* actor = *(Actors + i);
		int ShapeNumber = actor->getNbShapes();
		NxShape*const* shapes = actor->getShapes();

		for (size_t a = 0; a < ShapeNumber; a++)
		{
			NxShape* shape = *(shapes + a);

			if (actor->getGroup() == 0)
				Shape_me = shape;

			if (shape->userData != NULL && actor->getName() == NULL)
			{
				DWORD* addres = (DWORD*)shape->userData;
				Player* player = (Player*)addres;
				Character character(player, shape);
				if (actor->getGroup() != 0)
				{
					chars.push_back(character);
				}
				else
				{
					foundMe = true;
					chars.insert(chars.begin(), character);
				}
			}
		}
	}

	int size = chars.size();

	if (!foundMe || size < 2)
		return;

	for (int i = 1; i < size; ++i)
	{
		for (int a = 1; a < size; ++a)
			if (a != i)
				chars[a].shape->setFlag(NX_SF_DISABLE_RAYCASTING, true);

		chars[i].UpdateVisibility(Scene, CameraPos);

		for (int a = 1; a < size; ++a)
			if (a != i)
				chars[a].shape->setFlag(NX_SF_DISABLE_RAYCASTING, false);
	}

	D3DXVECTOR2 points[9];

	for (int i = 1; i < size; ++i)
	{


		if (chars[i].Team == chars[0].Team || !chars[i].IsAlive)
			continue;
		NxVec3* char_pos = chars[i].body->Midle->GetTransform(7);

		D3DXVECTOR2 ScreenCoord;

		if (To2D(&ScreenCoord, char_pos))
		{
			drawer->DrawRect(ScreenCoord.x - 50, ScreenCoord.y, 100, 36, red);
			drawer->Text<int>(chars[i].Health, ScreenCoord.x, ScreenCoord.y, 1, 0, false, white, white);
			drawer->Text<char*>(chars[i].Name, ScreenCoord.x, ScreenCoord.y + 20, 1, 0, false, white, white);
		}

		if (!chars[i].IsVisible)
		{

			for (int a = 0; a < 8; ++a)
				To2D(&points[a], chars[i].body->Midle->GetTransform(a + 1));

			drawer->DrawLine(points, 8, white, white);

			for (int a = 0; a < 3; ++a)
				To2D(&points[a], chars[i].body->Left_Arm->GetTransform(a));
			drawer->DrawLine(points, 3, white, white);

			for (int a = 0; a < 3; ++a)
				To2D(&points[a], chars[i].body->Right_Arm->GetTransform(a));
			drawer->DrawLine(points, 3, white, white);

			for (int a = 0; a < 3; ++a)
				To2D(&points[a], chars[i].body->Right_Foot->GetTransform(a));
			drawer->DrawLine(points, 3, white, white);


			for (int a = 0; a < 3; ++a)
				To2D(&points[a], chars[i].body->Left_Foot->GetTransform(a));
			drawer->DrawLine(points, 3, white, white);

		}

	}


	NxRay* ray = new NxRay(CameraPos, CameraForward);
	NxRaycastHit hit;


	Shape_me->setFlag(NX_SF_DISABLE_RAYCASTING, true);
	NxShape* shape = Scene->raycastClosestShape(*ray, NxShapesType::NX_ALL_SHAPES, hit);
	Shape_me->setFlag(NX_SF_DISABLE_RAYCASTING, false);

	if (shape != NULL)
	{
		//LPDIRECT3DBASETEXTURE9 texture = sprites->ReturnTexture("lol");
		D3DXMATRIX matTranslate;
		D3DXMatrixTranslation(&matTranslate, hit.worldImpact.x, hit.worldImpact.y, hit.worldImpact.z);

		d3dDevice->SetTransform(D3DTS_WORLD, &(matTranslate));

		d3dDevice->SetPixelShader(NULL);
		d3dDevice->SetVertexShader(NULL);
		d3dDevice->SetRenderTarget(0, NULL);
		d3dDevice->SetDepthStencilSurface(NULL);
		d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		d3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		d3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		d3dDevice->SetRenderState(D3DRS_ZENABLE, true);
		d3dDevice->SetFVF(FVF_VERTEX);
		d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		//d3dDevice->SetTexture(0, texture);
		mesh->DrawSubset(0);
	}


	//sprites->EndSprites();
}

void Hacks::CreateTexture(IDirect3DDevice9 *d3dDevice, LPCVOID colour, UINT ColourSize, LPDIRECT3DTEXTURE9 *texture)
{
	D3DXCreateTextureFromFileInMemory(d3dDevice, colour, ColourSize, texture);
}

#define PlayerStride m_Stride == 48 || m_Stride == 72  || m_Stride == 56
#define ItemStride (m_Stride == 32 && NumVertices > 80 && NumVertices != 149)


bool PlayerWire = true;
bool ItemWire = true;


void Hacks::ApplyWallhack(IDirect3DDevice9 *d3dDevice, D3DPRIMITIVETYPE device, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	d3dDevice->DrawIndexedPrimitive(device, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);

	if (PlayerWire && PlayerStride)
	{
	d3dDevice->SetRenderState(D3DRS_ZENABLE, false);
	d3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	d3dDevice->DrawIndexedPrimitive(device, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	d3dDevice->SetRenderState(D3DRS_ZENABLE, true);
	d3dDevice->SetRenderState(D3DRS_FILLMODE, 3);
	d3dDevice->DrawIndexedPrimitive(device, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	}

	if (ItemWire &&  ItemStride)
	{
	d3dDevice->SetRenderState(D3DRS_ZENABLE, false);
	d3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	d3dDevice->DrawIndexedPrimitive(device, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	d3dDevice->SetRenderState(D3DRS_ZENABLE, true);
	d3dDevice->SetRenderState(D3DRS_FILLMODE, 3);
	d3dDevice->DrawIndexedPrimitive(device, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	}

}


void Hacks::KeyboardInput(byte KeyState[])
{

	if (KeyState[DIK_F1] & 0x80)
	{
		esp = !esp;
		Beep(1000, 100);
	}

	//dik_
}

