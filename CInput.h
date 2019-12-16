#include <Windows.h>
#include "SDK.h"
/*////////////////////////////////////////////////PARAMS////////////////////////////////////////////////*/
typedef LRESULT(__stdcall* tGameInput)(int, WPARAM, LPARAM);
// Params: nCode, wParam, lParam
/*////////////////////////////////////////////////PARAMS////////////////////////////////////////////////*/

/*/////////////////////////////////////////Functions - Address/////////////////////////////////////////*/
tGameInput oGameInput = reinterpret_cast<tGameInput>(0x005053F0); //GameInput
/*/////////////////////////////////////////Functions - Address/////////////////////////////////////////*/
KBDLLHOOKSTRUCT kbdStruct;
HHOOK _hook;

typedef SHORT(__stdcall* tGetAsyncKeyState)(int);
tGetAsyncKeyState oGetAsyncKeyState;

__declspec(naked) SHORT GetAsyncKeyState_(int vKey)
{
	__asm
	{
		mov eax, 1044h
			mov ecx, 3
			lea edx, [esp + 4]
			call dword ptr fs : [0C0h]
			add esp, 4
			retn 4
	}
}



LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT *pKeyBoard = (KBDLLHOOKSTRUCT*)lParam;

	switch (wParam)
	{
	case WM_KEYDOWN:
		switch (pKeyBoard->vkCode)
		{
		case VK_NUMPAD6:
			CChat::PrintChatMessage("Num6 was pressed", "", CChat::HELP, true, false, CChat::GREEN);
			break;
		case VK_NUMPAD5:
			CChat::PrintChatMessage("Num5 was pressed", "", CChat::HELP, true, false, CChat::GREEN);
			break;
		}
	default:
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	return 0;
}

