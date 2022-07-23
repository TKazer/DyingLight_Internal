#include <windows.h>
#include <fstream>
#include "Debug.hpp"
#include "ImGuiHook.hpp"
#include "GameData.h"
#include "Game.h"
#include "Object.h"
#include "Render.h"
#include "Menu.hpp"
#include "Memory.h"

void Main()
{
    if (GetAsyncKeyState(VK_END))
    {
        delete pGame;
        delete pGameData;

        if (Memory::LockHealthFlag)
            Memory::UnLockHealth();
        if (Memory::BulletLockFlag)
            Memory::UnLockBullet();

        FreeConsole();
        imgui_hook::get().End();
        return;
    }

    Menu::Render();

    pGame->Update();
    if (!pGame->CheckAllPtr())
        return;
    if (!pGame->pLevelDI->Level->ModelList.Size())
        return;

    //Memory::Enable();
    // 锁血
    if (MenuConfig::LockHealth)
    {
        if (Memory::LockHealthFlag == false)
        {
            Memory::LockHealthFlag = true;
            Memory::LockHealth();
;           pGame->pLocalClientDI->Player->CurrentHealth = 200;
        }
    }
    else
    {
        if (Memory::LockHealthFlag == true)
        {
            Memory::LockHealthFlag = false;
            Memory::UnLockHealth();
            pGame->pLocalClientDI->Player->CurrentHealth = pGame->pLocalClientDI->Player->MaxHealth;
        }
    }

    // 锁子弹
    if (MenuConfig::LockBullet)
    {
        if (Memory::BulletLockFlag == false)
        {
            Memory::BulletLockFlag = true;
            Memory::LockBullet();
        }
    }
    else
    {
        if (Memory::BulletLockFlag == true)
        {
            Memory::BulletLockFlag = false;
            Memory::UnLockBullet();
        }
    }

    //// 高跳
    //if (MenuConfig::HighJump)
    //{
    //    if (GetAsyncKeyState(VK_SPACE))
    //    {
    //        Memory::HighJump(8);
    //    }
    //}

    // 加速
    if (MenuConfig::SpeedUp)
    {
        static bool SpeedUpFlag = false;
        if (GetAsyncKeyState(VK_LSHIFT))
        {
            SpeedUpFlag = true;
            Memory::SpeedUp(MenuConfig::SpeedNum);
        }
        else
        {
            if (SpeedUpFlag)
            {
                SpeedUpFlag = false;
                Memory::SpeedUp(1);
            }
        }
    }

    // 设置时间
    if (MenuConfig::SetTime)
    {
        Memory::SetTime(MenuConfig::Time);
    }

    Engine::CameraFPPDI* Camera = pGame->pCameraManage->Camera;
    Engine::PlayerDI* LocalPlayer = pGame->pLocalClientDI->Player;

    // 自瞄选项
    Vec3 AimPos;
    float AimPosDistoLocal = INT_MAX;

    for (int i = 0; i < pGame->pLevelDI->Level->ModelList.Size(); i++)
    {
        Object Obj(pGame->pLevelDI->Level->ModelList.At(i));
        
        if (!Obj.Update())
            continue;
        if (Obj.Type == 3)
            continue;
        if (!Camera->WorldToScreen(Obj.Pos, Obj.ScreenPos))
            continue;

        Vec3 HeadPos, HeadScreenPos;
        bool HeadPosVisible = false;

        HeadPos = Obj.GetBonePos(Bone::Head);
        if (!Camera->WorldToScreen(HeadPos, HeadScreenPos))
            continue;

        HeadPosVisible = Obj.ModelObj->Model->IsVisible(Camera->GetPosition(), HeadPos);

        // 调试信息
        if (MenuConfig::DebugText)
        {
            char Buffer[256]{};
            sprintf_s(Buffer, "%llX", (DWORD64)Obj.ModelObj);
            Gui.StrokeText(Buffer, { HeadScreenPos.x,HeadScreenPos.y + 15 }, ImColor(0, 0, 0, 255), ImColor(255, 255, 255, 255));
            Gui.StrokeText(("Yaw:"+std::to_string(Obj.Yaw)).c_str(), {HeadScreenPos.x,HeadScreenPos.y + 30}, ImColor(0, 0, 0, 255), ImColor(255, 255, 255, 255));
        }
        
        // 距离
        float Distance = Obj.ModelObj->Model->GetDistanceTo(Camera->GetPosition());
        char DistanceText[256]{};
        sprintf_s(DistanceText, "%.1fM", Distance);
        Gui.StrokeText(DistanceText, HeadScreenPos, ImColor(0, 0, 0, 255), ImColor(255, 255, 255, 255));

        // 自瞄选择
        if (Obj.Type != 2)
        {
            if (HeadPosVisible)
            {
                if (Distance < AimPosDistoLocal)
                {
                    AimPos = HeadPos;
                    AimPosDistoLocal = Distance;
                }
            }
        }

        // 可视标识
        if (HeadPosVisible)
            Gui.Text("*", HeadScreenPos, ImColor(255, 0, 0, 255));
        else
            Gui.Text("*", HeadScreenPos, ImColor(130, 130, 130, 255));

        // 朝向
        if (MenuConfig::LosLine)
        {
            Render::DrawLosLine(Obj, MenuConfig::LosLineLength, ImColor(255, 255, 0, 255), 1);
        }

        Gui.Line({(float)pGame->pCGame->WindowWidth/2,(float)pGame->pCGame->WindowHeight },HeadScreenPos,ImColor(255,255,0,255),1);

        // 骨骼
        Render::DrawBone(Obj, ImColor(Gui.Color(MenuConfig::BoneColor)), 1, MenuConfig::Bone);
       
        // 方框
        if (MenuConfig::Box)
        {
            switch (MenuConfig::BoxType)
            {
            case 2:
            {
                ImColor BoxColor(255, 255, 255, 255);
                switch (Obj.Type)
                {
                case 0:
                    BoxColor = Gui.Color(MenuConfig::BoxColor_Zombie);
                    break;
                case 1:
                    BoxColor = Gui.Color(MenuConfig::BoxColor_SpZombie);
                    break;
                case 2:
                    BoxColor = Gui.Color(MenuConfig::BoxColor_Survivor);
                    break;
                default:
                    break;
                }
                Render::AABB(Obj, BoxColor, 1);
            }
                break;
            default:
                break;
            }
        }

    }

    if (MenuConfig::AimBot)
    {
        if(GetAsyncKeyState(AimControl::HotKey))
            AimControl::AimBot(AimPos, Camera->GetPosition(), LocalPlayer->Yaw, LocalPlayer->Pitch);
    }

    // Object数量
    std::string ObjectCountText = "Object Counts:" + std::to_string(pGame->pLevelDI->Level->ModelList.ItemCount);
    Vec2 WindowCenter{ (float)pGame->pCGame->WindowWidth,(float)pGame->pCGame->WindowHeight };
    Gui.StrokeText(ObjectCountText.c_str(), { WindowCenter.x / 2 - Gui.CalcTextWidth(15,ObjectCountText.c_str()) / 2,WindowCenter.y - 18}, IM_COL32(0, 0, 0, 255), IM_COL32(255, 255, 255, 255));

}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        // 初始化控制台
        AllocConsole();
        freopen_s((FILE**)stdout, "conout$", "w", stdout);
        freopen_s((FILE**)stdin, "conin$", "r", stdin);

        pGameData = new GameData;
        pGame = new Game;

        // 奔溃异常处理器
        AddVectoredExceptionHandler(1, Debug);

        imgui_hook::get().SetFont("C:\\Windows\\Fonts\\msyh.ttc", 15, false);
        imgui_hook::get().Begin("Menu", hModule, Main, DXTYPE::AUTO);
    }
    return TRUE;
}

