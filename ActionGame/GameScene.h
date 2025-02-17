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
    Object* bg;             // �w�i
    Object* score_bg;       // �X�R�A
    std::vector<std::unique_ptr<Object>> scoreNum;

    int state = 0;
    int score = 0;
   
    std::vector<std::unique_ptr<Object>> health;        // �̗͂̃��X�g

    //�G�֌W
    std::vector<std::unique_ptr<Enemy>> enemies;    // �G�̃��X�g
    float spawnTimer =  0.0f;
    int enemyType = 0;

    std::vector<Entity*> entities;
    std::vector<std::unique_ptr<MapObject>> blocks;
    std::vector<MapObject*> blockPtrs;

    //�u���b�N�֌W
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

