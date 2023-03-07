#include "SphereComponent.h"

void SphereComponent::CalculateVertices(int points, int lines) {
    verticesAmount = points * lines * 2;
    vertices = new DirectX::XMFLOAT4[verticesAmount];
    indicesAmount = points * (lines - 1) * 6;
    indices = new int[indicesAmount];

    auto circleVertices = CircleComponent::CalculateCircle(points);

    auto step = DirectX::XM_PI / static_cast<float>(lines - 1);
    auto runner = .0f;
    for (int l = 0; l < lines; ++l) {
        for (int p = 0; p < points; ++p) {
            auto i = 2 * (l * points + p);
            vertices[i] = circleVertices[p];
            vertices[i + 1] = DirectX::XMFLOAT4(vertices[i].x, vertices[i].y, cos(runner), 1.f);
            vertices[i] = DirectX::XMFLOAT4(vertices[i].x * sin(runner), vertices[i].y * sin(runner), cos(runner), 1.f);
        }
        runner += step;
    }

    int i = 0;
    for (int l = 0; l < lines - 1; ++l) {
        for (int p = 0; p < points; ++p) {
            // indices[i] = l * points + p;
            // indices[i + 1] = l * points + p + 1;
            // indices[i + 2] = (l + 1) * points + p + 1;
            // indices[i + 3] = l * points + p;
            // indices[i + 4] = (l + 1) * points + p + 1;
            // indices[i + 5] = (l + 1) * points + p;
            indices[i] = l * points + p;
            indices[i + 2] = l * points + p + 1;
            indices[i + 1] = (l + 1) * points + p + 1;
            indices[i + 3] = l * points + p;
            indices[i + 5] = (l + 1) * points + p + 1;
            indices[i + 4] = (l + 1) * points + p;
            i += 6;
        }
        indices[i - 1] = (l + 1) * points;
        indices[i - 5] = (l + 1) * points;
        indices[i - 4] = l * points;
    }

    delete circleVertices;
}

SphereComponent::SphereComponent() {
    CalculateVertices(50, 50);
}
