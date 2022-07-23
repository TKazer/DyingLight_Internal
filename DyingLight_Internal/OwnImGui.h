#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_internal.h"
#include "Struct.h"
#include <d3d11.h>
#include <iostream>
#include <tchar.h>
#include <dwmapi.h>
#include <atlstr.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dwmapi.lib")

class OwnImGui
{
public:
	ImFont* Font1 = nullptr;
	ImFont* Font2 = nullptr;

	// �����ı����Ƴ���
	int  CalcTextWidth(float FontSize,const char* Text);
	// �ı�
	void Text(const char* Text, Vec2 Pos, int color);
	// ����
	void Rectangle(Vec2 Pos, int width, int height, int color, int thickness);
	// ��
	void Line(Vec2 From, Vec2 To, int color, int thickness);
	// Բ
	void Circle(Vec2 Center, float radius, int color, float thickness);
	// ���Բ
	void CircleFilled(Vec2 Center, float radius, int color, int num = 360);
	// ������
	void RectangleFilled(Vec2 Pos, int width, int height, int color);
	// ����ı�
	void StrokeText(const char* Text, Vec2 Pos, int color_bg, int color);
	// ���ӵ�
	void ConnectPoint(std::vector<Vec2> Points, int color, float thickness);
	// ��ɫת��
	int  Color(float* Color_);

	// ��ѡ��
	void MyCheckBox(const char* str_id, bool* v);
	void MyCheckBox2(const char* str_id, bool* v);
	void MyCheckBox3(const char* str_id, bool* v);
	void MyCheckBox4(const char* str_id, bool* v);

	// Բͷ������
	bool SliderIntEx(const char* label, int* v, int v_min, int v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	bool SliderFloatEx(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	// ��们����
	bool SliderIntEx2(const char* label, int* v, int v_min, int v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	bool SliderFloatEx2(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
}Gui;