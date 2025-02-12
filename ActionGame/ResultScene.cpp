#include "ResultScene.h"
#include "Game.h"
#include <cmath>

ResultScene::ResultScene(int score) : score(score) {
	sound.Init();
	TextureManager* textureManager = new TextureManager(g_pDevice);

	result_bg = new Object;
	result_bg->Init(L"asset/Result.png");
	result_bg->SetPos(0.0f, 0.0f, 0.0f);			//ˆÊ’u
	result_bg->SetSize(800.0f, 600.0f, 0.0f);		//‘å‚«‚³
	result_bg->SetColor(0.8f, 0.8f, 0.8f, 1.0f);

	for (int i = 0; i < 5; i++) {
		// UI‚Ì¶¬
		scoreNum.emplace_back(std::make_unique<Object>());
		scoreNum.back()->Init(L"asset/num.png", 10, 1);
		scoreNum.back()->SetPos((i * 60.0f) - 125.0f, 0.0f, 0.0f);
		scoreNum.back()->SetSize(60.0f, 60.0f, 0.0f);
		scoreNum.back()->SetUV(0, 0);
	}
}

ResultScene::~ResultScene() {
	delete result_bg;

}

void ResultScene::Update() {
	input.Update();

	if (input.GetKeyRelease(VK_RETURN)) {
		SceneManager::ChangeScene(SceneManager::TITLE);
	}
}

void ResultScene::Draw() {
	result_bg->Draw();
	for (auto& obj : scoreNum) {
		obj->Draw();
	}
}