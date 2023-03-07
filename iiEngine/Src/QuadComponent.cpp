#include "QuadComponent.h"

QuadComponent::QuadComponent() {
	points = new DirectX::XMFLOAT4[8];
	pointsAmount = 8;
	indices = new int[] { 0, 1, 2, 1, 0, 3 };
	indicesAmount = 6;
	
	points[0] = DirectX::XMFLOAT4(1.f, 1.f, .0f, 1.f);
	points[1] = DirectX::XMFLOAT4(1.f , .0f, .0f, 1.f);

	points[2] = DirectX::XMFLOAT4(-1.f, -1.f, .0f, 1.f);
	points[3] = DirectX::XMFLOAT4(.0f, .0f, 1.f, 1.f);

	points[4] = DirectX::XMFLOAT4(1.f, -1.f, .0f, 1.f);
	points[5] = DirectX::XMFLOAT4(.0f, 1.f, .0f, 1.f);

	points[6] = DirectX::XMFLOAT4(-1.f, 1.f, .0f, 1.f);
	points[7] = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
}
