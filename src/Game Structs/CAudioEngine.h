// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "CAudioManager.h"


#pragma pack(push, 1)
struct CAudioEngine	// sizeof = 0x4BC
{
	BYTE field_0[672];
	CAudioManager audioManager;
};
#pragma pack(pop)

static_assert(sizeof(CAudioEngine) == 0x4BC, "Incorrect struct size: CAudioEngine");