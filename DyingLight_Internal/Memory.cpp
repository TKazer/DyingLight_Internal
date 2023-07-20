#include "Memory.h"

void Memory::LockHealth()
{
	DWORD64 HealthAddress = pGameData->Module_GameDLL + 0x9FB160;
	byte ShellCode[]{ 0x90,0x90 ,0x90 ,0x90 ,0x90 ,0x90 ,0x90 ,0x90 };
	DWORD OldProtect = 0;
	VirtualProtect((LPVOID)HealthAddress, 8 * sizeof(byte), PAGE_EXECUTE_READWRITE, &OldProtect);

	ZeroMemory(OldHealthBytes, 8 * sizeof(byte));
	Function::Read(HealthAddress, OldHealthBytes, 8 * sizeof(byte));

	Function::Write(HealthAddress, ShellCode, 8 * sizeof(byte));
}

void Memory::UnLockHealth()
{
	DWORD64 HealthAddress = pGameData->Module_GameDLL + 0x9FB160;
	Function::Write(HealthAddress, *OldHealthBytes, 8 * sizeof(byte));
}


void Memory::SetAmmo(Engine::InventoryContainerDI* pInventoryContainerDI,DWORD Count)
{
	if (pInventoryContainerDI)
	{
		auto InventoryAmmo_ = pInventoryContainerDI->InventoryAmmo_;
		InventoryAmmo_->InventoryList.pItems[0].Ammo = 0;
		for (int i = 0; i < InventoryAmmo_->InventoryList.ItemCount; i++)
		{
			InventoryAmmo_->InventoryList.At(i).Ammo = Count;
		}
	}
}

void Memory::LockBullet()
{
	DWORD64 BulletAddress = pGameData->Module_GameDLL + 0xDA168F;
	byte ShellCode[]{ 0x90,0x90 ,0x90 ,0x90 ,0x90 };
	DWORD OldProtect = 0;
	VirtualProtect((LPVOID)BulletAddress, 5 * sizeof(byte), PAGE_EXECUTE_READWRITE, &OldProtect);

	ZeroMemory(OldBulletBytes, 5 * sizeof(byte));
	Function::Read(BulletAddress, OldBulletBytes, 5 * sizeof(byte));

	Function::Write(BulletAddress, ShellCode, 5 * sizeof(byte));
}

void Memory::UnLockBullet()
{
	DWORD64 BulletAddress = pGameData->Module_GameDLL + 0xDA168F;
	Function::Write(BulletAddress, OldBulletBytes, 5 * sizeof(byte));
}

void Memory::SpeedUp(float Speed)
{
	DWORD64 SpeedAddress = 0;

	SpeedAddress = Function::Read<DWORD64>(reinterpret_cast<DWORD64>(pGame->pLevelDI), sizeof(DWORD64), { 0x8,0x770,0x280 });

	if (SpeedAddress == 0)
		return;

	for (int i = 0; i < 13; i++)
	{
		DWORD64 Address = 0;

		if (Function::BadReadPtr(SpeedAddress + i * 0x8))
			continue;
	
		Address = *reinterpret_cast<DWORD64*>(SpeedAddress + i * 0x8);

		if (Function::BadWritePtr(Address + 0x3F0, sizeof(float)))
			continue;

		*reinterpret_cast<float*>(Address + 0x3F0) = Speed;
	}
}

void Memory::HighJump(float Height)
{
	DWORD64 HighJumpAddress = 0;
	HighJumpAddress = Function::Read<DWORD64>(reinterpret_cast<DWORD64>(pGame->pLevelDI), sizeof(DWORD64), { 0x8,0x770,0x280 });

	for (int i = 0; i < 3; i++)
	{
		DWORD64 Address = 0;

		if (Function::BadReadPtr(HighJumpAddress + i * 0x8))
			continue;
		Address = *reinterpret_cast<DWORD64*>(HighJumpAddress + i * 0x8);

		if (Function::BadReadPtr(Address + 0x710))
			continue;
		Address = *reinterpret_cast<DWORD64*>(Address + 0x710);

		if (Function::BadWritePtr(Address + 0xDC, sizeof(float)))
			continue;

		Function::Write<float>(Address + 0xDC, Height, sizeof(float));
	}
}

void Memory::SetTime(float Time)
{
	DWORD64 TimeAddress = 0;
	TimeAddress = Function::Read<DWORD64>(reinterpret_cast<DWORD64>(pGame->pLevelDI), sizeof(DWORD64), { 0x9F0,0x318,-8 }) + 0xA4;

	if (Time >= 0.0f && Time <= 1.0f)
	{
		*reinterpret_cast<float*>(TimeAddress) = Time;
	}
}

LONG NTAPI Handler(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP)
	{
		if ((DWORD64)ExceptionInfo->ExceptionRecord->ExceptionAddress == Memory::BulletTrackAddress)
		{
			DWORD64 rcx = ExceptionInfo->ContextRecord->Rcx;
			*(Vec3*)(rcx + 0x60) = Memory::BulletTrackPos;
			ExceptionInfo->ContextRecord->Rip += 3;
		}
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

void Memory::InitBulletTrackHook()
{
	BulletTrackPos = { 0.f,0.f,0.f };
	BulletTrackAddress = pGameData->Module_GameDLL + 0x71283f;
	BulletTrackHook.Hook({ BulletTrackAddress }, Handler);
}

void Memory::UnHookBulletTrack()
{
	BulletTrackHook.UnHook();
}

void Memory::SetBulletTrackPos(Vec3 Pos)
{
	BulletTrackPos = Pos;
}
