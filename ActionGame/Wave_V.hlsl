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

// 定数バッファ（オブジェクト用）
cbuffer ObjectBuffer : register(b0)
{
	float4 vertexColor; // 頂点カラー
	matrix matrixTex;   // UV座標移動行列
	matrix matrixProj;  // プロジェクション変換行列
	matrix matrixWorld; // ワールド変換行列
}

// 定数バッファ（波の影響用）
cbuffer WaveBuffer : register(b1)
{
	float time;         // 時間
	float waveAmplitude; // 波の振幅
	float waveFrequency; // 波の周波数
	float padding;
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
