#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "..\src\lua.h"

#define DEBUG_PRINT (FALSE)

typedef void(__stdcall* lpOut32)(short, short);
typedef short(__stdcall* lpInp32)(short);
typedef BOOL(__stdcall* lpIsInpOutDriverOpen)(void);
typedef BOOL(__stdcall* lpIsXP64Bit)(void);

extern lpOut32 gfpOut32;
extern lpInp32 gfpInp32;
extern lpIsInpOutDriverOpen gfpIsInpOutDriverOpen;
extern lpIsXP64Bit gfpIsXP64Bit;

void InstallInpOut32(lua_State* L);
