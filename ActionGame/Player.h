#pragma once
#include "Entity.h"
#include "MapObject.h"
#include "Input.h"

#define MAP_WIDTH (560)	// �E�C���h�E�̕�
#define MAP_HEIGHT (720)	// �E�C���h�E�̍���

class Player : public Entity {
private:
	int movementType = 0;	// �ړ����@�@0:�����Ɏ~�܂�@1:����
	float moveSpeed = 1.0f;	// �ړ����x
	float jumpSpeed = 20.0f;	// �W�����v��
	int health;				// HP
	float radius = 10.0f;   // �����蔻��̔��a
	bool deadFlg = false;	// ���S���Ă��邩�ǂ���
	bool JumpFlg = false;

public:
	Player(int maxhp);  // �R���X�g���N�^
	void Update(Input input, float, const std::vector<MapObject*>& blocks);  // �v���C���[�̍X�V
	void move(Input input, float);


	float GetRadius() const;
	void DecreaseHealth(int);
	bool GetdeadFlg() const;
	int GetHealth() const;

	void SetVelocity(DirectX::SimpleMath::Vector3 velo);

	void CheckHitBlock(const std::vector<MapObject*>& blocks);
};

