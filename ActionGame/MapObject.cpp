#include "MapObject.h"

#include <DirectXMath.h>
#include <algorithm>
#include <cmath>

bool MapObject::CheckCollisionForDestruction(Entity* target) {
	// �l�p�`�̒��S���W
	DirectX::XMFLOAT3 rectPos = this->GetPos();
	float rectCenterX = rectPos.x;
	float rectCenterY = rectPos.y;
	float halfWidth = this->GetWidth() / 2.0f;
	float halfHeight = this->GetHeight() / 2.0f;

	// �~�̒��S���W
	DirectX::XMFLOAT3 circlePos = target->GetPos();
	float circleCenterX = circlePos.x;
	float circleCenterY = circlePos.y;
	float radius = target->GetRadius();

	// �~�̒��S�ɍł��߂��l�p�`�̓_�����߂�
	float closestX = std::max<float>(rectCenterX - halfWidth, std::min<float>(circleCenterX, rectCenterX + halfWidth));
	float closestY = std::max<float>(rectCenterY - halfHeight, std::min<float>(circleCenterY, rectCenterY + halfHeight));

	// �ŒZ���������߂�
	float distanceX = circleCenterX - closestX;
	float distanceY = circleCenterY - closestY;
	float distanceSquared = distanceX * distanceX + distanceY * distanceY;

	// �~�̔��a�̓��Ɣ�r
	return distanceSquared <= radius * radius;
}

bool MapObject::CheckCollision(Entity* target) {
	// �l�p�`A�Ǝl�p�`B�͈̔͂��擾
	float leftA		 = pos.x - size.x / 2;
	float rightA	 = pos.x + size.x / 2;
	float topA		 = pos.y + size.y / 2;
	float bottomA	 = pos.y - size.y / 2;

	float leftB		= target->GetPos().x - target->GetWidth() / 2;
	float rightB	= target->GetPos().x + target->GetWidth() / 2;
	float topB		= target->GetPos().y + target->GetHeight() / 2;
	float bottomB	= target->GetPos().y - target->GetHeight() / 2;

	// �������Əc�����ŏd�Ȃ��Ă��邩�`�F�b�N
	if (rightA > leftB && leftA < rightB && bottomA < topB && topA > bottomB)
	{
		return true; // �d�Ȃ��Ă���
	}

	return false; // �d�Ȃ��Ă��Ȃ�
}


