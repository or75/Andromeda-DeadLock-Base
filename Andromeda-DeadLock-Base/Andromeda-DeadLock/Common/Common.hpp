#pragma once

#include <Windows.h>
#include <cstdint>

#include "Include/Config.hpp"
#include "Include/XorStr/XorStr.hpp"

#include "DevLog.hpp"

#if ENABLE_XOR_STR == 1

#define XorStr(str) xorstr_(str)

#else

#define XorStr(str) str

#endif

#if ENABLE_XOR_VMP_STR == 1

#include "Include/VMProtect/VMProtectSDK.h"

#define VmpStr(str) VMProtectDecryptStringA(str)

#else

#define VmpStr(str) str

#endif

#define IS_KEY_PRESS( KeyNum ) ( GetAsyncKeyState(KeyNum) & 0x1 )
#define IS_KEY_DOWN( KeyNum ) ( GetKeyState(KeyNum) & 0x100 )

#define CS_CLASS_NO_ASSIGNMENT(CLASS)	\
	CLASS& operator=(CLASS&&) = delete;		\
	CLASS& operator=(const CLASS&) = delete;
