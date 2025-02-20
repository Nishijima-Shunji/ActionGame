#include "TitleScene.h"
#include "Game.h"
#include <cmath>

TitleScene::TitleScene() {
	TextureManager* textureManager = new TextureManager(g_pDevice);

	sound.Init();
	//sound.Play(SOUND_LABEL_BGM000);	//BGM���Đ�

	title_bg = new Object;  // �I�u�W�F�N�g����
	title_bg->Init(L"asset/title.png",vertexShader1,pixelShader1);
	title_bg->SetPos(0.0f, 50.0f, 0.0f);	//�ʒu��ݒ�
	title_bg->SetSize(800.0f, 800.0f, 0.0f);	//�傫����ݒ�

}
TitleScene::~TitleScene() {
	delete title_bg;

}

//�V�[���̃A�b�v�f�[�g
void TitleScene::Update() {
	input.Update();	//�L�[���͂̔���
	if (input.GetKeyTrigger(VK_RETURN)) {
		SceneManager::ChangeScene(SceneManager::GAME);
	}
}

//�e�V�[���̕`��
void TitleScene::Draw() {
	title_bg->Draw();
}