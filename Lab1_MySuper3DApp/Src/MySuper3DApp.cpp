#include "Game.h"
#include "QuadComponent.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Game::CreateInstance(L"My Game");
	auto quad1 = new QuadComponent();
	quad1->SetPosition(DirectX::XMFLOAT3(-0.2f, -0.2f, 0.0f));
	quad1->SetScale(0.5, 0.5, 0.5);
	Game::instance->components.push_back(quad1);
	auto quad2 = new QuadComponent();
	quad2->SetPosition(DirectX::XMFLOAT3(0.2f, 0.2f, 0.0f));
	quad2->SetScale(0.5, 0.5, 0.5);
	Game::instance->components.push_back(quad2);
	
	Game::instance->Run(800, 800);

	delete Game::instance;
}

int main() {
	return WinMain(GetModuleHandle(nullptr), nullptr, GetCommandLineA(), SW_SHOWNORMAL);
}