#include "Blade.h"
#include "Block.h"
#include "Player.h"

void Blade::Update(std::vector<Entity*>& entities, std::vector<std::unique_ptr<MapObject>>& blocks, std::vector<std::unique_ptr<BlockFragment>>& fragmentList) {
    input.Update();

    // ����U�蓮��
    Swing(entities);

    // ����Ɣj��\�u���b�N�̔���Ə���
    HitBlocks(blocks, fragmentList);
}

void Blade::Swing(std::vector<Entity*>& entities) {
    for (auto& obj : entities) {
        Player* player = dynamic_cast<Player*>(obj);
        if (input.GetKeyTrigger(VK_SHIFT) && !isAttack) {
            isAttack = true;
            currentAngle = 0.0f;

            // �v���C���[�̌����ɉ������J�n�p�x��ݒ�
            if (player->GetDirection()) {
                swingDirection = -1;   // �E�����Ȃ甽���v���
            }
            else {
                swingDirection = 1;  // �������Ȃ玞�v���
            }
        }

        // �U�铮�쒆
        if (isAttack) {
            currentAngle += swingSpeed;

            // 120�� ��]������U�蓮����I��
            if (currentAngle >= 120.0f) {
                isAttack = false;
                currentAngle = 120.0f;
            }
        }

        // ����̊p�x���X�V
        angle = startAngle + swingDirection * currentAngle;
        float radian = DirectX::XMConvertToRadians(angle);
        float radius = 120.0f; // �v���C���[����̋���

        // �v���C���[�̍��W����Ƀu���[�h�̍��W���v�Z
        pos.x = player->GetPos().x + radius * sin(radian);
        pos.y = player->GetPos().y - radius * cos(radian);
    }
}

void Blade::HitBlocks(std::vector<std::unique_ptr<MapObject>>& blocks, std::vector<std::unique_ptr<BlockFragment>>& fragmentList) {
    // �u���b�N�̔��菈��
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