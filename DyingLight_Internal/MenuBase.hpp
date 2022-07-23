#pragma once
#include <windows.h>
#include "ImGuiHook.hpp"

namespace MenuBase
{
	static int Tab1 = 0;
	static int Tab2 = 0;

    void StyleSet()
    {
        ImGui::GetStyle().FrameRounding = 7;
        ImGui::GetStyle().PopupRounding = 7;

        ImVec4* Colors = ImGui::GetStyle().Colors;

        Colors[ImGuiCol_Button] = ImColor(0, 0, 0, 0);
        Colors[ImGuiCol_ButtonActive] = ImColor(0, 0, 0, 0);
        Colors[ImGuiCol_ButtonHovered] = ImColor(0, 0, 0, 0);
        Colors[ImGuiCol_PopupBg] = ImColor(15, 20, 35, 230);
        Colors[ImGuiCol_FrameBg] = ImColor(40, 45, 55, 240);
        Colors[ImGuiCol_FrameBgHovered] = ImColor(35, 40, 50, 240);
        Colors[ImGuiCol_FrameBgActive] = ImColor(43, 48, 58, 240);
        Colors[ImGuiCol_Header] = ImColor(40, 45, 55, 240);
        Colors[ImGuiCol_HeaderActive] = ImColor(43, 48, 58, 240);
        Colors[ImGuiCol_HeaderHovered] = ImColor(35, 40, 50, 240);
        Colors[ImGuiCol_Border] = ImColor(15, 20, 35, 230);
    }

    void TabSelect1(ImVec2 TextPos, const char* Text, int Index)
    {
        ImDrawList* Draw = ImGui::GetWindowDrawList();
        ImColor C_TabActive = ImColor(255, 255, 255, 255);
        ImColor C_TabHovered = ImColor(220, 220, 220, 225);
        ImColor C_TabDefault = ImColor(180, 180, 180, 225);
        ImColor C_Text;

        ImVec2 MousePos = ImGui::GetMousePos();
        ImVec2 RectPos, RectSize;
        ImVec2 MenuPos = ImGui::GetWindowPos();

        RectPos = { TextPos.x - 30,TextPos.y - 7 };
        RectSize = { 120,37 };

        ImGui::SetCursorPos({ RectPos.x - MenuPos.x  ,RectPos.y - MenuPos.y });
        if (ImGui::Button((std::string("##") + Text).c_str(), RectSize))
        {
            Tab1 = Index;
            Tab2 = 0;
        }

        if (MousePos.x > RectPos.x && MousePos.x<RectPos.x + RectSize.x && MousePos.y>RectPos.y && MousePos.y < RectPos.y + RectSize.y)
        {
            if (Tab1 != Index)
            {
                C_Text = C_TabHovered;
            }
            else
            {
                C_Text = C_TabActive;
            }
        }
        else if (Tab1 == Index)
        {
            C_Text = C_TabActive;
        }
        else
        {
            C_Text = C_TabDefault;
        }

        if (Tab1 == Index)
            Draw->AddRectFilled({ RectPos.x,RectPos.y }, { RectPos.x + RectSize.x, RectPos.y + RectSize.y }, ImColor(65, 75, 90, 255), 10);
        Draw->AddText(Gui.Font1, 20, TextPos, C_Text, Text);
    }

    void TabSelect2(ImVec2 RectPos, const char* Text, int Index)
    {
        ImDrawList* Draw = ImGui::GetWindowDrawList();
        ImColor C_TabActive = ImColor(255, 255, 255, 255);
        ImColor C_TabHovered = ImColor(220, 220, 220, 225);
        ImColor C_TabDefault = ImColor(180, 180, 180, 225);
        ImColor C_Text;

        ImVec2 MousePos = ImGui::GetMousePos();
        ImVec2 TextPos, RectSize;
        ImVec2 MenuPos = ImGui::GetWindowPos();
        ImVec2 TextSize = Gui.Font1->CalcTextSizeA(15, FLT_MAX, 0, Text);

        RectSize = { 70,30 };
        // ¾ÓÖÐ
        TextPos = { RectPos.x + (RectSize.x - TextSize.x) / 2 ,RectPos.y + (RectSize.y - TextSize.y) / 2 };


        ImGui::SetCursorPos({ RectPos.x - MenuPos.x  ,RectPos.y - MenuPos.y });
        if (ImGui::Button((std::string("##") + Text).c_str(), RectSize))
        {
            Tab2 = Index;
        }

        if (MousePos.x > RectPos.x && MousePos.x<RectPos.x + RectSize.x && MousePos.y>RectPos.y && MousePos.y < RectPos.y + RectSize.y)
        {
            if (Tab2 != Index)
            {
                C_Text = C_TabHovered;
            }
            else
            {
                C_Text = C_TabActive;
            }
        }
        else if (Tab2 == Index)
        {
            C_Text = C_TabActive;
        }
        else
        {
            C_Text = C_TabDefault;
        }

        if (Tab2 == Index)
            Draw->AddLine({ TextPos.x,TextPos.y + 20 }, { TextPos.x + TextSize.x,TextPos.y + 20 }, ImColor(65, 75, 90, 255), 2);
        Draw->AddText(Gui.Font1, 15, TextPos, C_Text, Text);
    }
}