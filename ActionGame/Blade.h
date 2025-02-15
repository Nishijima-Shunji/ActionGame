#pragma once
#include "Object.h"
#include "MapObject.h"
#include "Entity.h"
#include "BlockFragment.h"
#include "Input.h"

class Blade :
    public Object
{
private:
    Input input;
    bool isAttack = false;        // 
    float currentAngle = 0.0f;    // ���݂̐U��p�x
    float startAngle = 180.0f;    // �U��n�߂̊p�x (�ォ��J�n)
    float swingSpeed = 7.5f;     // 1�t���[��������̉�]���x
    int swingDirection = 1;       // ��]���� (1: ���v���, -1: �����v���)
public:
    void Update(std::vector<Entity*>& eneties, std::vector<std::unique_ptr<MapObject>>& blocks, std::vector<std::unique_ptr<BlockFragment>>&fragmentList);
};

