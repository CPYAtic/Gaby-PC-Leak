#pragma once

#include <iostream>
#include <Windows.h>
#include <ctime>
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#include <dwmapi.h>
#include <intrin.h>
#include <ole2.h>
#include <comutil.h>
#include <oleauto.h>
#include <wbemidl.h>
#include <comdef.h>
#include <functional>
#include <shlobj.h> 
#include <iphlpapi.h>
#include <fileapi.h>
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "wbemuuid.lib")
#include <vector>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <random>
#include <Tlhelp32.h>
#include <winbase.h>
#define M_RADPI 57.295779513082f
#define M_PI 3.14159265358979323846
#define M_2PI 6.28318530717958647692 
#include "detours.h"
#pragma comment(lib, "detours.lib" )
#define Deg2Rad(x) 0.01745329251f * x
#include <psapi.h>
#include <string>
#include <shlobj.h> //for config stuff
#include<sstream>
#include <chrono>
#include <time.h>  
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_dx9.h"
#include "ImGui\imgui_internal.h"
typedef unsigned char Byte;
typedef unsigned short Offset;
typedef unsigned long Address;
typedef void* Pointer;
typedef unsigned long Size;
typedef unsigned long Count;
typedef unsigned long Protection;
typedef unsigned long Value;

struct Point { int x, y; };
#include "RWM.h"
#include "Enums.h"
#include "Config\Config.h"
#include "GAME_SDK.h"
#include "Protections\cx_fnv1.h"
#include "Protections\cx_pcg32.h"
#include "Protections\cx_strenc.h"
#include "Drawing.h"
#include "Aimbot.h"
#include "Visuals.h"
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#ifndef IM_ASSERT
#include <assert.h>
#define IM_ASSERT(_EXPR)    assert(_EXPR)
#endif

#define interval_per_tick 1.0f / 60
