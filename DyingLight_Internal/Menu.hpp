#pragma once
#include "MenuBase.hpp"
#include "MenuConfig.h"
#include "AimBot.h"
namespace Menu
{
    static bool MenuOpen = true;

    // Tab栏
    void Tab()
    {
        ImVec2 MenuPos = ImGui::GetWindowPos();
        ImDrawList* Draw = ImGui::GetWindowDrawList();

        // 相对坐标
        auto Calc = [&](float x, float y)->ImVec2
        {
            return ImVec2(MenuPos.x + x, MenuPos.y + y);
        };

        // Tab1
        MenuBase::TabSelect1(Calc(45, 30), "Visual", 0);
        MenuBase::TabSelect1(Calc(45, 90), "Aimbot", 1);
        MenuBase::TabSelect1(Calc(45, 150), "Memory", 2);

        // Tab2
        switch (MenuBase::Tab1)
        {
        case 0:
        {
            // #Visual
            MenuBase::TabSelect2(Calc(170, 25), "Esp", 0);
            MenuBase::TabSelect2(Calc(240, 25), "Radar", 1);
            MenuBase::TabSelect2(Calc(310, 25), "Color", 2);
            break;
        }
        case 1:
        {
            // #Aimbot
            MenuBase::TabSelect2(Calc(170, 25), "Aim", 0);
            break;
        }
        case 2:
        {
            // #Memory
            MenuBase::TabSelect2(Calc(170, 25), "Local", 0);
            MenuBase::TabSelect2(Calc(240, 25), "Game", 1);
            break;
        }
        default:
            break;
        }
    }

    // 主菜单

    void Render()
    {
        // 鼠标点击事件

        if (GetAsyncKeyState(VK_CAPITAL))
        {
            if (MenuOpen)
            {
                POINT MousePos;
                GetCursorPos(&MousePos);
                ScreenToClient(imgui_hook::get().hWindow, &MousePos);
                ImGuiIO& io = ImGui::GetIO();
                io.MousePos.x = MousePos.x;
                io.MousePos.y = MousePos.y;
                ImGui::GetIO().MouseDrawCursor = true;
                if (GetAsyncKeyState(VK_LBUTTON))
                {
                    io.MouseDown[0] = true;
                }
                else
                {
                    io.MouseDown[0] = false;
                }
            }
        }
        else
        {
            ImGui::GetIO().MouseDrawCursor = false;
        }

        // 显隐菜单
        if (GetAsyncKeyState(VK_HOME) & 0x8000)
        {
            // 定时事件
            static DWORD PreTimeCount = GetTickCount();
            DWORD CurrentTickCount = GetTickCount();
            // 170ms/count
            if (CurrentTickCount - PreTimeCount > 170)
            {
                PreTimeCount = CurrentTickCount;
                MenuOpen = !MenuOpen;
            }
        }

        MenuBase::StyleSet();

        ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);

        if (!MenuOpen)
        {
            ImGui::SetWindowSize({ 0, 0 });
            return;
        }

        const ImVec2 MenuSize{ 600,300 };// 高度可调，宽度调整后请自行校准其他组件位置
        ImGui::SetWindowSize(MenuSize);

        ImVec2 MenuPos = ImGui::GetWindowPos();
        ImDrawList* Draw = ImGui::GetWindowDrawList();

        // 相对坐标计算函数
        auto Calc = [&](float x, float y)->ImVec2
        {
            return ImVec2(MenuPos.x + x, MenuPos.y + y);
        };

