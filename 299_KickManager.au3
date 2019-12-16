#Region Adlib
Dim $a_UserName, $a_PlayerID, $a_ActorClass, $a_Reason, $Form_KickMgr, $a_localactor, $a_Status = 0

Func KickInitiater($UserName, $PlayerID, $ActorClass, $Reason)
	$a_Status = 1
	$a_UserName = $UserName
	$a_PlayerID = $PlayerID
	$a_ActorClass = $ActorClass
	$a_Reason = $Reason
	$a_localactor = ReadProcessMemory($hHandle, $dwBase + $off_CGameManager, "dword") + 0x124
;	AdlibRegister("KickWaiter", $KickMsgRefresh)
EndFunc

#cs
Func KickWaiter()
	If ReadProcessMemory($hHandle, $a_localactor, "int") = 0 Then
		$a_Status = 0
		Return AdlibUnRegister("KickWaiter")
	EndIf

	If ReadProcessMemory($hHandle, $a_ActorClass + 4, "int") = 0 Then
		;KickMsg($a_PlayerID, $a_UserName, $a_Reason)
		$a_Status = 0
		AdlibUnRegister("KickWaiter")
	EndIf
EndFunc
#EndRegion
#ce

Func Kick()
	$iPlayerID = GetSelectedPlayerID()
	If $iPlayerID = 0 Then Return 0

	$dwActorClass = Call_GetActorClass(ReadProcessMemory($hHandle, $dwBase + $off_CTeamManager, "dword"), $iPlayerID)
	$sID = GetSID($iPlayerID)
	$playerName = GetPlayerName($dwActorClass)
	$HostID = ReadProcessMemory($hHandle, $dwActorClass + 0x4F0, "int")

	$SilentKick = true
	If GUICtrlRead($Check_SilentKick) = 4 Then $SilentKick = false

	If $UseKickMgr = 1 And $SilentKick = false Then Return KickGUI($playerName, $iPlayerID, $sID, $HostID, $dwActorClass)
    If $SilentKick = False Then
		Do

		Until ReadProcessMemory($hHandle, $a_ActorClass + 4, "int") = 0
		KickMsg($iPlayerID, $playerName)
	EndIf
	Sleep(500)
    Call_Kick($iPlayerID, $sID, $HostID)
	Sleep(25)
	Call_Kick($iPlayerID, $sID, $HostID)

EndFunc   ;==>Kick

#Region KickGUI
Func KickGUI($UserName, $PlayerID, $SessionID, $HostID, $ActorClass)
	GUISetState(@SW_DISABLE, $Form_Main)
	Opt("GUIOnEventMode", 0)

	$Form_KickMgr = GUICreate("KickManager - kick dis faq", 435, 258)
	GUICtrlCreateGroup("", 16, 8, 401, 233)
	GUICtrlCreateLabel("Kick Type", 32, 24, 52, 17)
	$Radio_GSDC = GUICtrlCreateRadio("GameServer DC", 40, 48, 113, 17)
	GUICtrlSetState(-1, $GUI_CHECKED)
	$Radio_U = GUICtrlCreateRadio("Bind Crash", 40, 72, 113, 17)
	GUICtrlSetState(-1, $GUI_DISABLE)
	GUICtrlCreateLabel("Reason", 32, 112, 41, 17)
	$Input_Reason = GUICtrlCreateInput(IniRead($KickList, $PlayerID, "LatestReason", ""), 40, 136, 193, 21)
	GUICtrlCreateGroup("", 264, 24, 1, 201)
	GUICtrlCreateGroup("", -99, -99, 1, 1)
	GUICtrlCreateLabel("Selected User", 280, 40, 71, 17)
	$Label_UsrName = GUICtrlCreateLabel($UserName, 288, 64, 200, 17)
	GUICtrlCreateLabel("PlayerID", 280, 104, 44, 17)
	$Label_PlayerID = GUICtrlCreateLabel($PlayerID, 288, 128, 200, 17)
	GUICtrlCreateLabel("HostID", 280, 168, 37, 17)
	$Label_HostID = GUICtrlCreateLabel($HostID, 288, 192, 200, 17)
	$Button_Kick = GUICtrlCreateButton("Kick", 32, 192, 75, 25)
	$Button_Abort = GUICtrlCreateButton("Abort", 152, 192, 75, 25)
	GUICtrlCreateGroup("", -99, -99, 1, 1)
	GUISetState(@SW_SHOW)

	While 1
		Switch GUIGetMsg()
			Case $GUI_EVENT_CLOSE
				Return KillGUI()
			Case $Button_Abort
				Return KillGUI()
			Case $Button_Kick
				PreKick($UserName, $PlayerID, $SessionID, $HostID, $ActorClass, GUICtrlRead($Input_Reason))
				Return KillGUI()
		EndSwitch

		If _IsPressed("0D") And ControlGetFocus($Form_KickMgr) = "Edit1" Then
			PreKick($UserName, $PlayerID, $SessionID, $HostID, $ActorClass, GUICtrlRead($Input_Reason))
			Return KillGUI()
		EndIf
	WEnd
