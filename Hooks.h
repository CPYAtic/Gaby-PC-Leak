#include <Windows.h>
#include "CChat.h"
#include "CPlayer.h"
#include "CInput.h"
#include "CSpeed.h"
#include "CRoom.h"
#include "SDK.h"
char * GetPlayers = "";
std::string mailboxmsg = "DopeHax Mailbox Test qq";
uint8_t chatmode = 0;
std::string chatspammsg = "";

PBYTE DetourFunction(PBYTE src, PBYTE dst, size_t len) //That's detour function that u use for hooking functions.
{
	PBYTE jmp = reinterpret_cast<BYTE*>(malloc(len + 5));
	DWORD dwOldProt;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &dwOldProt);
	memcpy(jmp, src, len);
	jmp += len;
	jmp[0] = 0xE9;
	*reinterpret_cast<DWORD*>(jmp + 1) = (DWORD)(src + len - jmp) - 5;
	src[0] = 0xE9;
	*reinterpret_cast<DWORD*>(src + 1) = (DWORD)(dst - src) - 5;
	for (size_t i = 5; i < len; i++) src[i] = 0x90; 
	VirtualProtect(src, len, dwOldProt, NULL);
	return (jmp - len);
}
bool contains_number(const std::string &c) //power of google
{
	return (
		c.find('0') != std::string::npos ||
		c.find('1') != std::string::npos ||
		c.find('2') != std::string::npos ||
		c.find('3') != std::string::npos ||
		c.find('4') != std::string::npos ||
		c.find('5') != std::string::npos ||
		c.find('6') != std::string::npos ||
		c.find('7') != std::string::npos ||
		c.find('8') != std::string::npos ||
		c.find('9') != std::string::npos
		);
}

DWORD yawA;
DWORD xcoordA;
DWORD pitchA;

float fYaw1;
float fYaw2;
float fPitch;

