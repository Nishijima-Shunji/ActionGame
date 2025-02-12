#pragma once
#include "Object.h"
class BlockFragment : public Object
{
private:
	DirectX::XMFLOAT3 velocity;  // ��юU�鑬�x
	float lifetime; // �j�Ђ̎��� (���Ԍo�߂ŏ�����)
public:
	BlockFragment(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 vel);
	void Update(float deltatime);

	float GetLifeTime() { return lifetime; };
};

