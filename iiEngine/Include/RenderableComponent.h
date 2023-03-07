#pragma once

#include "Game.h"

class RenderableComponent {
protected:
	DirectX::XMFLOAT4* vertices;
	int verticesAmount = 0;
	int* indices;
	int indicesAmount = 0;
	
	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.f, .0f, 1.f, 1.f);
	DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1.f, 1.f, 1.f);
	DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(.0f, .0f, .0f);
	DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(.0f, .0f, .0f);
	
	ID3D11InputLayout* layout;
	ID3D11PixelShader* pixelShader;
	ID3DBlob* pixelShaderByteCode;
	ID3D11RasterizerState* rastState;
	ID3D11VertexShader* vertexShader;
	ID3DBlob* vertexShaderByteCode;
	
	D3D11_BUFFER_DESC vertexBufDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	ID3D11Buffer* vb;
	D3D11_BUFFER_DESC indexBufDesc;
	D3D11_SUBRESOURCE_DATA indexData;
	CD3D11_RASTERIZER_DESC rastDesc;
	ID3D11Buffer* ib;
	D3D11_BUFFER_DESC constBufDesc;
	ID3D11Buffer* cb;

	DirectX::XMMATRIX worldMatrix;

	LPCWSTR shaderPath = L"./Src/Shaders/BasicShader.hlsl";
	
	virtual ~RenderableComponent();
	void DeleteData();
	
	virtual void InitBufferVertex();
	virtual void InitBufferIndex();
	virtual void InitBufferConstant();
	virtual void InitBuffers();
	virtual void InitRasterizer();
	virtual void InitShaders();

public:
	virtual void Initialize();
	virtual void Update();
	virtual void Draw();
	virtual void Reload();
	virtual void DestroyResources();

	virtual void UpdateWorldMatrix();
	
	virtual void SetColor(DirectX::XMFLOAT4);
	virtual void SetColor(float, float, float, float);
	virtual DirectX::XMFLOAT4 GetColor();
	virtual void SetScale(DirectX::XMFLOAT3);
	virtual void SetScale(float, float, float);
	virtual DirectX::XMFLOAT3 GetScale();
	virtual void SetRotation(DirectX::XMFLOAT3);
	virtual void SetRotation(float, float, float);
	virtual DirectX::XMFLOAT3 GetRotation();
	virtual void SetPosition(DirectX::XMFLOAT3);
	virtual void SetPosition(float, float, float);
	virtual DirectX::XMFLOAT3 GetPosition();
};
