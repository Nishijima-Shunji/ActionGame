#pragma once
#include "Entity.h"
#include "MapObject.h"
#include "Input.h"

#define MAP_WIDTH (1080)	// ウインドウの幅
#define MAP_HEIGHT (720)	// ウインドウの高さ

class Player : public Entity {
private:
	int movementType = 0;	// 移動方法　0:すぐに止まる　1:滑る
	float moveSpeed = 1.0f;	// 移動速度
	float jumpSpeed = 20.0f;// ジャンプ力
	int health;				// HP
	float radius = 10.0f;   // 当たり判定の半径
	bool deadFlg = false;	// 死亡しているかどうか
	bool JumpFlg = false;
	bool direction = false;

public:
	Player(int maxhp);  // コンストラクタ
	void Update(Input input, float, const std::vector<MapObject*>& blocks);  // プレイヤーの更新
	void move(Input input, float);

	// ゲッターセッター
	float GetRadius() const		 { return radius; };
	bool GetdeadFlg() const		 { return deadFlg; };
	int GetHealth() const		 { return health; };
	bool GetDirection() const	 { return direction; };
	void SetVelocity(DirectX::SimpleMath::Vector3 velo) { velocity = velo; };

	// 処理
	void DecreaseHealth(int damage)						{ health -= damage; };
	void CheckHitBlock(const std::vector<MapObject*>& blocks);
};

