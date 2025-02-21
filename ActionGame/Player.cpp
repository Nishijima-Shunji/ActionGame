#include "Player.h"
#include <iostream>

// ステータスの初期設定
Player::Player(int maxhp) {
	// プレイヤーの移動速度
	moveSpeed = 3.0f;

	// プレイヤーのHP
	health = maxhp;
	deadFlg = false;
}

void Player::Update(Input input, float deltatime, const std::vector<MapObject*>& blocks) {
	prevPos = pos;
	CheckHitBlock(blocks);
	move(input, deltatime);    //移動
	if (health <= 0) {
		deadFlg = true;
	}
}

void Player::move(Input input, float deltaTime) {
	// 移動中フラグ
	moveFlg = false;
	DirectX::SimpleMath::Vector3 oldPos = pos;

	// キャラクター移動
	velocity.x = 0.0f;
	if (input.GetKeyPress(VK_A) && HitState != 2) {
		velocity.x -= moveSpeed;
		moveFlg = true;
		direction = false;
		Object::direction = 1;
	}
	if (input.GetKeyPress(VK_D) && HitState != 1) {
		velocity.x += moveSpeed;
		moveFlg = true;
		direction = true;
		Object::direction = 0;
	}
	if (input.GetKeyPress(VK_R)) {
		pos = { 0.0f, -300.0f, 0.0f };
	}

	if (input.GetKeyTrigger(VK_SPACE) && prevOnFlg) {
		velocity.y += jumpSpeed;
		OnGround = false;
		prevOnFlg = false;
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

	// 落下の最大速度
	if (abs(velocity.y) > 100.0f) {
		velocity.y = 100.0f;
	}
	// 座標更新
	pos += velocity;

	Animation();
	
}

void Player::CheckHitBlock(const std::vector<MapObject*>& blocks) {
	// 前フレームの接地状態を保存
	if (!prevOnFlg) {
		prevOnFlg = OnGround;
	}
	// 毎フレーム初めにリセット
	//SetOnGround(false);
	OnGround = false;
	for (MapObject* block : blocks) {
		if (block->CheckCollision(this)) {
			block->ResolveCollision(this);
			OnGround = true; // 1つでも接触していればtrueにする
		}
	}
	// 着地していなくてコヨーテタイム内なら
	if (!OnGround && prevOnFlg) {
		CoyoteCount++;
	}
	else if (prevOnFlg && CoyoteCount > 30) {
		CoyoteCount = 0;
		prevOnFlg = false;
	}
}

void Player::Animation() {
	static int animCount = 0;
	static int animTimer = 0;
	const int animSpeed = 5; // 数値を大きくするとアニメーションが遅くなる

	SetUV(animCount % 4, 1);
	if (moveFlg && OnGround) {
		animTimer++;
		if (animTimer >= animSpeed) {
			animTimer = 0;
			animCount++;
		}
	}
	else {
		animCount = 0;
		animTimer = 0;
	}
}