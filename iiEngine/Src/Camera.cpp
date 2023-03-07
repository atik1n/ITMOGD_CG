#include "Camera.h"

#include "DisplayWin32.h"

void Camera::SetLookAt(DirectX::SimpleMath::Vector3 target) {
    lookAtPos = target;
}

void Camera::SetPosition(DirectX::SimpleMath::Vector3 pos) {
    cameraPos = pos;
}

void Camera::SetForward(DirectX::SimpleMath::Vector3 target) {
    forwardVector = target;
}

void Camera::SetLook(bool target) {
    lookAt = target;
}

void Camera::SetOrtho(bool target) {
    isOrtho = target;
}

DirectX::SimpleMath::Vector3 Camera::GetLookAt() {
    return lookAtPos;
}

DirectX::SimpleMath::Vector3 Camera::GetPosition() {
    return cameraPos;
}

DirectX::SimpleMath::Vector3 Camera::GetForward() {
    return forwardVector;
}

DirectX::SimpleMath::Vector3 Camera::GetUp() {
    return upVector;
}

DirectX::SimpleMath::Vector3 Camera::GetRight() {
    return forwardVector.Cross(upVector);
}

DirectX::XMMATRIX Camera::GetViewMatrix() {
    return viewMatrix;
}

DirectX::XMMATRIX Camera::GetProjMatrix() {
    return projMatrix;
}

void Camera::Update() {
    std::cout << cameraPos.x << ' ' << cameraPos.y << std::endl;
    
    if (lookAt) {
        viewMatrix = DirectX::XMMatrixLookAtLH(cameraPos, lookAtPos, upVector);
    } else {
        auto lookAt = cameraPos + forwardVector;
        viewMatrix = DirectX::XMMatrixLookAtLH(cameraPos, lookAt, upVector);
    }

    float fovDegrees = 90.f;
    float fovRadians = (fovDegrees / 360.f) * DirectX::XM_2PI;
    float aspectRatio = static_cast<float>(Game::instance->GetDisplay().GetScreenWidth()) / Game::instance->GetDisplay().GetScreenHeight();

    if (isOrtho) {
        projMatrix = DirectX::XMMatrixOrthographicLH(aspectRatio * 1.5f, 1.5f, .1f, 1000.f);
    } else {
        projMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, .1f, 1000.f);
    }
}
