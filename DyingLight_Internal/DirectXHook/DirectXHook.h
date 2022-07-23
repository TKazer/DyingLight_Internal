#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxgi.h>
#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#include "./MinHook.h"

#if _WIN64
typedef DWORD64 Address;
#else
typedef DWORD	Address;
#endif

template <typename T> class Single
{
protected:
	Single() {}
	~Single() {}
public:
	static T& get()
	{
		static T instance{};
		return instance;
	}
};

enum DXTYPE 
{
	NONE,
	D3D9,
	D3D11,
	AUTO
};

class DirectXHook : public Single<DirectXHook>
{
private:
	DXTYPE DirectXType = DXTYPE::NONE;
	Address* MethodsTable = nullptr;
	void* PTarget;
	WNDCLASSEXA WClass;
	HWND hWindow = NULL;
private:
	bool CreateDrawWindow();
public:
	bool Init(DXTYPE Type, void** Original, void* Function);
	bool End();
};