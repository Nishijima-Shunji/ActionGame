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
	bool moveFlg = false;
	DirectX::SimpleMath::Vector3 oldPos = pos;

	// キャラクター移動
	velocity.x = 0.0f;
	if (input.GetKeyPress(VK_A) && HitState != 2) {
		velocity.x -= moveSpeed;
		moveFlg = true;
		direction = false;
	}
	if (input.GetKeyPress(VK_D) && HitState != 1) {
		velocity.x += moveSpeed;
		moveFlg = true;
		direction = true;
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
	// 座標更新
	pos += velocity;

	// 範囲外なら元に戻す（移動出来ない）
	if (pos.x > MAP_WIDTH / 2 || pos.x < -(MAP_WIDTH / 2)) {
		pos.x = oldPos.x;
	}
	else if (pos.y < -(MAP_HEIGHT / 2)) {
		pos.y = oldPos.y;
	}
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