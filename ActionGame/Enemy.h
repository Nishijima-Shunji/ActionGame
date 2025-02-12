#pragma once
#include "Object.h"
#include <DirectXMath.h>
#include "TextureManager.h"
#include "GameScene.h"

//前方宣言
class GameScene;

class Enemy : public Entity {
private:
    GameScene* gameScene;

    DirectX::SimpleMath::Vector3 targetPos;
    DirectX::SimpleMath::Vector3 velocity;


    int enemyType;
    float moveSpeed;                // 速度
    int health = 3;			    // HP
    float radius = 20.0f;       // 当たり判定の半径

    bool deadFlg = false;       //死亡済みかどうかのフラグ
    int state = 0;

    int attackTime;

public:
    Enemy(int enemyType);

    void Update(); 

    void move();

    float GetRadius() const;
    int GetHealth() const;
    bool GetDeadFlg() const;
    int GetState() const;
    int GetEnemyType() const;

    void DecreaseHealth(int);
    void ResetCoolTime(); // クールダウンをリセット
};