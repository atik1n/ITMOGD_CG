#pragma once
#include "RenderableComponent.h"

class CircleComponent : public RenderableComponent {
protected:
    void CalculateVertices(int);
public:
    CircleComponent();
    static DirectX::XMFLOAT4* CalculateCircle(int);
};
