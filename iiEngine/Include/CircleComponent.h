#pragma once
#include "RenderableComponent.h"

class CircleComponent : public RenderableComponent {
protected:
    static DirectX::XMFLOAT4* CalculateCircle(int);
    void CalculateVertices(int);
public:
    CircleComponent();
};
