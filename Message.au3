#include <Array.au3>
Func WriteAll()
	$Message = InputBox("Enter Message", "GIMME A MESSAGE")
	If Prep_MessageCall($Message) = 0 Then Return 0
	Hook_WriteAs(1337)
	For $i = 1 To $Array_PlayerID[0]
		;WriteWriteAsPlayerNumb
		WriteProcessMemory($hHandle, $dwBuffer, "0", "int")
		WriteProcessMemory($hHandle, $dwWA_View + 2, $Array_PlayerID[$i], "int")
		WriteProcessMemory($hHandle, $dwWA_Send + 2, $Array_PlayerID[$i], "int")
		DllCall("kernel32.dll", "int", "CreateRemoteThread", "int", $hHandle, "ptr", 0, "int", 0, "int", $dwMFunction, "ptr", 0, "int", 0, "int", 0)
		Do
			$Read = ReadProcessMemory($hHandle, $dwBuffer, "int")
		Until $Read <> 0
		Sleep($BroadcastSleep)
	Next
	DisableWriteAs()
EndFunc

Func Prep_MessageCall($Message)
	WriteProcessMemory($hHandle, $dwMessageBuffer, $Message, "char[" & StringLen($Message) + 1 & "]")

	Local $wByte = "0x68"
	$wByte &= Byte_Reverse($dwMessageBuffer) & "8B0D"
	$wByte &= StringReplace(Byte_Reverse($dwBase + $off_CChatProxy), "0x", "") & "E8"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_SendMessage, $dwMFunction + 0xB)) & "A3"
	$wByte &= Byte_Reverse($dwBuffer) & "C3"
	$ret = WriteProcessMemory($hHandle, $dwMFunction, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")
	If $ret <> 0 Then Return 0
	Return 1
EndFunc

Func WriteAs()
	Hook_WriteAs(0)
EndFunc

Func DisableWriteAs()
	;View
	Local $wByte = "0x89118B45A4"
	WriteProcessMemory($hHandle, $dwBase + $off_WriteAs_ViewHook, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")

	;Write
	Local $wByte = "0x89088B55A4"
	WriteProcessMemory($hHandle, $dwBase + $off_WriteAs_WriteHook, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")
	ContextMenu(0)
EndFunc   ;==>DisableWriteAs

Func SendMessage()
	$Message = GUICtrlRead($Input_Message)
	$Prefix = SelectedPrefix()
	Call_SendFakeMessage($Message, $Prefix)
	GUICtrlSetData($Input_Message, "")
	$LatestMessage = $Message
EndFUnc

Func Hook_WriteAs($iPlayerID = 0)
	#cs
		WriteAs Hook Struct - View
		Hook:
		E9 <Byterev: Calc: page>

		Page:
		C7 01 <Byterev: PlayerID>
		8B 45 A4
		E9 <Byterev: Calc: Hook + 0x5>

		WriteAs Hook Struct - Send
		Hook: E9 <Byterev: Calc: page>

		Page:
		C7 00 <Byterev: PlayerID>
		8B 55 A4
		E9 <Byterev: Calc: Hook + 0x5>
	#ce

	If $iPlayerID = 0 Then
		$iPlayerID = GetSelectedPlayerID()
		If $iPlayerID = 0 Then Return 0
	EndIf

	;<--- View Hook --->
	;Codecave:
	Local $wByte = "0xC701"
	$wByte &= Byte_Reverse($iPlayerID)
	$wByte &= "8B45A4E9"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_WriteAs_ViewHook + 0x5, $dwWA_View + 0x9))
	WriteProcessMemory($hHandle, $dwWA_View, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")

	;Hook(jmp)
	$wByte = "0xE9" & Byte_Reverse(Calc($dwWA_View, $dwBase + $off_WriteAs_ViewHook))
	WriteProcessMemory($hHandle, $dwBase + $off_WriteAs_ViewHook, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")


	;<--- Write Hook --->
	;Codecave:
	$wByte = "0xC700"
	$wByte &= Byte_Reverse($iPlayerID)
	$wByte &= "8B55A4E9"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_WriteAs_WriteHook + 0x5, $dwWA_Send + 0x9))
	WriteProcessMemory($hHandle, $dwWA_Send, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")

	;Hook(jmp)
	$wByte = "0xE9" & Byte_Reverse(Calc($dwWA_Send, $dwBase + $off_WriteAs_WriteHook))
	WriteProcessMemory($hHandle, $dwBase + $off_WriteAs_WriteHook, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")
	ContextMenu(1)
EndFunc   ;==>Hook_WriteAs

Func Call_SendFakeMessage($Message, $Prefix = "")
	#cs
		Structure:
		68 <MsgBuffer>
		8B 0D <Byterev: CGameManager>
		E8 <Byterev: Calc: Send>

		NEW:
		A3 <dwBuffer>
		C3
	#ce
	if $hHandle = 0 Then
		MsgBox(16, "error", "Reload all players first!")
	EndIf
	;Push Hook
	WriteProcessMemory($hHandle, $dwBuffer, "0", "int")
	WriteProcessMemory($hHandle, $dwBase + $off_ViewPush, "0x02", "Byte[1]")
	WriteProcessMemory($hHandle, $dwBase + $off_WritePush, "0x02", "Byte[1]")


	Local $wMessage = $Prefix
	$wMessage &= $Message
	$wMessage &= $Spaces
	WriteProcessMemory($hHandle, $dwMessageBuffer, $wMessage, "char[" & StringLen($wMessage) + 1 & "]")

	Local $wByte = "0x68"
	$wByte &= Byte_Reverse($dwMessageBuffer) & "8B0D"
	$wByte &= StringReplace(Byte_Reverse($dwBase + $off_CChatProxy), "0x", "") & "E8"
	$wByte &= Byte_Reverse(Calc($dwBase + $off_SendMessage, $dwMFunction + 0xB)) & "A3"
	$wByte &= Byte_Reverse($dwBuffer) & "C3"
	$ret = WriteProcessMemory($hHandle, $dwMFunction, $wByte, "Byte[" & StringLen($wByte) / 2 - 1 & "]")
	If $ret <> 0 Then
		Return 0
	EndIf

	DllCall("kernel32.dll", "int", "CreateRemoteThread", "int", $hHandle, "ptr", 0, "int", 0, "int", $dwMFunction, "ptr", 0, "int", 0, "int", 0)

	Do
		$Read = ReadProcessMemory($hHandle, $dwBuffer, "int")
	Until $Read <> 0

	WriteProcessMemory($hHandle, $dwBase + $off_ViewPush, "0x0D", "Byte[1]")
	WriteProcessMemory($hHandle, $dwBase + $off_WritePush, "0x0D", "Byte[1]")
EndFunc