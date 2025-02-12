#include "Block.h"

void Block::Update(const std::vector<Entity*>& entities) {
    for (Entity* target : entities) {
        if (CheckCollision(target)) {
            target->SetOnGround(true);

            // �G���e�B�e�B�̒��S�ʒu�ƃT�C�Y���擾
            DirectX::SimpleMath::Vector3 targetPos = target->GetPos(); // ���S���W
            DirectX::SimpleMath::Vector3 targetSize = target->GetSize();
            DirectX::SimpleMath::Vector3 blockPos = GetPos();  // �u���b�N�̒��S���W
            DirectX::SimpleMath::Vector3 blockSize = GetSize();  // �u���b�N�̃T�C�Y

            // �u���b�N�͈̔͂��v�Z�i���S����̔����̕��E�������g���j
            float leftA = blockPos.x - blockSize.x * 0.5f;
            float rightA = blockPos.x + blockSize.x * 0.5f;
            float topA = blockPos.y + blockSize.y * 0.5f;
            float bottomA = blockPos.y - blockSize.y * 0.5f;

            // �G���e�B�e�B�͈̔͂��v�Z
            float leftB = targetPos.x - targetSize.x * 0.5f;
            float rightB = targetPos.x + targetSize.x * 0.5f;
            float topB = targetPos.y + targetSize.y * 0.5f;
            float bottomB = targetPos.y - targetSize.y * 0.5f;

            // X�����̒���
            if (rightA > leftB && leftA < rightB) {
                // �E�����ɏd�Ȃ��Ă���ꍇ�A�G���e�B�e�B���u���b�N�̉E���ɔz�u
                if (targetPos.x < leftA) {
                    target->SetPos(DirectX::SimpleMath::Vector3(target->GetPrevPos().x, targetPos.y, 0.0f)); // �����̏d�Ȃ�
                }
                // �������ɏd�Ȃ��Ă���ꍇ�A�G���e�B�e�B���u���b�N�̍����ɔz�u
                else if (targetPos.x > rightA) {
                    target->SetPos(DirectX::SimpleMath::Vector3(target->GetPrevPos().x, targetPos.y, 0.0f)); // �E���̏d�Ȃ�
                }
            }

            // Y�����̒���
            if (bottomA < topB && topA > bottomB) {
                // ������ɏd�Ȃ��Ă���ꍇ
                if (targetPos.y > topA) {
                    target->SetPos(DirectX::SimpleMath::Vector3(targetPos.x, pos.y + size.y, 0.0f)); // �㑤�̏d�Ȃ�
                }
                // �������ɏd�Ȃ��Ă���ꍇ�͔��������A�߂荞�܂Ȃ��ʒu�ɒ���
                else if (targetPos.y < bottomA) {
                    target->SetPos(DirectX::SimpleMath::Vector3(targetPos.x, target->GetPrevPos().y, 0.0f)); // �����̏d�Ȃ�
                }
            }
        }
        else {
            // target->SetOnGround(false);
        }
    }
}





void Block::Destroy(std::vector<std::unique_ptr<BlockFragment>>& fragmentList) {
	// �j�Ђ̐�
	const int numFragments = 30;

	// �u���b�N�̈ʒu
	DirectX::XMFLOAT3 blockPos = this->pos;

	for (int i = 0; i < numFragments; i++) {
		// �����_���ȑ��x
		float vx = (rand() % 100 - 50) * 10.0f; // -1.0 ~ 1.0
		float vy = (rand() % 100 - 50) * 10.0f; // -1.0 ~ 1.0

		// �j�Ђ𐶐�
		fragmentList.push_back(std::make_unique<BlockFragment>(
			blockPos, DirectX::XMFLOAT3(vx, vy, 0.0f)
		));
	}
	deadFlg = true;
}