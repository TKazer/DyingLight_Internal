#pragma once
#include <windows.h>
#include "Struct.h"
#include "Offset.h"

class GameData
{
public:

	GameData();

	DWORD64 Module_Engine = 0;
	DWORD64 Module_GameDLL = 0;

}*pGameData;

GameData::GameData()
{
	this->Module_Engine = (DWORD64)GetModuleHandleA("engine_x64_rwdi.dll");
	this->Module_GameDLL = (DWORD64)GetModuleHandleA("gamedll_x64_rwdi.dll");
}