// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "CAudioFile.h"
#include "ov_callbacks.h"


#pragma pack(push, 1)
struct CAudioFileVorbis	// sizeof = 0x28
{
	CAudioFile __parent;
	ov_callbacks callbacks;
	DWORD vf;
	BYTE initialized;
	BYTE field_1D;
	BYTE field_1E;
	BYTE field_1F;
	DWORD datasource;
	DWORD vorbisInfo;
};
#pragma pack(pop)

static_assert(sizeof(CAudioFileVorbis) == 0x28, "Incorrect struct size: CAudioFileVorbis");