#RequireAdmin
#include <Process.au3>
#include <ButtonConstants.au3>
#include <GUIConstantsEx.au3>
#include <WindowsConstants.au3>
#Region ### START Koda GUI section ### Form=
$Form1 = GUICreate("1337", 133, 117, 192, 124)
$Button1 = GUICtrlCreateButton("Step1", 24, 24, 75, 25)
$Button2 = GUICtrlCreateButton("Step2", 24, 64, 75, 25)
GUISetState(@SW_SHOW)
#EndRegion ### END Koda GUI section ###

While 1
	$nMsg = GUIGetMsg()
	Switch $nMsg
		Case $GUI_EVENT_CLOSE
			Exit
		 Case $Button1
			 _GetPrivilege_SEDEBUG()
			;_NtSuspendProcess("BEService.exe")
			_ProcessSuspend("BEService.exe")
		Case $Button2
_RunDos("taskkill /F /IM BEService.exe")
ShellExecute(@ScriptDir & "\S4Client_BE.exe","1")
Sleep(1150)
_RunDos("taskkill /F /IM S4Client_BE.exe")
_RunDos("taskkill /F /IM BEService.exe")
	EndSwitch
 WEnd

#cs
 Func _NtSuspendProcess($hProc)
	Local $aCall = DllCall("ntdll.dll", "int", "NtSuspendProcess", "handle", $hProc)
    If Not NT_SUCCESS($aCall[0]) Then
        Return SetError(1, 0, $aCall[0])
	Else
		Return True
	EndIf
 EndFunc

 Func NT_SUCCESS($status)
    If 0 <= $status And $status <= 0x7FFFFFFF Then
        Return True
    Else
        Return False
    EndIf
 EndFunc
#ce

 Func _GetPrivilege_SEDEBUG()
    Local $return = False
    Local $tagLUIDANDATTRIB = "int64 Luid;dword Attributes"
    Local $count = 1
    Local $tagTOKENPRIVILEGES = "dword PrivilegeCount;byte LUIDandATTRIB[" & $count * 12 & "]" ; count of LUID structs * sizeof LUID struct
    Local $TOKEN_ADJUST_PRIVILEGES = 0x20
    Local $SE_PRIVILEGE_ENABLED = 0x2
    Local $curProc = DllCall("kernel32.dll", "ptr", "GetCurrentProcess")
    If @error Then Return False
    Local $call = DllCall("advapi32.dll", "int", "OpenProcessToken", "ptr", $curProc[0], "dword", $TOKEN_ADJUST_PRIVILEGES, "ptr*", 0)
    If (@error Or (Not $call[0])) Then Return False
    Local $hToken = $call[3]
    $call = DllCall("advapi32.dll", "int", "LookupPrivilegeValue", "ptr", 0, "str", "SeDebugPrivilege", "int64*", 0)
    If ((Not @error) And $call[0]) Then
        Local $iLuid = $call[3]
        Local $TP = DllStructCreate($tagTOKENPRIVILEGES)
        Local $LUID = DllStructCreate($tagLUIDANDATTRIB, DllStructGetPtr($TP, "LUIDandATTRIB"))
        DllStructSetData($TP, "PrivilegeCount", $count)
        DllStructSetData($LUID, "Luid", $iLuid)
        DllStructSetData($LUID, "Attributes", $SE_PRIVILEGE_ENABLED)
        $call = DllCall("advapi32.dll", "int", "AdjustTokenPrivileges", "ptr", $hToken, "int", 0, "ptr", DllStructGetPtr($TP), "dword", 0, "ptr", 0, "ptr", 0)
        If Not @error Then $return = ($call[0] <> 0) ; $call[0] <> 0 is success
    EndIf
    DllCall("kernel32.dll", "int", "CloseHandle", "ptr", $hToken)
    Return SetError(Number(Not $return), 0, $return)
EndFunc   ;==>_GetPrivilege_SEDEBUG

Func _ProcessSuspend($process)
$processid = ProcessExists($process)
If $processid Then
    $ai_Handle = DllCall("kernel32.dll", 'int', 'OpenProcess', 'int', 0x1f0fff, 'int', False, 'int', $processid)
    $i_sucess = DllCall("ntdll.dll","int","NtSuspendProcess","int",$ai_Handle[0])
    DllCall('kernel32.dll', 'ptr', 'CloseHandle', 'ptr', $ai_Handle)
    If IsArray($i_sucess) Then
        Return 1
    Else
        SetError(1)
        Return 0
    Endif
Else
    SetError(2)
    Return 0
Endif
EndFunc