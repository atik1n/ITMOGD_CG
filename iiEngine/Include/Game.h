#pragma once

#include <windows.h>
#include <WinUser.h>
#include <wrl.h>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "Camera.h"
#include "RenderableComponent.h"
#include "DisplayWin32.h"
#include <iostream>
#include <chrono>
#include <vector>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

class Camera;
class DisplayWin32;
class RenderableComponent;

class Game {
protected:
	DisplayWin32* display;

	LPCWSTR name;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapChain;
	D3D_FEATURE_LEVEL featureLevel[1]; // Array
	DXGI_SWAP_CHAIN_DESC swapDesc;
	HRESULT res;
	ID3D11Texture2D* backTex;
	ID3D11RenderTargetView* rtv; // Back buffer?
	D3D11_VIEWPORT viewport;

	bool isExitRequested;

	Game(LPCWSTR name) {
		this->name = name;
	}

public:
	static Game* instance;
	Camera* camera;
	
	std::vector<RenderableComponent*> components;
	std::chrono::time_point<std::chrono::steady_clock>* startTime = nullptr;
	std::chrono::time_point<std::chrono::steady_clock>* prevTime = nullptr;
	float totalTime = 0;
	float deltaTime = 0;
	unsigned int frameCount;

	virtual ~Game() = default;
	
	static void CreateInstance(LPCWSTR name);

	virtual void CreateBackBuffer();
	virtual void PrepareResources();
	virtual void Initialize();
	virtual void PrepareFrame();
	virtual void Update();
	virtual void Draw();
	virtual void RestoreTargets();
	virtual void EndFrame();
	virtual void UpdateInternal();
	virtual void Run(int screenWidth, int screenHeight);
	virtual LRESULT MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
	virtual void Exit();
	virtual void DestroyResources();
	
	DisplayWin32& GetDisplay();

	Microsoft::WRL::ComPtr<ID3D11Device>& GetDevice();

	ID3D11DeviceContext& GetContext();

	IDXGISwapChain* GetSwapChain();

	HRESULT GetRes();
	void SetRes(HRESULT res);

	ID3D11RenderTargetView* GetRTV();

	float GetTotalTime();
	void SetTotalTime(float totalTime);

	unsigned int GetFrameCount();
	void SetFrameCount(unsigned int frameCount);
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
