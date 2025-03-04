#pragma once

#include <d3d11.h>  // DirectX11を使うためのヘッダーファイル
#include <DirectXMath.h> //DirectXの数学関連のヘッダーファイル

#define SCREEN_WIDTH (1080)	// ウインドウの幅
#define SCREEN_HEIGHT (720)	// ウインドウの高さ

// 関数のプロトタイプ宣言
HRESULT D3D_Create(HWND hwnd);
void    D3D_Release();
void    D3D_StartRender();
void    D3D_FinishRender();

// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
HRESULT CreateVertexShader(ID3D11Device* device, const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel,
	D3D11_INPUT_ELEMENT_DESC* layout, unsigned int numElements, ID3D11VertexShader** ppVertexShader, ID3D11InputLayout** ppVertexLayout);
// ピクセルシェーダーオブジェクトを生成
HRESULT CreatePixelShader(ID3D11Device* device, const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3D11PixelShader** ppPixelShader);
// シェーダーを読み込む
HRESULT LoadShader(const wchar_t* vertexShaderFile, const wchar_t* pixelShaderFile, ID3D11VertexShader** ppVertexShader, ID3D11PixelShader** ppPixelShader);

// 構造体の定義
// 頂点データを表す構造体
struct Vertex
{
	// 頂点の位置座標
	float x, y, z;

	//色
	float r, g, b, a;

	//テクスチャ座標(UV座標)
	float u, v;
};

//定数バッファ用構造体
struct ConstBuffer {
	//頂点カラー行列
	DirectX::XMFLOAT4 color;
	//UV座標移動行列
	DirectX::XMMATRIX matrixTex;
	//プロジェクション変換行列
	DirectX::XMMATRIX matrixProj;
	//ワールド変換行列
	DirectX::XMMATRIX matrixWorld;
};
struct WaveConstBuffer {
	// 時間
	float time;
	// 波の振幅
	float waveAmplitude;
	// 波の周波数
	float waveFrequency;

	float padding;
};

// ※ID3D11で始まるポインタ型の変数は、解放する必要がある
extern ID3D11Device* g_pDevice; // デバイス＝DirectXの各種機能を作る
// コンテキスト＝描画関連を司る機能
extern ID3D11DeviceContext* g_pDeviceContext;
extern ID3D11Buffer* g_pConstantBuffer;
extern ID3D11Buffer* g_pWaveConstantBuffer;

// Direct3D解放の簡略化マクロ
#define SAFE_RELEASE(p) { if( NULL != p ) { p->Release(); p = NULL; } }

#pragma once
