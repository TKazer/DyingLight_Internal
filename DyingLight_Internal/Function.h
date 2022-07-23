#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <Windows.h>
#include <math.h>
#include "Struct.h"
#include "Bytes.hpp"

namespace Function
{
	// Jmp跳转
	Bytes JMPCode(DWORD64 DestAddress, int NopNum);

	// 旋转坐标系
	Vec2 RevolveCoordinatesSystem(float RevolveAngle, Vec2 OriginPos, Vec2 DestPos);

	// 特征搜索
	DWORD64 FindPattern(DWORD64 dwModule, BYTE* bMask, std::string szMask);

	// 计算3d坐标距离
	float CalcPosOppDis(Vec3 OriginPos, Vec3 DestPos);

	// 计算角度
	Rotator CalcAngle(Vec3 OriginPos, Vec3 DestPos);

	// 读写

	template <typename Type>
	Type Read(DWORD64 Address,int Size)
	{
		Type Buffer;
		ZeroMemory(&Buffer, Size);
		if (!IsBadHugeReadPtr(reinterpret_cast<PVOID>(Address), Size))
			memcpy_s(&Buffer, Size, reinterpret_cast<PVOID>(Address), Size);
		return Buffer;
	}

	template <typename Type>
	void Read(DWORD64 Address,Type& Value, int Size)
	{
		memcpy_s(reinterpret_cast<LPVOID>(Value), Size, reinterpret_cast<PVOID>(Address), Size);
	}

	template <typename Type>
	void Write(DWORD64 Address, Type Value,int Size)
	{
		memcpy_s(reinterpret_cast<PVOID>(Address), Size, &Value, Size);
	}

	template <typename Type>
	bool BadReadPtr(Type Ptr)
	{
		return IsBadReadPtr(reinterpret_cast<PVOID>(Ptr), sizeof(Type));
		//return (bool)(IsBadHugeReadPtr(reinterpret_cast<PVOID>(Ptr), sizeof(Type)) || (DWORD64)Ptr < 0xFFFF || (DWORD64)Ptr > 0x7FFFFFFFFFFF || (DWORD64)Ptr % sizeof(DWORD64));
	}

	template <typename Type>
	bool BadReadPtr(Type Ptr,int Size)
	{
		return IsBadReadPtr(reinterpret_cast<PVOID>(Ptr), Size);
		//return (bool)(IsBadHugeReadPtr(reinterpret_cast<PVOID>(Ptr), Size) || (DWORD64)Ptr < 0xFFFF || (DWORD64)Ptr > 0x7FFFFFFFFFFF || (DWORD64)Ptr % sizeof(DWORD64));
	}

	template <typename Type>
	bool BadWritePtr(Type Ptr,int Size)
	{
		return (bool)(IsBadHugeWritePtr(reinterpret_cast<PVOID>(Ptr), Size) || (DWORD64)Ptr < 0xFFFF || (DWORD64)Ptr > 0x7FFFFFFFFFFF);
	}

	template <typename Type>
	Type Read(DWORD64 Address,int Size,std::initializer_list<int> OffsetList)
	{
		Type Buffer;
		ZeroMemory(&Buffer, Size);
		DWORD64 TempAddress = Address;
		if (BadReadPtr(Address))
			return Type{};
		for (auto Offset = OffsetList.begin(); Offset != OffsetList.end(); ++Offset)
		{
			if (Offset == OffsetList.end() - 1)
			{
				if (BadReadPtr(TempAddress + *Offset,Size))
					return Type{};
				memcpy_s(&Buffer, Size, reinterpret_cast<PVOID>(TempAddress + *Offset), Size);
			}
			else
			{
				if (BadReadPtr(TempAddress + *Offset))
					return Type{};
				TempAddress = *reinterpret_cast<DWORD64*>(TempAddress + *Offset);
			}
		}
		return Buffer;
	}
}