#include "Floor.h"

Floor::Floor() {
    // 初期サイズの設定
    Width = size.x * 0.5f;
    Height = size.y * 0.5f;
}

void Floor::Update(const std::vector<Entity*>& entities) {
    for (Entity* target : entities) {
        if (CheckCollision(target)) {
            target->SetOnGround(true);

            // エンティティの中心位置とサイズを取得
            DirectX::SimpleMath::Vector3 targetPos = target->GetPos(); // 中心座標
            DirectX::SimpleMath::Vector3 targetSize = target->GetSize();
            DirectX::SimpleMath::Vector3 blockPos = GetPos();  // ブロックの中心座標
            DirectX::SimpleMath::Vector3 blockSize = GetSize();  // ブロックのサイズ

            // ブロックの範囲を計算（中心からの半分の幅・高さを使う）
            float leftA = blockPos.x - blockSize.x * 0.5f;
            float rightA = blockPos.x + blockSize.x * 0.5f;
            float topA = blockPos.y + blockSize.y * 0.5f;
            float bottomA = blockPos.y - blockSize.y * 0.5f;

            // エンティティの範囲を計算
            float leftB = targetPos.x - targetSize.x * 0.5f;
            float rightB = targetPos.x + targetSize.x * 0.5f;
            float topB = targetPos.y + targetSize.y * 0.5f;
            float bottomB = targetPos.y - targetSize.y * 0.5f;

            // X方向の調整
            if (rightA > leftB && leftA < rightB) {
                // 右方向に重なっている場合、エンティティをブロックの右側に配置
                if (targetPos.x < leftA) {
                    target->SetPos(DirectX::SimpleMath::Vector3(target->GetPrevPos().x, targetPos.y, 0.0f)); // 左側の重なり
                }
                // 左方向に重なっている場合、エンティティをブロックの左側に配置
                else if (targetPos.x > rightA) {
                    target->SetPos(DirectX::SimpleMath::Vector3(target->GetPrevPos().x, targetPos.y, 0.0f)); // 右側の重なり
                }
            }

            // Y方向の調整
            if (bottomA < topB && topA > bottomB) {
                // 上方向に重なっている場合
                if (bottomB < topA) {
                    //target->SetPos(DirectX::SimpleMath::Vector3(targetPos.x, target->GetPrevPos().y, 0.0f)); // 上側の重なり
                    target->SetPos(DirectX::SimpleMath::Vector3(targetPos.x, pos.y + size.y, 0.0f)); // 上側の重なり
                    Vec3 resetVelocityY = target->GetVelocity();
                    resetVelocityY.y = 0.0f;
                    target->SetVelocity(resetVelocityY);
                }
                // 下方向に重なっている場合は反発せず、めり込まない位置に調整
                else if (targetPos.y > bottomA) {
                    target->SetPos(DirectX::SimpleMath::Vector3(targetPos.x, target->GetPrevPos().y, 0.0f)); // 下側の重なり
                }
            }
        }
    }
}