#include "TitleScene.h"
#include "Game.h"
#include <cmath>

TitleScene::TitleScene() {
	TextureManager* textureManager = new TextureManager(g_pDevice);

	sound.Init();
	//sound.Play(SOUND_LABEL_BGM000);	//BGMを再生

	title_bg = new Object;  // オブジェクト生成
	title_bg->Init(L"asset/title.png",vertexShader1,pixelShader1);
	title_bg->SetPos(0.0f, 50.0f, 0.0f);	//位置を設定
	title_bg->SetSize(800.0f, 800.0f, 0.0f);	//大きさを設定

}
TitleScene::~TitleScene() {
	delete title_bg;

}

//シーンのアップデート
void TitleScene::Update() {
	input.Update();	//キー入力の判定
	if (input.GetKeyTrigger(VK_RETURN)) {
		SceneManager::ChangeScene(SceneManager::GAME);
	}
}

//各シーンの描画
void TitleScene::Draw() {
	title_bg->Draw();
}