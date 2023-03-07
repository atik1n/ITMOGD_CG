#include "PlanetGame.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    PlanetGame::CreateInstance(L"My Game");
    PlanetGame::instance->Run(1920, 1000);
    delete PlanetGame::instance;
}

int main() {
    return WinMain(GetModuleHandle(nullptr), nullptr, GetCommandLineA(), SW_SHOWNORMAL);
}
