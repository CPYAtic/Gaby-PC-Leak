#define text_left 0
#define text_center 1
#define text_right 2

struct tvertex
{
	float x;
	float y;
	float z;
	float fRHW;
	D3DCOLOR Color;
};

class CDrawing
{

	LPDIRECT3DDEVICE9 Device;
	
	D3DXVECTOR2* Rect;

	/*Saves*/
	DWORD state1, state2, state3, state4;
	IDirect3DPixelShader9* shader;

	tvertex Vtex[360 * 4];

	

	int FontNr = 0;
	

public:
	//CDrawing(LPDIRECT3DDEVICE9);
	//~CDrawing();
	ID3DXFont * pFont[10];
	ID3DXLine * Line = NULL;
	LPD3DXFONT g_pFont = NULL;
	void ReleaseObjects();
	bool CreateObjects();
	void UpdateInfo(LPDIRECT3DDEVICE9 device);
	void DrawLine(D3DXVECTOR2* points, int count, D3DCOLOR color, int Width = -1);
	void LineSettings(bool, float);
	void DrawRect(float, float, float, float, D3DCOLOR);
	void DrawRectOutlined(float, float, float, float, float, D3DCOLOR, D3DCOLOR);
	void DrawUnfilledCircle(float, float, float, float, float, D3DCOLOR);
	void DrawFilledCircle(float x, float y, float radius, float rez, D3DCOLOR color);
	void DrawCrosshair(LPDIRECT3DDEVICE9 pDevice, int size, int strong, D3DCOLOR xcolor);
	void FillRGB(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* pDevice);
	void DrawCircle(int X, int Y, float radiusx, float radiusy, int numSides, DWORD Color);
	void DrawPoint(int x, int y, int w, int h, DWORD color, IDirect3DDevice9* pDevice);
	void DrawLine2(float x, float y, float x2, float y2, float width, DWORD color);
	void DrawShadowText(int x, int y, D3DCOLOR color, char* String);
	void DrawFilledRect(IDirect3DDevice9* pDevice, int x, int y, int w, int h, DWORD color);
	void Draw_Text(LPCSTR TextToDraw, int x, int y, D3DCOLOR Colour, LPD3DXFONT m_font);
	void DrawBorderBox(int x, int y, int w, int h, int thickness, D3DCOLOR Colour, IDirect3DDevice9 *d3dDevice);
	void Draw2DBox(int x, int y, int w, int h, D3DCOLOR Color);
	void DrawTexture(int x, int y, LPDIRECT3DTEXTURE9 dTexture, LPD3DXSPRITE sprite);
	void DrawImage(LPDIRECT3DDEVICE9 pDevice, int x, int y, LPDIRECT3DTEXTURE9 dTexture, LPD3DXSPRITE sprite, BYTE MyImage);
	void DrawPoint(IDirect3DDevice9* Device, int baseX, int baseY, int baseW, int baseH, D3DCOLOR Cor);

	void AddFont(const char *Caption, float size, bool bold, bool italic)
	{
		D3DXCreateFont(Device,
			size,
			0,
			(bold) ? FW_BOLD : FW_NORMAL,
			1,
			(italic) ? 1 : 0,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			ANTIALIASED_QUALITY,
			DEFAULT_PITCH,
			Caption,
			&pFont[FontNr++]);
	}

	void Text(char *text, float x, float y, int orientation, int font, bool bordered, DWORD color, DWORD bcolor)
	{
		RECT rect;

		switch (orientation)
		{
		case 0:
			if (bordered)
			{
				SetRect(&rect, x - 1, y, x - 1, y);
				pFont[font - 1]->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
				SetRect(&rect, x + 1, y, x + 1, y);
				pFont[font - 1]->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
				SetRect(&rect, x, y - 1, x, y - 1);
				pFont[font - 1]->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
				SetRect(&rect, x, y + 1, x, y + 1);
				pFont[font - 1]->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
			}
			SetRect(&rect, x, y, x, y);
			pFont[font - 1]->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, color);
			break;
		case 1:
			if (bordered)
			{
				SetRect(&rect, x - 1, y, x - 1, y);
				pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
				SetRect(&rect, x + 1, y, x + 1, y);
				pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
				SetRect(&rect, x, y - 1, x, y - 1);
				pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
				SetRect(&rect, x, y + 1, x, y + 1);
				pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
			}
			SetRect(&rect, x, y, x, y);
			pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
			break;
		case 2:
			if (bordered)
			{
				SetRect(&rect, x - 1, y, x - 1, y);
				pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
				SetRect(&rect, x + 1, y, x + 1, y);
				pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
				SetRect(&rect, x, y - 1, x, y - 1);
				pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
				SetRect(&rect, x, y + 1, x, y + 1);
				pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
			}
			SetRect(&rect, x, y, x, y);
			pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, color);
			break;
		}
	}

	template <typename t>	void Text(t te, float x, float y, int orientation, int font, bool bordered, DWORD color, DWORD bcolor)
	{

		std::stringstream ss;//create a stringstream
		ss << te;
		std::string str = ss.str();
		char *text = new char[str.length() + 1];
		strcpy(text, str.c_str());


		RECT rect;

		switch (orientation)
		{
		case 0:
			if (bordered)
			{
				SetRect(&rect, x - 1, y, x - 1, y);
				pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
				SetRect(&rect, x + 1, y, x + 1, y);
				pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
				SetRect(&rect, x, y - 1, x, y - 1);
				pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
				SetRect(&rect, x, y + 1, x, y + 1);
				pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, bcolor);
			}
			SetRect(&rect, x, y, x, y);
			pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, color);
			break;
		case 1:
			if (bordered)
			{
				SetRect(&rect, x - 1, y, x - 1, y);
				pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
				SetRect(&rect, x + 1, y, x + 1, y);
				pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
				SetRect(&rect, x, y - 1, x, y - 1);
				pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
				SetRect(&rect, x, y + 1, x, y + 1);
				pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, bcolor);
			}
			SetRect(&rect, x, y, x, y);
			pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
			break;
		case 2:
			if (bordered)
			{
				SetRect(&rect, x - 1, y, x - 1, y);
				pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
				SetRect(&rect, x + 1, y, x + 1, y);
				pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
				SetRect(&rect, x, y - 1, x, y - 1);
				pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
				SetRect(&rect, x, y + 1, x, y + 1);
				pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, bcolor);
			}
			SetRect(&rect, x, y, x, y);
			pFont[font]->DrawTextA(NULL, text, -1, &rect, DT_RIGHT | DT_NOCLIP, color);
			break;
		}
	}


};

extern CDrawing* cDrawing;