#include "Object.h"
#include "Function.h"

bool Object::Update()
{
	if (this->ModelObj == nullptr)
		return false;
	if (this->ModelObj->Name == nullptr)
		return false;

	this->Active = this->ModelObj->Active;
	if (this->Active == 0)
		return false;

	this->Pos = Vec3{ this->ModelObj->x,this->ModelObj->y,this->ModelObj->z };
	if (Pos == 0)
		return false;

	this->Logo = this->ModelObj->Logo;

	this->Yaw = this->ModelObj->Model->GetYaw();

	// 血量
	//if (this->Logo == 64)
	//{
	//	if (!Function::BadReadPtr(this->ModelObj->Model->Status))
	//	{
	//	if (!Function::BadReadPtr((DWORD64)(this->ModelObj->Model->Status) + 0x78, sizeof(float))
	//			&& !Function::BadReadPtr((DWORD64)(this->ModelObj->Model->Status) + 0xC8, sizeof(float)))
	//		{
	//			this->CurrentHealth = this->ModelObj->Model->Status->CurrentHealth;
	//			this->MaxHealth = this->ModelObj->Model->Status->MaxHealth;
	//		}
	//	}
	//}


	this->Name = this->ModelObj->Name;

	if (Name.find("zombie") == std::string::npos && Name.find("survivor") == std::string::npos&& Name.find("player") == std::string::npos)
		return false;

	Name = Name.substr(0, Name.find('.'));

	// 判断对象类型
	if (Name.find("survivor") != std::string::npos)
	{
		this->Type = (this->Logo == 8192 ? 2 : 1);
	}
	else if(Name.find("player")!=std::string::npos)
	{
		this->Type = 3;
	}
	else
	{
		this->Type = 0;
	}

	return true;
}

Vec3 Object::GetBonePos(int Index)
{
	if (Index < 0)
		return { 0,0,0 };
	return this->ModelObj->Model->GetBoneJointPos(Index);
}