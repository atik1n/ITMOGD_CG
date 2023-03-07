#include "PongGame.h"

PongGame::PongGame(LPCWSTR name) :
    Game::Game(name), ballPos(DirectX::XMFLOAT2(0, 0)),
    racket1(new QuadComponent()),
    racket2(new QuadComponent()),
    ball(new QuadComponent()) {
    racket1->SetColor(.9f, .9f, .9f, 1.f);
	racket1->SetPosition(-.9f, 0, 0);
	racket1->SetScale(racketWidth, racketHeight, 1);
    racket2->SetColor(.9f, .9f, .9f, 1.f);
	racket2->SetPosition(.9f, 0, 0);
	racket2->SetScale(racketWidth, racketHeight, 1);
    ball->SetColor(.9f, .9f, .9f, 1.f);
	ball->SetScale(racketWidth, racketWidth, 1);

    components.push_back(racket1);
    components.push_back(racket2);
    components.push_back(ball);
}

void PongGame::CreateInstance(LPCWSTR name) {
    if (!instance)
        instance = new PongGame(name);
}

LRESULT PongGame::MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	switch (umessage) {
		case WM_KEYDOWN:
			std::cout << "Key Down: " << static_cast<unsigned int>(wparam) << std::endl;
			switch (static_cast<unsigned int>(wparam)) {
				case 27:
					PostQuitMessage(0);
					return 0;
				case 32: //space
					if (!started) {
						started = true;
						ballSpeed = racketSpeed;
					};
					break;
				case 38: //arrow up
					racketInput = 1.f;
					break;
				case 40: //arrow down
					racketInput = -1.f;
					break;
				default:
					break;
			}
			break;
		case WM_KEYUP:
			std::cout << "Key Up: " << static_cast<unsigned int>(wparam) << std::endl;
			switch (static_cast<unsigned int>(wparam)) {
				case 38:
				case 40:
					racketInput = .0f;
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

void PongGame::PrepareFrame() {
	Game::PrepareFrame();
	float color[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	context->ClearRenderTargetView(rtv, color);
}


void PongGame::Update() {
	float x = 0;
	float y = 0;
	
	Game::Update();
	
	ballDirection.Normalize();
	auto ballVelocity = ballDirection * ballSpeed * Game::instance->deltaTime;
	x = ball->GetPosition().x + ballVelocity.x;
	y = ball->GetPosition().y + ballVelocity.y;
	if (abs(x) > .9f) {
		auto& racket = x > 0 ? racket2 : racket1;
		if (abs(racket->GetPosition().y - y) <= racketHeight / 2) {
			x = (1.8f - abs(x)) * ((x > 0) - (x < 0));
			ballDirection.x *= -1;
			ballDirection.y += racketInput * 0.1;
			ballSpeed = min(ballSpeedMax, ballSpeed * 1.5);
		}
	}

	if (abs(y) > 1.f)
	{
		y = (2.f - abs(y)) * ((y > 0) - (y < 0));
		ballDirection.y *= -1;
	}
	ball->SetPosition(x, y, 0);

	if (abs(x) > 1.f) {
		if (x > 0) {
			score[0] += 1;
			ballDirection = DirectX::SimpleMath::Vector2(-1, 0);
		} else {
			score[1] += 1;
			ballDirection = DirectX::SimpleMath::Vector2(1, 0);
		}
		ball->SetPosition(0, 0, 0);
		ballSpeed = 0;
		started = false;
		std::cout << "AI " << score[0] << " | " << score[1] << " Player" << std::endl;
	}

	auto racketVelocity = racketInput * racketSpeed * Game::instance->deltaTime;
	racket2->SetPosition(racket2->GetPosition().x, racket2->GetPosition().y + racketVelocity, 0);

	float racketAi = ball->GetPosition().y - racket1->GetPosition().y < 0 ? -1 : 1;
	racketVelocity = racketAi * racketSpeed * Game::instance->deltaTime;
	racket1->SetPosition(racket1->GetPosition().x, racket1->GetPosition().y + racketVelocity, 0);
}
