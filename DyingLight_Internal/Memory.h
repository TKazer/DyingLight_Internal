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

	// 子弹追踪
	void InitBulletTrackHook();
	void UnHookBulletTrack();
	void SetBulletTrackPos(Vec3 Pos);
	
	// 锁血(未更新)
	void LockHealth();
	void UnLockHealth();

	// 无限子弹
	void SetAmmo(Engine::InventoryContainerDI* pInventoryContainerDI, DWORD Count);
	void LockBullet();
	void UnLockBullet();

	// 加速(未更新)
	void SpeedUp(float Speed);
	// 高跳(未更新)
	void HighJump(float Height);
	// 设置世界时间
	void SetTime(float Time);

}