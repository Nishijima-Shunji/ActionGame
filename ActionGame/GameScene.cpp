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

	// �e�N�X�`���}�l�[�W���[�𐶐��@�I�u�W�F�N�g�ɓo�^
	textureManager = new TextureManager(g_pDevice);
	Object::SetTextureManager(textureManager);

	// �v���C���[�̐���
	player = new Player(maxhp);
	player->Init(L"asset/Player.png");
	player->SetPos(0.0f, 50.0f, 0.0f);
	player->SetSize(50.0f, 50.0f, 0.0f);
	player->SetHeight(50.0f);
	player->SetWidth(30.0f);

	blade = new Blade;
	blade->Init(L"asset/Block.png");
	blade->SetPos(player->GetPos().x, player->GetPos().y, 0.0f);
	blade->SetSize(30.0f, 30.0f, 0.0f);


	// �u���b�N�̐���
	for (int i = 0; i < 1; i++) {
		auto block = std::make_unique<Block>();
		block->Init(L"asset/Block.png");
		block->SetPos((i * 30.0f) - 30.0f, -200.0f, 0.0f);
		block->SetSize(30.0f, 30.0f, 0.0f);

		blockPtrs.push_back(block.get());  // MapObject* �̃��X�g�ɒǉ�
		blocks.emplace_back(std::move(block));  // unique_ptr �����X�g�ɒǉ�
	}

	for (int i = 0; i < 20; i++) {
		auto floorBlock = std::make_unique<Floor>();
		floorBlock->Init(L"asset/Block.png");
		floorBlock->SetPos((i * 30.0f) - 300.0f, -300.0f, 0.0f);
		floorBlock->SetSize(30.0f, 30.0f, 0.0f);

		blockPtrs.push_back(floorBlock.get());  // MapObject* �̃��X�g�ɒǉ�
		blocks.emplace_back(std::move(floorBlock));  // unique_ptr �����X�g�ɒǉ�
	}

	entities.emplace_back(player);
}

GameScene::~GameScene() {
	// �v���C���[�̉��
	delete player;
	// TextureManager�����
	delete textureManager;
}

// �X�V
void GameScene::Update() {
	float deltaTime = time.GetDeltaTime();
	input.Update();
	// ====================�v���C���[�̍X�V====================
	player->Update(input, deltaTime, blockPtrs);
	for (auto& fragment : fragmentList) {
		fragmentPtrs.push_back(fragment.get()); // �|�C���^���擾
	}
	blade->Update(entities,blocks,fragmentList);

	// ====================�u���b�N�̍X�V====================


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
