#include "Function.h"

Bytes Function::JMPCode(DWORD64 DestAddress, int NopNum)
{
	Bytes Result;
	Result = Bytes("FF 25 00 00 00 00") + Bytes(static_cast<long long>(DestAddress));
	for (int i = 0; i < NopNum; i++)
		Result.Add(0x90);
	return Result;
}

Rotator Function::CalcAngle(Vec3 OriginPos, Vec3 DestPos)
{
	Rotator Result;
	Vec3 OppPos;
	float Distance = 0;

	OppPos.x = DestPos.x - OriginPos.x;
	OppPos.y = DestPos.z - OriginPos.z;
	OppPos.z = DestPos.y - OriginPos.y;

	Distance = sqrt(pow(OppPos.x, 2) + pow(OppPos.y, 2));

	Result.Yaw = atan(OppPos.y / OppPos.x) * 180 / M_PI;
	Result.Pitch = atan(OppPos.z / Distance) * 180 / M_PI;
	Result.Roll = 0;

	if (Result.Yaw < 0 && OppPos.y > 0)
		Result.Yaw += 180;
	if (Result.Yaw > 0 && OppPos.y < 0)
		Result.Yaw -= 180;

	return Result;
}

float Function::CalcPosOppDis(Vec3 OriginPos, Vec3 DestPos)
{
	Vec3 OppPos;
	float Distance = 0;

	OppPos.x = DestPos.x - OriginPos.x;
	OppPos.y = DestPos.y - OriginPos.y;
	OppPos.z = DestPos.z - OriginPos.z;

	Distance = sqrt(pow(OppPos.x, 2) + pow(OppPos.y, 2) + pow(OppPos.z, 2));
	return Distance;
}

Vec2 Function::RevolveCoordinatesSystem(float RevolveAngle, Vec2 OriginPos, Vec2 DestPos)
{
	Vec2 ResultPos;

	if (RevolveAngle == 0)
		return DestPos;

	ResultPos.x = OriginPos.x + (DestPos.x - OriginPos.x) * cos(RevolveAngle * M_PI / 180) + (DestPos.y - OriginPos.y) * sin(RevolveAngle * M_PI / 180);
	ResultPos.y = OriginPos.y - (DestPos.x - OriginPos.x) * sin(RevolveAngle * M_PI / 180) + (DestPos.y - OriginPos.y) * cos(RevolveAngle * M_PI / 180);
	return ResultPos;
}

BOOL Compare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return FALSE;
	return (*szMask) == NULL;
}

DWORD64 Function::FindPattern(DWORD64 dwModule, BYTE* bMask, std::string szMask)
{
	PIMAGE_DOS_HEADER pImageDos = (PIMAGE_DOS_HEADER)dwModule;

	PIMAGE_NT_HEADERS32 pImageNt = (PIMAGE_NT_HEADERS32)(dwModule + pImageDos->e_lfanew);

	ULONG ImageLen = pImageNt->OptionalHeader.SizeOfImage;

	INT MaskLen = szMask.length();

	for (ULONG i = 0; i < ImageLen - MaskLen; i++)
	{
		if (IsBadHugeReadPtr((PVOID)(dwModule + i), 1) == 0)
		{
			if (Compare((BYTE*)(dwModule + i), bMask, szMask.c_str()))
			{
				return (DWORD64)(dwModule + i);
			}
		}
	}

	return 0;
}