        if (MenuOpen)
        {
            // 菜单背景
            Draw->AddRectFilled(MenuPos, { MenuPos.x + MenuSize.x,MenuPos.y + MenuSize.y }, ImColor(15, 20, 35, 230), 12);

            ImGui::PushFont(Gui.Font1);

            // 分隔符
            Draw->AddLine(Calc(150, 20), Calc(150, MenuSize.y - 20), ImColor(45, 55, 75, 240), 2);

            // Tab2背景
            Draw->AddRectFilled(Calc(165, 20), Calc(580, 60), ImColor(25, 30, 40, 240), 7);
            Draw->AddRectFilled(Calc(165, 70), Calc(580, MenuSize.y - 20), ImColor(25, 30, 40, 240), 7);

            Tab();

            if (MenuBase::Tab1 == 0)
            {
                // Visual
                switch (MenuBase::Tab2)
                {
                case 0:
                {
                    // #Esp
                    ImGui::SetCursorPos({ 175,80 });
                    Gui.MyCheckBox2("Box", &MenuConfig::Box);

                    ImGui::SetCursorPos({ 400,80 });
                    const char* TypeCombo[]{
                        "2D Normal","3D Normal","3D Dynamic"
                    };
                    ImGui::Combo("Type", &MenuConfig::BoxType, TypeCombo, 3);

                    ImGui::SetCursorPos({ 175,110 });
                    Gui.MyCheckBox2("Bone", &MenuConfig::Bone);

                    ImGui::SetCursorPos({ 175,140 });
                    Gui.MyCheckBox2("LosLine", &MenuConfig::LosLine);

                    ImGui::SetCursorPos({ 175,170 });
                    Gui.SliderFloatEx2("LosLineLength", &MenuConfig::LosLineLength, 0.5, 3);

                    ImGui::SetCursorPos({ 175,200 });
                    Gui.MyCheckBox2("DebugText", &MenuConfig::DebugText);

                    break;
                }
                case 1:
                {
                    // #Radar


                    break;
                }
                case 2:
                {
                    // #Color
                    ImGui::SetCursorPos({ 175,80 });
                    ImGui::ColorEdit3("Box Zombie", MenuConfig::BoxColor_Zombie, ImGuiColorEditFlags_NoInputs);

                    ImGui::SetCursorPos({ 175,110 });
                    ImGui::ColorEdit3("Box Survivor", MenuConfig::BoxColor_Survivor, ImGuiColorEditFlags_NoInputs);

                    ImGui::SetCursorPos({ 175,140 });
                    ImGui::ColorEdit3("Box SpZombie", MenuConfig::BoxColor_SpZombie, ImGuiColorEditFlags_NoInputs);

                    ImGui::SetCursorPos({ 175,170 });
                    ImGui::ColorEdit3("Bone ", MenuConfig::BoneColor, ImGuiColorEditFlags_NoInputs);
                    break;
                }
                default:
                    break;
                }
            }

            if (MenuBase::Tab1 == 1)
            {
                // Aimbot
                switch (MenuBase::Tab2)
                {
                case 0:
                {
                    // #Aim
                    ImGui::SetCursorPos({ 175,80 });
                    Gui.MyCheckBox2("AimBot", &MenuConfig::AimBot);

                    ImGui::SetCursorPos({ 175,110 });
                    const char* TypeCombo[]{
                          "Memory","BulletTrack"
                    };
                    ImGui::Combo("Type ", &MenuConfig::AimType, TypeCombo, 2);

                    ImGui::SetCursorPos({ 175,140 });
                    Gui.MyCheckBox2("ShowAimRange", &MenuConfig::ShowAimRange);

                    ImGui::SetCursorPos({ 400 ,140 });
                    ImGui::ColorEdit3("AimRangeColor", MenuConfig::AimRangeColor, ImGuiColorEditFlags_NoInputs);

                    ImGui::SetCursorPos({ 175,170 });
                    Gui.SliderFloatEx2("AimRange", &AimControl::AimRange, 5, 400);

                    break;
                }
                default:
                    break;
                }
            }

            if (MenuBase::Tab1 == 2)
            {
                // Memory

                switch (MenuBase::Tab2)
                {
                case 0:
                {
                    // #Local
                    ImGui::SetCursorPos({ 175,80 });
                    Gui.MyCheckBox2("LockHealth", &MenuConfig::LockHealth);

                    ImGui::SetCursorPos({ 175,110 });
                    Gui.MyCheckBox2("LockBullet", &MenuConfig::LockBullet);

                    ImGui::SetCursorPos({ 175,140 });
                    Gui.MyCheckBox2("SpeedUp", &MenuConfig::SpeedUp);

                    ImGui::SetCursorPos({ 300,140 });
                    Gui.SliderFloatEx2("Speed", &MenuConfig::SpeedNum, 1.5, 5);

                    ImGui::SetCursorPos({ 175,170 });
                    Gui.MyCheckBox2("LockAmmo", &MenuConfig::LockAmmo);

                    ImGui::SetCursorPos({ 300,170 });
                    Gui.SliderIntEx2("Ammo", &MenuConfig::AmmoNum, 1, 1000);

                    ImGui::SetCursorPos({ 175,200 });
                    Gui.MyCheckBox2("HighJump", &MenuConfig::HighJump);

                    break;
                }
                case 1:
                {
                    // #Game
                    ImGui::SetCursorPos({ 175,80 });
                    Gui.MyCheckBox2("SetTime", &MenuConfig::SetTime);

                    ImGui::SetCursorPos({ 175,110 });
                    Gui.SliderFloatEx2("Time", &MenuConfig::Time, 0.0f, 1.0f);
                    break;
                }
                default:
                    break;
                }
            }
            ImGui::PopFont();
        }
    }
}