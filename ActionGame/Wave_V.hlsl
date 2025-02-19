//// 頂点シェーダー
//
//// 頂点のデータを表す構造体（受け取り用）
//struct VS_IN
//{
//    float4 pos : POSITION;
//    float4 col : COLOR0;
//    float2 tex : TEX;   //UV座標
//};
//
//// 頂点のデータを表す構造体（送信用） 
//struct VS_OUT
//{
//    float4 pos : SV_POSITION;
//    float4 col : COLOR0;
//    float2 tex : TEXCOORD;  //UV座標
//};
//
//// グローバル変数を宣言
//// 定数バッファ受け取り用
//cbuffer ConstantBuffer : register(b0) {
//    // 頂点カラー
//    float4 vertexColor;
//    // UV座標移動行列
//    matrix matrixTex;
//    // プロジェクション変換行列
//    matrix matrixProj;
//    // ワールド変換行列
//    matrix matrixWorld;
//    // 時間のパラメータ
//    float time;
//    // 波のパラメータ
//    float waveAmplitude; // 波の振幅
//    float waveFrequency; // 波の周波数
//}
//
//// 頂点シェーダーのエントリポイント
//VS_OUT vs_main(VS_IN input)
//{
//    VS_OUT output;
//
//    // 波の高さを計算（時間と位置に基づいてsin波を生成）
//    float waveHeight = waveAmplitude * sin(input.pos.x * waveFrequency + time);
//
//    // 波の高さをZ軸方向に加算
//    input.pos.z += waveHeight;
//
//    // ワールド変換行列を頂点座標に掛けて、移動、回転、拡大縮小する
//    output.pos = mul(input.pos, matrixWorld);
//
//    // 頂点座標に投影行列を掛けて、平易面上の座標にする
//    output.pos = mul(output.pos, matrixProj);
//
//    // UV座標を移動させる
//    float4 uv;
//    uv.xy = input.tex;  // 行列掛け算のためfloat4型
//    uv.z = 0.0f;
//    uv.w = 1.0f;
//    uv = mul(uv, matrixTex);    // UV座標と移動行列を掛け算
//    output.tex = uv.xy;         // 掛け算の結果を送信用変数にセット
//
//    // 頂点カラーを設定
//    output.col = input.col * vertexColor;
//
//    return output;
//}

// 頂点のデータを表す構造体（受け取り用）
struct VS_IN
{
	float4 pos : POSITION;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;  // UV座標
};

// 頂点のデータを表す構造体（送信用） 
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;  // UV座標
};

// 定数バッファ（グローバル変数として宣言）
cbuffer ConstantBuffer : register(b0)
{
	float4 vertexColor; // 頂点カラー
	matrix matrixTex;   // UV座標移動行列
	matrix matrixProj;  // プロジェクション変換行列
	matrix matrixWorld; // ワールド変換行列
	float time;         // 時間
	float waveAmplitude; // 波の振幅
	float waveFrequency; // 波の周波数
}

// 頂点シェーダーのエントリポイント
VS_OUT vs_main(VS_IN input)
{
	VS_OUT output;

	// 波の高さを計算（X軸の位置と時間でsin波を生成）
	float waveHeight = waveAmplitude * sin(input.pos.x * waveFrequency + time);

	// 値を制限
	waveHeight = clamp(waveHeight, -0.1, 0.1);

	// 位置を変更（Y軸に波を適用）
	float4 modifiedPos = input.pos;
	modifiedPos.y += waveHeight;

	// ワールド変換行列を適用（移動・回転・スケール）
	modifiedPos = mul(modifiedPos, matrixWorld);

	// 投影変換行列を適用（画面座標へ変換）
	output.pos = mul(modifiedPos, matrixProj);

	// UV座標を変換
	output.tex = mul(float4(input.tex, 0.0f, 1.0f), matrixTex).xy;

	// 頂点カラーを適用
	output.col = input.col * vertexColor;


	return output;
}
