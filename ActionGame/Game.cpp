#include "Game.h"
#include <time.h>
#include <vector>
#include <iostream>

ID3D11VertexShader* vertexShader1 = nullptr;
ID3D11PixelShader* pixelShader1 = nullptr;
ID3D11VertexShader* vertexShader2 = nullptr;
ID3D11PixelShader* pixelShader2 = nullptr;

void Game::Init(HWND hWnd) {
	srand((unsigned)time(NULL));

	D3D_Create(hWnd);
	sound.Init();	//サウンドを初期化

	// それぞれのシェーダーをロードする
	LoadShader(L"VertexShader.hlsl", L"PixelShader.hlsl", &vertexShader1, &pixelShader1);
	LoadShader(L"Wave_V.hlsl", L"Wave_P.hlsl", &vertexShader2, &pixelShader2);

	//初期シーンをタイトルにする
	//sm.ChangeScene(sm.GAME, 1);
	//sm.ChangeScene(sm.RESULT, 1);
	sm.ChangeScene(sm.TITLE, 1);

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
