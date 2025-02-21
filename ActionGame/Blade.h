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
    float currentAngle = 0.0f;    // 現在の振る角度
    float startAngle = 180.0f;    // 振り始めの角度 (上から開始)
    float swingSpeed = 12.0f;      // 1フレームあたりの回転速度
    int swingDirection = 1;       // 回転方向 (1: 反時計回り, -1: 時計回り)
public:
    void Update(std::vector<Entity*>& eneties, std::vector<std::unique_ptr<MapObject>>& blocks, std::vector<std::unique_ptr<BlockFragment>>&fragmentList, Sound sound);
    void Swing(std::vector<Entity*>& entities, Sound);
    void HitBlocks(std::vector<std::unique_ptr<MapObject>>& blocks, std::vector<std::unique_ptr<BlockFragment>>& fragmentList, Sound sound);

    bool GetisAttack() { return isAttack; };
};

