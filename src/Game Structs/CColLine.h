// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "RwV3D.h"


#pragma pack(push, 1)
struct CColLine	// sizeof = 0x1C
{
	RwV3D Point1;
	DWORD field_C;
	RwV3D Point2;
};
#pragma pack(pop)

static_assert(sizeof(CColLine) == 0x1C, "Incorrect struct size: CColLine");