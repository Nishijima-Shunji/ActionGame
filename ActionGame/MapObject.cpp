#include "MapObject.h"
#include <DirectXMath.h>
#include <algorithm>
#include <cmath>
#include "Blade.h"

bool MapObject::CheckCollision(Entity* target) {
	// �l�p�`A�Ǝl�p�`B�͈̔͂��擾
	float leftA = pos.x - size.x / 2;
	float rightA = pos.x + size.x / 2;
	float topA = pos.y + size.y / 2;
	float bottomA = pos.y - size.y / 2;

	float leftB = target->GetPos().x - target->GetWidth() / 2;
	float rightB = target->GetPos().x + target->GetWidth() / 2;
	float topB = target->GetPos().y + target->GetHeight() / 2;
	float bottomB = target->GetPos().y - target->GetHeight() / 2;

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

bool MapObject::CheckOBBHit(const Object* target) {
	const MapObject* mapTarget = dynamic_cast<const MapObject*>(target);
	if (mapTarget) {
		return Collision_OBB_OBB(this->GetOBBVertices(), mapTarget->GetOBBVertices());
	}

	const Blade* bladeTarget = dynamic_cast<const Blade*>(target);
	if (bladeTarget) {
		return Collision_OBB_OBB(this->GetOBBVertices(), bladeTarget->GetOBBVertices());
	}

	return false; // �ǂ���ɂ��Y�����Ȃ��ꍇ�A�Փ˔���͍s��Ȃ�
}

bool MapObject::Collision_OBB_OBB(const std::array<Vector2, 4>& vertsA, const std::array<Vector2, 4>& vertsB)
{
	Vector2 sideDirs[] =
	{
		vertsA[0] - vertsA[1],
		vertsA[1] - vertsA[2],
		vertsB[0] - vertsB[1],
		vertsB[1] - vertsB[2]
	};

	// �e�x�N�g���𐳋K��
	for (int i = 0; i < 4; i++)
	{
		sideDirs[i].Normalize();
	}

	for (const auto& u : sideDirs)
	{
		float minA = vertsA[0].Dot(u);
		float maxA = minA;
		float minB = vertsB[0].Dot(u);
		float maxB = minB;

		for (int i = 1; i < 4; i++)
		{
			float projA = vertsA[i].Dot(u);
			minA = (std::min)(minA, projA); 
			maxA = (std::max)(maxA, projA);

			float projB = vertsB[i].Dot(u);
			minB = (std::min)(minB, projB);
			maxB = (std::max)(maxB, projB);
		}

		if (maxB < minA || maxA < minB)
		{
			return false;
		}
	}
	return true;
}

