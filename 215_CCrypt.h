#pragma once
#include <Windows.h>
#include <cstring>
class CCrypt
{
public:
	bool Decrypt4(char *_this, char *Src, char *Dst, size_t Size);
	bool Decrypt8(char *_this, char *Src, char *Dst, size_t Size);
	bool Decrypt16(char *_this, char *Src, char *Dst, size_t Size);
	bool Encrypt4(char *_this, char *Src, char *Dst, size_t Size);
	bool Encrypt8(char *_this, char *Src, char *Dst, size_t Size);
	bool Encrypt16(char *_this, char *Src, char *Dst, size_t Size);
};