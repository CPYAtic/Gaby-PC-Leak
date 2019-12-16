Func Kill()
	$iPlayerID = GetSelectedPlayerID()
	If $iPlayerID = 0 Then Return 0
	$dwLocalActor = ReadProcessMemory($hHandle, $dwBase + $off_CGameManager, "dword")
	$dwLocalActor = ReadProcessMemory($hHandle, $dwLocalActor + 0x124)
	$iKillerPID = ReadProcessMemory($hHandle, $dwLocalActor + 0x48, "dword")
	Call_Kill($iPlayerID, 6969, 10)
EndFunc   ;==>Suicide

 Func SessionCopy()
   $iPlayerID = GetSelectedPlayerID()
	If $iPlayerID = 0 Then Return 0

	$dwActorClass = Call_GetActorClass(ReadProcessMemory($hHandle, $dwBase + $off_CTeamManager, "dword"), $iPlayerID)
	$sID = GetSID($iPlayerID)
	 ClipPut(Hex($sID,8))
  EndFunc

Func Touchdown()
	$iPlayerID = GetSelectedPlayerID()
	If $iPlayerID = 0 Then Return 0
	Call_Touchdown($iPlayerID)
 EndFunc   ;==>TD

 Func Suicide()
	$iPlayerID = GetSelectedPlayerID()
	If $iPlayerID = 0 Then Return 0
    For $i = 1 To 5
    Call_Suicide($iPlayerID, 28)
    Sleep(80)
    If IsInt($i/10) Then Sleep(80)
    If _IsPressed(23) then ExitLoop
Next
EndFunc   ;==>Suicide

 Func SuicideCrash()
	$iPlayerID = GetSelectedPlayerID()
	If $iPlayerID = 0 Then Return 0
    For $i = 1 To 20
    Call_Suicide($iPlayerID, 28)
    Sleep(50)
    If IsInt($i/10) Then Sleep(50)
    If _IsPressed(23) then ExitLoop
