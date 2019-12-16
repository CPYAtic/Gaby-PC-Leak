/**************************************************************/
/*				Base Par SystemAce & hans221                  */
/**************************************************************/

#include "sBase.h"
#include <stdio.h>
#include "sDeclaration.h"
#include "sCouleur.h"
#include "sDraw.h"
#include "ShellApi.h"
#include "sDetour.h"
#include "Xor.h"
oPresent	pPresent;
ID3DXFont *pFont = NULL;

MenuD3D	*Sys=NULL;

void MenuRebuild(void)
{
	Sys->Group("[ DX Cheat ]",&DirectGPR,GRP);
	if (DirectGPR)
	{
		Sys->Item("    WallHack",&wallhack,onf);
		Sys->Item("    GlassWalls",&glass,onf);
		Sys->Item("    FullBright",&full,onf);
		Sys->Item("    NoFog",&fog,onf);
	}
	Sys->Text("");
	Sys->Text("");
	Sys->Text("[ ----------------------------------]");
	Sys->Text("[		           By ProGaMeR	 	          ]");
	Sys->Text("[		              	  	        ]");
	Sys->Text("[		               Version: 1.0	 	           ]");
	Sys->Text("[ ----------------------------------]");
}

VOID WriteText( INT x, INT y, DWORD Color, CHAR *Text, ... )
{
	RECT rect;
	SetRect(&rect, x, y, x, y);
	pFont->DrawTextA(NULL, Text, -1, &rect, DT_LEFT | DT_NOCLIP, Color);
}

void EMenu(pD3DdeviceX pDevice)
{
	if (Sys==0) {												
	    Sys = new MenuD3D("DopeHax",10,160);	
		Sys->visible=1;										
		Sys->TITLE_COL=TextYellow;						
	} else {
		if (Sys->noitems==0) MenuRebuild();
		if (Sys->visible){
	    BoxColor((float)13,(float)61,(float)152,(float)Sys->noitems*Sys->height+11,ColorBox,pDevice);
		BoxColor((float)13,(float)38,(float)152,(float)21,ColorBox,pDevice);
		BordColor((float)13,38,(float)152,(float)21,1,ColorBorder,pDevice);
		BordColor((float)13,61,(float)152,(float)Sys->noitems*Sys->height+11,1,ColorBorder,pDevice);
	}
	Sys->Show(pFont);
	Sys->Nav();
	}
}


void PreReset(void)
{
	if(pFont)
	{
	if(pFont->Release())
	pFont = NULL;
	}
}
void MyRelease( LPDIRECT3DDEVICE9 pDevice)
{
   D3DXCreateFont(pDevice, 14, 0,  400, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &pFont );
 //D3DXCreateFontA(pDevice, 16, 0, 400, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Tahoma", &pFont );
}
LPDIRECT3DTEXTURE9 Cyan,Red,White,Yellow,Black,Green,Blue,Purple,Pink,Orange,Grey;
HRESULT GenerateTexture(IDirect3DDevice9 *pD3Ddev, IDirect3DTexture9 **ppD3Dtex, DWORD colour32)
{

	if(FAILED(pD3Ddev->CreateTexture(8, 8, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, ppD3Dtex, NULL)))
		return E_FAIL;
	WORD colour16 = ((WORD)((colour32>>28)&0xF)<<12)
		|(WORD)(((colour32>>20)&0xF)<<8)
		|(WORD)(((colour32>>12)&0xF)<<4)
		|(WORD)(((colour32>>4)&0xF)<<0);
	D3DLOCKED_RECT d3dlr;    
	(*ppD3Dtex)->LockRect(0, &d3dlr, 0, 0);
	WORD *pDst16 = (WORD*)d3dlr.pBits;
	for(int xy=0; xy < 8*8; xy++)
		*pDst16++ = colour16;
	(*ppD3Dtex)->UnlockRect(0);
	return S_OK;

}
HRESULT __stdcall myPresent (LPDIRECT3DDEVICE9 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion )
{
	MyRelease(pDevice);
	EMenu   (pDevice);
	pFont->OnLostDevice();
	pFont->OnResetDevice();
	PreReset();

	return pPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion );
}







DWORD_PTR* FindDevice(DWORD Base,DWORD Len)
{
	unsigned long i = 0, n = 0;

	for( i = 0; i < Len; i++ )
	{
		 if(*(BYTE *)(Base+i+0x00)==/*0xC7*/0xC7)n++;
	    if(*(BYTE *)(Base+i+0x01)==0x06)n++;
	    if(*(BYTE *)(Base+i+0x06)==/*0x89*/0x89)n++;
	    if(*(BYTE *)(Base+i+0x07)==0x86)n++;	
        if(*(BYTE *)(Base+i+0x0C)==/*0x89*/0x89)n++;
	    if(*(BYTE *)(Base+i+0x0D)==0x86)n++;

	    if(n == 6) return (DWORD_PTR*)
			(Base + i + 2);n = 0;
	}
	return(0);
}

int System(void)
{
	HMODULE hD3D = NULL;
	do {
		hD3D = GetModuleHandleA(XorStr<0xA7,9,0x498BF121>("\xC3\x9B\xCD\x93\x85\xC8\xC1\xC2"+0x498BF121).s);
		Sleep(10);
	}while(!hD3D);
	DWORD_PTR * VTablePtr = FindDevice((DWORD)hD3D,0x128000);
    if(VTablePtr == NULL)
	{
		MessageBoxA(NULL,XorStr<0x20,22,0x645E32A9>("\x64\x12\x66\x67\x41\x53\x4F\x44\x4D\x09\x6F\x59\x5E\x42\x5C\x0F\x00\x49\x02\x02\x15"+0x645E32A9).s,0,MB_ICONSTOP);
        ExitProcess(0);
	}

    DWORD_PTR * VTable = 0;
    *(DWORD_PTR *)&VTable = *(DWORD_PTR *)VTablePtr;
	pPresent							  = (oPresent)	             DetourCreate((PBYTE)VTable[17], (PBYTE)myPresent,5);
	return 0;
}


void Acces()
{
	__asm CALL [System];
}



extern "C" __declspec(dllexport) BOOL WINAPI DllMain ( HMODULE hDll, DWORD lpReason, VOID * lpvReason )
{
	DisableThreadLibraryCalls( hDll );
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Acces, NULL, NULL, NULL);
	return TRUE;
}
