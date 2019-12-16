#pragma once

#include <iostream>
#include <Windows.h>
#include <ctime>
#include <vector>
#include <psapi.h>
#include <string>
#include <stdio.h>
#include <process.h>
#include <Winternl.h>

typedef unsigned char Byte;
typedef unsigned short Offset;
typedef unsigned long Address;
typedef void* Pointer;
typedef unsigned long Size;
typedef unsigned long Count;
typedef unsigned long Protection;
typedef unsigned long Value;

struct Point { int x, y; };