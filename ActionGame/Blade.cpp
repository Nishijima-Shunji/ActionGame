#include "Blade.h"
#include "Block.h"

void Blade::Update(std::vector<Entity*>& eneties, std::vector<std::unique_ptr<MapObject>>&  blocks, std::vector<std::unique_ptr<BlockFragment>>& fragmentList) {
    for (auto it = blocks.begin(); it != blocks.end();) {
        // Block �^�ɃL���X�g
        Block* blo = dynamic_cast<Block*>(it->get());
        if (blo) {
            // �u���b�N�ƐڐG
            if (blo->CheckOBBHit(this)) {
                // �u���b�N�j�� & �j�А���
                blo->Destroy(fragmentList);
            }

            // �j��t���O�������Ă�����폜
            if (blo->GetFlg()) {
                it = blocks.erase(it);  // erase() �̖߂�l�� it �ɑ��
                continue;
            }
        }
        ++it;  // ���̗v�f��
    }
}
