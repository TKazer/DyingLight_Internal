#pragma once
#include <windows.h>
#include "Struct.h"
#include "Offset.h"
#include "MemorySearch.hpp"
#include "Debug.hpp"

class GameData
{
public:

	GameData();

	// 初始获取数据偏移
	bool InitOffsets();

	DWORD64 Module_Engine = 0;
	DWORD64 Module_GameDLL = 0;

}*pGameData;

GameData::GameData()
{
	this->Module_Engine = (DWORD64)GetModuleHandleA("engine_x64_rwdi.dll");
	this->Module_GameDLL = (DWORD64)GetModuleHandleA("gamedll_x64_rwdi.dll");
}

bool GameData::InitOffsets()
{
	std::optional<DWORD64> Temp = 0;

	Temp = MemorySearch::Search("?? 8B ?? ?? 8B ?? 41 FF 90 98 07 00 00 8B 50 40 48 8D 48 40 FF CA", this->Module_GameDLL);
	if (Temp.has_value())
	{
		Offset::BulletCutDown = Temp.value() + 0x16 - this->Module_GameDLL;
	}
	else
		return false;

	Temp = MemorySearch::Search("8B 41 ?? 89 41 ?? 8B 41 ?? 89 41 ?? 8B 41 ?? 89 41 ?? 8B 41 ?? 89 41 ??", this->Module_GameDLL);
	if (Temp.has_value())
	{
		Offset::BulletTrack = Temp.value() - 0x8 - this->Module_GameDLL;
	}
	else
		return false;

	Temp = MemorySearch::Search("48 83 EC 50 48 8B 05 ?? ?? ?? ?? 49 8B F8 48 8B ?? ", this->Module_Engine);
	if (Temp.has_value())
	{
		DWORD64 Address = Temp.value() + 4;
		Address += *reinterpret_cast<DWORD*>(Address + 3) + 7;
		Offset::CGame = Address - this->Module_Engine;
	}
	else
		return false;

	Debug::Log("[BulletCutDown] Offset:%X", Offset::BulletCutDown);
	Debug::Log("[BulletTrack] Offset:%X", Offset::BulletTrack);
	Debug::Log("[CGame] Offset:%X", Offset::CGame);

	return true;
}