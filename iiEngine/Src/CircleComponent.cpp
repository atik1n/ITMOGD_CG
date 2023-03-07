#include "CircleComponent.h"

DirectX::XMFLOAT4* CircleComponent::CalculateCircle(int points) {
    auto result = new DirectX::XMFLOAT4[points];
    
    auto step = DirectX::XM_2PI / static_cast<float>(points);
    auto runner = .0f;
    for (int i = 0; i < points; ++i) {
        result[i] = DirectX::XMFLOAT4(cos(runner), sin(runner), .0f, 1.f);
        runner += step;
    }

    return result;
}

void CircleComponent::CalculateVertices(int points) {
    auto tmpVertices = CalculateCircle(points);

    verticesAmount = (points + 1) * 2;
    vertices = new DirectX::XMFLOAT4[verticesAmount];
    indicesAmount = points * 3;
    indices = new int[indicesAmount];

    vertices[0] = DirectX::XMFLOAT4(.0f, .0f, .0f, 1.f);

    for (int i = 0; i < points; ++i) {
        vertices[2 * (i + 1)] = tmpVertices[i];
        std::cout << tmpVertices[i].x << std::endl;
    }
    for (int i = 0; i <= points; ++i) {
        vertices[2 * i + 1] = color;
    }

    for (int i = 0; i < points; ++i) {
        indices[i * 3] = 0;
        indices[i * 3 + 1] = i + 1;
        indices[i * 3 + 2] = i + 2;
    }
    indices[points * 3 - 1] = 1;

    delete tmpVertices;
}

CircleComponent::CircleComponent() {
    CalculateVertices(6);
}

