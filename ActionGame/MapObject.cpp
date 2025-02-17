#include "MapObject.h"
#include <DirectXMath.h>
#include <algorithm>
#include <cmath>
#include "Blade.h"
#undef min
#undef max


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

	// �u���b�N�̋�`
	float leftA = blockPos.x - blockSize.x * 0.5f;
	float rightA = blockPos.x + blockSize.x * 0.5f;
	float topA = blockPos.y + blockSize.y * 0.5f;
	float bottomA = blockPos.y - blockSize.y * 0.5f;
	float midA = (topA + bottomA) * 0.5f;  // �u���b�N�̒���Y���W

	// �Ώۂ̋�`
	float leftB = targetPos.x - targetSize.x * 0.5f;
	float rightB = targetPos.x + targetSize.x * 0.5f;
	float topB = targetPos.y + targetSize.y * 0.5f;
	float bottomB = targetPos.y - targetSize.y * 0.5f;

	// �I�[�o�[���b�v�ʁi�H�����ݗʁj���v�Z�istd::min �Ȃ��j
	float overlapX = (rightA - leftB < rightB - leftA) ? (rightA - leftB) : (rightB - leftA);
	float overlapY = (topA - bottomB < topB - bottomA) ? (topA - bottomB) : (topB - bottomA);

	target->SetHitState(0);

	// �ł������������ŕ␳
	if (overlapX < overlapY) {
		// X�����̕␳
		if (targetPos.x < blockPos.x) {
			target->SetPos(DirectX::SimpleMath::Vector3(leftA - size.x * 0.5f - 3.0f, targetPos.y, 0.0f));
			target->SetHitState(1);
		}
		else {
			target->SetPos(DirectX::SimpleMath::Vector3(rightA + size.x * 0.5f + 3.0f, targetPos.y, 0.0f));
			target->SetHitState(2);
		}
		target->SetVelocity(DirectX::SimpleMath::Vector3(0.0f, target->GetVelocity().y, 0.0f)); // X�����̑��x�����Z�b�g
	}
	else {
		// Y�����̕␳�i�u���b�N�̏㔼���E�������𔻒�j
		float velocityY = target->GetVelocity().y;

		if (targetPos.y > midA) {
			// �㔼�� �� �n�ʂɏ��
			target->SetPos(DirectX::SimpleMath::Vector3(targetPos.x, topA + size.y * 0.5f + 3.0f, 0.0f));
			target->SetVelocity(DirectX::SimpleMath::Vector3(target->GetVelocity().x, 0.0f, 0.0f)); // Y���x�����Z�b�g
			target->SetHitState(3);  // �n�ʂɐڐG
		}
		else {
			// ������ �� �V��ɂԂ���
			target->SetPos(DirectX::SimpleMath::Vector3(targetPos.x, bottomA - size.y * 0.5f - 3.0f, 0.0f));
			target->SetVelocity(DirectX::SimpleMath::Vector3(target->GetVelocity().x, 0.0f, 0.0f)); // Y���x�����Z�b�g
			target->SetHitState(4);  // �V��ɐڐG
		}
	}
}



// ��]���Ă��`�̓����蔻��
bool MapObject::CheckOBBHit(const Object* target) {
	// �Ώۂ���
	const MapObject* mapTarget = dynamic_cast<const MapObject*>(target);
	if (mapTarget) {
		return Collision_OBB_OBB(this->GetOBBVertices(), mapTarget->GetOBBVertices());
	}

	// �Ώۂ�����
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

