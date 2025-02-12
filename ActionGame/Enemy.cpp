#include "Enemy.h"
#include <cmath>

// コンストラクタ　敵情報を最初に入れる
Enemy::Enemy(int enemyType) : enemyType(enemyType) {
	targetPos.x = rand() % MAP_WIDTH - (MAP_WIDTH / 2);
	switch (enemyType) {
	case 0:
		pos.x = rand() % MAP_WIDTH - (MAP_WIDTH / 2);  // -470 ～ 390の範囲でX位置をランダム生成
		pos.y = (MAP_HEIGHT / 2);
		 
		targetPos.y = rand() % (MAP_HEIGHT / 2) - 10.0f;
		moveSpeed = 2.0f;
		break;
	case 1:
		pos.x = rand() % MAP_WIDTH - (MAP_WIDTH / 2);  // -470 ～ 390の範囲でX位置をランダム生成
		pos.y = -500.0f;
		moveSpeed = 1.25f;
		state = 1;
		break;
	}
}

void Enemy::Update() {
	prevPos = pos;
	if (health <= 0) {
		deadFlg = true;
	}
	move();
}

void Enemy::move() {
	//方向と距離を計算する
	DirectX::SimpleMath::Vector3 direction = targetPos - pos;
	float distance = direction.Length();

	//targetposに到着しているか
	if (distance > 0.1f && state == 0) {
		// 正規化して移動方向ベクトルを取得
		direction.Normalize();

		// 移動量を計算 方向×速度
		velocity = direction * moveSpeed;
		pos += velocity;

		if ((targetPos - pos).LengthSquared() < velocity.LengthSquared()) {
			pos = targetPos; // 目的地に到達したとみなす
			state = 1;		 //到着状態に変更
		}
	}

	if (enemyType == 1) {
		// 上方向へ移動
		velocity = DirectX::SimpleMath::Vector3{0.0f,1.0f,0.0f} * moveSpeed;
		pos += velocity;
	}
}

float Enemy::GetRadius() const {
	return radius;
}

int Enemy::GetHealth() const {
	return health;
}

bool Enemy::GetDeadFlg() const {
	return deadFlg;
}

int Enemy::GetState() const {
	return state;
}

int Enemy::GetEnemyType() const {
	return enemyType;
}

void Enemy::DecreaseHealth(int damage) {
	health -= damage;
}

void Enemy::ResetCoolTime() {
	attackTime = 0;
}