Next
EndFunc   ;==>Suicide

 Func BindUser()
	$iPlayerID = GetSelectedPlayerID()
	$sID = GetSID($iPlayerID)
	If $iPlayerID = 0 Then Return 0
    If $sID = 0 Then Return 0
    Call_Bind($iPlayerID, $sID)
 EndFunc

 Func Leave()
	Call_Leave(1)
 EndFunc

 ;byterev masterkick
  Func Call_MasterKick($iPlayerID)
	#cs
	6a 04 // was cast away by Moderator.
	6A 00
	6a <pid>
	E8 <Byterev: Calc: GetInstance(GS)
	8b C8
	E8 <Byterev: Calc: Packet
	C3
	#ce

	Local $wByte = "0x6A046A0068"
	$wByte &= Byte_Reverse($iPlayerID) & "E8"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_GetInstanceGS, $dwMFunction + 0x9)) & "8BC8E8"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_MasterKick, $dwMFunction + 0x10)) & "C3"
	$ret = WriteProcessMemory($hHandle, $dwMFunction, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")

	If $ret <> 0 Then
		Return 0
	 EndIf

	$ret = DllCall("kernel32.dll", "int", "CreateRemoteThread", "int", $hHandle, "ptr", 0, "int", 0, "int", $dwMFunction, "ptr", 0, "int", 0, "int", 0)
	If $ret = 0 Then
		Return 0
	EndIf
 EndFunc   ;==>RM Kick

 ;byterev leave room
  Func Call_Leave($idk)
	#cs
	6A 01 // prm
	E8 <Byterev: Calc: GetInstance(GS)
	8b C8
	E8 <Byterev: Calc: Leave
	C3
	#ce

	Local $wByte = "0x6A01E8"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_GetInstanceGS, $dwMFunction + 0x2)) & "8BC8E8"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_Leave, $dwMFunction + 0x9)) & "C3"
	$ret = WriteProcessMemory($hHandle, $dwMFunction, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")
	If $ret <> 0 Then
		Return 0
	 EndIf

	$ret = DllCall("kernel32.dll", "int", "CreateRemoteThread", "int", $hHandle, "ptr", 0, "int", 0, "int", $dwMFunction, "ptr", 0, "int", 0, "int", 0)
	If $ret = 0 Then
		Return 0
	EndIf
 EndFunc   ;==>Leave

   Func Call_Touchdown($iPlayerID)
	#cs
		Structure:
		68 <SID (CActor + 4C)>
		68 <playerid>
		E8 <Byterev: Calc: GetInstance(GS)>
		8B C8
		E8 <Byterev: Calc: TD>
		C3
	#ce

	$dwSID = GetSID($iPlayerID)

	Local $wByte = "0x68" & Byte_Reverse($dwSID) & "68"
	$wByte &= Byte_Reverse($iPlayerID) & "E8"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_GetInstanceGS, $dwMFunction + 0xA)) & "8BC8E8"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_Touchdown, $dwMFunction + 0x11)) & "C3"
	$ret = WriteProcessMemory($hHandle, $dwMFunction, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")
	If $ret <> 0 Then
		Return 0
	EndIf

	$ret = DllCall("kernel32.dll", "int", "CreateRemoteThread", "int", $hHandle, "ptr", 0, "int", 0, "int", $dwMFunction, "ptr", 0, "int", 0, "int", 0)
	If $ret = 0 Then
		Return 0
	EndIf
EndFunc   ;==>Call_TD

Func Call_Kill($iTargetPID, $iKillerPID, $iWepID)
	#cs
		Structure:
		6A 01
		68 <TargetSID>
		68 <TargetPID>
		6A <WepID> lets just use 1
		68 <KillerSID>
		68 <KillerPID>
		E8 <Byterev: Calc: GetInstance(GS)>
		8B C8
		E8 <Byterev: Calc: Kill>
		C3
	#ce

#cs
	If StringLen($iWepID) <> 2 Then
		Return 0
	EndIf
	#ce

	$iTargetSID = GetSID($iTargetPID)
	$iKillerSID = GetSID($iKillerPID)

	Local $wByte = "0x6A0168"
	$wByte &= Byte_Reverse($iTargetSID) & "68"
	$wByte &= Byte_Reverse($iTargetPID) & "6A1A68"
	$wByte &= Byte_Reverse($iKillerSID) & "68"
	$wByte &= Byte_Reverse($iKillerPID) & "E8"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_GetInstanceGS, $dwMFunction + 0x18)) & "8BC8E8"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_KillReq, $dwMFunction + 0x1F)) & "C3"
	$ret = WriteProcessMemory($hHandle, $dwMFunction, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")
	If $ret <> 0 Then
		Return 0
	EndIf

	DllCall("kernel32.dll", "int", "CreateRemoteThread", "int", $hHandle, "ptr", 0, "int", 0, "int", $dwMFunction, "ptr", 0, "int", 0, "int", 0)
EndFunc   ;==>Call_Kill

Func Call_Suicide($iPlayerID, $iWepID)
	#cs
		Structure:
		6A 12
		68 <SID (CActor + 4C)>
		68 <playerid>
		E8 <Byterev: Calc: GetInstance(GS)>
		8B C8
		E8 <Byterev: Calc: Suicide>
		C3
	#ce
	If StringLen($iWepID) <> 2 Then
		Return 0
	EndIf

	$dwSID = GetSID($iPlayerID)

	Local $wByte = "0x6A" & $iWepID & "68"
	$wByte &= Byte_Reverse($dwSID) & "68"
	$wByte &= Byte_Reverse($iPlayerID) & "E8"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_GetInstanceGS, $dwMFunction + 0xC)) & "8BC8E8"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_SuicideReq, $dwMFunction + 0x13)) & "C3"
	$ret = WriteProcessMemory($hHandle, $dwMFunction, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")
	If $ret <> 0 Then
		Return 0
	EndIf

	;ClipPut(Hex($dwMFunction,8))
	$ret = DllCall("kernel32.dll", "int", "CreateRemoteThread", "int", $hHandle, "ptr", 0, "int", 0, "int", $dwMFunction, "ptr", 0, "int", 0, "int", 0)
	If $ret = 0 Then
		Return 0
	EndIf
EndFunc   ;==>Call_Suicide

Func Call_SetRoomMaster($iPlayerID)
	#cs
		Structure:
		6A 00
		68 <playerid>
		E8 <Byterev: Calc: GetInstance(GS)>
		8B C8
		E8 <Byterev: Calc: SetMaster>
		C3
	#ce

	Local $wByte = "0x6A0068"
	$wByte &= Byte_Reverse($iPlayerID) & "E8"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_GetInstanceGS, $dwMFunction + 0x7)) & "8BC8E8"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_MasterChangeReq, $dwMFunction + 0xE)) & "C3"
	$ret = WriteProcessMemory($hHandle, $dwMFunction, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")
	If $ret <> 0 Then
		Return 0
	EndIf

	$ret = DllCall("kernel32.dll", "int", "CreateRemoteThread", "int", $hHandle, "ptr", 0, "int", 0, "int", $dwMFunction, "ptr", 0, "int", 0, "int", 0)
	If $ret = 0 Then
		Return 0
	EndIf
 EndFunc   ;==>Call_SetRoomMaster

 Func Call_HealPts($iPlayerID)
	#cs
		Structure:
		6A 00
		68 <playerid>
		E8 <Byterev: Calc: GetInstance(GS)>
		8B C8
		E8 <Byterev: Calc: HealPts>
		C3
	#ce

	Local $wByte = "0x6A0068"
	$wByte &= Byte_Reverse($iPlayerID) & "E8"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_GetInstanceGS, $dwMFunction + 0x7)) & "8BC8E8"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_HealPts, $dwMFunction + 0xE)) & "C3"
	$ret = WriteProcessMemory($hHandle, $dwMFunction, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")
	If $ret <> 0 Then
		Return 0
	EndIf

	$ret = DllCall("kernel32.dll", "int", "CreateRemoteThread", "int", $hHandle, "ptr", 0, "int", 0, "int", $dwMFunction, "ptr", 0, "int", 0, "int", 0)
	If $ret = 0 Then
		Return 0
	EndIf
 EndFunc   ;==>Call_HealPoints

 Func Call_Crash($HostId)
	#cs
		Structure:
		68 <1C2B3E02> 5
		68 <A6 03> 3
		68 <Proud> 5
		68 <hostid> 5
        B9 <CRoomClient> 5
		83 C1 30 (add ecx,30) 3
		E8 <Byterev: Calc: EVENT_MONSTER_COMMAND>  4
		C3
	#ce

	$dwBuffer1 = VirtualAllocEx($hHandle, 0, 16, $MEM_COMMIT, $PAGE_EXECUTE_READWRITE)
	WriteProcessMemory($hHandle, $dwBuffer1, "0xA603", "Byte[2]")
	$dwBuffer44 = VirtualAllocEx($hHandle, 0, 16, $MEM_COMMIT, $PAGE_EXECUTE_READWRITE)
	WriteProcessMemory($hHandle, $dwBuffer44, "0x1C2B3E02", "Byte[4]")

    Local $wByte = "0x68"
    $wByte &= Byte_Reverse($dwBuffer44) & "68"
    $wByte &= Byte_Reverse($dwBuffer1) & "68"	;680001C71668" ;70E4530268"
    $wByte &= Byte_Reverse($dwBase + $off_Proud) & "68"
	$wByte &= Byte_Reverse($HostId) & "B9"
	$wByte &= Byte_Reverse($dwBase + $off_CRoomClient) & "83C130E8"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_Monster, $dwMFunction + 0x1C)) & "C3"
	$ret = WriteProcessMemory($hHandle, $dwMFunction, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")
	If $ret <> 0 Then
		Return 0
	 EndIf

	 ;ClipPut(Hex($dwMFunction,8))

	$ret = DllCall("kernel32.dll", "int", "CreateRemoteThread", "int", $hHandle, "ptr", 0, "int", 0, "int", $dwMFunction, "ptr", 0, "int", 0, "int", 0)
	If $ret = 0 Then
		Return 0
	EndIf
EndFunc

Func Call_P2PEvent($HostID)
    $dwBuffer2 = VirtualAllocEx($hHandle, 0, 64, $MEM_COMMIT, $PAGE_EXECUTE_READWRITE)
	$iEventID = VirtualAllocEx($hHandle, 0, 64, $MEM_COMMIT, $PAGE_EXECUTE_READWRITE)
	WriteProcessMemory($hHandle, $iEventID, "0x17", "Byte[1]")
	$iPlayerID = VirtualAllocEx($hHandle, 0, 64, $MEM_COMMIT, $PAGE_EXECUTE_READWRITE)
	WriteProcessMemory($hHandle, $iPlayerID, "0xC2A4F3", "Byte[3]")

    Local $wByte = "0x68"
	$wByte &= Byte_Reverse($dwBuffer2) & "68"
	$wByte &= Byte_Reverse($iPlayerID) & "68"
	$wByte &= Byte_Reverse($iEventID) & "68"
    $wByte &= Byte_Reverse($dwBase + $off_Proud1) & "68"
	$wByte &= Byte_Reverse($HostId) & "B9"
	$wByte &= Byte_Reverse($dwBase + $off_CRoomClient) & "83C130E8"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_P2P, $dwMFunction + 0x21)) & "C3"
	$ret = WriteProcessMemory($hHandle, $dwMFunction, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")
	If $ret <> 0 Then
		Return 0
	 EndIf

	 ;ClipPut(Hex($dwMFunction,8))

	$ret = DllCall("kernel32.dll", "int", "CreateRemoteThread", "int", $hHandle, "ptr", 0, "int", 0, "int", $dwMFunction, "ptr", 0, "int", 0, "int", 0)
	If $ret = 0 Then
		Return 0
	EndIf
 EndFunc

  Func Call_Bind($iTargetPID, $iTargetSID)
	#cs
		Structure:
		6A 00
		6A 00
		68 <BindStruct>
		6A 10
		68 <SID>
		68 <PID>
		E8 <Byterev: Calc: GetInstance(CRoom)>
		8B C8
		E8 <Byterev: Calc: Send>

		NEW:
		A3 <Byterev: dwBuffer>
		C3

		BindStruct:
		BindStruct+14: PlayerID
		BindStruct+18: SessionID
		BindStruct+1C: 1
	#ce

	;WriteProcessMemory($hHandle, $dwBuffer, -1, "int")


   $dwabc = VirtualAllocEx($hHandle, 0, 128, $MEM_COMMIT, $PAGE_EXECUTE_READWRITE)
   $dwabc1 = VirtualAllocEx($hHandle, 0, 128, $MEM_COMMIT, $PAGE_EXECUTE_READWRITE)
   WriteProcessMemory($hHandle, $dwabc, $dwabc1, "Byte[4]")
   ;yazılan - my struct

    Local $wByte1 = "0x0000000000000000D7BEAF4F284150B0D7BEAF4F"
	$wByte1 &= Byte_Reverse($iTargetPID)
	$wByte1 &= Byte_Reverse($iTargetSID)
	$wByte1 &= "0x00D0359B412DEF37459D652DC500000000000000000000000000000000000000000000000000000000000000000000000000000000000000030000002EDD613B000000801E0597010000000000000000006E5F6D6F7573655F7570007665000000000000"
	;$wByte1 &= _StringRepeat("02", 14)
	;$wByte &= Byte_Reverse($iTargetPID)
	;$wByte &= Byte_Reverse($iTargetSID)
	;$wByte &= Byte_Reverse(0x00000539)
	WriteProcessMemory($hHandle, $dwabc1, $wByte1, "Byte[" & StringLen($wByte1) / 2 - 1 & "]")

    ;Setting BindStruct
    Local $wByte = "0x"
	;$wByte &= _StringRepeat("02", 14)
	$wByte &= Byte_Reverse($iTargetPID)
	$wByte &= Byte_Reverse($iTargetSID)
	$wByte &= Byte_Reverse(0x00000539)
	WriteProcessMemory($hHandle, $dwabc + 14, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")

	Local $wByte2 = "0x"
	;$wByte &= _StringRepeat("02", 14)
	$wByte2 &= Byte_Reverse($iTargetPID)
	$wByte2 &= Byte_Reverse($iTargetSID)
	$wByte2 &= Byte_Reverse(0x00000539)
	WriteProcessMemory($hHandle, $dwabc + 14, $wByte2, "Byte[" & StringLen($wByte2) / 2 - 1 & "]")

	 WriteProcessMemory($hHandle, $dwabc + 4, "0x54000000", "Byte[4]")
	 WriteProcessMemory($hHandle, $dwabc + 8, "0x29000000", "Byte[4]")


	;Setting MCall
	$wByte = "0x6A006A0068"
	$wByte &= Byte_Reverse($dwabc) & "6A1068"
	$wByte &= Byte_Reverse($iTargetSID) & "68"
	$wByte &= Byte_Reverse($iTargetPID) & "E8"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_GetInstanceCRoom, $dwMFunction + 0x15)) & "8BC8E8"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_SendBind, $dwMFunction + 0x1C)) & "C3"
	;$wByte &= Byte_Reverse($dwBuffer) & "C3"
	$ret = WriteProcessMemory($hHandle, $dwMFunction, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")
	If $ret <> 0 Then
		Return 0
	 EndIf

	DllCall("kernel32.dll", "int", "CreateRemoteThread", "int", $hHandle, "ptr", 0, "int", 0, "int", $dwMFunction, "ptr", 0, "int", 0, "int", 0)
EndFunc

#cs
 Func _IsPressed($s_hexKey, $v_dll = 'user32.dll')
; $hexKey must be the value of one of the keys.
; _Is_Key_Pressed will return 0 if the key is not pressed, 1 if it is.
    Local $a_R = DllCall($v_dll, "int", "GetAsyncKeyState", "int", '0x' & $s_hexKey)
    If Not @error And BitAND($a_R[0], 0x8000) = 0x8000 Then Return 1
    Return 0
EndFunc  ;==>_IsPressed
   #ce