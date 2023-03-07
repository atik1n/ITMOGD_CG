#include "Game.h"

Game* Game::instance = nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	return Game::instance->MessageHandler(hwnd, umessage, wparam, lparam);
}

void Game::CreateInstance(LPCWSTR name) {
	if (!instance)
		instance = new Game(name);
}

void Game::CreateBackBuffer() {
	featureLevel[0] = { D3D_FEATURE_LEVEL_11_1 };

	swapDesc = {}; // SwapChain Descriptor
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = display->GetScreenWidth();
	swapDesc.BufferDesc.Height = display->GetScreenHeight();
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = display->GetHWnd();
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDesc, // Descriptor
		&swapChain,
		&device,
		nullptr, // Feature level for device
		&context
	);

	if (FAILED(res)) {
		// Well, that was unexpected
	}

	res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backTex);
	res = device->CreateRenderTargetView(backTex, nullptr, &rtv);
}

void Game::PrepareResources() {
	CreateBackBuffer();
	
	viewport = {};
	viewport.Width = static_cast<float>(display->GetScreenWidth());
	viewport.Height = static_cast<float>(display->GetScreenHeight());
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;
}

void Game::Initialize() {
	// Compile shaders
	for (auto component : components)
		component->Initialize();
}

void Game::PrepareFrame() {
	context->ClearState();
	context->OMSetRenderTargets(1, &rtv, nullptr);
	context->RSSetViewports(1, &viewport);

	float color[] = { Game::instance->GetTotalTime(), 0.1f, 0.1f, 1.0f };
	context->ClearRenderTargetView(rtv, color);
}

void Game::Update() {
	camera->Update();
	for (auto component : components)
		component->Update();
}

void Game::Draw() {
	for (auto component : components)
		component->Draw();
}

void Game::RestoreTargets() {

}

void Game::EndFrame() {
	context->OMSetRenderTargets(0, nullptr, nullptr);

	swapChain->Present(1, 0); // Show what we've drawn
}

void Game::UpdateInternal() {
	auto curTime = std::chrono::steady_clock::now();
	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - *prevTime).count() / 1000000.0f;
	*prevTime = curTime;

	totalTime += deltaTime;
	frameCount++;

	if (totalTime > 1.0f) {
		float fps = frameCount / totalTime;

		totalTime -= 1.0f;

		WCHAR text[256];
		swprintf_s(text, TEXT("FPS: %f"), fps);
		SetWindowText(display->GetHWnd(), text);

		frameCount = 0;
	}

	PrepareFrame();
	Update();
	Draw();
	RestoreTargets();
	EndFrame();
}

void Game::Run(int screenWidth, int screenHeight) {
	display = new DisplayWin32(name, screenWidth, screenHeight, WndProc);
	camera = new Camera();

	PrepareResources();
	Initialize();

	isExitRequested = false;
	startTime = new std::chrono::time_point<std::chrono::steady_clock>();
	prevTime = new std::chrono::time_point<std::chrono::steady_clock>();
	*startTime = std::chrono::steady_clock::now();
	*prevTime = *startTime;
	totalTime = 0;
	frameCount = 0;
	
	MSG msg = {};
	while (!isExitRequested) {
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) {
				isExitRequested = true;
				break;
			}
		}
		UpdateInternal();
	}
	
	delete startTime;
	delete prevTime;
	DestroyResources();
	Exit();
}

LRESULT Game::MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	switch (umessage) {
		case WM_KEYDOWN: {
			// If a key is pressed send it to the input object so it can record that state.
			std::cout << "Key: " << static_cast<unsigned int>(wparam) << std::endl;
			if (static_cast<unsigned int>(wparam) == 27)
				PostQuitMessage(0);
			return 0;
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}
		default: {
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
	}
}

void Game::Exit() { }

void Game::DestroyResources() {
	for (auto component : components)
		component->DestroyResources();
}


DisplayWin32& Game::GetDisplay() {
	return *display;
}

HRESULT Game::GetRes() {
	return res;
}

void Game::SetRes(HRESULT res) {
	this->res = res;
}

Microsoft::WRL::ComPtr<ID3D11Device>& Game::GetDevice() {
	return device;
}

ID3D11RenderTargetView* Game::GetRTV() {
	return rtv;
}

IDXGISwapChain* Game::GetSwapChain() {
	return swapChain;
}

ID3D11DeviceContext& Game::GetContext() {
	return *context;
}

float Game::GetTotalTime() {
	return totalTime;
}

void Game::SetTotalTime(float totalTime) {
	this->totalTime = totalTime;
}

unsigned int Game::GetFrameCount() {
	return frameCount;
}

void Game::SetFrameCount(unsigned int frameCount) {
	this->frameCount = frameCount;
}
