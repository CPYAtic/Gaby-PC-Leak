#include "SDK.h"



void CDrawing::ReleaseObjects()
{
 
	g_pFont->Release();
	
}

bool CDrawing::CreateObjects()
{
	if (FAILED(D3DXCreateFont(Device, 15, 0, FW_BOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Verdana", &g_pFont)))
	{
		return false;
	}
	else
	{
		return true;
	}
}

void CDrawing::UpdateInfo(LPDIRECT3DDEVICE9 device)
{
	Device = device;
	Rect = new D3DXVECTOR2[2];
	D3DXCreateLine(device, &Line);
	D3DXCreateFont(device, 15, 0, FW_BOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Verdana", &g_pFont);

	Line->SetAntialias(true);
	Line->SetWidth(5);

}

void CDrawing::DrawTexture(int x, int y, LPDIRECT3DTEXTURE9 dTexture, LPD3DXSPRITE sprite)
{
	sprite->Draw(dTexture, NULL, NULL, &D3DXVECTOR3(x, y, 0.0f), 0xFFFFFFFF);
	return;
}
void CDrawing::DrawPoint(IDirect3DDevice9* Device, int baseX, int baseY, int baseW, int baseH, D3DCOLOR Cor)
{
	D3DRECT BarRect = { baseX, baseY, baseX + baseW, baseY + baseH };
	Device->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, Cor, 0, 0);
}
void CDrawing::DrawImage(LPDIRECT3DDEVICE9 pDevice, int x, int y, LPDIRECT3DTEXTURE9 dTexture, LPD3DXSPRITE sprite, BYTE MyImage)
{
	if (dTexture == NULL)D3DXCreateTextureFromFileInMemory(pDevice, &MyImage, sizeof(MyImage), &dTexture); //Create image from array
	if (sprite == NULL)D3DXCreateSprite(pDevice, &sprite); //sprite
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	DrawTexture(x, y, dTexture, sprite);
	sprite->End();
}
void CDrawing::DrawFilledRect(IDirect3DDevice9* pDevice, int x, int y, int w, int h, DWORD color)
{
	D3DRECT BarRect = { x, y, x + w, y + h };
	pDevice->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0);
}
void CDrawing::Draw_Text(LPCSTR TextToDraw, int x, int y, D3DCOLOR Colour, LPD3DXFONT m_font)
{
	// Create a rectangle to indicate where on the screen it should be drawn
	RECT rct = { x - 120, y, x + 120, y + 15 };

	// Draw some text 
	m_font->DrawText(NULL, TextToDraw, -1, &rct, DT_NOCLIP, Colour);
}
void CDrawing::DrawBorderBox(int x, int y, int w, int h, int thickness, D3DCOLOR Colour, IDirect3DDevice9 *pDevice)
{
	//Top horiz line
	DrawFilledRect(pDevice, x, y, w, thickness, Colour);
	//Left vertical line
	DrawFilledRect(pDevice, x, y, thickness, h, Colour);
	//right vertical line
	DrawFilledRect(pDevice, (x + w), y, thickness, h, Colour);
	//bottom horiz line
	DrawFilledRect(pDevice, x, y + h, w + thickness, thickness, Colour);
}

void CDrawing::Draw2DBox(int x, int y, int w, int h, D3DCOLOR Color)
{
	DrawLine2(x - w, y, x + w, y, 2, Color); //up line
	DrawLine2(x - w, y + h, x + w, y + h, 2, Color); //botom line
	DrawLine2(x - w, y, x - w, y + h, 2, Color); //right line
	DrawLine2(x + w, y, x + w, y + h, 2, Color); //left line
}

void CDrawing::FillRGB(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* pDevice)
{
	D3DRECT rec = { x, y, x + w, y + h };
	pDevice->Clear(1, &rec, D3DCLEAR_TARGET, color, 0, 0);
}

void CDrawing::DrawPoint(int x, int y, int w, int h, DWORD color, IDirect3DDevice9* pDevice)
{
	FillRGB((int)x, (int)y, (int)w, (int)h, color, pDevice);
}
void CDrawing::DrawShadowText(int x, int y, D3DCOLOR color, char* String)
{
	RECT Font;
	Font.bottom = 0;
	Font.left = x;
	Font.top = y;
	Font.right = 0;

	RECT Fonts;
	Fonts.bottom = 0;
	Fonts.left = x + 1;
	Fonts.top = y + 1;
	Fonts.right = 0;

	g_pFont->DrawTextA(0, String, strlen(String), &Fonts, DT_NOCLIP, 0xFF010101);
	g_pFont->DrawTextA(0, String, strlen(String), &Font, DT_NOCLIP, color);
}
void CDrawing::DrawLine2(float x, float y, float x2, float y2, float width, DWORD color)
{
	if (x != 0 || y != 0 || x2 != 0 || y2 != 0)
	{
		D3DXVECTOR2 vLine[2];
		Line->SetWidth(width);
		Line->SetAntialias(false);
		Line->SetGLLines(true);


		vLine[0].x = x;
		vLine[0].y = y;
		vLine[1].x = x2;
		vLine[1].y = y2;
		Line->Begin();
		Line->Draw(vLine, 2, color);
		Line->End();
	}
}
void CDrawing::DrawLine(D3DXVECTOR2* points, int count, D3DCOLOR color, int Width)
{
	float SavedWidth = 0;
	if (Width != -1)
	{
		float SavedWidth = Line->GetWidth();
		Line->SetWidth(Width);
	}

	for (int a = 0; a < count - 1; ++a)
	{
		if (points[a].x == 0 || points[a].y == 0 || points[a + 1].x == 0 || points[a + 1].y == 0)
			continue;
		Line->Draw(points + a, 2, color);
	}

	if (Width != -1)
		Line->SetWidth(SavedWidth);
}

void CDrawing::LineSettings(bool antialias, float width)
{
	Line->SetAntialias(antialias);
	Line->SetWidth(width);
}

void CDrawing::DrawRect(float x, float y, float w, float h, D3DCOLOR color)
{
	Rect[0].x = x;
	Rect[0].y = Rect[1].y = y + h / 2;
	Rect[1].x = x + w;

	float SavedWidth = Line->GetWidth();
	Line->SetWidth(h);
	Line->Draw(Rect, 2, color);
	Line->SetWidth(SavedWidth);
}

void CDrawing::DrawRectOutlined(float x, float y, float w, float h, float wid, D3DCOLOR color, D3DCOLOR colorB)
{
	int hw = wid / 2;
	DrawRect(x + hw, y + hw, w - hw, h - hw, color);

	DrawRect(x - hw, y - hw, wid, h + wid + hw, colorB);
	DrawRect(x + w - wid, y - hw, wid, h + wid + hw, colorB);

	DrawRect(x + hw, y - hw, w - hw - wid, wid, colorB);
	DrawRect(x + hw, y + h, w - hw - wid, wid, colorB);

}
void CDrawing::DrawCrosshair(LPDIRECT3DDEVICE9 pDevice, int size, int strong, D3DCOLOR xcolor)
{
	int iCenterX = GetSystemMetrics(0) / 2;
	int iCenterY = GetSystemMetrics(1) / 2;
	if (iCenterX < 20 && iCenterY < 20)
	{
		iCenterX = (GetSystemMetrics(0) / 2);
		iCenterY = (GetSystemMetrics(1) / 2);
	}
	D3DRECT rec2 = { iCenterX - size, iCenterY, iCenterX + size, iCenterY + strong };
	D3DRECT rec3 = { iCenterX, iCenterY - size, iCenterX + strong,iCenterY + size };
	pDevice->Clear(1, &rec2, D3DCLEAR_TARGET, xcolor, 1000, 0);
	pDevice->Clear(1, &rec3, D3DCLEAR_TARGET, xcolor, 100, 0);
}
void CDrawing::DrawCircle(int X, int Y, float radiusx, float radiusy, int numSides, DWORD Color)
{

	D3DXVECTOR2 pLine[128];
	float Step = M_PI * 2.0 / numSides;
	int Count = 0;
	for (float a = 0; a < M_PI*2.0; a += Step)
	{
		float X1 = radiusx * cos(a) + X;
		float Y1 = radiusy * sin(a) + Y;
		float X2 = radiusx * cos(a + Step) + X;
		float Y2 = radiusy * sin(a + Step) + Y;
		pLine[Count].x = X1;
		pLine[Count].y = Y1;
		pLine[Count + 1].x = X2;
		pLine[Count + 1].y = Y2;
		Count += 2;
	}
	Line->Begin();
	Line->Draw(pLine, Count, Color);
	Line->End();
}
void CDrawing::DrawUnfilledCircle(float x, float y, float radius, float width, float rez, D3DCOLOR color)
{
	float si, co, nsi, nco, sw;
	bool antialiasing = Line->GetAntialias();
	Line->SetAntialias(true);
	sw = Line->GetWidth();
	Line->SetWidth(width);


	float Step = M_PI * 2.0 / rez;
	float Dec = Step / 50;

	for (float i = 0; i < M_PI*2.0; i += Step)
	{
		si = x + (sin(i - Dec)*radius);
		co = y + (cos(i - Dec)*radius);

		nsi = x + (sin(i + Step + Dec)*radius);
		nco = y + (cos(i + Step + Dec)*radius);

		Rect[0].x = si;
		Rect[0].y = co;

		Rect[1].x = nsi;
		Rect[1].y = nco;

		Line->Draw(Rect, 2, color);
	}
	Line->SetAntialias(antialiasing);
	Line->SetWidth(sw);
}

void CDrawing::DrawFilledCircle(float x, float y, float radius, float rez, D3DCOLOR color)
{

	float si, co;
	float Step = M_PI * 2.0 / rez;

	int a = 1;
	Vtex[0].x = x;
	Vtex[0].y = y;
	Vtex[0].Color = color;

	for (float i = 0; i <= M_PI * 2.0f; i += Step)
	{
		si = y + (sin(i)*radius);
		co = x + (cos(i)*radius);

		Vtex[a].x = co;
		Vtex[a].y = si;
		Vtex[a].Color = color;
		a++;
	}

	si = y + (sin(0.0f)*radius);
	co = x + (cos(0.0f)*radius);
	Vtex[a].x = co;
	Vtex[a].y = si;
	Vtex[a].Color = color;

	Device->GetPixelShader(&shader);
	Device->GetRenderState(D3DRS_CULLMODE, &state1);
	Device->GetRenderState(D3DRS_ALPHABLENDENABLE, &state2);
	Device->GetRenderState(D3DRS_SRCBLEND, &state3);
	Device->GetRenderState(D3DRS_DESTBLEND, &state4);

	Device->SetPixelShader(NULL);
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, a - 1, Vtex, sizeof(tvertex));

	Device->SetRenderState(D3DRS_CULLMODE, state1);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, state2);
	Device->SetRenderState(D3DRS_SRCBLEND, state3);
	Device->SetRenderState(D3DRS_DESTBLEND, state4);
	Device->SetPixelShader(shader);

}