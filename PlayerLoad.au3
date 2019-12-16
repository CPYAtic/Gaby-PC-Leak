Func Reload()
	$iPID = ProcessExists($targetProcess)
	If $iPID = 0 Then
		Error("Unable to get ProcessID of '" & $targetProcess & "'")
		Return 0
	EndIf

	If $iOPID <> $iPID Then
		$hHandle = OpenProcess($PROCESS_ALL_ACCESS, False, $iPID)
		If $hHandle = 0 Then
			Error("Unable to open process")
			Return 0
		EndIf

		$dwBase = _MemoryModuleGetBaseAddress($iPID, $targetProcess)
		If $dwBase = 0 Then
			Error("Unable to get baseaddress")
			Return 0
		EndIf

		$dwMFunction = VirtualAllocEx($hHandle, 0, 128, $MEM_COMMIT, $PAGE_EXECUTE_READWRITE)
		$dwBuffer = VirtualAllocEx($hHandle, 0, 4, $MEM_COMMIT, $PAGE_EXECUTE_READWRITE)
		$dwWA_View = VirtualAllocEx($hHandle, 0, 32, $MEM_COMMIT, $PAGE_EXECUTE_READWRITE)
		$dwWA_Send = VirtualAllocEx($hHandle, 0, 32, $MEM_COMMIT, $PAGE_EXECUTE_READWRITE)
		$dwBindStruct = VirtualAllocEx($hHandle, 0, 32, $MEM_COMMIT, $PAGE_EXECUTE_READWRITE)
		$dwHHook = VirtualAllocEx($hHandle, 0, 32, $MEM_COMMIT, $PAGE_EXECUTE_READWRITE)
		$dwHHook2 = VirtualAllocEx($hHandle, 0, 32, $MEM_COMMIT, $PAGE_EXECUTE_READWRITE)
		$dwMessageBuffer = VirtualAllocEx($hHandle, 0, 4096, $MEM_COMMIT, $PAGE_EXECUTE_READWRITE)
		$iOPID = $iPID

		$AllocStat = True
	EndIf

	LoadSettings()
	;LoadPrefix()
   LoadPlayers()
	;RoomCheck()
EndFunc   ;==>Reload

Func LoadPlayers()
	$dwInstance = GetInstance()
    If $dwInstance = 0 Then Return Error("Unable to get instance - are you even in a room?")

	$dwCTeamManager = ReadProcessMemory($hHandle, $dwBase + $off_CTeamManager, "dword")
	_GUICtrlListView_DeleteAllItems($List_Players)

	$PlayerAmount = 0
	For $i = 0 To 2
		$TeamAmount = Call_GetPlayerCountPerTeam($dwInstance,$i)
		For $x = 0 To $TeamAmount - 1
			$iPlayerID = Call_GetPlayerIDByIndex($dwInstance, $i, $x)
			$dwActorClass = Call_GetActorClass($dwCTeamManager, $iPlayerID)
			$HostID = ReadProcessMemory($hHandle, $dwActorClass + 0x4F0, "int")
			If $dwActorClass = 0 Then
				MsgBox(16, "error", "Actor error: PlayerID exists but Actor doesnt - the fuck is wrong?" & @CRLF & "Skipped player: " & $iPlayerID)
				ContinueLoop
			EndIf
			$sName = GetPlayerName($dwActorClass)
			$sName = StringReplace($sName, "|", ":")

			$MaxHP = Actor_GetMaxHP($dwActorClass)
			$MaxSP = Actor_GetMaxSP($dwActorClass)

			$sID = GetSID($iPlayerID)

			GUICtrlCreateListViewItem($sName & "|" & $iPlayerID & "|" & $HostID & "|" & (Hex($sID,8)) & "|" & $MaxHP, $List_Players)
			$PlayerAmount += 1
			$Array_PlayerID[$PlayerAmount] = $iPlayerID
		Next
	Next
	$Array_PlayerID[0] = $PlayerAmount
EndFunc   ;==>LoadPlayers

Func Call_GetPlayerIDByIndex($ecx, $iTeamID, $iIndex)
	#cs
		Structure:
		6A <index>
		6A <teamID>
		8B 0D <Byterev: Buffer(instance)>
		E8 <Byterev: Calc: GetPlayerIDBy..>
		A3 <Byterev: Buffer(result)>
		C3
	#ce
	Local $wByte = "0x6A"
	If StringLen($iTeamID) <> 1 Then
		Return 0
	EndIf
	If StringLen($iIndex) <> 1 Then
		Return 0
	EndIf

	$ret = WriteProcessMemory($hHandle, $dwBuffer, "0x" & Byte_Reverse($ecx), "Byte[4]")
	If $ret <> 0 Then
		Return 0
	EndIf

	$wByte &= "0" & $iIndex & "6A0" & $iTeamID & "8B0D"
	$wByte &= Byte_Reverse($dwBuffer)
	$wByte &= "E8" & Byte_Reverse(Calc($dwBase + $off_GetPlayerIDByIndex, $dwMFunction + 0xA))
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
EndFunc   ;==>Call_GetPlayerIDByIndex

Func Call_GetPlayerCountPerTeam($ecx, $iTeamID)
	#cs
		Structure:
		6A <teamID>
		8B 0D <Byterev: Buffer(instance)>
		E8 <Byterev: Calc: GetPlayerC..>
		A3 <Byterev: Buffer(result)>
		C3
	#ce
	Local $wByte = "0x6A"
	If StringLen($iTeamID) <> 1 Then
		Return 0
	EndIf

	$ret = WriteProcessMemory($hHandle, $dwBuffer, "0x" & Byte_Reverse($ecx), "Byte[4]")
	If $ret <> 0 Then
		Return 0
	EndIf

	$wByte &= "0" & $iTeamID & "8B0D"
	$wByte &= Byte_Reverse($dwBuffer)
	$wByte &= "E8" & Byte_Reverse(Calc($dwBase + $off_GetPlayerCountPerTeam, $dwMFunction + 0x8))
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
EndFunc   ;==>Call_GetPlayerCountPerTeam