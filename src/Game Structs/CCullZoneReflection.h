// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "CCullZoneBase.h"


#pragma pack(push, 1)
struct CCullZoneReflection	// sizeof = 0x18
{
	CCullZoneBase __parent;
	DWORD cm;
	BYTE vx;
	BYTE vy;
	BYTE vz;
	BYTE flags;
};
#pragma pack(pop)

static_assert(sizeof(CCullZoneReflection) == 0x18, "Incorrect struct size: CCullZoneReflection");