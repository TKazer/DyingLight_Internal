#pragma once
#include <windows.h>
#include "Bytes.hpp"
#include "Game.h"

namespace Memory
{
	byte OldHealthBytes[8]{};
	bool LockHealthFlag = false;

	byte OldBulletBytes[5]{};
	bool BulletLockFlag = false;

	// Functions

	void LockHealth();
	void UnLockHealth();

	void LockBullet();
	void UnLockBullet();

	void SpeedUp(float Speed);

	void HighJump(float Height);

	void SetTime(float Time);
}