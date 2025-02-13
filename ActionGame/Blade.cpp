#include "Blade.h"
#include "Block.h"

void Blade::Update(std::vector<Entity*>& eneties, std::vector<std::unique_ptr<MapObject>>&  blocks, std::vector<std::unique_ptr<BlockFragment>>& fragmentList) {
    for (auto it = blocks.begin(); it != blocks.end();) {
        // Block 型にキャスト
        Block* blo = dynamic_cast<Block*>(it->get());
        if (blo) {
            // ブロックと接触
            if (blo->CheckOBBHit(this)) {
                // ブロック破壊 & 破片生成
                blo->Destroy(fragmentList);
            }

            // 破壊フラグが立っていたら削除
            if (blo->GetFlg()) {
                it = blocks.erase(it);  // erase() の戻り値を it に代入
                continue;
            }
        }
        ++it;  // 次の要素へ
    }
}
