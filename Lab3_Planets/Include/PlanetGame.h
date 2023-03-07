#pragma once

#include "Game.h"
#include "SphereComponent.h"
#include <windowsx.h>

class PlanetGame : public Game {
    bool isOrbiting = false;
    float timer = 0;
    int orbitTarget = 0;
    std::vector<SphereComponent*> planets;
    DirectX::XMINT2 mousePos = DirectX::XMINT2(0, 0);
    DirectX::XMINT2 mouseDelta = DirectX::XMINT2(0, 0);
    DirectX::XMINT3 cameraInput = DirectX::XMINT3(0, 0, 0);
    DirectX::SimpleMath::Vector3 cameraRotation = DirectX::SimpleMath::Vector3(0, 0, 0);
    DirectX::SimpleMath::Vector3 orbitRotation = DirectX::SimpleMath::Vector3(0, 0, 0);
    
    void Update() override;
public:
    PlanetGame(LPCWSTR name);
    
    static void CreateInstance(LPCWSTR name);
    void PrepareFrame() override;
    LRESULT MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) override;
};