DWORD temp, temp1, temp2, temp3, temp4, temp5;
DWORD v1, v2;
DWORD jmp1 = 0x007D5EB6 + 0x6;
void __declspec(naked) Wallwalk()
{
	__asm
	{
		    mov[v2], ecx
			mov[temp], eax //x coord
			mov eax, [ecx + 0x4]
			mov[temp1], eax//x coord
			mov eax, [ecx + 0x8]
			mov[temp2], eax//z coord
			mov eax, [ecx + 0xC]
			mov[temp3], eax//z coord
			mov eax, [ecx + 0x10]
			mov[temp4], eax//y coord
			mov ecx, [ecx + 0x14]
			mov[temp5], ecx//y coord
			mov[v1], edx
	}
	//cuz eax is set right after you enter dis funct,
	//so I set its value to temporary variable
	if (elevator && Read.Dword(0x11C5060) == 25 && !flyhax && !Read.Bool(0x011A4D49))//if elevator is on, flyhax off && serverstate == 25 && not chatinput
	{
		if (ValidatePointer(xcoordA))//avoid crash
		{
			if (!wallwalk)
			{
				*(DWORD*)v1 = temp;
				*(DWORD*)(v1 + 0x4) = temp1;
				*(DWORD*)(v1 + 0x8) = temp2;
				*(DWORD*)(v1 + 0xC) = temp3;
			}
			if (Read.Bool(0x11B20EA))//down
			{
				Write.Double(xcoordA + 0x10, Read.Double(xcoordA + 0x10) - espeed);
			}
			else if (Read.Bool(0x11B20EB))//up
			{
				Write.Double(xcoordA + 0x10, Read.Double(xcoordA + 0x10) + espeed);
			}
			else
			{
				*(DWORD*)(v1 + 0x10) = temp4;
				*(DWORD*)(v1 + 0x14) = temp5;
			}
		}
	}
	if (wallwalk && Read.Dword(0x11C5060) == 25 && !Read.Bool(0x011A4D49))//if wallwalk && serverstate == 25( if cuz u can actualy start elevator and wallwalk together, but flyhax only alone)
	{
		*(DWORD*)(v1 + 0x10) = temp4;
		*(DWORD*)(v1 + 0x14) = temp5;

		yawA = Func.GetDMA(0x11B5FBC, yoff, 4);
		xcoordA = Func.GetDMA(0x11B5FBC, xoff, 4);

		if (ValidatePointer(yawA) && ValidatePointer(xcoordA))//avoid crash
		{
			fYaw1 = -sin(Read.Float(yawA));
			fYaw2 = cos(Read.Float(yawA));

			if (Read.Bool(0x11B20E6))//forward
			{
				Write.Double(xcoordA, Read.Double(xcoordA) + fYaw2 * wspeed);
				Write.Double(xcoordA + 0x8, Read.Double(xcoordA + 0x8) + fYaw1 * wspeed);
			}
			else if (Read.Bool(0x11B20E7))//backward
			{
				Write.Double(xcoordA, Read.Double(xcoordA) - fYaw2 * wspeed);
				Write.Double(xcoordA + 0x8, Read.Double(xcoordA + 0x8) - fYaw1 * wspeed);
			}
			if (Read.Bool(0x11B20E8))//left
			{
				Write.Double(xcoordA, Read.Double(xcoordA) - fYaw1 * wspeed);
				Write.Double(xcoordA + 0x8, Read.Double(xcoordA + 0x8) + fYaw2 * wspeed);
			}
			else if (Read.Bool(0x11B20E9))//right
			{
				Write.Double(xcoordA, Read.Double(xcoordA) + fYaw1 * wspeed);
				Write.Double(xcoordA + 0x8, Read.Double(xcoordA + 0x8) - fYaw2 * wspeed);
			}
		}
	}
	else if (flyhax && Read.Dword(0x11C5060) == 25 && !elevator && !Read.Bool(0x011A4D49))//if wallwalk is off, flyhax on, elevator off && serverstate == 25
	{
		yawA = Func.GetDMA(WeaponBase, yoff, 4);
		pitchA = Func.GetDMA(WeaponBase, poff, 3);
		xcoordA = Func.GetDMA(WeaponBase, xoff, 4);

		if (ValidatePointer(yawA) && ValidatePointer(xcoordA) && ValidatePointer(pitchA))//avoid crash
		{
			fYaw1 = -sin(Read.Float(yawA));
			fYaw2 = cos(Read.Float(yawA));
			fPitch = sin(Read.Float(pitchA));

			if (Read.Bool(0x11B20E6))//forward
			{
				Write.Double(xcoordA, Read.Double(xcoordA) + fYaw2 * fspeed);
				Write.Double(xcoordA + 0x8, Read.Double(xcoordA + 0x8) + fYaw1 * fspeed);
				Write.Double(xcoordA + 0x10, Read.Double(xcoordA + 0x10) + fPitch * fspeed);
			}
			else if (Read.Bool(0x11B20E7))//backward
			{
				Write.Double(xcoordA, Read.Double(xcoordA) - fYaw2 * fspeed);
				Write.Double(xcoordA + 0x8, Read.Double(xcoordA + 0x8) - fYaw1 * fspeed);
			}
			if (Read.Bool(0x11B20E8))//left
			{
				Write.Double(xcoordA, Read.Double(xcoordA) - fYaw1 * fspeed);
				Write.Double(xcoordA + 0x8, Read.Double(xcoordA + 0x8) + fYaw2 * fspeed);
			}
			else if (Read.Bool(0x11B20E9))//right
			{
				Write.Double(xcoordA, Read.Double(xcoordA) + fYaw1 * fspeed);
				Write.Double(xcoordA + 0x8, Read.Double(xcoordA + 0x8) - fYaw2 * fspeed);
			}
			if (Read.Bool(0x11B20EA))//down
			{
				Write.Double(xcoordA + 0x10, Read.Double(xcoordA + 0x10) - fspeed);
			}
			else if (Read.Bool(0x11B20EB))//up
			{
				Write.Double(xcoordA + 0x10, Read.Double(xcoordA + 0x10) + fspeed);
			}
		}
	}
	else if (!elevator || Read.Dword(0x11C5060) != 25 || Read.Bool(0x011A4D49))
	{
		__asm {
			mov eax, [temp]
				mov ecx, [v2]
				mov edx, [v1]
				mov[edx], eax
				mov eax, [ecx + 0x4]
				mov[edx + 0x4], eax
				mov eax, [ecx + 0x8]
				mov[edx + 0x8], eax
				mov eax, [ecx + 0xC]
				mov[edx + 0xC], eax
				mov eax, [ecx + 0x10]
				mov[edx + 0x10], eax
				mov ecx, [ecx + 0x14]
				mov[edx + 0x14], ecx
		}
	}
	__asm jmp[jmp1] //jump back
}




