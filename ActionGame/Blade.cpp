#include "Blade.h"
#include "Block.h"
#include "Player.h"

void Blade::Update(std::vector<Entity*>& entities, std::vector<std::unique_ptr<MapObject>>& blocks, std::vector<std::unique_ptr<BlockFragment>>& fragmentList) {
    input.Update();
    for (auto& obj : entities) {
        Player* player = dynamic_cast<Player*>(obj);
        if (input.GetKeyTrigger(VK_SHIFT) && !isAttack) {
            isAttack = true;
            currentAngle = 0.0f;

            // プレイヤーの向きに応じた開始角度を設定
            if (player->GetDirection()) {
                swingDirection = -1;   // 右向きなら時計回り
            }
            else {
                swingDirection = 1;  // 左向きなら反時計回り
            }
        }

        // 振る動作中
        if (isAttack) {
            currentAngle += swingSpeed;

            // 120° 回転したら振り動作を終了
            if (currentAngle >= 120.0f) {
                isAttack = false;
                currentAngle = 120.0f;
            }
        }

        // 武器の角度を更新
        angle = startAngle + swingDirection * currentAngle;
        float radian = DirectX::XMConvertToRadians(angle);
        float radius = 120.0f; // プレイヤーからの距離

        // プレイヤーの座標を基準にブレードの座標を計算
        pos.x = player->GetPos().x + radius * sin(radian);
        pos.y = player->GetPos().y - radius * cos(radian);
    }

    // ブロックの判定処理
    for (auto it = blocks.begin(); it != blocks.end();) {
        Block* blo = dynamic_cast<Block*>(it->get());
        if (blo && isAttack) {
            if (blo->CheckOBBHit(this)) {
                blo->Destroy(fragmentList);
            }
            if (blo->GetFlg()) {
                it = blocks.erase(it);
                continue;
            }
        }
        ++it;
    }
}


