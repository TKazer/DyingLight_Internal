#pragma once
#include <windows.h>
#include "OwnImGui.h"
#include "Game.h"
#include "Object.h"
#include "Bone.h"
#include "Function.h"

namespace Render
{
	void AABB(Object& Obj, ImColor Color, float ThickNess);
	void DrawBone(Object& Obj, ImColor Color, float ThickNess, bool View = true);
	void DrawLosLine(Object& Obj, float Length, ImColor Color, float ThickNess);
}