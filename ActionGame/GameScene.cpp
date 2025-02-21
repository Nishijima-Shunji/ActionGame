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

	// �e�N�X�`���}�l�[�W���[�𐶐��@�I�u�W�F�N�g�ɓo�^
	textureManager = new TextureManager(g_pDevice);
	Object::SetTextureManager(textureManager);

	// �v���C���[�̐���
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

	// ���̐���
	for (int i = 0; i < 40; i++) {
		auto floorBlock = std::make_unique<Floor>();
		floorBlock->Init(L"asset/Block.png", vertexShader1, pixelShader1);
		floorBlock->SetPos((i * 30.0f) - 500.0f, -330.0f, 0.0f);
		floorBlock->SetSize(30.0f, 30.0f, 0.0f);

		blockPtrs.push_back(floorBlock.get());  // MapObject* �̃��X�g�ɒǉ�
		blocks.emplace_back(std::move(floorBlock));  // unique_ptr �����X�g�ɒǉ�
	}

	for (int i = 0; i < 4; i++) {
		// UI�̐���
		scoreNum.emplace_back(std::make_unique<UI>());
		scoreNum.back()->Init(L"asset/num.png", vertexShader1, pixelShader1, 10, 1);
		scoreNum.back()->SetPos((i * 27.0f) + 400.0f, -330.0f, 0.0f);
		scoreNum.back()->SetSize(30.0f, 30.0f, 0.0f);
		scoreNum.back()->SetUV(0, 0);
	}

	entities.emplace_back(player);
	// ���Ԍv���J�n
	start = std::chrono::high_resolution_clock::now();
}

GameScene::~GameScene() {
	// �v���C���[�̉��
	delete player;
	// TextureManager�����
	delete textureManager;
	delete blade;
	delete wave;
	delete bg;
	delete score_bg;
}

// �X�V
void GameScene::Update() {
	// �A�E�g���A�j���[�V����
	if (state == 9) {
		Outro();
		return;
	}
	// ========================================
	// �Q�[��������
	// ========================================

	// ���݂̎��Ԃ��擾
	auto now = std::chrono::high_resolution_clock::now();
	// �o�ߎ��Ԃ��v�Z
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
	// �f���^�^�C��
	float deltaTime = time.GetDeltaTime();

	input.Update();

	// ====================�v���C���[�̍X�V====================
	player->Update(input, deltaTime, blockPtrs);
	for (auto& fragment : fragmentList) {
		fragmentPtrs.push_back(fragment.get()); // �|�C���^���擾
	}
	blade->Update(entities, blocks, fragmentList, sound);

	// ====================�u���b�N�̍X�V====================
	if (elapsed.count() > 250) {
		DirectX::XMFLOAT3 playerPos = player->GetPos();
		if (playerPos.y > -350.0f) {
			SpawnBlock(playerPos.x, playerPos.y);
		}
		// �^�C�}�[�����Z�b�g
		start = now;
	}

	// ====================�j�Ђ̍X�V====================
	for (auto it = fragmentList.begin(); it != fragmentList.end(); ) {
		(*it)->Update(deltaTime);

		// lifetime��0�ȉ��Ȃ�폜
		BlockFragment* fragment = dynamic_cast<BlockFragment*>(it->get());
		if (fragment && fragment->GetLifeTime() <= 0.0f) {
			it = fragmentList.erase(it);
			blockScore += 0.1f;
		}
		else {
			++it;
		}
	}

	// ====================�g�̍X�V====================
	wave->Update(elapsed.count() * 0.001);

	// ===================�X�R�A�̍X�V====================

	if (heightScore < (player->GetPos().y / 100) + 3) {
		heightScore = (player->GetPos().y / 100) + 3;
	}
	score = heightScore * 10 + blockScore;
	if (score < 0) {
		score = 0;
	}

	// ==========�X�R�A�\���p==========
	int tempScore = score;
	for (int i = static_cast<int>(scoreNum.size()) - 1; i >= 0; i--) {
		int digit = tempScore % 10;  // �E�[�̌����擾
		tempScore /= 10;            // �X�R�A��10�Ŋ����Ď��̌��֐i��

		// UV�����̒l�Ɋ�Â��Đݒ�
		scoreNum[i]->SetUV(digit, 0);
	}

	// �����Ă�����̂�艺�ɗ�������I��
	if (wave->GetPos().y + wave->GetSize().y * 0.5f > player->GetPos().y) {
		state = 9;
	}

	// �J�����␳
	g_Camera.Update();
	DirectX::SimpleMath::Vector3 playerPos = player->GetPos();
	g_Camera.MoveCamera(playerPos.x, playerPos.y, 1.0f, 30);

	// ���׌y��
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
	float blockSize = 30.0f;  // �u���b�N�T�C�Y
	float rangeX = 300.0f;    // X�����͈̔́i�v���C���[�𒆐S�Ɂ}300.0f�j
	float rangeY = 300.0f;    // Y�����͈̔́i�v���C���[�𒆐S�Ɂ}300.0f�j

	// X���W�͈̔͂�30.0f�Ԋu�ɌŒ�
	float xMin = std::floor((playerX - rangeX) / blockSize) * blockSize;
	float xMax = std::floor((playerX + rangeX) / blockSize) * blockSize;
	int xRange = static_cast<int>((xMax - xMin) / blockSize);
	float randomX = xMin + (std::rand() % (xRange + 1)) * blockSize;

	// Y���W�͈̔͂�-200.0f�ȏ�100.0f�Ԋu�ɌŒ�
	float yMin = std::max(-200.0f, std::floor((playerY - rangeY) / 100.0f) * 100.0f);
	float yMax = std::floor((playerY + rangeY) / 100.0f) * 100.0f;
	int yRange = static_cast<int>((yMax - yMin) / 100.0f);
	float randomY = yMin + (std::rand() % (yRange + 1)) * 100.0f;

	auto block = std::make_unique<Block>();
	block->Init(L"asset/Block.png", vertexShader1, pixelShader1);
	block->SetPos(randomX, randomY, 0.0f);
	block->SetSize(blockSize, blockSize, 0.0f);

	blockPtrs.push_back(block.get());		// MapObject* �̃��X�g�ɒǉ�
	blocks.emplace_back(std::move(block));  // unique_ptr �����X�g�ɒǉ�
}

void GameScene::Outro() {
	bg->SetColor(0.0f, 0.0f, 0.0f, bg->GetColor().w + 0.01f);
	if (bg->GetColor().w >= 1.0f) {
		sound.Stop(SOUND_LABEL_BGM001);
		SceneManager::ChangeScene(SceneManager::RESULT, score);
	}
}


