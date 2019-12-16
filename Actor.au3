#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.14.2

 Script Function:
	Prototype Actor Functions
	-> completed soon.. maybe

#ce ----------------------------------------------------------------------------

Func GetPlayerName($dwActorClass)
	$NameLength = ReadProcessMemory($hHandle, $dwActorClass + 0x60, "int")
	If $NameLength = 0 Then Return ""
	$ReadAddress = $dwActorClass + 0x50
	If $NameLength >= 16 Then $ReadAddress = ReadProcessMemory($hHandle, $ReadAddress, "dword")
	Return ReadProcessMemory($hHandle, $ReadAddress, "char[" & $NameLength & "]")
EndFunc


Func GetSID($iPlayerID)
	$dwCTeamManager = ReadProcessMemory($hHandle, $dwBase + $off_CTeamManager, "dword")
	$dwActorClass = Call_GetActorClass($dwCTeamManager, $iPlayerID)
	Return ReadProcessMemory($hHandle, $dwActorClass + 0x4C, "dword")
EndFunc   ;==>GetSID

#cs
Func Actor_GetCurrentHP($Actor)
	Return Round(ActorCall($Actor, 0x94, $dwBuffer, "float", 0),1)
EndFunc

Func Actor_IsAlive($Actor)
	Return Int(ActorCall($Actor, 0xD4, $dwBuffer, "Byte[1]", -1))
EndFunc
#ce

#cs
Func Actor_GetMaxHP($Actor)
	Return Round(ActorCall($Actor, 0x9C, $dwBuffer, "float", 0),1)
EndFunc

Func Actor_GetMaxSP($Actor)
	Return Round(ActorCall($Actor, 0xD0, $dwBuffer, "float", 0),1)
EndFunc
#ce

Func Actor_GetMaxHP($Actor)
	Return Round(ActorCall($Actor, 0x9C, $dwBuffer, "float", 0),1)
EndFunc

Func Actor_GetMaxSP($Actor)
	Return Round(ActorCall($Actor, 0xD0, $dwBuffer, "float", 0),1)
EndFunc

#Region Internal
Func ActorCall($Actor, $offset, $returnBuffer, $type = "int", $waitfor = -1)
	WriteProcessMemory($hHandle, $returnBuffer, $waitfor, $type)

	Local $wByte = "0xB9"  & Byte_Reverse($Actor)
	$wByte &= "8B01FF90"
	$wByte &= Byte_Reverse(Make8Hex($offset)) & "A3"
	$wByte &= Byte_Reverse($returnBuffer) & "C3"

	$ret = WriteProcessMemory($hHandle, $dwMFunction, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")
	If $ret <> 0 Then
		Return SetError(-1)
	EndIf

	DllCall("kernel32.dll", "int", "CreateRemoteThread", "int", $hHandle, "ptr", 0, "int", 0, "int", $dwMFunction, "ptr", 0, "int", 0, "int", 0)
	Do
		$read = ReadProcessMemory($hHandle, $returnBuffer, $type)
	Until $read <> $waitFor
	Return $read
EndFunc

Func Make8Hex($Val)
	If StringInStr($Val, "0x") = 0 And IsInt($Val) = 0 Then Return "0x" & $Val
	If IsInt($Val) Then Return "0x" & Hex($Val,8)
	If StringInStr($Val, "0x") = 1 And StringLen($Val) <> 10 Then Return "0x" & Hex($Val,8)
	Return $Val
EndFunc
#EndRegion