#include "DirectXHook.h"

bool DirectXHook::Init(DXTYPE Type, void** Original, void* Function)
{
	if (Type <= DXTYPE::NONE && Type > DXTYPE::AUTO)
		return false;
	if (!this->CreateDrawWindow())
		return false;

	HMODULE hDxModule = NULL;

	auto ShutDownWindow = [&]() {
		DestroyWindow(this->hWindow);
		UnregisterClassA(this->WClass.lpszClassName, this->WClass.hInstance);
	};

	this->DirectXType = Type;

	if (this->DirectXType == DXTYPE::D3D9)
	{
		void* PDirect3DCreate9;
		LPDIRECT3DDEVICE9 Device;
		IDirect3D9* PDirect3D9;
		D3DDISPLAYMODE DisplayMode;
		D3DPRESENT_PARAMETERS D3dParam;

		// Get d3d module address
		hDxModule = GetModuleHandleA("d3d9.dll");
		if (hDxModule == NULL)
		{
			ShutDownWindow();
			return false;
		}

		PDirect3DCreate9 = GetProcAddress(hDxModule, "Direct3DCreate9");
		if (PDirect3DCreate9 == NULL)
		{
			ShutDownWindow();
			return false;
		}

		PDirect3D9 = ((LPDIRECT3D9(__stdcall*)(DWORD))(PDirect3DCreate9))(D3D_SDK_VERSION);
		if (PDirect3D9 == NULL)
		{
			ShutDownWindow();
			return false;
		}

		if (PDirect3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DisplayMode) < 0)
		{
			ShutDownWindow();
			return false;
		}

		D3dParam = { 0,0,DisplayMode.Format,0,D3DMULTISAMPLE_NONE,NULL,D3DSWAPEFFECT_DISCARD,this->hWindow,1,0,D3DFMT_UNKNOWN,NULL,0,0 };

		// Create d3d device
		if (PDirect3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, this->hWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT, &D3dParam, &Device) < 0)
		{
			ShutDownWindow();
			PDirect3D9->Release();
			return false;
		}

		// Get methodstable
		this->MethodsTable = reinterpret_cast<Address*>(calloc(119, sizeof(Address)));
		memcpy(this->MethodsTable, *reinterpret_cast<Address**>(Device), 119 * sizeof(Address));
		// MinHook Init
		MH_Initialize();

		// Release
		PDirect3D9->Release();
		Device->Release();
		ShutDownWindow();
		
		this->PTarget = reinterpret_cast<LPVOID>(this->MethodsTable[42]);
		if (MH_CreateHook(this->PTarget, Function, Original) == MH_OK)
		{
			if (MH_EnableHook(this->PTarget) == MH_OK)
			{
				return true;
			}
		}

		return false;
	}
	else if (this->DirectXType == DXTYPE::D3D11)
	{
		void* D3D11CreateDeviceAndSwapChain;
		D3D_FEATURE_LEVEL FeatureLevel;
		D3D_FEATURE_LEVEL FeatureLevels[] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };
		DXGI_RATIONAL RefreshRate;
		DXGI_MODE_DESC ModeDesc;
		DXGI_SAMPLE_DESC SampleDesc;
		DXGI_SWAP_CHAIN_DESC SwapChainDesc;
		IDXGISwapChain* SwapChain;
		ID3D11Device* Device;
		ID3D11DeviceContext* Context;

		hDxModule = GetModuleHandleA("d3d11.dll");

		if (hDxModule == NULL)
		{
			ShutDownWindow();
			return false;
		}
		D3D11CreateDeviceAndSwapChain = GetProcAddress(hDxModule, "D3D11CreateDeviceAndSwapChain");
		if (D3D11CreateDeviceAndSwapChain == 0)
		{
			ShutDownWindow();
			return false;
		}
		// Datas init.
		RefreshRate.Numerator = 60;
		RefreshRate.Denominator = 1;
		SampleDesc.Count = 1;
		SampleDesc.Quality = 0;

		ModeDesc.Width = 100;
		ModeDesc.Height = 100;
		ModeDesc.RefreshRate = RefreshRate;
		ModeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		ModeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		ModeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		SwapChainDesc.BufferDesc = ModeDesc;
		SwapChainDesc.SampleDesc = SampleDesc;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.BufferCount = 1;
		SwapChainDesc.OutputWindow = this->hWindow;
		SwapChainDesc.Windowed = 1;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// Create D3D11 device and swapchain.
		if (((long(__stdcall*)(
			IDXGIAdapter*,
			D3D_DRIVER_TYPE,
			HMODULE,
			UINT,
			const D3D_FEATURE_LEVEL*,
			UINT,
			UINT,
			const DXGI_SWAP_CHAIN_DESC*,
			IDXGISwapChain**,
			ID3D11Device**,
			D3D_FEATURE_LEVEL*,
			ID3D11DeviceContext**))(D3D11CreateDeviceAndSwapChain))(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, FeatureLevels, 1, D3D11_SDK_VERSION, &SwapChainDesc, &SwapChain, &Device, &FeatureLevel, &Context) < 0)
		{
			ShutDownWindow();
			return false;
		}

		// Get methodstable
		this->MethodsTable = reinterpret_cast<Address*>(calloc(205, sizeof(Address)));
		memcpy(this->MethodsTable, *reinterpret_cast<Address**>(SwapChain), sizeof(Address) * 18);
		memcpy(this->MethodsTable+18, *reinterpret_cast<Address**>(Device), sizeof(Address) * 43);
		memcpy(this->MethodsTable + 61, *reinterpret_cast<Address**>(Context), sizeof(Address) * 144);

		// MinHook Init
		MH_Initialize();

		// Release
		SwapChain->Release();
		Device->Release();
		Context->Release();

		ShutDownWindow();

		this->PTarget = reinterpret_cast<LPVOID>(this->MethodsTable[8]);
		if (MH_CreateHook(this->PTarget, Function, Original) == MH_OK)
		{
			if (MH_EnableHook(this->PTarget) == MH_OK)
			{
				return true;
			}
		}
		return false;
	}
}

bool DirectXHook::End()
{
	if (this->DirectXType != DXTYPE::NONE)
	{
		if (this->PTarget != nullptr)
		{
			return MH_DisableHook(this->PTarget) == MH_OK;
		}
	}
	return false;
}

bool DirectXHook::CreateDrawWindow()
{
	this->WClass.cbSize = sizeof(WNDCLASSEXA);
	this->WClass.style = CS_HREDRAW | CS_VREDRAW;
	this->WClass.lpfnWndProc = DefWindowProc;
	this->WClass.cbClsExtra = 0;
	this->WClass.cbWndExtra = 0;
	this->WClass.hInstance = GetModuleHandle(NULL);
	this->WClass.hIcon = NULL;
	this->WClass.hCursor = NULL;
	this->WClass.hbrBackground = NULL;
	this->WClass.lpszMenuName = NULL;
	this->WClass.lpszClassName = "Draw";
	this->WClass.hIconSm = NULL;

	RegisterClassExA(&this->WClass);

	this->hWindow = ::CreateWindowA(this->WClass.lpszClassName, "DrawWindow", WS_OVERLAPPEDWINDOW, 0, 0, 50, 50, NULL, NULL, this->WClass.hInstance, NULL);
	return this->hWindow != NULL;
}