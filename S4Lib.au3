#include <String.au3>
#include "Detour.au3"
#include "Actor.au3"
#include "GameServer.au3"
#include "PlayerLoad.au3"
#include "Message.au3"
#include "KickManager.au3"
#Region Decl
$targetProcess = "S4Client.exe"
Dim $iPID, $hHandle, $dwBase, $AllocStat = False, $iOPID, $dwMFunction, $dwBuffer, $dwWA_View, $dwWA_Send, $dwBindStruct, $dwHHook, $dwHHook2, $dwMessageBuffer
Dim $Array_PlayerID[20]
$Array_PlayerID[0] = 0

;REG - Offsets
$off_CGameManager = 0x17293C0 ;0F B6 4D FE 85 C9 0F 84 C0 00
$off_CTeamManager = 0x172B144
;$off_CChatProxy = 0x17991F4 ; 1.sıra alt pointer 8B 95 20 FF FF FF 52 E8

;6A 00 6A 00 83 EC 18 8B CC 89
;$off_Room = 0xA0A0C9

;75 61 6A 00 E8
;$off_KickStructure = 0xB82173

;0F B6 C8 85 C9 75 0F 6A 01 8B
$off_Loading = 0x4A972B

;Pattern: 55 8B EC 83 EC 40 89 4D F4 C7 45 F8 00 00 00 00 8B 45 F4 8B 48 08 89 4D E4 8B
$off_GetPlayerIDByIndex = 0xD35330

;Pattern: 55 8B EC 83 EC 20 89 4D F0 8B 45 F0 8B 48 04 89 4D F4 8B 55 F4 8B 45 F4 8B 4A 04 2B 08 C1 F9 02 89 4D E8 C7 45 FC 00 00 00 00 EB 09 8B 55 FC 83 C2 01 89 55 FC 8B 45 FC 3B 45 E8 73 5D (2nd)
$off_GetActorClass = 0x769140

;Pattern: 55 8B EC 83 EC 3C 89 4D F4 C7 45 F8 00 00 00 00 8B 45 F4 8B 48 08 89 4D E4
$off_GetPlayerCountPerTeam = 0xD35170

;Pattern: 55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 0C A1 ?? ?? ?? ?? 33 C5 50 8D 45 F4 64 A3 00 00 00 00 C7 45 E8 ?? ?? ?? ?? C6 (8)
$off_GetInstanceGS = 0xAF530
;Pattern: 55 8B EC 51 89 4D FC 8D 45 08 50 68 ?? ?? ?? ?? 6A 01 8B 4D FC E8 ?? ?? ?? ?? 8B C8 E8 ?? ?? ?? ?? 8B E5 5D C2 08
$off_MasterChangeReq = 0xB78C40

$off_SuicideReq = 0xB746D0
$off_KillReq = 0xB7A680

;Normal Chat
;89 11 8B 45 A4 89 41 04 8B 55 A8 89 51 08 66 - E75
$off_SendMessage = 0x398180
$off_WriteAs_ViewHook = $off_SendMessage + 0xE75
$off_WriteAs_WriteHook = $off_WriteAs_ViewHook + 0x64

$off_ViewPush = $off_WriteAs_ViewHook + 0x17
$off_WritePush = $off_WriteAs_WriteHook + 0x17
;$Spaces = _StringRepeat(" ", 350)
$Spaces = "{M-2000,0}"

;55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 0C A1 ?? ?? ?? ?? 33 C5 50 8D 45 F4 64 A3 00 00 00 00 C7 45 E8 ?? ?? ?? ?? C6 (6)
$off_GetInstanceCRoom = 0xAF350

;0F 84 ?? ?? ?? ?? 8B 95 C4 FE FF FF 83
$off_ChatBypass = 0x398888

;$off_Leave = 0xB734A0

;$off_Proud = 0x172BF70
;$off_Proud1 = 0x172BF70
;$off_CRoomClient = 0x177D420
;$off_Monster = 0xBD4D10
;$off_P2P = 0xBD6040

;C7 44 24 20 00 00 00 00 8B 4C 24 28 (sec)
$off_HostIDHook = 0xC462F4
$off_HostIDHook2 = 0xC47933

;55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC B4 00 00 00 A1 ?? ?? ?? ?? 33 C5 50 8D 45 F4 64 A3 00 00 00 00 89 4D 80 8D 8D 60 FF FF FF
$off_SendBind = 0xBE17B0

;$off_HealPts = 0xB86740
;$off_Hit = 0xBF13A0
;55 8B EC 51 89 4D FC 8B 45 FC 83 78 1C 00 75 07 (1)
;$off_crash0 = 0xC13070
;$off_crash1 = 0xBFBE20
;$off_crash2 = 0xBDB2F0

;####################### Files & Settings
$KickList = @ScriptDir & "\KickList.ini"
$dbginfo = @ScriptDir & "\dbginfo.ini"
#EndRegion Decl

