#pragma once
#include "GameData.h"
#include "Function.h"

namespace Engine
{
	template <typename T>
	class Array
	{
	public:
		DWORD64* pItems;
		DWORD ItemCount;
		DWORD ItemMaxCount;
		T* At(int index)
		{
			return reinterpret_cast<T*>(this->pItems[index]);
		}
		int Size()
		{
			return this->ItemCount;
		}
		int MaxSize()
		{
			return this->ItemMaxCount;
		}
	};

	class StdModel
	{
	public:
		char pad_0000[824]; //0x0000
		class AIVis* AIVisStatus; //0x0338
		char pad_0340[2472]; //0x0340
		class ModelStatus* Status; //0x0CE8

		// Function

		Vec3 GetPos()
		{
			using GetPos_ = Vec3 * (*__fastcall)(StdModel*, Vec3*);
			static GetPos_ GetPos_Fn = (GetPos_)GetProcAddress((HMODULE)pGameData->Module_Engine, "?GetWorldPosition@IControlObject@@QEBA?AVvec3@@XZ");
			Vec3 Temp;
			return *GetPos_Fn(this, &Temp);
		}

		Vec3 GetBoneJointPos(unsigned char index)
		{
			using GetBoneJointPos_ = void(*__fastcall)(StdModel*, Vec3*, unsigned char);
			static GetBoneJointPos_ GetBoneJointPos_Fn = (GetBoneJointPos_)GetProcAddress((HMODULE)pGameData->Module_Engine, "?GetBoneJointPos@IModelObject@@QEBA?AVvec3@@E@Z");
			Vec3 Pos;
			GetBoneJointPos_Fn(this, &Pos, index);
			return Pos;
		}

		bool IsVisible(Vec3 Start,Vec3 End)
		{
			// *this,Targer,Start,End,Type=4
			using RaytestToTarget_ = bool(*__fastcall)(StdModel*, StdModel*, Vec3*, Vec3*, unsigned char);
			static RaytestToTarget_ RaytestToTarget_Fn = (RaytestToTarget_)GetProcAddress((HMODULE)pGameData->Module_Engine, "?RaytestToTarget@IControlObject@@QEAA_NPEBV1@AEBVvec3@@1E@Z");

			return RaytestToTarget_Fn(this, this, &Start, &End, 4);
		}

		Vec3 GetForwardVector(Vec3& Vector)
		{
			// *this,Targer,Start,End,Type=4
			using GetForwardVector_ = Vec3*(*__fastcall)(StdModel*, Vec3*);
			static GetForwardVector_ GetForwardVector_Fn = (GetForwardVector_)GetProcAddress((HMODULE)pGameData->Module_Engine, "?GetForwardVector@IControlObject@@QEBA?AVvec3@@XZ");

			return *GetForwardVector_Fn(this, &Vector);
		}

		float GetDistanceTo(Vec3 Pos)
		{
			using GetDistanceTo_ = float (*__fastcall)(StdModel*, Vec3*);
			static GetDistanceTo_ GetDistanceTo_Fn = (GetDistanceTo_)GetProcAddress((HMODULE)pGameData->Module_Engine, "?GetDistanceTo@IControlObject@@QEBAMAEBVvec3@@@Z");

			return GetDistanceTo_Fn(this, &Pos);
		}

		void SetWorldPosition(Vec3 Pos)
		{
			using SetWorldPosition_ = void (*__fastcall)(StdModel*, Vec3*);
			static SetWorldPosition_ SetWorldPosition_Fn = (SetWorldPosition_)GetProcAddress((HMODULE)pGameData->Module_Engine, "?SetLocalScale@IControlObject@@QEAAXAEBVvec3@@@Z");//"?SetWorldPosition@IControlObject@@QEAAXAEBVvec3@@@Z");

			SetWorldPosition_Fn(this, &Pos);
		}

		float GetYaw()
		{
			Vec3 ForwardVector;
			this->GetForwardVector(ForwardVector);

			float Yaw = 0;
			float X = ForwardVector.z, Y = ForwardVector.x;

			Yaw = 90 - atan2f(Y, X) * 180 / M_PI;

			if (Yaw > 180)
				Yaw -= 360;

			return Yaw;
		}

	}; //Size: 0x0CF0

	class AIVis
	{
	public:
		char pad_0000[404]; //0x0000
		float Yaw; //0x0194
	}; //Size: 0x0198

	class ModelStatus
	{
	public:
		char pad_0000[120]; //0x0000
		float CurrentHealth; //0x0078
		char pad_007C[76]; //0x007C
		float MaxHealth; //0x00C8
	}; //Size: 0x00CC

	class IModelObject
	{
	public:
		char pad_0000[284]; //0x0000
		float x; //0x011C
		char pad_0120[12]; //0x0120
		float y; //0x012C
		char pad_0130[12]; //0x0130
		float z; //0x013C
		char pad_0140[496]; //0x0140
		int Active; //0x0330
		char pad_0334[12]; //0x0334
		int Logo; //0x0340
		char pad_0344[116]; //0x0344
		class StdModel* Model; //0x03B8
		char pad_03C0[16]; //0x03C0
		char* Name; //0x03D0
	}; //Size: 0x03C0
}