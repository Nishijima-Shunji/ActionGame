#include "GameScene.h"
#include "Game.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <sstream>

std::chrono::high_resolution_clock::time_point start;

GameScene::GameScene(int maxhp) {
	sound.Init();
	//sound.Play(SOUND_LABEL_BGM001);
	// テクスチャマネージャーを生成　オブジェクトに登録
	textureManager = new TextureManager(g_pDevice);
	Object::SetTextureManager(textureManager);

	// プレイヤーの生成
	player = new Player(maxhp);
	player->Init(L"asset/Player.png");
	player->SetPos(0.0f, 50.0f, 0.0f);
	player->SetSize(50.0f, 50.0f, 0.0f);
	player->SetHeight(50.0f);
	player->SetWidth(30.0f);
	
	// ブロックの生成
	for (int i = 0; i < 1; i++) {
		block.emplace_back(std::make_unique<Block>());
		block.back()->Init(L"asset/Block.png");
		block.back()->SetPos((i * 30.0f) - 30.0f, -200.0f, 0.0f);
		block.back()->SetSize(30.0f, 30.0f, 0.0f);
	}

	for (int i = 0; i < 20; i++) {
		floor.emplace_back(std::make_unique<Floor>());
		floor.back()->Init(L"asset/Block.png");
		floor.back()->SetPos((i * 30.0f) - 300.0f, -300.0f, 0.0f);
		floor.back()->SetSize(30.0f, 30.0f, 0.0f);
	}

	entities.emplace_back(player);
}
GameScene::~GameScene() {
	// プレイヤーの解放
	delete player;
	// TextureManagerを解放
	delete textureManager;
}

// 更新
void GameScene::Update() {
	float deltaTime = time.GetDeltaTime();
	input.Update();
	// ====================プレイヤーの更新====================
	player->Update(input,deltaTime);

	for (auto& obj : floor) {
		obj->Update(entities);
	}
	// ====================ブロックの更新====================
	for (auto it = block.begin(); it != block.end();) {
		(*it)->Update(entities);

		// ブロックと接触
		if ((*it)->CheckCollision(player)) {
			// ブロック破壊 & 破片生成
			(*it)->Destroy(fragmentList);
		}
		// blockの破壊フラグ立っているものを消す
		Block* blo = dynamic_cast<Block*>(it->get());
		if (blo->GetFlg()) {
			it = block.erase(it);
		}
		else {
			++it;
		}
	}

	// ====================破片の更新====================
	for (auto it = fragmentList.begin(); it != fragmentList.end(); ) {
		(*it)->Update(deltaTime);

		// lifetimeが0以下なら削除
		BlockFragment* fragment = dynamic_cast<BlockFragment*>(it->get());
		if (fragment && fragment->GetLifeTime() <= 0.0f) {
			it = fragmentList.erase(it);
		}
		else {
			++it;
		}
	}

}


void GameScene::Draw() {
	player->Draw();
	for (const auto& obj : floor) {
		obj->Draw();
	}
	for (const auto& obj : block) {
		obj->Draw();
	}
	for (const auto& obj : fragmentList) {
		obj->Draw();
	}
}

void GameScene::SpawnEnemy(int type) {
	// 敵の生成
	enemies.emplace_back(std::make_unique<Enemy>(type));
	// 敵の初期化
	AddEnemy(L"asset/enemy", type);
	// 指定の座標に生成
	enemies.back()->SetSize(enemies.back()->GetRadius() * 2, enemies.back()->GetRadius() * 2, 0.0f);
}

void GameScene::AddEnemy(const std::wstring& baseTexturePath, int enemyType) {
	// テクスチャパスを生成
	std::wstringstream texturePath;
	texturePath << baseTexturePath << enemyType << L".png";

	enemies.back()->Init(texturePath.str().c_str());
}

void GameScene::RemoveEnemy(Enemy* enemy) {
	if (!enemy) {
		return;
	}
	// 対象の敵を探して削除
	enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
		[enemy](const std::unique_ptr<Enemy>& e) {
			return e.get() == enemy;
		}),
		enemies.end());
}

void GameScene::TakeDamege() {
	// 体力減らす
	health.erase(health.end() - 1);
	// 背景真っ赤に（ダメージエフェクト）
	bg->SetColor(1.0f, 0.0f, 0.0f, 0.7f);
}
