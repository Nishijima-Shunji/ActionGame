#include "Player.h"
#include <iostream>

// �X�e�[�^�X�̏����ݒ�
Player::Player(int maxhp) {
	// �v���C���[�̈ړ����x
	moveSpeed = 3.0f;

	// �v���C���[��HP
	health = maxhp;
	deadFlg = false;
}

void Player::Update(Input input, float deltatime, const std::vector<MapObject*>& blocks) {
	prevPos = pos;
	CheckHitBlock(blocks);
	move(input, deltatime);    //�ړ�
	if (health <= 0) {
		deadFlg = true;
	}
}

void Player::move(Input input, float deltaTime) {
	// �ړ����t���O
	bool moveFlg = false;
	DirectX::SimpleMath::Vector3 oldPos = pos;

	// �L�����N�^�[�ړ�
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
		velocity.y += gravity.y * deltaTime;  // �d�͂̉e�����󂯂�
	}
	else {
		// �n�ʂɐڐG���Ă���ꍇ�A�c�����̑��x���[���Ƀ��Z�b�g
		velocity.y = 0.0f;
	}
	// ���W�X�V
	pos += velocity;

	// �͈͊O�Ȃ猳�ɖ߂��i�ړ��o���Ȃ��j
	if (pos.x > MAP_WIDTH / 2 || pos.x < -(MAP_WIDTH / 2)) {
		pos.x = oldPos.x;
	}
	else if (pos.y < -(MAP_HEIGHT / 2)) {
		pos.y = oldPos.y;
	}
}

void Player::CheckHitBlock(const std::vector<MapObject*>& blocks) {
	// �O�t���[���̐ڒn��Ԃ�ۑ�
	if (!prevOnFlg) {
		prevOnFlg = OnGround;
	}
	// ���t���[�����߂Ƀ��Z�b�g
	//SetOnGround(false);
	OnGround = false;
	for (MapObject* block : blocks) {
		if (block->CheckCollision(this)) {
			block->ResolveCollision(this);
			OnGround = true; // 1�ł��ڐG���Ă����true�ɂ���
		}
	}
	// ���n���Ă��Ȃ��ăR���[�e�^�C�����Ȃ�
	if (!OnGround && prevOnFlg) {
		CoyoteCount++;
	}
	else if (prevOnFlg && CoyoteCount > 30) {
		CoyoteCount = 0;
		prevOnFlg = false;
	}
}