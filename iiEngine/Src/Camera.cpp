#include "Camera.h"

#include "DisplayWin32.h"

void Camera::SetLookAt(DirectX::XMFLOAT4 target) {
    lookAtPos = target;
}

void Camera::SetPosition(DirectX::XMFLOAT4 pos) {
    cameraPos = pos;
}

void Camera::SetLook(bool target) {
    lookAt = target;
}

DirectX::XMFLOAT4 Camera::GetLookAt() {
    return lookAtPos;
}

DirectX::XMFLOAT4 Camera::GetPosition() {
    return cameraPos;
}

DirectX::XMMATRIX Camera::GetViewMatrix() {
    return viewMatrix;
}

DirectX::XMMATRIX Camera::GetProjMatrix() {
    return projMatrix;
}

void Camera::Update() {
    auto cameraPosVec = DirectX::XMLoadFloat4(&cameraPos);
    auto lookAtPosVec = DirectX::XMLoadFloat4(&lookAtPos);
    auto upVectorVec = DirectX::XMLoadFloat4(&upVector);
    if (lookAt) {
        viewMatrix = DirectX::XMMatrixLookAtLH(cameraPosVec, lookAtPosVec, upVectorVec);
    } else {
        viewMatrix = DirectX::XMMatrixLookAtLH(cameraPosVec, lookAtPosVec, upVectorVec);
    }

    float fovDegrees = 90.f;
    float fovRadians = (fovDegrees / 360.f) * DirectX::XM_2PI;
    float aspectRatio = static_cast<float>(Game::instance->GetDisplay().GetScreenWidth()) / Game::instance->GetDisplay().GetScreenHeight();
    projMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, .1f, 1000.f);
}
