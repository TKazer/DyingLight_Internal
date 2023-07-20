#pragma once
#include <windows.h>
#include "Function.h"
#include "MemorySearch.hpp"
#include "Game.h"
#include "VEH.hpp"

namespace Memory
{
	inline byte OldHealthBytes[8]{};
	inline bool LockHealthFlag = false;

	inline byte OldBulletBytes[5]{};
	inline bool BulletLockFlag = false;

	inline VEH::DrHook BulletTrackHook;
	inline bool BulletTrackFlag = false;
	inline Vec3 BulletTrackPos{0, 0, 0};
	inline DWORD64 BulletTrackAddress;

	// Functions

	// �ӵ�׷��
	void InitBulletTrackHook();
	void UnHookBulletTrack();
	void SetBulletTrackPos(Vec3 Pos);
	
	// ��Ѫ(δ����)
	void LockHealth();
	void UnLockHealth();

	// �����ӵ�
	void SetAmmo(Engine::InventoryContainerDI* pInventoryContainerDI, DWORD Count);
	void LockBullet();
	void UnLockBullet();

	// ����(δ����)
	void SpeedUp(float Speed);
	// ����(δ����)
	void HighJump(float Height);
	// ��������ʱ��
	void SetTime(float Time);

}