#include "Block.h"

Block::Block() {
	// �����T�C�Y�̐ݒ�
	Width = size.x * 0.5f;
	Height = size.y * 0.5f;
}

void Block::Update(const std::vector<Entity*>& entities) {

}

void Block::Destroy(std::vector<std::unique_ptr<BlockFragment>>& fragmentList) {
	// �j�Ђ̐�
	const int numFragments = 30;

	// �u���b�N�̈ʒu
	DirectX::XMFLOAT3 blockPos = this->pos;

	for (int i = 0; i < numFragments; i++) {
		// �����_���ȑ��x
		float vx = (rand() % 100 - 50) * 3.0f;
		float vy = (rand() % 100 - 50) * 3.0f;

		// �j�Ђ𐶐�
		fragmentList.push_back(std::make_unique<BlockFragment>(
			blockPos, DirectX::XMFLOAT3(vx, vy, 0.0f)
		));
	}
	deadFlg = true;
}