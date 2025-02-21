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
	sound.Play(SOUND_LABEL_BGM001);

	// テクスチャマネージャーを生成　オブジェクトに登録
	textureManager = new TextureManager(g_pDevice);
	Object::SetTextureManager(textureManager);

	// プレイヤーの生成
	player = new Player(maxhp);
	player->Init(L"asset/Player.png", vertexShader1, pixelShader1, 4, 4);
	player->SetPos(0.0f, -200.0f, 0.0f);
	player->SetSize(50.0f, 50.0f, 0.0f);
	player->SetWidth(30.0f);
	player->SetHeight(50.0f);
	player->SetUV(0, 1);

	blade = new Blade;
	blade->Init(L"asset/Blade.png", vertexShader1, pixelShader1);
	blade->SetPos(player->GetPos().x, player->GetPos().y, 0.0f);
	blade->SetSize(30.0f, 200.0f, 0.0f);
	blade->SetWidth(30.0f);
	blade->SetHeight(200.0f);

	wave = new Wave;
	wave->Init(L"asset/Block.png", vertexShader1, pixelShader1);
	wave->InitWave(20, 1);
	wave->SetPos(0.0f, -2200.0f, 0.0f);
	wave->SetSize(3000.0f, 3000.0f, 0.0f);

	bg = new UI;
	bg->Init(L"asset/Gamebg.png", vertexShader1, pixelShader1);
	bg->SetPos(0.0f, 0.0f, 0.0f);
	bg->SetSize(2000.0f, 2000.0f, 0.0f);
	bg->SetColor(0.0f, 0.0f, 0.0f, 0.0f);

	// 床の生成
	for (int i = 0; i < 40; i++) {
		auto floorBlock = std::make_unique<Floor>();
		floorBlock->Init(L"asset/Block.png", vertexShader1, pixelShader1);
		floorBlock->SetPos((i * 30.0f) - 500.0f, -330.0f, 0.0f);
		floorBlock->SetSize(30.0f, 30.0f, 0.0f);

		blockPtrs.push_back(floorBlock.get());  // MapObject* のリストに追加
		blocks.emplace_back(std::move(floorBlock));  // unique_ptr をリストに追加
	}

	for (int i = 0; i < 4; i++) {
		// UIの生成
		scoreNum.emplace_back(std::make_unique<UI>());
		scoreNum.back()->Init(L"asset/num.png", vertexShader1, pixelShader1, 10, 1);
		scoreNum.back()->SetPos((i * 27.0f) + 400.0f, -330.0f, 0.0f);
		scoreNum.back()->SetSize(30.0f, 30.0f, 0.0f);
		scoreNum.back()->SetUV(0, 0);
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
	delete blade;
	delete wave;
	delete bg;
	delete score_bg;
}

// 更新
void GameScene::Update() {
	// アウトロアニメーション
	if (state == 9) {
		Outro();
		return;
	}
	// ========================================
	// ゲーム中処理
	// ========================================

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
	blade->Update(entities, blocks, fragmentList, sound);

	// ====================ブロックの更新====================
	if (elapsed.count() > 250) {
		DirectX::XMFLOAT3 playerPos = player->GetPos();
		if (playerPos.y > -350.0f) {
			SpawnBlock(playerPos.x, playerPos.y);
		}
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
			blockScore += 0.1f;
		}
		else {
			++it;
		}
	}

	// ====================波の更新====================
	wave->Update(elapsed.count() * 0.001);

	// ===================スコアの更新====================

	if (heightScore < (player->GetPos().y / 100) + 3) {
		heightScore = (player->GetPos().y / 100) + 3;
	}
	score = heightScore * 10 + blockScore;
	if (score < 0) {
		score = 0;
	}

	// ==========スコア表示用==========
	int tempScore = score;
	for (int i = static_cast<int>(scoreNum.size()) - 1; i >= 0; i--) {
		int digit = tempScore % 10;  // 右端の桁を取得
		tempScore /= 10;            // スコアを10で割って次の桁へ進む

		// UVを桁の値に基づいて設定
		scoreNum[i]->SetUV(digit, 0);
	}

	// 迫ってくるものより下に落ちたら終了
	if (wave->GetPos().y + wave->GetSize().y * 0.5f > player->GetPos().y) {
		state = 9;
	}

	// カメラ補正
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
	wave->Draw();
	for (const auto& obj : scoreNum) {
		obj->Draw();
	}
	bg->Draw();
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
	block->Init(L"asset/Block.png", vertexShader1, pixelShader1);
	block->SetPos(randomX, randomY, 0.0f);
	block->SetSize(blockSize, blockSize, 0.0f);

	blockPtrs.push_back(block.get());		// MapObject* のリストに追加
	blocks.emplace_back(std::move(block));  // unique_ptr をリストに追加
}

void GameScene::Outro() {
	bg->SetColor(0.0f, 0.0f, 0.0f, bg->GetColor().w + 0.01f);
	if (bg->GetColor().w >= 1.0f) {
		sound.Stop(SOUND_LABEL_BGM001);
		SceneManager::ChangeScene(SceneManager::RESULT, score);
	}
}


