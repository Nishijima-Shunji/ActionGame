#pragma once
#include "Entity.h"
#include "MapObject.h"
#include "Input.h"

#define MAP_WIDTH (560)	// ウインドウの幅
#define MAP_HEIGHT (720)	// ウインドウの高さ

class Player : public Entity {
private:
	int movementType = 0;	// 移動方法　0:すぐに止まる　1:滑る
	float moveSpeed = 1.0f;	// 移動速度
	float jumpSpeed = 20.0f;	// ジャンプ力
	int health;				// HP
	float radius = 10.0f;   // 当たり判定の半径
	bool deadFlg = false;	// 死亡しているかどうか
	bool JumpFlg = false;

public:
	Player(int maxhp);  // コンストラクタ
	void Update(Input input, float, const std::vector<MapObject*>& blocks);  // プレイヤーの更新
	void move(Input input, float);


	float GetRadius() const;
	void DecreaseHealth(int);
	bool GetdeadFlg() const;
	int GetHealth() const;

	void SetVelocity(DirectX::SimpleMath::Vector3 velo);

	void CheckHitBlock(const std::vector<MapObject*>& blocks);
};

