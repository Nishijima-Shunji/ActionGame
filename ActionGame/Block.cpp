#include "Block.h"

Block::Block() {
	// 初期サイズの設定
	Width = size.x * 0.5f;
	Height = size.y * 0.5f;
}

void Block::Update(const std::vector<Entity*>& entities) {

}

void Block::Destroy(std::vector<std::unique_ptr<BlockFragment>>& fragmentList) {
	// 破片の数
	const int numFragments = 30;

	// ブロックの位置
	DirectX::XMFLOAT3 blockPos = this->pos;

	for (int i = 0; i < numFragments; i++) {
		// ランダムな速度
		float vx = (rand() % 100 - 50) * 3.0f;
		float vy = (rand() % 100 - 50) * 3.0f;

		// 破片を生成
		fragmentList.push_back(std::make_unique<BlockFragment>(
			blockPos, DirectX::XMFLOAT3(vx, vy, 0.0f)
		));
	}
	deadFlg = true;
}