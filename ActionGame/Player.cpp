#include "Player.h"

// ステータスの初期設定
Player::Player(int maxhp) {
	// 1/100の確率でキャラ滑る
	if ((rand() % 1000) == 0)
	{
		movementType = 1;
	}

	// プレイヤーの移動速度
	if (movementType == 0) {
		moveSpeed = 3.0f;
	}
	else if (movementType == 1) {
		moveSpeed = 0.1f;
	}

	// プレイヤーのHP
	health = maxhp;
	deadFlg = false;
}

void Player::Update(Input input, float deltatime) {
	prevPos = pos;
	move(input, deltatime);    //移動
	if (health <= 0) {
		deadFlg = true;
	}
}

void Player::move(Input input, float deltaTime) {
	// 移動中フラグ
	bool moveFlg = false;
	DirectX::SimpleMath::Vector3 oldPos = pos;

	// キャラクター移動
	velocity.x = 0.0f;
	if (input.GetKeyPress(VK_A)) {
		velocity.x -= moveSpeed;
		moveFlg = true;
	}
	if (input.GetKeyPress(VK_D)) {
		velocity.x += moveSpeed;
		moveFlg = true;
	}

	if (input.GetKeyTrigger(VK_SPACE) && OnGround) {
		velocity.y += 20.0f;
		OnGround = false;
	}
	if (OnGround) {
		velocity.y = 0.0f;
	}

	if (!OnGround) {
		velocity.y += gravity.y * deltaTime;  // 重力の影響を受ける
	}
	else {
		// 地面に接触している場合、縦方向の速度をゼロにリセット
		velocity.y = 0.0f;
	}
	// 座標更新
	pos += velocity;

	// 範囲外なら元に戻す（移動出来ない）
	if (pos.x > MAP_WIDTH / 2 || pos.x < -(MAP_WIDTH / 2)) {
		pos.x = oldPos.x;
	}
	else if (pos.y > MAP_HEIGHT / 2 || pos.y < -(MAP_HEIGHT / 2)) {
		pos.y = oldPos.y;
	}
}

float Player::GetRadius() const {
	return radius;
}

void Player::DecreaseHealth(int damage) {
	health -= damage;
}

bool Player::GetdeadFlg() const {
	return deadFlg;
}

int Player::GetHealth() const {
	return health;
}

void Player::SetVelocity(DirectX::SimpleMath::Vector3 velo) {
	velocity = velo;
}