int8_t __cdecl hkSendChatMessage(uint8_t p1, uint8_t p2, LPCSTR lpText) //Hooked SendChatMessage function
{
	//LPCSTR temp = lpText;
	//char * c_arg[5];
	//auto command = SplitStringA((char*)temp, c_arg, 5, ' ');
	
	std::string strText = lpText;
	
	if (strcmp(lpText, "/commands") == 0)
	{

		CChat::PrintChatMessage("[DopeHax Commands Overview]", "", CChat::INFO, true, false, CChat::YELLOW);
		CChat::PrintChatMessage("/commands -> Prints Commands Overview", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/nocensore -> Bypasses all censore of game.", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/nopenalty -> Bypasses penalty chat(lobby one,not gm one)", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/nc -> Opens Namechanger Dialog", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/setname [name] -> Sets your name", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/reset -> Sends reset room packet", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/respawn -> Sends respawn player packet", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/teamchange -> Sends team change packet", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/mode_ai -> Changes room's mode to ai", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/meleerange -> Increases melee range", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/firingrate -> Shoots very fast", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/ua -> Unlimited Ammo", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/uar -> Unlimited Ammo Reload", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/sendmsg [player] -> Sends message to a player", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/setmsg [message] -> Sets message for /sendmsg", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/setmode [id] -> Sets mode using ids", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/setmap [id] -> Sets map using ids", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/lagg [ammount] -> Fake lagg", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/speed [ammount] -> Speed hax", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/wallwalk -> Turns on/off wallwalk", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/fly -> Turns on/off fly", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/elevator -> Turns on/off elevator", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/wallwalk speed [ammount] -> Sets speed for wallwalk", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/fly speed [ammount] -> Sets speed for fly", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/elevator speed [ammount] -> Sets speed for elevator", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/contact -> Shows my contact", "", CChat::HELP, true, false, CChat::NORMAL);
		CChat::PrintChatMessage("/credits -> Shows my credits", "", CChat::HELP, true, false, CChat::NORMAL);
		return 1; //cancel sending the message
	}
	if (strcmp(lpText, "/nocensore") == 0)
	{
		bypass_censore = !bypass_censore;
		if (bypass_censore)
			CChat::PrintChatMessage("[DopeHax] Bypass Censore enabled.", "", CChat::INFO, true, false, CChat::GREEN);
		else
			CChat::PrintChatMessage("[DopeHax] Bypass Censore disabled.", "", CChat::INFO, true, false, CChat::PINK);
		return 1; // cancel sending the message
	}
	if (strcmp(lpText, "/nopenalty") == 0)
	{
		bypass_penalty = !bypass_penalty;
		if (bypass_penalty)
			CChat::PrintChatMessage("[DopeHax] Bypass Penalty enabled.", "", CChat::INFO, true, false, CChat::GREEN);
		else
			CChat::PrintChatMessage("[DopeHax] Bypass Penalty disabled.", "", CChat::INFO, true, false, CChat::PINK);
		return 1; //cancel sending the message
	}
	if (strcmp(lpText, "/meleerange") == 0)
	{
		isMeleeRange = !isMeleeRange;
		if (isMeleeRange)
			CChat::PrintChatMessage("[DopeHax] Melee Range enabled.", "", CChat::INFO, true, false, CChat::GREEN);
		else
			CChat::PrintChatMessage("[DopeHax] Melee Range disabled.", "", CChat::INFO, true, false, CChat::PINK);
		return 1; //cancel sending the message
	}
	if (strcmp(lpText, "/firingrate") == 0)
	{
		isFiringRate = !isFiringRate;
		if (isFiringRate)
			CChat::PrintChatMessage("[DopeHax] Firing Rate enabled.", "", CChat::INFO, true, false, CChat::GREEN);
		else
			CChat::PrintChatMessage("[DopeHax] Firing Rate disabled.", "", CChat::INFO, true, false, CChat::PINK);
		return 1; //cancel sending the message
	}
	
	if (strcmp(lpText, "/nc") == 0)
	{
		CChat::CreateDlg("E_DLG_AVATAR_NAME", 0);
		CChat::PrintChatMessage("[DopeHax] Namechanger dialog created.", "", CChat::INFO, true, false, CChat::GREEN);
		return 1; //cancel sending the message
	}
	if (strcmp(lpText, "/reset") == 0)
	{
		CPlayer::ResetGameMatch();
		CChat::PrintChatMessage("[DopeHax] Room reseted.", "", CChat::INFO, true, false, CChat::GREEN);
		return 1; //cancel sending the message
	}
	if (strcmp(lpText, "/respawn") == 0)
	{
		CPlayer::Respawn();
		CChat::PrintChatMessage("[DopeHax] Respawned.", "", CChat::INFO, true, false, CChat::GREEN);
		return 1; //cancel sending the message
	}
	if (strcmp(lpText, "/teamchange") == 0)
	{
		CPlayer::TeamChange();
		CChat::PrintChatMessage("[DopeHax] Team Changed.", "", CChat::INFO, true, false, CChat::GREEN);
		return 1; //cancel sending the message
	}
	if (strcmp(lpText, "/mode_ai") == 0)
	{
		CPlayer::SetRoomMod(0xE);
		CChat::PrintChatMessage("[DopeHax] Changed mode to AI Battle.", "", CChat::INFO, true, false, CChat::GREEN);
		return 1; //cancel sending the message
	}
	if (strcmp(lpText, "/ua") == 0)
	{
		ua = !ua;
		if (ua)
			CChat::PrintChatMessage("[DopeHax] Unlimited Ammo enabled.", "", CChat::INFO, true, false, CChat::GREEN);
		else
			CChat::PrintChatMessage("[DopeHax] Unlimited Ammo disabled.", "", CChat::INFO, true, false, CChat::PINK);
		return 1; //cancel sending the message
	}
	if (strcmp(lpText, "/uar") == 0)
	{
		uar = !uar;
		if (uar)
			CChat::PrintChatMessage("[DopeHax] Unlimited Ammo Reload enabled.", "", CChat::INFO, true, false, CChat::GREEN);
		else
			CChat::PrintChatMessage("[DopeHax] Unlimited Ammo Reload disabled.", "", CChat::INFO, true, false, CChat::PINK);
		return 1; //cancel sending the message
	}
	
	if (strcmp(lpText, "/contact") == 0)
	{
		CChat::PrintChatMessage("[DopeHax] Skype: gaby_afganistanu", "", CChat::INFO, true, false, CChat::YELLOW);
		return 1;//cancel sending the message
	}
	if (strcmp(lpText, "/credits") == 0)
	{
		CChat::PrintChatMessage("[DopeHax] ProGaMeR is the creator of this cheat.Have fun ^_^", "", CChat::INFO, true, false, CChat::YELLOW);
		return 1; //cancel sending the message
	}
	if (strText.find("/setname") == 0)
	{
		std::string input = strText.replace(0, strlen("/setname "), "");
		CPlayer::ChangeName(input.c_str());
		std::string echo = "[DopeHax] Succesfully changed name to " + input + ".";
		CChat::PrintChatMessage(echo.c_str(), "", CChat::INFO, true, false, CChat::GREEN);
		return 1;
	}

	
	if (strText.find("/lagg") == 0)//lpText starts with "/speed"
	{
		std::string input = strText.replace(0, strlen("/lagg "), "");
		float value = std::stof(input);
		CPlayer::FakeLagg(value);
		std::string echo = "[DopeHax] Fake-Lagg has been setted to (" + input + ").";
		CChat::PrintChatMessage(echo.c_str(), "", CChat::HELP, true, false, CChat::PINK);
		return 1;
	}

	if (strText.find("/sendmsg") == 0)
	{
		std::string input = strText.replace(0, strlen("/sendmsg "), "");
		CPlayer::SendPaperMessage(input.c_str(), mailboxmsg.c_str());
		std::string echo = "[DopeHax] Sent message: " + mailboxmsg + "thought mailbox to Player: " + input + ".";
		CChat::PrintChatMessage(echo.c_str(), "", CChat::INFO, true, false, CChat::PINK);
		return 1;
	}
	if (strText.find("/setmsg") == 0)
	{
		std::string input = strText.replace(0, strlen("/setmsg "), "");
		mailboxmsg = input;
		std::string echo = "[DopeHax] Setted mailbox's message to: " + input + ".";
		CChat::PrintChatMessage(echo.c_str(), "", CChat::INFO, true, false, CChat::PINK);
		return 1;
	}


	if (strText.find("/setmode") == 0)
	{
		std::string input = strText.replace(0, strlen("/setmode "), "");
		DWORD Value = std::stoi(input);
		CPlayer::SetRoomMod(Value);
		return 1;
	}
	if (strText.find("/setmap") == 0)
	{
		std::string input = strText.replace(0, strlen("/setmap "), "");
		DWORD Value = std::stoi(input);

		CPlayer::SetRoomMap(Value);

		return 1;
	}

	

	if (strText.find("/speed") == 0)//lpText starts with "/speed"
	{
		std::string input = "100";
		float value = 100;
		if (strcmp(lpText, "/speed") == 0) //nothing's followin' after "/speed"
		{
			value = 100;
		}
		else //there's smth following command
		{
			input = strText.replace(0, strlen("/speed "), "");
			value = std::stof(input);
		}
		CPlayer::SetSpeed(value);
		std::string echo = "[DopeHax] Speed has been set to (" + input + ").";
		CChat::PrintChatMessage(echo.c_str(), "", CChat::HELP, true, false, CChat::PINK);
		return 1;
	}
	if (strText.find("/wallwalk") == 0) //STARTS with "/wallwalk"
	{
		if (strcmp(lpText, "/wallwalk") == 0)//if lpText == "/wallwalk"
		{
			wallwalk = !wallwalk;
			if (wallwalk)
			{
				CChat::PrintChatMessage("[DopeHax] Wallwalk enabled.", "", CChat::HELP, true, false, CChat::GREEN);
				flyhax = false; //turn off flyhax
			}
			else
				CChat::PrintChatMessage("[DopeHax] Wallwalk disabled.", "", CChat::HELP, true, false, CChat::PINK);
		}
		else if (strText.find("/wallwalk speed ") == 0)//set wallwalk's speed
		{
			if (contains_number(strText))//avoid crash
			{
				std::string input = strText.replace(0, strlen("/wallwalk speed "), "");
				wspeed = std::stof(input);
				std::string wallwalkmsg = "[DopeHax] Wallwalk's speed has been set to (" + std::to_string(wspeed) + ").";
				CChat::PrintChatMessage(wallwalkmsg.c_str(), "", CChat::HELP, true, false, CChat::PINK);
			}
			else
				return 0;
		}

		return 1;
	}
	else if (strText.find("/fly") == 0)
	{

		if (strcmp(lpText, "/fly") == 0)
		{
			flyhax = !flyhax;
			if (flyhax)
			{
				CChat::PrintChatMessage("[DopeHax] Fly enabled.", "", CChat::HELP, true, false, CChat::GREEN);
				wallwalk = false; //turn off wallwalk
				elevator = false; //turn off elevator
			}
			else
				CChat::PrintChatMessage("[DopeHax] Fly disabled.", "", CChat::HELP, true, false, CChat::PINK);
		}

		else if (strText.find("/fly speed ") == 0)//set wallwalk's speed
		{
			if (contains_number(strText))//avoid crash
			{
				std::string input = strText.replace(0, strlen("/fly speed "), "");
				fspeed = std::stof(input);
				std::string flyspeedmsg = "[DopeHax] Flyhack's speed has been set to (" + std::to_string(fspeed) + ").";
				CChat::PrintChatMessage(flyspeedmsg.c_str(), "", CChat::HELP, true, false, CChat::PINK);
			}
			else
				return 0;
		}

		return 1;
	}
	else if (strText.find("/elevator") == 0)
	{
		if (strcmp(lpText, "/elevator") == 0)
		{
			elevator = !elevator;
			if (elevator)
			{
				CChat::PrintChatMessage("[DopeHax] Elevator enabled.", "", CChat::HELP, true, false, CChat::GREEN);
				flyhax = false; //turn off flyhax
			}
			else
				CChat::PrintChatMessage("[DopeHax] Elevator enabled.", "", CChat::HELP, true, false, CChat::PINK);
		}
		else if (strText.find("/elevator speed ") == 0)//set wallwalk's speed
		{
			if (contains_number(strText))//avoid crash
			{
				std::string input = strText.replace(0, strlen("/elevator speed "), "");
				espeed = std::stof(input);
				std::string elevatorspeedmsg = "[DopeHax] Elevator's speed has been set to (" + std::to_string(espeed) + ").";
				CChat::PrintChatMessage(elevatorspeedmsg.c_str(), "", CChat::HELP, true, false, CChat::PINK);
			}
			else
				return 0;
		}

		return 1;
	}

	return oSendLobbyChatMessage(p1, p2, lpText);
	//return true values if no command is detected
}