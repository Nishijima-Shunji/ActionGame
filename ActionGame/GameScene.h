#pragma once
#include <memory>
#include "BaseScene.h"
#include "Sound.h"
#include "Timer.h"

#include "Player.h"
#include "Blade.h"
#include "Enemy.h"
#include "Floor.h"
#include "Block.h"
#include "BlockFragment.h"
#include "wave.h"
#include "UI.h"

#define MAP_WIDTH (560)	// �E�C���h�E�̕�
#define MAP_HEIGHT (720)	// �E�C���h�E�̍���

// �O���錾
class Enemy;

class GameScene : public BaseScene
{
private:
	Timer time;
	Sound sound;
	TextureManager* textureManager;     // �e�N�X�`���Ǘ��̌Ăяo��

	Player* player;         // �v���C���[
	Blade* blade;
	Wave* wave;
	UI* bg;             // �w�i
	UI* score_bg;       // �X�R�A
	std::vector<std::unique_ptr<Object>> scoreNum;
	
	//�G�֌W
	std::vector<std::unique_ptr<Enemy>> enemies;    // �G�̃��X�g
	float spawnTimer = 0.0f;
	int enemyType = 0;
	std::vector<Entity*> entities;

	//�u���b�N�֌W
	std::vector<std::unique_ptr<MapObject>> blocks;
	std::vector<MapObject*> blockPtrs;
	std::vector<std::unique_ptr<BlockFragment>> fragmentList;
	std::vector<BlockFragment*> fragmentPtrs;

	int state = 0;
	int score = 0;
	int heightScore = 0;
	float blockScore = 0;
public:

	GameScene(int maxhp);
	~GameScene();

	void Update() override;
	void Draw() override;

	void SpawnBlock(float, float);

	void Intro();
	void Outro();
};

