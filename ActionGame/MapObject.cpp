#include "MapObject.h"

#include <DirectXMath.h>
#include <algorithm>
#include <cmath>

bool MapObject::CheckCollisionForDestruction(Entity* target) {
	// 四角形の中心座標
	DirectX::XMFLOAT3 rectPos = this->GetPos();
	float rectCenterX = rectPos.x;
	float rectCenterY = rectPos.y;
	float halfWidth = this->GetWidth() / 2.0f;
	float halfHeight = this->GetHeight() / 2.0f;

	// 円の中心座標
	DirectX::XMFLOAT3 circlePos = target->GetPos();
	float circleCenterX = circlePos.x;
	float circleCenterY = circlePos.y;
	float radius = target->GetRadius();

	// 円の中心に最も近い四角形の点を求める
	float closestX = std::max<float>(rectCenterX - halfWidth, std::min<float>(circleCenterX, rectCenterX + halfWidth));
	float closestY = std::max<float>(rectCenterY - halfHeight, std::min<float>(circleCenterY, rectCenterY + halfHeight));

	// 最短距離を求める
	float distanceX = circleCenterX - closestX;
	float distanceY = circleCenterY - closestY;
	float distanceSquared = distanceX * distanceX + distanceY * distanceY;

	// 円の半径の二乗と比較
	return distanceSquared <= radius * radius;
}

bool MapObject::CheckCollision(Entity* target) {
	// 四角形Aと四角形Bの範囲を取得
	float leftA		 = pos.x - size.x / 2;
	float rightA	 = pos.x + size.x / 2;
	float topA		 = pos.y + size.y / 2;
	float bottomA	 = pos.y - size.y / 2;

	float leftB		= target->GetPos().x - target->GetWidth() / 2;
	float rightB	= target->GetPos().x + target->GetWidth() / 2;
	float topB		= target->GetPos().y + target->GetHeight() / 2;
	float bottomB	= target->GetPos().y - target->GetHeight() / 2;

	// 横方向と縦方向で重なっているかチェック
	if (rightA > leftB && leftA < rightB && bottomA < topB && topA > bottomB)
	{
		return true; // 重なっている
	}

	return false; // 重なっていない
}	

void MapObject::ResolveCollision(Entity* target) {
	DirectX::SimpleMath::Vector3 targetPos = target->GetPos();
	DirectX::SimpleMath::Vector3 targetSize = target->GetSize();
	DirectX::SimpleMath::Vector3 blockPos = GetPos();
	DirectX::SimpleMath::Vector3 blockSize = GetSize();

	float leftA = blockPos.x - blockSize.x * 0.5f;
	float rightA = blockPos.x + blockSize.x * 0.5f;
	float topA = blockPos.y + blockSize.y * 0.5f;
	float bottomA = blockPos.y - blockSize.y * 0.5f;

	float leftB = targetPos.x - targetSize.x * 0.5f;
	float rightB = targetPos.x + targetSize.x * 0.5f;
	float topB = targetPos.y + targetSize.y * 0.5f;
	float bottomB = targetPos.y - targetSize.y * 0.5f;

	// X方向の調整
	if (rightA > leftB && leftA < rightB) {
		if (targetPos.x < leftA) {
			target->SetPos(DirectX::SimpleMath::Vector3(target->GetPrevPos().x, targetPos.y, 0.0f));
		}
		else if (targetPos.x > rightA) {
			target->SetPos(DirectX::SimpleMath::Vector3(target->GetPrevPos().x, targetPos.y, 0.0f));
		}
	}

	//// Y方向の調整
	//if (bottomA < topB && topA > bottomB) {
	//	if (targetPos.y > topA) {
	//		//target->SetPos(DirectX::SimpleMath::Vector3(targetPos.x, blockPos.y + blockSize.y * 0.5f, 0.0f));
	//		target->SetPos(DirectX::SimpleMath::Vector3(targetPos.x, target->GetPrevPos().y, 0.0f));
	//	}
	//	else if (targetPos.y < bottomA) {
	//		target->SetPos(DirectX::SimpleMath::Vector3(targetPos.x, target->GetPrevPos().y, 0.0f));
	//	}
	//}
	if (bottomA < topB && topA > bottomB) {
    float velocityY = target->GetVelocity().y;

    // 上からブロックにめり込んだ場合
    if (velocityY < 0.0f && targetPos.y > topA) {
        target->SetPos(DirectX::SimpleMath::Vector3(targetPos.x, topA + targetSize.y * 0.5f, 0.0f));
        target->SetVelocity(DirectX::SimpleMath::Vector3(target->GetVelocity().x, 0.0f, 0.0f)); // Y速度をリセット
    }
    // 下からブロックにめり込んだ場合
    else if (velocityY > 0.0f && targetPos.y < bottomA) {
        target->SetPos(DirectX::SimpleMath::Vector3(targetPos.x, bottomA - targetSize.y * 0.5f, 0.0f));
        target->SetVelocity(DirectX::SimpleMath::Vector3(target->GetVelocity().x, 0.0f, 0.0f)); // Y速度をリセット
    }
}
}


