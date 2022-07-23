#pragma once
#include <Windows.h>

namespace MenuConfig
{
	static bool Box = true;
	static int  BoxType = 2;
	static bool Bone = true;

	static bool Fov = true;

	static float BoxColor_Zombie[]{ 0.66,0.66,0.66,0.9 };
	static float BoxColor_SpZombie[]{ 1,0,0,0.9 };
	static float BoxColor_Survivor[]{ 0.11,0.56,1,0.9 };
	static float BoneColor[]{ 0.58, 0.58, 0.58,0.86 };

	static bool AimBot = true;
	static int  AimType = 0;
	static bool ShowAimRange = true;
	static float AimRangeColor[]{ 1, 1, 1,1 };

	static bool BulletTrack = false;

	static bool LockHealth = true;
	static bool LockBullet = false;

	static bool SpeedUp = false;
	static float SpeedNum = 2;

	static bool DebugText = false;

	static bool LockAmmo = false;
	static int  AmmoNum = 100;

	static bool HighJump = false;

	static bool SetTime = false;
	static float Time = 0.5;

	static bool LosLine = true;
	static float LosLineLength = 1.5;
}