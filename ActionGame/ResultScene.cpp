#include "ResultScene.h"
#include "Game.h"
#include <cmath>

ResultScene::ResultScene(int score) : score(score) {
	sound.Init();
	TextureManager* textureManager = new TextureManager(g_pDevice);
	Object::SetTextureManager(textureManager);

	result_bg = new UI;
	result_bg->Init(L"asset/Result.png",vertexShader1, pixelShader1);
	result_bg->SetPos(0.0f, 0.0f, 0.0f);			//位置
	result_bg->SetSize(800.0f, 600.0f, 0.0f);		//大きさ
	result_bg->SetColor(0.8f, 0.8f, 0.8f, 1.0f);

	for (int i = 0; i < 5; i++) {
		// UIの生成
		scoreNum.emplace_back(std::make_unique<UI>());
		scoreNum.back()->Init(L"asset/num.png",vertexShader1,pixelShader1, 10, 1);
		scoreNum.back()->SetPos((i * 60.0f) - 125.0f, 0.0f, 0.0f);
		scoreNum.back()->SetSize(60.0f, 60.0f, 0.0f);
		scoreNum.back()->SetUV(0, 0);
	}
	int tempScore = score;
	for (int i = static_cast<int>(scoreNum.size()) - 1; i >= 0; i--) {
		int digit = tempScore % 10;  // 右端の桁を取得
		tempScore /= 10;            // スコアを10で割って次の桁へ進む

		// UVを桁の値に基づいて設定
		scoreNum[i]->SetUV(digit, 0);
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