#pragma once

#include "Game.h"
#include "CircleComponent.h"
#include "QuadComponent.h"
#include "SimpleMath.h"
#include "SimpleMath.inl"

class PongGame : public Game {
protected:
    DirectX::SimpleMath::Vector2 ballPos;
    float racketSpeed = .5f;
    float racketWidth = .02f;
    float racketHeight = .2f;
    float ballSpeed = .0f;
    int score[2] = {0, 0};
    DirectX::SimpleMath::Vector2 ballDirection = DirectX::SimpleMath::Vector2(1, 0);
    float ballSpeedMax = racketSpeed * 2;
    QuadComponent* racket1;
    float racketInput = .0f;
    QuadComponent* racket2;
    CircleComponent* ball;
    bool started = false;
    
    void Update() override;

public:
    PongGame(LPCWSTR name);

    void PrepareFrame() override;
    static void CreateInstance(LPCWSTR name);
    LRESULT MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) override;
};
