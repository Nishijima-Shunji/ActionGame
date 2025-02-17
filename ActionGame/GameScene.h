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
    Object* bg;             // 背景
    Object* score_bg;       // スコア
    std::vector<std::unique_ptr<Object>> scoreNum;

    int state = 0;
    int score = 0;
   
    std::vector<std::unique_ptr<Object>> health;        // 体力のリスト

    //敵関係
    std::vector<std::unique_ptr<Enemy>> enemies;    // 敵のリスト
    float spawnTimer =  0.0f;
    int enemyType = 0;

    std::vector<Entity*> entities;
    std::vector<std::unique_ptr<MapObject>> blocks;
    std::vector<MapObject*> blockPtrs;

    //ブロック関係
    std::vector<std::unique_ptr<BlockFragment>> fragmentList;
    std::vector<BlockFragment*> fragmentPtrs;

public:
    GameScene(int maxhp);
    ~GameScene();

    void Update() override;
    void Draw() override;



    void SpawnEnemy(int type);
    void AddEnemy(const std::wstring& baseTexturePath, int enemyType);
    void RemoveEnemy(Enemy* enemy);
    void TakeDamege();

    void SpawnBlock(float,float);

    void Intro();
    void Outro();
};

