#pragma once

#include "Game.h"

class Camera {
protected:
    bool lookAt = false;
    bool isOrtho = false;
    DirectX::XMFLOAT4 lookAtPos = DirectX::XMFLOAT4(.0f, .0f, 1.f, 1.f);;
    DirectX::XMFLOAT4 cameraPos = DirectX::XMFLOAT4(.0f, .0f, -1.f, 1.f);;
    DirectX::XMFLOAT4 upVector = DirectX::XMFLOAT4(.0f, 1.f, .0f, 1.f);
    DirectX::XMMATRIX viewMatrix;
    DirectX::XMMATRIX projMatrix;
public:
    void SetLookAt(DirectX::XMFLOAT4);
    void SetPosition(DirectX::XMFLOAT4);
    void SetLook(bool);

    DirectX::XMFLOAT4 GetLookAt();
    DirectX::XMFLOAT4 GetPosition();

    DirectX::XMMATRIX GetViewMatrix();
    DirectX::XMMATRIX GetProjMatrix();
    
    void Update();
};
