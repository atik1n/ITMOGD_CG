#include "RenderableComponent.h"

void RenderableComponent::Initialize() {
	InitBuffers();
	InitShaders();
	InitRasterizer();
}

void RenderableComponent::DeleteData() {
	delete vertices;
	verticesAmount = 0;
	delete indices;
	indicesAmount = 0;
}

RenderableComponent::~RenderableComponent() {
	delete vb;
	delete ib;
	delete cb;
	DeleteData();
}


void RenderableComponent::InitBufferVertex() {
	vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * verticesAmount;

	vertexData = {};
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	Game::instance->GetDevice()->CreateBuffer(&vertexBufDesc, &vertexData, &vb);
}

void RenderableComponent::InitBufferIndex() {
	indexBufDesc = {};
	indexBufDesc.ByteWidth = sizeof(int) * indicesAmount;
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;

	indexData = {};
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	Game::instance->GetDevice()->CreateBuffer(&indexBufDesc, &indexData, &ib);
}

void RenderableComponent::InitBufferConstant() {
	constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DEFAULT;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = 0;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = 64;

	Game::instance->GetDevice()->CreateBuffer(&constBufDesc, nullptr, &cb);
}

void RenderableComponent::InitBuffers() {
	InitBufferVertex();
	InitBufferIndex();
	InitBufferConstant();
}

void RenderableComponent::InitRasterizer() {
	rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	Game::instance->SetRes(Game::instance->GetDevice()->CreateRasterizerState(&rastDesc, &rastState));
	Game::instance->GetContext().RSSetState(rastState);
}

void RenderableComponent::InitShaders() {
	D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };
	ID3DBlob* errorPixelCode;
	ID3DBlob* errorVertexCode = nullptr;

	// Compile pixel shader
	Game::instance->SetRes(
		D3DCompileFromFile(
			shaderPath,
			Shader_Macros,
			nullptr,
			"PSMain",
			"ps_5_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&pixelShaderByteCode,
			&errorPixelCode
		)
	);

	// Compile vertex shader
	Game::instance->SetRes(
		D3DCompileFromFile(
			shaderPath,
			nullptr,
			nullptr,
			"VSMain",
			"vs_5_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // Different flags (for example code translation)
			0,
			&vertexShaderByteCode,
			&errorVertexCode
		)
	);

	if (FAILED(Game::instance->GetRes())) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());
			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
			MessageBox(Game::instance->GetDisplay().GetHWnd(),
				L"MyVeryFirstShader.hlsl", L"Missing Shader File", MB_OK);
		return;
	}

	Game::instance->GetDevice()->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, &pixelShader
	);

	Game::instance->GetDevice()->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, &vertexShader
	);

	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	Game::instance->GetDevice()->CreateInputLayout(
		inputElements,
		2,
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		&layout
	);
}

void RenderableComponent::Update() {
	auto camera = Game::instance->camera;
	auto mat = worldMatrix * camera->GetViewMatrix() * camera->GetProjMatrix();;
	mat = DirectX::XMMatrixTranspose(mat);
	auto cbMat = DirectX::XMFLOAT4X4();
	DirectX::XMStoreFloat4x4(&cbMat, mat);
	Game::instance->GetContext().UpdateSubresource(cb, 0, 0, &cbMat, 0, 0);
}

void RenderableComponent::Draw() {
	Game::instance->GetContext().IASetInputLayout(layout);
	Game::instance->GetContext().IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Game::instance->GetContext().IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
	Game::instance->GetContext().IASetVertexBuffers(0, 1, &vb, new UINT[1] {alignof(DirectX::XMFLOAT4) * 8}, new UINT[1] {0});

	Game::instance->GetContext().VSSetConstantBuffers(0, 1, &cb);
	Game::instance->GetContext().VSSetShader(vertexShader, nullptr, 0);
	Game::instance->GetContext().PSSetShader(pixelShader, nullptr, 0);

	Game::instance->GetContext().RSSetState(rastState);
	Game::instance->GetContext().DrawIndexed(indicesAmount, 0, 0);
}

void RenderableComponent::Reload() {
	
}

void RenderableComponent::DestroyResources() {
	
}

void RenderableComponent::UpdateWorldMatrix() {
	worldMatrix = DirectX::XMMatrixIdentity();
	worldMatrix *= DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	worldMatrix *= DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	worldMatrix *= DirectX::XMMatrixTranslation(position.x, position.y, position.z);
}

void RenderableComponent::SetColor(DirectX::XMFLOAT4 newColor) {
	color = newColor;
	for (int i = 1; i < verticesAmount; i += 2) {
		vertices[i] = color;
	}
}

void RenderableComponent::SetColor(float r, float g, float b, float a) {
	SetColor(DirectX::XMFLOAT4(r, g, b, a));
}

DirectX::XMFLOAT4 RenderableComponent::GetColor() {
	return color;
}

void RenderableComponent::SetScale(DirectX::XMFLOAT3 newScale) {
	scale = newScale;
	UpdateWorldMatrix();
}

void RenderableComponent::SetScale(float x, float y, float z) {
	SetScale(DirectX::XMFLOAT3(x, y, z));
}

DirectX::XMFLOAT3 RenderableComponent::GetScale() {
	return scale;
}

void RenderableComponent::SetRotation(DirectX::XMFLOAT3 newRotation) {
	rotation = newRotation;
	UpdateWorldMatrix();
}

void RenderableComponent::SetRotation(float x, float y, float z) {
	SetRotation(DirectX::XMFLOAT3(x, y, z));
}

DirectX::XMFLOAT3 RenderableComponent::GetRotation() {
	return rotation;
}

void RenderableComponent::SetPosition(DirectX::XMFLOAT3 newPosition) {
	position = newPosition;
	UpdateWorldMatrix();
}

void RenderableComponent::SetPosition(float x, float y, float z) {
	SetPosition(DirectX::XMFLOAT3(x, y, z));
}

DirectX::XMFLOAT3 RenderableComponent::GetPosition() {
	return position;
}
