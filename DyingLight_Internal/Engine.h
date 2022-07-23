#pragma once
#include "ModelObject.h"

namespace Engine
{
    class CLevel
    {
    public:
        char _pad[0x928];
        Engine::Array<IModelObject> ModelList;
    };

    class LevelDI
    {
    public:
        char _pad[0x8];
        class CLevel* Level;
    };

    class PlayerDI
    {
    public:
        char pad_0000[1968]; //0x0000
        float x; //0x07B0
        float y; //0x07B4
        float z; //0x07B8
        char pad_07BC[2400]; //0x07BC
        float Yaw; //0x111C
        float Pitch; //0x1120
        char pad_1124[224]; //0x1124
        float CurrentHealth; //0x1204
        float MaxHealth; //0x1208

        // Functions

        Vec3 GetPos()
        {
            return Vec3{ x,y,z };
        }

    }; //Size: 0x120C

    class LocalClientDI
    {
    public:
        char pad_0000[80]; //0x0000
        class PlayerDI* Player; //0x0050
    }; //Size: 0x0058

    class SessionCooperativeDI
    {
    public:
        char _pad[0xB0];
        class LevelDI* LevelDI_;
        class LocalClientDI* LocalClientDI_;
        class CameraManagerDI* CameraManager_;
    };

    class GameDI
    {
    public:
        char _pad[0x5A0];
        SessionCooperativeDI* Session_;
    };

    class CGame
    {
    public:
        char pad_0000[152]; //0x0000
        class GameDI* GameDI_; //0x0098
        char pad_00A0[40]; //0x00A0
        int WindowWidth; //0x00C8
        int WindowHeight; //0x00CC
        static CGame* GetInstance()
        {
            static CGame* pCGame = nullptr;
            if (pCGame == nullptr)
            {
                DWORD64 FunctionAddress = (DWORD64)GetProcAddress((HMODULE)pGameData->Module_Engine, "CreateGame");
                pCGame = *reinterpret_cast<CGame**>(FunctionAddress + Offset::CGame);
            }
            return pCGame;
        }
    };

    class CBaseCamera
    {
    public:
        char pad_0000[76]; //0x0000
        float x; //0x004C
        char pad_0050[12]; //0x0050
        float y; //0x005C
        char pad_0060[12]; //0x0060
        float z; //0x006C
        char pad_0070[64]; //0x0070
        float Matrix[4][4];
    }; //Size: 0x0070

    class CameraFPPDI
    {
    public:
        char pad_0000[8]; //0x0000
        class CBaseCamera* Base; //0x0008

        //Functions:

        bool WorldToScreen(Vec3 coords, Vec3& Ret)
        {
            float ViewW, SightX = Engine::CGame::GetInstance()->WindowWidth / 2, SightY = Engine::CGame::GetInstance()->WindowHeight / 2;
            ViewW = Base->Matrix[3][0] * coords.x + Base->Matrix[3][1] * coords.y + Base->Matrix[3][2] * coords.z + Base->Matrix[3][3];
            if (ViewW <= 0.01)
                return false;
            ViewW = 1 / ViewW;
            Ret.x = SightX + (Base->Matrix[0][0] * coords.x + Base->Matrix[0][1] * coords.y + Base->Matrix[0][2] * coords.z + Base->Matrix[0][3]) * ViewW * SightX;
            Ret.y = SightY - (Base->Matrix[1][0] * coords.x + Base->Matrix[1][1] * coords.y + Base->Matrix[1][2] * coords.z + Base->Matrix[1][3]) * ViewW * SightY;
            Ret.z = SightY - (Base->Matrix[1][0] * coords.x + Base->Matrix[1][1] * coords.y + Base->Matrix[1][2] * coords.z + Base->Matrix[1][3]) * ViewW * SightY;
            return true;
        }

        float GetFov()
        {
            using GetFov_T = float(*__fastcall)(CameraFPPDI*);
            static GetFov_T GetFov_Fn = (GetFov_T)GetProcAddress((HMODULE)pGameData->Module_Engine, "?GetFOV@IBaseCamera@@QEAAMXZ");
            return GetFov_Fn(this);
        }

        Vec3 GetPosition()
        {
            Vec3 Temp;
            using GetPosition_ = Vec3*(*__fastcall)(CameraFPPDI*,Vec3*);
            static GetPosition_ GetPosition_Fn = (GetPosition_)GetProcAddress((HMODULE)pGameData->Module_Engine, "?GetPosition@IBaseCamera@@QEBA?BVvec3@@XZ");
            return *GetPosition_Fn(this, &Temp);
        }

    };

    class CameraManagerDI
    {
    public:
        char _pad[0x50];
        CameraFPPDI* Camera;
    };
}