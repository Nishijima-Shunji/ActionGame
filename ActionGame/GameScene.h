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

#define MAP_WIDTH (560)	// ウインドウの幅
#define MAP_HEIGHT (720)	// ウインドウの高さ

// 前方宣言
class Enemy;

class GameScene : public BaseScene
{
private:
	Timer time;
	Sound sound;
	TextureManager* textureManager;     // テクスチャ管理の呼び出し

	Player* player;         // プレイヤー
	Blade* blade;
	Wave* wave;
	UI* bg;             // 背景
	UI* score_bg;       // スコア
	std::vector<std::unique_ptr<Object>> scoreNum;
	
	//敵関係
	std::vector<std::unique_ptr<Enemy>> enemies;    // 敵のリスト
	float spawnTimer = 0.0f;
	int enemyType = 0;
	std::vector<Entity*> entities;

	//ブロック関係
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

