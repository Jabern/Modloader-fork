// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>


#pragma pack(push, 1)
struct CAnimationStyleDescriptor	// sizeof = 0x30
{
	BYTE name[16];
	BYTE baseName[16];
	DWORD field_20;
	DWORD nameCount;
	DWORD namesPtr;
	DWORD dataPtr;
};
#pragma pack(pop)

static_assert(sizeof(CAnimationStyleDescriptor) == 0x30, "Incorrect struct size: CAnimationStyleDescriptor");