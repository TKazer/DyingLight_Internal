#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include "Engine.h"

class Object
{
public:
	Object(Engine::IModelObject* ModelObj_) :ModelObj(ModelObj_) {};

	Engine::IModelObject* ModelObj = nullptr;

	std::string Name = "";

	/*
		0-Normal Zombie
		1-Super  Zombie
		2-Survivor
		3-Player
	*/
	int Type = 0;

	Vec3 Pos{ 0,0,0 };
	Vec3 ScreenPos{ 0,0,0 };

	float Yaw = 0;

	float CurrentHealth = 0;
	float MaxHealth = 0;

	int Logo = 0;
	int Active = 0;
	
	std::vector<Vec3>BonePosList;
	
	// Function
	bool Update();
	Vec3 GetBonePos(int Index);
};
