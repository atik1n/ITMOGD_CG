#include "PlanetGame.h"

PlanetGame::PlanetGame(LPCWSTR name) :
    Game::Game(name) {
    auto planet = new SphereComponent(); // Sun
    planet->SetScale(0.3, 0.3, 0.3);
    planet->SetPosition(0, 0, 0);
    components.push_back(planet);
    planets.push_back(planet);
    
    planet = new SphereComponent(); // Earth
    planet->SetScale(0.15, 0.15, 0.15);
    planet->SetPosition(-0.8, 0.6, 0);
    components.push_back(planet);
    planets.push_back(planet);

    planet = new SphereComponent(); // Mun
    planet->SetScale(0.05, 0.05, 0.05);
    planet->SetPosition(0.5, 0.5, 0);
    components.push_back(planet);
    planets.push_back(planet);

    planet = new SphereComponent(); // Dual A
    planet->SetScale(0.1, 0.1, 0.1);
    planet->SetPosition(0.5, 0.5, 0);
    components.push_back(planet);
    planets.push_back(planet);

    planet = new SphereComponent(); // Dual B
    planet->SetScale(0.1, 0.1, 0.1);
    planet->SetPosition(-0.6, -0.6, 0);
    components.push_back(planet);
    planets.push_back(planet);
}

void PlanetGame::CreateInstance(LPCWSTR name) {
    if (!instance)
        instance = new PlanetGame(name);
}

void PlanetGame::PrepareFrame() {
    Game::PrepareFrame();
    float color[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    context->ClearRenderTargetView(rtv, color);
}

void PlanetGame::Update() {
    timer += deltaTime;
    
    Game::Update();
    for (auto planet : planets) {
        planet->SetRotation(planet->GetRotation().x + deltaTime, planet->GetRotation().y + deltaTime, 0);
    }

    planets.at(1)->SetPosition(DirectX::SimpleMath::Vector3(sin(timer / 2),  0, cos(timer / 2)) * 0.9);
    
    auto earthPos = planets.at(1)->GetPosition();
    auto moonOffset = DirectX::SimpleMath::Vector3(sin(timer), sin(timer) / 3, cos(timer)) * 0.35;
    planets.at(2)->SetPosition(earthPos + moonOffset);

    auto dualPos = DirectX::SimpleMath::Vector3(sin(timer * 1.2), 0, cos(timer * 1.2)) * 1.5;
    moonOffset = DirectX::SimpleMath::Vector3(sin(timer * 5), sin(timer * 5) / 3, cos(timer * 5)) * 0.20;
    planets.at(3)->SetPosition(dualPos + moonOffset);
    planets.at(4)->SetPosition(dualPos - moonOffset);
    
    POINT newCursorPos;
    GetCursorPos(&newCursorPos);

    if (mousePos.x != 0 && mousePos.y != 0) {
        mouseDelta.x = newCursorPos.x - mousePos.x;
        mouseDelta.y = mousePos.y - newCursorPos.y;
    }
    mousePos = DirectX::XMINT2(newCursorPos.x, newCursorPos.y);

    auto mouseInput = DirectX::SimpleMath::Vector3(mouseDelta.x * 0.005, mouseDelta.y * -0.005, 0);
    cameraRotation += mouseInput;
    camera->SetForward(DirectX::SimpleMath::Vector3::Transform(
        DirectX::SimpleMath::Vector3(0, 0, 1),
        DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(cameraRotation.x, cameraRotation.y, 0)
    ));
    
    auto cameraMovement = cameraInput.x * camera->GetRight() + cameraInput.y * camera->GetUp() + cameraInput.z * camera->GetForward();
    
    if (isOrbiting) {
        auto parentPos = planets.at(orbitTarget)->GetPosition();
        camera->SetPosition(camera->GetPosition() + cameraMovement.z * (camera->GetPosition() - parentPos) * 0.01);
        
        orbitRotation += DirectX::SimpleMath::Vector3(cameraInput.x, cameraInput.y, cameraInput.z) * 0.001;
        auto tmp = DirectX::SimpleMath::Vector3::Transform(
            camera->GetPosition() - parentPos,
            DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(cameraInput.x * 0.01, cameraInput.y * 0.01, 0)
        );
        camera->SetPosition(parentPos + tmp);
    } else {
        camera->SetPosition(camera->GetPosition() + cameraMovement * 0.01);
    }
}

LRESULT PlanetGame::MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
    switch (umessage) {
        case WM_KEYDOWN:
            std::cout << "Key Down: " << static_cast<unsigned int>(wparam) << std::endl;
        switch (static_cast<unsigned int>(wparam)) {
            case 27:
                PostQuitMessage(0);
                return 0;
            case 17:
                cameraRotation = DirectX::SimpleMath::Vector3(0, 0, 0);
                break;
            case 87:
                cameraInput.z = 1;
                break;
            case 83:
                cameraInput.z = -1;
                break;
            case 65:
                cameraInput.x = 1;
                break;
            case 68:
                cameraInput.x = -1;
                break;
            case 32:
                cameraInput.y = 1;
                break;
            case 16:
                cameraInput.y = -1;
                break;
            case 112:
                camera->SetOrtho(false);
                break;
            case 113:
                camera->SetOrtho(true);
                break;
            case 114:
                isOrbiting = true;
                camera->SetPosition(planets.at(orbitTarget)->GetPosition() + DirectX::SimpleMath::Vector3(0, 0, 1));
                orbitRotation = DirectX::SimpleMath::Vector3(0, 0, 0);
                break;
            case 115:
                isOrbiting = false;
                camera->SetPosition(DirectX::SimpleMath::Vector3(0, 0, 0));
                break;
            case 37:
                orbitTarget = orbitTarget == 0 ? planets.size() - 1 : orbitTarget - 1;
                camera->SetPosition(DirectX::SimpleMath::Vector3(0, 0, 0));
                orbitRotation = DirectX::SimpleMath::Vector3(0, 0, 0);
                break;
            case 39:
                orbitTarget = orbitTarget == planets.size() - 1 ? 0 : orbitTarget + 1;
                camera->SetPosition(DirectX::SimpleMath::Vector3(0, 0, 0));
                orbitRotation = DirectX::SimpleMath::Vector3(0, 0, 0);
                break; 
            default:
                break;
        }
        break;
        case WM_KEYUP:
            std::cout << "Key Up: " << static_cast<unsigned int>(wparam) << std::endl;
        switch (static_cast<unsigned int>(wparam)) {
            case 87:
            case 83:
                cameraInput.z = 0;
                break;
            case 65:
            case 68:
                cameraInput.x = 0;
                break;
            case 32:
            case 16:
                cameraInput.y = 0;
            default:
                break;
        }
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        return 0;
        default:
            return DefWindowProc(hwnd, umessage, wparam, lparam);
    }
}
