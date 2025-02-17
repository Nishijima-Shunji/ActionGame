#pragma once
#include "Entity.h"
#include "MapObject.h"
#include "Input.h"

#define MAP_WIDTH (1080)	// �E�C���h�E�̕�
#define MAP_HEIGHT (720)	// �E�C���h�E�̍���

class Player : public Entity {
private:
	int movementType = 0;	// �ړ����@�@0:�����Ɏ~�܂�@1:����
	float moveSpeed = 1.0f;	// �ړ����x
	float jumpSpeed = 20.0f;// �W�����v��
	int health;				// HP
	float radius = 10.0f;   // �����蔻��̔��a
	bool deadFlg = false;	// ���S���Ă��邩�ǂ���
	bool JumpFlg = false;
	bool direction = false;

public:
	Player(int maxhp);  // �R���X�g���N�^
	void Update(Input input, float, const std::vector<MapObject*>& blocks);  // �v���C���[�̍X�V
	void move(Input input, float);

	// �Q�b�^�[�Z�b�^�[
	float GetRadius() const		 { return radius; };
	bool GetdeadFlg() const		 { return deadFlg; };
	int GetHealth() const		 { return health; };
	bool GetDirection() const	 { return direction; };
	void SetVelocity(DirectX::SimpleMath::Vector3 velo) { velocity = velo; };

	// ����
	void DecreaseHealth(int damage)						{ health -= damage; };
	void CheckHitBlock(const std::vector<MapObject*>& blocks);
};

