#pragma once

#include "Game.h"
#include "SimpleMath.h"

class Camera {
protected:
    bool lookAt = false;
    bool isOrtho = false;
    DirectX::SimpleMath::Vector3 lookAtPos = DirectX::SimpleMath::Vector3(.0f, .0f, 1.f);
    DirectX::SimpleMath::Vector3 cameraPos = DirectX::SimpleMath::Vector3(.0f, .0f, -1.f);
    DirectX::SimpleMath::Vector3 upVector = DirectX::SimpleMath::Vector3(.0f, 1.f, .0f);
    DirectX::SimpleMath::Vector3 forwardVector = DirectX::SimpleMath::Vector3(.0f, .0f, 1.f);
    DirectX::XMMATRIX viewMatrix;
    DirectX::XMMATRIX projMatrix;
public:
    void SetLookAt(DirectX::SimpleMath::Vector3);
    void SetPosition(DirectX::SimpleMath::Vector3);
    void SetForward(DirectX::SimpleMath::Vector3);
    void SetLook(bool);
    void SetOrtho(bool);

    DirectX::SimpleMath::Vector3 GetLookAt();
    DirectX::SimpleMath::Vector3 GetPosition();
    DirectX::SimpleMath::Vector3 GetForward();
    DirectX::SimpleMath::Vector3 GetRight();
    DirectX::SimpleMath::Vector3 GetUp();

    DirectX::XMMATRIX GetViewMatrix();
    DirectX::XMMATRIX GetProjMatrix();
    
    void Update();
};
