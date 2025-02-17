#include "GameScene.h"
#include "Game.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <sstream>
#include "Camera.h"
#undef max

std::chrono::high_resolution_clock::time_point start;

GameScene::GameScene(int maxhp) {
	sound.Init();
	//sound.Play(SOUND_LABEL_BGM001);

	// テクスチャマネージャーを生成　オブジェクトに登録
	textureManager = new TextureManager(g_pDevice);
	Object::SetTextureManager(textureManager);

	// プレイヤーの生成
	player = new Player(maxhp);
	player->SetShaders(g_pDevice,"VertexShader.hlsl", "PixelShader.hlsl");
	player->Init(L"asset/Player.png");
	player->SetPos(0.0f, 50.0f, 0.0f);
	player->SetSize(50.0f, 50.0f, 0.0f);
	player->SetWidth(30.0f);
	player->SetHeight(50.0f);

	blade = new Blade;
	blade->SetShaders(g_pDevice, "VertexShader.hlsl", "PixelShader.hlsl");
	blade->Init(L"asset/Block.png");
	blade->SetPos(player->GetPos().x, player->GetPos().y, 0.0f);
	blade->SetSize(30.0f, 200.0f, 0.0f);
	blade->SetWidth(30.0f);
	blade->SetHeight(200.0f);

	// ブロックの生成
	for (int i = 0; i < 1; i++) {
		auto block = std::make_unique<Block>();
		block->SetShaders(g_pDevice, "VertexShader.hlsl", "PixelShader.hlsl");
		block->Init(L"asset/Block.png");
		block->SetPos((i * 30.0f) - 500.0f, -200.0f, 0.0f);
		block->SetSize(30.0f, 30.0f, 0.0f);

		blockPtrs.push_back(block.get());  // MapObject* のリストに追加
		blocks.emplace_back(std::move(block));  // unique_ptr をリストに追加
	}

	for (int i = 0; i < 40; i++) {
		auto floorBlock = std::make_unique<Floor>();
		floorBlock->SetShaders(g_pDevice, "VertexShader.hlsl", "PixelShader.hlsl");
		floorBlock->Init(L"asset/Block.png");
		floorBlock->SetPos((i * 30.0f) - 500.0f, -350.0f, 0.0f);
		floorBlock->SetSize(30.0f, 30.0f, 0.0f);

		blockPtrs.push_back(floorBlock.get());  // MapObject* のリストに追加
		blocks.emplace_back(std::move(floorBlock));  // unique_ptr をリストに追加
	}

	entities.emplace_back(player);
	// 時間計測開始
	start = std::chrono::high_resolution_clock::now();
}

GameScene::~GameScene() {
	// プレイヤーの解放
	delete player;
	// TextureManagerを解放
	delete textureManager;
}

// 更新
void GameScene::Update() {
	// 現在の時間を取得
	auto now = std::chrono::high_resolution_clock::now();
	// 経過時間を計算
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
	// デルタタイム
	float deltaTime = time.GetDeltaTime();

	input.Update();

	// ====================プレイヤーの更新====================
	player->Update(input, deltaTime, blockPtrs);
	for (auto& fragment : fragmentList) {
		fragmentPtrs.push_back(fragment.get()); // ポインタを取得
	}
	blade->Update(entities, blocks, fragmentList);

	// ====================ブロックの更新====================
	if (elapsed.count() > 500) {
		DirectX::XMFLOAT3 playerPos = player->GetPos();
		SpawnBlock(playerPos.x, playerPos.y);
		// タイマーをリセット
		start = now;
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
	g_Camera.Update();
	DirectX::SimpleMath::Vector3 playerPos = player->GetPos();
	g_Camera.MoveCamera(playerPos.x, playerPos.y, 1.0f, 30);

	// 負荷軽減
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}


void GameScene::Draw() {
	for (const auto& obj : blocks) {
		obj->Draw();
	}
	for (const auto& obj : fragmentList) {
		obj->Draw();
	}
	player->Draw();
	blade->Draw();
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

void GameScene::SpawnBlock(float playerX, float playerY) {
	float blockSize = 30.0f;  // ブロックサイズ
	float rangeX = 300.0f;    // X方向の範囲（プレイヤーを中心に±300.0f）
	float rangeY = 300.0f;    // Y方向の範囲（プレイヤーを中心に±300.0f）

	// X座標の範囲を30.0f間隔に固定
	float xMin = std::floor((playerX - rangeX) / blockSize) * blockSize;
	float xMax = std::floor((playerX + rangeX) / blockSize) * blockSize;
	int xRange = static_cast<int>((xMax - xMin) / blockSize);
	float randomX = xMin + (std::rand() % (xRange + 1)) * blockSize;

	// Y座標の範囲を-200.0f以上100.0f間隔に固定
	float yMin = std::max(-200.0f, std::floor((playerY - rangeY) / 100.0f) * 100.0f);
	float yMax = std::floor((playerY + rangeY) / 100.0f) * 100.0f;
	int yRange = static_cast<int>((yMax - yMin) / 100.0f);
	float randomY = yMin + (std::rand() % (yRange + 1)) * 100.0f;

	auto block = std::make_unique<Block>();
	block->Init(L"asset/Block.png");
	block->SetPos(randomX, randomY, 0.0f);
	block->SetSize(blockSize, blockSize, 0.0f);

	blockPtrs.push_back(block.get());  // MapObject* のリストに追加
	blocks.emplace_back(std::move(block));  // unique_ptr をリストに追加
}


