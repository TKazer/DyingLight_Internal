#pragma once
#include <windows.h>
#include "DirectXHook/DirectXHook.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "imgui/imgui_impl_dx9.h"
#include <d3d11.h>
#include <dxgi.h>
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_internal.h"
#include <functional>
#include "OwnImGui.h"
#ifdef _WIN64
#define GWL_WNDPROC GWLP_WNDPROC
#endif

// _D3D9_
typedef long(__stdcall* EndScene)(LPDIRECT3DDEVICE9);
// _D3D11_
typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef uintptr_t PTR;

typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// _D3D9_
EndScene oEndScene = NULL;
// _D3D11_
Present oPresent;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

std::function<void(void)> CallBack = NULL;
char MenuName[256]{};

class imgui_hook : public Single<imgui_hook>
{
public:
	bool InitFlag = false;
	bool UnInstall = false;
	bool FreeDll = false;
	HWND hWindow = NULL;
	WNDPROC oWndProc = NULL;
	DXTYPE DxType = DXTYPE::NONE;
	struct _Font
	{
		int FontSize = 15;
		bool Chinese = false;
		char FontFilePath[512]{};
	}Font;

public:
	imgui_hook(){}
	~imgui_hook(){}

	void Begin(const char* MenuName_, HMODULE hModule_, std::function<void(void)> CallBack_Fn, DXTYPE DxType_);
	void End();

	void SetFont(const char* FontFilePath_, int Size_, bool Chinese_);
private:
	static DWORD WINAPI MainThread(LPVOID lpReserved);
	// _D3D9_
	void Init_DX9(LPDIRECT3DDEVICE9 pDevice);
	static long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice);
	// _D3D11_
	bool Init_DX11(IDXGISwapChain* pSwapChain);
	static HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
};

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(imgui_hook::get().oWndProc, hWnd, uMsg, wParam, lParam);
}


BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	DWORD wndProcId;
	GetWindowThreadProcessId(handle, &wndProcId);

	if (GetCurrentProcessId() != wndProcId)
		return TRUE;

	imgui_hook::get().hWindow = handle;
	return FALSE;
}

HWND GetProcessWindow()
{
	imgui_hook::get().hWindow = NULL;
	EnumWindows(EnumWindowsCallback, NULL);
	return imgui_hook::get().hWindow;
}

bool imgui_hook::Init_DX11(IDXGISwapChain* pSwapChain)
{
	if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&pDevice))))
	{
		pDevice->GetImmediateContext(&pContext);
		DXGI_SWAP_CHAIN_DESC sd;
		pSwapChain->GetDesc(&sd);
		imgui_hook::get().hWindow = sd.OutputWindow;
		ID3D11Texture2D* pBackBuffer;
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
		pBackBuffer->Release();
		oWndProc = (WNDPROC)SetWindowLongPtr(imgui_hook::get().hWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
		Gui.Font1 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arialbd.ttf", imgui_hook::get().Font.FontSize);
		if (strcmp(imgui_hook::get().Font.FontFilePath, "") != 0)
		{
			if (imgui_hook::get().Font.Chinese)
				io.Fonts->AddFontFromFileTTF(imgui_hook::get().Font.FontFilePath, imgui_hook::get().Font.FontSize, 0, io.Fonts->GetGlyphRangesChineseFull());
			else
				io.Fonts->AddFontFromFileTTF(imgui_hook::get().Font.FontFilePath, imgui_hook::get().Font.FontSize);
		}
		ImGui::StyleColorsDark();
		ImGui_ImplWin32_Init(imgui_hook::get().hWindow);
		ImGui_ImplDX11_Init(pDevice,pContext);
	}
	else
		return false;
}

