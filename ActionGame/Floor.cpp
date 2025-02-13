#include "Floor.h"

Floor::Floor() {
    // �����T�C�Y�̐ݒ�
    Width = size.x * 0.5f;
    Height = size.y * 0.5f;
}

void Floor::Update(const std::vector<Entity*>& entities) {
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
                if (bottomB < topA) {
                    //target->SetPos(DirectX::SimpleMath::Vector3(targetPos.x, target->GetPrevPos().y, 0.0f)); // �㑤�̏d�Ȃ�
                    target->SetPos(DirectX::SimpleMath::Vector3(targetPos.x, pos.y + size.y, 0.0f)); // �㑤�̏d�Ȃ�
                    Vec3 resetVelocityY = target->GetVelocity();
                    resetVelocityY.y = 0.0f;
                    target->SetVelocity(resetVelocityY);
                }
                // �������ɏd�Ȃ��Ă���ꍇ�͔��������A�߂荞�܂Ȃ��ʒu�ɒ���
                else if (targetPos.y > bottomA) {
                    target->SetPos(DirectX::SimpleMath::Vector3(targetPos.x, target->GetPrevPos().y, 0.0f)); // �����̏d�Ȃ�
                }
            }
        }
    }
}