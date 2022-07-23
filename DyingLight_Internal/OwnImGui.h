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

	// 计算文本绘制长度
	int  CalcTextWidth(float FontSize,const char* Text);
	// 文本
	void Text(const char* Text, Vec2 Pos, int color);
	// 矩形
	void Rectangle(Vec2 Pos, int width, int height, int color, int thickness);
	// 线
	void Line(Vec2 From, Vec2 To, int color, int thickness);
	// 圆
	void Circle(Vec2 Center, float radius, int color, float thickness);
	// 填充圆
	void CircleFilled(Vec2 Center, float radius, int color, int num = 360);
	// 填充矩形
	void RectangleFilled(Vec2 Pos, int width, int height, int color);
	// 描边文本
	void StrokeText(const char* Text, Vec2 Pos, int color_bg, int color);
	// 连接点
	void ConnectPoint(std::vector<Vec2> Points, int color, float thickness);
	// 颜色转换
	int  Color(float* Color_);

	// 勾选框
	void MyCheckBox(const char* str_id, bool* v);
	void MyCheckBox2(const char* str_id, bool* v);
	void MyCheckBox3(const char* str_id, bool* v);
	void MyCheckBox4(const char* str_id, bool* v);

	// 圆头滑动条
	bool SliderIntEx(const char* label, int* v, int v_min, int v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	bool SliderFloatEx(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	// 填充滑动条
	bool SliderIntEx2(const char* label, int* v, int v_min, int v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	bool SliderFloatEx2(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
}Gui;