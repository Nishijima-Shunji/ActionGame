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

	// �e�N�X�`���}�l�[�W���[�𐶐��@�I�u�W�F�N�g�ɓo�^
	textureManager = new TextureManager(g_pDevice);
	Object::SetTextureManager(textureManager);

	// �v���C���[�̐���
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

	// �u���b�N�̐���
	for (int i = 0; i < 1; i++) {
		auto block = std::make_unique<Block>();
		block->SetShaders(g_pDevice, "VertexShader.hlsl", "PixelShader.hlsl");
		block->Init(L"asset/Block.png");
		block->SetPos((i * 30.0f) - 500.0f, -200.0f, 0.0f);
		block->SetSize(30.0f, 30.0f, 0.0f);

		blockPtrs.push_back(block.get());  // MapObject* �̃��X�g�ɒǉ�
		blocks.emplace_back(std::move(block));  // unique_ptr �����X�g�ɒǉ�
	}

	for (int i = 0; i < 40; i++) {
		auto floorBlock = std::make_unique<Floor>();
		floorBlock->SetShaders(g_pDevice, "VertexShader.hlsl", "PixelShader.hlsl");
		floorBlock->Init(L"asset/Block.png");
		floorBlock->SetPos((i * 30.0f) - 500.0f, -350.0f, 0.0f);
		floorBlock->SetSize(30.0f, 30.0f, 0.0f);

		blockPtrs.push_back(floorBlock.get());  // MapObject* �̃��X�g�ɒǉ�
		blocks.emplace_back(std::move(floorBlock));  // unique_ptr �����X�g�ɒǉ�
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
}

// �X�V
void GameScene::Update() {
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
	blade->Update(entities, blocks, fragmentList);

	// ====================�u���b�N�̍X�V====================
	if (elapsed.count() > 500) {
		DirectX::XMFLOAT3 playerPos = player->GetPos();
		SpawnBlock(playerPos.x, playerPos.y);
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
		}
		else {
			++it;
		}
	}
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
}

void GameScene::SpawnEnemy(int type) {
	// �G�̐���
	enemies.emplace_back(std::make_unique<Enemy>(type));
	// �G�̏�����
	AddEnemy(L"asset/enemy", type);
	// �w��̍��W�ɐ���
	enemies.back()->SetSize(enemies.back()->GetRadius() * 2, enemies.back()->GetRadius() * 2, 0.0f);
}

void GameScene::AddEnemy(const std::wstring& baseTexturePath, int enemyType) {
	// �e�N�X�`���p�X�𐶐�
	std::wstringstream texturePath;
	texturePath << baseTexturePath << enemyType << L".png";

	enemies.back()->Init(texturePath.str().c_str());
}

void GameScene::RemoveEnemy(Enemy* enemy) {
	if (!enemy) {
		return;
	}
	// �Ώۂ̓G��T���č폜
	enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
		[enemy](const std::unique_ptr<Enemy>& e) {
			return e.get() == enemy;
		}),
		enemies.end());
}

void GameScene::TakeDamege() {
	// �̗͌��炷
	health.erase(health.end() - 1);
	// �w�i�^���ԂɁi�_���[�W�G�t�F�N�g�j
	bg->SetColor(1.0f, 0.0f, 0.0f, 0.7f);
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
	block->Init(L"asset/Block.png");
	block->SetPos(randomX, randomY, 0.0f);
	block->SetSize(blockSize, blockSize, 0.0f);

	blockPtrs.push_back(block.get());  // MapObject* �̃��X�g�ɒǉ�
	blocks.emplace_back(std::move(block));  // unique_ptr �����X�g�ɒǉ�
}


