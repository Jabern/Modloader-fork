// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>


#pragma pack(push, 1)
struct CBaseModelInfo_VTBL	// sizeof = 0x3C
{
	DWORD Destructor;
	DWORD AsAtomicModelInfoPtr;
	DWORD AsDamageAtomicModelInfoPtr;
	DWORD AsLodAtomicModelInfoPtr;
	DWORD GetModelType;
	DWORD GetTimeInfo;
	DWORD Init;
	DWORD Shutdown;
	DWORD DeleteRwObject;
	DWORD GetRwModelType;
	DWORD CreateInstance_;
	DWORD CreateInstance;
	DWORD SetAnimFile;
	DWORD ConvertAnimFileIndex;
	DWORD GetAnimFileIndex;
};
#pragma pack(pop)

static_assert(sizeof(CBaseModelInfo_VTBL) == 0x3C, "Incorrect struct size: CBaseModelInfo_VTBL");