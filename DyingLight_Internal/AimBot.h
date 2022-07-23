#pragma once
#include <windows.h>
#include "Struct.h"
#include "Function.h"

namespace AimControl
{
	int HotKey = VK_RBUTTON;
	static float AimRange = 300;

	void AimBot(Vec3 AimPos, Vec3 CameraPos, float& Yaw, float& Pitch)
	{
		if (AimPos == 0)
			return;
		Rotator Angle = Function::CalcAngle(CameraPos, AimPos);
		Yaw = Angle.Yaw;
		Pitch = Angle.Pitch;
	}
}