Func WriteDbgInfo()
	;FileDelete("dbginfo.ini")
	IniWrite($dbginfo, "S4Client", "base", MakeHex($dwBase))
	IniWrite($dbginfo, "S4Client", "ProcessID", $iOPID)
	IniWrite($dbginfo, "Allocs", "dwMFunction", MakeHex($dwMFunction))
	IniWrite($dbginfo, "Allocs", "dwBuffer", MakeHex($dwBuffer))
	IniWrite($dbginfo, "Allocs", "dwWA_View", MakeHex($dwWA_View))
	IniWrite($dbginfo, "Allocs", "dwWA_View1", MakeHex($dwWA_View1))
	IniWrite($dbginfo, "Allocs", "dwWA_Send", MakeHex($dwWA_Send))
	IniWrite($dbginfo, "Allocs", "dwBindStruct", MakeHex($dwBindStruct))
	IniWrite($dbginfo, "Allocs", "dwHHook", MakeHex($dwHHook))
	IniWrite($dbginfo, "Allocs", "dwMessageBuffer", MakeHex($dwMessageBuffer))
EndFunc

#Region Functions
Func TeleportTo()
	$dwLocalActor = ReadProcessMemory($hHandle, $dwBase + $off_CGameManager, "dword")
	$dwLocalActor = ReadProcessMemory($hHandle, $dwLocalActor + 0x124)
	$dwLocalCCollision = ReadProcessMemory($hHandle, $dwLocalActor + 0x118, "dword")

	$iPlayerID = GetSelectedPlayerID()
	If $iPlayerID = 0 Then Return 0
	$dwCTeamManager = ReadProcessMemory($hHandle, $dwBase + $off_CTeamManager, "dword")
	$dwActorClass = Call_GetActorClass($dwCTeamManager, $iPlayerID)

	$CCollisionObject = ReadProcessMemory($hHandle, $dwActorClass + 0x118)
	$corA = ReadProcessMemory($hHandle, $CCollisionObject + 0x684, "Byte[12]")

	WriteProcessMemory($hHandle, $dwLocalCCollision + 0x684, $corA, "Byte[12]")
EndFunc   ;==>TeleportTo

Func CCActor()
	$iPlayerID = GetSelectedPlayerID()
	If $iPlayerID = 0 Then Return 0
	$dwCTeamManager = ReadProcessMemory($hHandle, $dwBase + $off_CTeamManager, "dword")
	ClipPut(Hex(Call_GetActorClass($dwCTeamManager, $iPlayerID),8))
EndFUnc
#EndRegion BaseFuncs

Func GetInstance()
    $dwInstance = ReadProcessMemory($hHandle, $dwBase + $off_CGameManager, "dword")
	$dwInstance = ReadProcessMemory($hHandle, $dwInstance + 0x138, "dword")
	Return ReadProcessMemory($hHandle, $dwInstance + 0x10, "dword")
EndFunc

Func Call_GetActorClass($ecx, $iPlayerID)
	#cs
		Structure:
		6A 00
		68 <playerid>
		8B 0D <Byterev: Buffer(instance)>
		E8 <Byterev: Calc: GetActorClass>
		A3 <Byterev: Buffer(result)>
		C3
	#ce
	Local $wByte = "0x6A0068"

	$ret = WriteProcessMemory($hHandle, $dwBuffer, "0x" & Byte_Reverse($ecx), "Byte[4]")
	If $ret <> 0 Then
		Return 0
	EndIf

	$wByte &= Byte_Reverse($iPlayerID) & "8B0D"
	$wByte &= Byte_Reverse($dwBuffer)
	$wByte &= "E8" & Byte_Reverse(Calc($dwBase + $off_GetActorClass, $dwMFunction + 0xD))
	$wByte &= "A3" & Byte_Reverse($dwBuffer) & "C3"
	$ret = WriteProcessMemory($hHandle, $dwMFunction, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")
	If $ret <> 0 Then
		Return 0
	EndIf

	$ret = DllCall("kernel32.dll", "int", "CreateRemoteThread", "int", $hHandle, "ptr", 0, "int", 0, "int", $dwMFunction, "ptr", 0, "int", 0, "int", 0)
	If $ret = 0 Then
		Return 0
	EndIf

	$read = $ecx
	Do
		$read = ReadProcessMemory($hHandle, $dwBuffer, "int")
	Until $read <> $ecx
	Return $read
EndFunc   ;==>Call_GetActorClass
#EndRegion S4Lib Calls

#Region Other shit
Func Calc($dwCall, $dwAddress, $i = 0)
	If Not IsInt($dwCall) Then $dwCall = Dec(StringReplace($dwCall, "0x", ""))
	If Not IsInt($dwAddress) Then $dwAddress = Dec(StringReplace($dwAddress, "0x", ""))

	If $i = 1 Then
		Local $tmp = $dwCall
		$dwCall = $dwAddress
		$dwAddress = $tmp
	EndIf
	Return Hex($dwCall - $dwAddress - 5, 8)
EndFunc   ;==>Calc

Func Byte_Reverse($sBytes)
	;If StringInStr($sBytes, "0x") = 1 Then StringReplace($sBytes, "0x", "")
	If IsInt($sBytes) Then
		$sBytes = Hex($sBytes, 8)
		$sBytes = StringReplace($sBytes, "0x", "")
	EndIf
	$sBytes = StringReplace($sBytes, "0x", "")
	Local $sReversed = ""
	For $i = StringLen($sBytes) - 1 To 1 Step -2
		$sReversed &= StringMid($sBytes, $i, 2)
	Next
	Return $sReversed
EndFunc   ;==>Byte_Reverse

Func MakeHex($Val)
	If StringInStr($Val, "0x") = 0 And IsInt($Val) = 0 Then Return "0x" & $Val
	If IsInt($Val) Then Return "0x" & Hex($Val,8)
	Return $Val
EndFunc
#EndRegion Other shit