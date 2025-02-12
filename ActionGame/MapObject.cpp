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

	// X�����̒���
	if (rightA > leftB && leftA < rightB) {
		if (targetPos.x < leftA) {
			target->SetPos(DirectX::SimpleMath::Vector3(target->GetPrevPos().x, targetPos.y, 0.0f));
		}
		else if (targetPos.x > rightA) {
			target->SetPos(DirectX::SimpleMath::Vector3(target->GetPrevPos().x, targetPos.y, 0.0f));
		}
	}

	//// Y�����̒���
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

    // �ォ��u���b�N�ɂ߂荞�񂾏ꍇ
    if (velocityY < 0.0f && targetPos.y > topA) {
        target->SetPos(DirectX::SimpleMath::Vector3(targetPos.x, topA + targetSize.y * 0.5f, 0.0f));
        target->SetVelocity(DirectX::SimpleMath::Vector3(target->GetVelocity().x, 0.0f, 0.0f)); // Y���x�����Z�b�g
    }
    // ������u���b�N�ɂ߂荞�񂾏ꍇ
    else if (velocityY > 0.0f && targetPos.y < bottomA) {
        target->SetPos(DirectX::SimpleMath::Vector3(targetPos.x, bottomA - targetSize.y * 0.5f, 0.0f));
        target->SetVelocity(DirectX::SimpleMath::Vector3(target->GetVelocity().x, 0.0f, 0.0f)); // Y���x�����Z�b�g
    }
}
}


