#pragma once

#include "RenderableComponent.h"
#include "CircleComponent.h"

class SphereComponent : public RenderableComponent {
protected:
    void CalculateVertices(int, int);
public:
    SphereComponent();
};
