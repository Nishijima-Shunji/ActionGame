#pragma once
#include "Object.h"
#include "MapObject.h"
#include "Entity.h"
#include "BlockFragment.h"
#include "Input.h"
#include "sound.h"

class Blade :
    public Object
{
private:
    Input input;
    bool isAttack = false;        // 
    float currentAngle = 0.0f;    // ���݂̐U��p�x
    float startAngle = 180.0f;    // �U��n�߂̊p�x (�ォ��J�n)
    float swingSpeed = 12.0f;      // 1�t���[��������̉�]���x
    int swingDirection = 1;       // ��]���� (1: �����v���, -1: ���v���)
public:
    void Update(std::vector<Entity*>& eneties, std::vector<std::unique_ptr<MapObject>>& blocks, std::vector<std::unique_ptr<BlockFragment>>&fragmentList, Sound sound);
    void Swing(std::vector<Entity*>& entities, Sound);
    void HitBlocks(std::vector<std::unique_ptr<MapObject>>& blocks, std::vector<std::unique_ptr<BlockFragment>>& fragmentList, Sound sound);

    bool GetisAttack() { return isAttack; };
};

