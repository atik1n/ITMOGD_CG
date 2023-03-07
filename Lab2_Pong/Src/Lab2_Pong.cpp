#include "PongGame.h"
#include "QuadComponent.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    PongGame::CreateInstance(L"My Game");

    Game::instance->Run(800, 800);
    
    delete Game::instance;
}

int main() {
    return WinMain(GetModuleHandle(nullptr), nullptr, GetCommandLineA(), SW_SHOWNORMAL);
}
