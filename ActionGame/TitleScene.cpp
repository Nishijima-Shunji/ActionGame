#include "TitleScene.h"
#include "Game.h"
#include <cmath>

TitleScene::TitleScene() {
	textureManager = new TextureManager(g_pDevice);
	Object::SetTextureManager(textureManager);

	sound.Init();
	sound.Play(SOUND_LABEL_BGM000);	//BGM���Đ�

	for (int i = 1; i < 5; i++) {
		std::wstring path = L"asset/TitleBG/" + std::to_wstring(i) + L".png";	// ���I�Ƀp�X���쐬
		title_bg.emplace_back(std::make_unique<UI>());
		title_bg.back()->Init(path.c_str(), vertexShader1, pixelShader1);
		title_bg.back()->SetPos(0.0f, 0.0f, 0.0f);
		title_bg.back()->SetSize(1080.0f, 720.0f, 0.0f);
	}
	button = new UI;
	button->Init(L"asset/Button.png", vertexShader1, pixelShader1, 4, 3);
	button->SetPos(0.0f, -200.0f, 0.0f);
	button->SetSize(200.0f, 100.0f, 0.0f);
	button->SetUV(3, 0);

	text = new UI;
	text->Init(L"asset/Enter.png", vertexShader1, pixelShader1);
	text->SetPos(0.0f, -200.0f, 0.0f);
	text->SetSize(100.0f, 100.0f, 0.0f);
}
TitleScene::~TitleScene() {
	delete textureManager;
}

//�V�[���̃A�b�v�f�[�g
void TitleScene::Update() {
	Animation();

	input.Update();	//�L�[���͂̔���
	if (input.GetKeyTrigger(VK_RETURN)) {
		sound.Play(SOUND_LABEL_SE001);
		sound.Stop(SOUND_LABEL_BGM000);
		SceneManager::ChangeScene(SceneManager::GAME);
	}
}

//�e�V�[���̕`��
void TitleScene::Draw() {
	for (const auto& obj : title_bg) {
		obj->Draw();
	}
	button->Draw();
	text->Draw();
}

void TitleScene::Animation() {
	static int animCount = 0;
	static int animTimer = 0;
	const int animSpeed = 30; // ���l��傫������ƃA�j���[�V�������x���Ȃ�

	button->SetUV(animCount % 2 + 2 , 1);
	animTimer++;
	if (animTimer >= animSpeed) {
		animTimer = 0;
		animCount++;
	}
}