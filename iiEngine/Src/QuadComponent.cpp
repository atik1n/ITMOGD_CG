#include "QuadComponent.h"

QuadComponent::QuadComponent() {
	verticesAmount = 8;
	vertices = new DirectX::XMFLOAT4[verticesAmount];
	indicesAmount = 6;
	indices = new int[indicesAmount] { 0, 1, 2, 1, 0, 3 };
	
	vertices[0] = DirectX::XMFLOAT4(1.f, 1.f, .0f, 1.f);
	vertices[1] = DirectX::XMFLOAT4(1.f , .0f, .0f, 1.f);

	vertices[2] = DirectX::XMFLOAT4(-1.f, -1.f, .0f, 1.f);
	vertices[3] = DirectX::XMFLOAT4(.0f, .0f, 1.f, 1.f);

	vertices[4] = DirectX::XMFLOAT4(1.f, -1.f, .0f, 1.f);
	vertices[5] = DirectX::XMFLOAT4(.0f, 1.f, .0f, 1.f);

	vertices[6] = DirectX::XMFLOAT4(-1.f, 1.f, .0f, 1.f);
	vertices[7] = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
}
