#include "BlockFragment.h"

BlockFragment::BlockFragment(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 vel) : velocity(vel), lifetime(3.0f) {
	this->pos = pos;
	Init(L"asset/Block.png");
	this->SetShaders(g_pDevice, "VertexSheder.hlsl", "PixelShader.hlsl");
	size.x = 10.0f;
	size.y = 10.0f;
	size.z = 0.0f;
}

void BlockFragment::Update(float deltaTime) {
	DirectX::XMFLOAT3 fragmentgravity = DirectX::XMFLOAT3(
		gravity.x * 80.0f,
		gravity.y * 5.0f,
		gravity.z
	);

	velocity.x += fragmentgravity.x * deltaTime;
	velocity.y += fragmentgravity.y * deltaTime;

	pos.x += velocity.x * deltaTime;
	pos.y += velocity.y * deltaTime;

	lifetime -= deltaTime;
	if (lifetime <= 0.0f) {

	}
}