EndFunc   ;==>KickGUI

Func PreKick($UserName, $PlayerID, $SessionID, $HostID, $ActorClass, $Reason)
	If $a_Status = 1 Then
		If MsgBox(32+4, "Warning", "Looks like there is an active 'KickWaiter'" & @CRLF & "Would you like to end it to continue?") <> 6 Then Return 0
		AdlibUnRegister("KickWaiter")
	EndIf

	Call_Kick($PlayerID, $SessionID, $HostID)
	Sleep(25)
	Call_Kick($PlayerID, $SessionID, $HostID)
	KickInitiater($UserName, $PlayerID, $ActorClass, $Reason)
EndFunc

Func KillGUI()
	GUIDelete($Form_KickMgr)
	GUISetState(@SW_ENABLE, $Form_Main)
	Opt("GUIOnEventMode", 1)
EndFunc
#EndRegion KickGUI

#Region BindKick
Func All_Kick()
	$iPlayerID = GetSelectedPlayerID()
	If $iPlayerID = 0 Then Return 0
	$sID = GetSID($iPlayerID)
	Call_Kick($iPlayerID, $sID, 0, 0)
EndFunc   ;==>All_Kick

Func HookHostID($HostID, $Offset, $dwHPage)
	;Writing Page
	Local $wByte = "0xC744242000000000C7442428"
	$wByte &= Byte_Reverse($HostID) & "E9"
	$wByte &= Byte_Reverse(Calc($dwBase + $Offset + 8, $dwHPage + 0x10))
	WriteProcessMemory($hHandle, $dwHPage, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")

	;Writing Jump
	$wByte = "0xE9"
	$wByte &= Byte_Reverse(Calc($dwHPage, $dwBase + $Offset)) & "909090"
	WriteProcessMemory($hHandle, $dwBase + $Offset, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")
EndFunc   ;==>HookHostID

Func DisableHostIDHook($Offset)
	WriteProcessMemory($hHandle, $dwBase + $Offset, "0xC744242000000000", "Byte[8]")
EndFunc   ;==>DisableHostIDHook

Func Call_Kick($iTargetPID, $iTargetSID, $HostID, $Mode = 1)
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

	$NewCrash = IniRead($Settings, "Kick", "NewCrash", 0)
	$Method = "FF"
	If $NewCrash = 0 Then $Method = "10"

	WriteProcessMemory($hHandle, $dwBuffer, -1, "int")

	;Setting HostID Hook
	If $Mode = 1 Then
		HookHostID($HostID, $off_HostIDHook, $dwHHook)
		HookHostID($HostID, $off_HostIDHook2, $dwHHook2)
	EndIf

	;Setting BindStruct
	Local $wByte = "0x"
	;$wByte &= _StringRepeat("02", 14)
	$wByte &= Byte_Reverse($iTargetPID)
	$wByte &= Byte_Reverse($iTargetSID)
	$wByte &= Byte_Reverse(0x00000539)
	WriteProcessMemory($hHandle, $dwBindStruct + 14, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")

	;Setting MCall
	$wByte = "0x6A006A0068"
	$wByte &= Byte_Reverse($dwBindStruct) & "6A" & $Method & "68"
	$wByte &= Byte_Reverse($iTargetSID) & "68"
	$wByte &= Byte_Reverse($iTargetPID) & "E8"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_GetInstanceCRoom, $dwMFunction + 0x15)) & "8BC8E8"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_SendBind, $dwMFunction + 0x1C)) & "A3"
	$wByte &= Byte_Reverse($dwBuffer) & "C3"
	$ret = WriteProcessMemory($hHandle, $dwMFunction, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")
	If $ret <> 0 Then
		Return 0
	EndIf

	DllCall("kernel32.dll", "int", "CreateRemoteThread", "int", $hHandle, "ptr", 0, "int", 0, "int", $dwMFunction, "ptr", 0, "int", 0, "int", 0)

	Do
		$Read = ReadProcessMemory($hHandle, $dwBuffer, "int")
	Until $Read <> -1

	If $Mode = 1 Then
		Sleep($HostIDSleep)
		DisableHostIDHook($off_HostIDHook)
		DisableHostIDHook($off_HostIDHook2)
	EndIf
EndFunc   ;==>Call_Kick
#EndRegion BindKick