void imgui_hook::Init_DX9(LPDIRECT3DDEVICE9 pDevice)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	if (strcmp(imgui_hook::get().Font.FontFilePath, "") != 0)
	{
		if (imgui_hook::get().Font.Chinese)
			io.Fonts->AddFontFromFileTTF(imgui_hook::get().Font.FontFilePath, imgui_hook::get().Font.FontSize, 0, io.Fonts->GetGlyphRangesChineseFull());
		else
			io.Fonts->AddFontFromFileTTF(imgui_hook::get().Font.FontFilePath, imgui_hook::get().Font.FontSize);
	}
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(imgui_hook::get().hWindow);
	ImGui_ImplDX9_Init(pDevice);
}

HRESULT __stdcall imgui_hook::hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!imgui_hook::get().InitFlag)
	{
		if (imgui_hook::get().Init_DX11(pSwapChain))
			imgui_hook::get().InitFlag = true;
		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	CallBack();
	if (imgui_hook::get().UnInstall)
	{
		DirectXHook::get().End();
		imgui_hook::get().oWndProc = (WNDPROC)SetWindowLongPtr(imgui_hook::get().hWindow, GWLP_WNDPROC, (LONG_PTR)(imgui_hook::get().oWndProc));
		imgui_hook::get().FreeDll = true;
	}
	ImGui::End();

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

long __stdcall imgui_hook::hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if (!imgui_hook::get().InitFlag)
	{
		imgui_hook::get().Init_DX9(pDevice);
		imgui_hook::get().InitFlag = true;
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	CallBack();
	if (imgui_hook::get().UnInstall)
	{
		DirectXHook::get().End();
		imgui_hook::get().oWndProc = (WNDPROC)SetWindowLongPtr(imgui_hook::get().hWindow, GWLP_WNDPROC, (LONG_PTR)(imgui_hook::get().oWndProc));
		imgui_hook::get().FreeDll = true;
	}
	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return oEndScene(pDevice);
}

DWORD WINAPI imgui_hook::MainThread(LPVOID lpReserved)
{
	bool InitHook = false;
	if (imgui_hook::get().DxType == DXTYPE::AUTO)
	{
		if (GetModuleHandleA("d3d11.dll"))
			imgui_hook::get().DxType = DXTYPE::D3D11;
		else if (GetModuleHandleA("d3d9.dll"))
			imgui_hook::get().DxType = DXTYPE::D3D9;
		else
			return NULL;
	}
	do
	{
		if (imgui_hook::get().DxType == DXTYPE::D3D9)
		{
			if (DirectXHook::get().Init(DXTYPE::D3D9, reinterpret_cast<void**>(&oEndScene), hkEndScene))
			{
				do
					imgui_hook::get().hWindow = GetProcessWindow();
				while (imgui_hook::get().hWindow == NULL);
				imgui_hook::get().oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(imgui_hook::get().hWindow, GWL_WNDPROC, (LONG_PTR)WndProc));
				InitHook = true;
			}
		}
		else if (imgui_hook::get().DxType == DXTYPE::D3D11)
		{
			if (DirectXHook::get().Init(DXTYPE::D3D11, reinterpret_cast<void**>(&oPresent), hkPresent))
				InitHook = true;
		}

	} while (!InitHook);

	while (true)
	{
		if (imgui_hook::get().FreeDll)
		{
			FreeLibraryAndExitThread((HMODULE)lpReserved, 0);
		}
	}
	return 1;
}

void imgui_hook::Begin(const char* MenuName_,HMODULE hModule_, std::function<void(void)> CallBack_Fn, DXTYPE DxType_)
{
	CallBack = CallBack_Fn;
	strcpy_s(MenuName, MenuName_);
	imgui_hook::get().DxType = DxType_;
	DisableThreadLibraryCalls(hModule_);
	CreateThread(nullptr, 0, MainThread, hModule_, 0, nullptr);
}

void imgui_hook::End()
{
	imgui_hook::get().UnInstall = true;
}

void imgui_hook::SetFont(const char* FontFilePath_, int Size_, bool Chinese_)
{
	strcpy_s(this->Font.FontFilePath, (strlen(FontFilePath_) + 1), FontFilePath_);
	this->Font.FontSize = Size_;
	this->Font.Chinese = Chinese_;
}