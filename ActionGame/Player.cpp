#include "Player.h"

// �X�e�[�^�X�̏����ݒ�
Player::Player(int maxhp) {
	// 1/100�̊m���ŃL��������
	if ((rand() % 1000) == 0)
	{
		movementType = 1;
	}

	// �v���C���[�̈ړ����x
	if (movementType == 0) {
		moveSpeed = 3.0f;
	}
	else if (movementType == 1) {
		moveSpeed = 0.1f;
	}

	// �v���C���[��HP
	health = maxhp;
	deadFlg = false;
}

void Player::Update(Input input, float deltatime) {
	prevPos = pos;
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