#include "Game.h"
#include <time.h>
#include <vector>
#include <iostream>

void Game::Init(HWND hWnd) {
	srand((unsigned)time(NULL));

	D3D_Create(hWnd);
	sound.Init();	//サウンドを初期化


	//初期シーンをタイトルにする
	sm.ChangeScene(sm.GAME, 1);

}

void Game::Draw() {
	// ゲーム処理実行
	D3D_StartRender();
	sm.Render();
	D3D_FinishRender();
}

void Game::Uninit() {
	
	sound.Uninit();
	// DirectXの解放処理
	D3D_Release();
}

void Game::Update() {
	sm.Update();	//現在シーンのアップデートを実行
}
