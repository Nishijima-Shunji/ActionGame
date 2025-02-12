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


