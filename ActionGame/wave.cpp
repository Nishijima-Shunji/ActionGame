#include "Wave.h"
#include "Camera.h"

void Wave::Update(float timeCount) {
	// シェーダーで動かす（未実装）
	time = timeCount;
	float waveHeight = 0.1f;  // 波の高さ

	for (int i = 0; i < waveVertexList.size(); i += 2) {
		// ワールド座標のX値を考慮
		float x = pos.x + waveVertexList[i].x * size.x;

		// 端では振幅を減衰させる
		float attenuation = 1.0f - abs(waveVertexList[i].x / 0.5f);
		float wave = attenuation * sin(x * waveFrequency + time) * waveAmp;

		// Y座標を変更
		waveVertexList[i].y = 0.5f + wave;   // 上の頂点
		waveVertexList[i + 1].y = -0.5f + wave; // 下の頂点
	}

	waveVertexList[10].y = 500.0f;

	// 頂点バッファを更新
	g_pDeviceContext->UpdateSubresource(m_pWaveVertexBuffer, 0, nullptr, waveVertexList.data(), 0, 0);


	pos.y += upSpeed;
}

void Wave::Draw() {
	UINT strides = sizeof(Vertex);
	UINT offsets = 0;
	g_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &strides, &offsets);

	// オブジェクトごとのシェーダーをセット
	if (m_pVertexShader && m_pPixelShader) {
		g_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
		g_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	}

	// テクスチャをピクセルシェーダーに渡す
	g_pDeviceContext->PSSetShaderResources(0, 1, &m_pTextureView);

	// 定数バッファをCPUにマップして現在の値を取得
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = g_pDeviceContext->Map(g_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr)) {
		return; // マッピング失敗時のエラーハンドリング
	}

	// **現在の定数バッファのデータを取得**
	ConstBuffer* pData = reinterpret_cast<ConstBuffer*>(mappedResource.pData);
	ConstBuffer cb = *pData;  // 既存データをコピー

	// プロジェクション変換行列を作成
	cb.matrixProj = DirectX::XMMatrixOrthographicLH(SCREEN_WIDTH / g_Camera.Camera_Pos.z, SCREEN_HEIGHT / g_Camera.Camera_Pos.z, 0.0f, 3.0f);
	cb.matrixProj = DirectX::XMMatrixTranspose(cb.matrixProj);

	// ワールド変換行列の作成
	cb.matrixWorld = DirectX::XMMatrixScaling(size.x, size.y, size.z);
	cb.matrixWorld *= DirectX::XMMatrixRotationZ(angle * 3.14f / 180);
	cb.matrixWorld *= DirectX::XMMatrixTranslation(pos.x - g_Camera.Camera_Pos.x, pos.y - g_Camera.Camera_Pos.y, pos.z);
	cb.matrixWorld = DirectX::XMMatrixTranspose(cb.matrixWorld);

	// UVアニメーションの行列作成
	float flipU = (direction == 1) ? -1.0f : 1.0f;  // 右向きならU軸を反転
	float offsetU = (direction == 1) ? 1.0f : 0.0f;  // 反転時に右側から開始するためにオフセットを適用
	cb.matrixTex = DirectX::XMMatrixScaling(flipU, 1.0f, 1.0f);
	cb.matrixTex *= DirectX::XMMatrixTranslation((offsetU + (float)numU) / splitX, (float)numV / splitY, 0.0f);
	cb.matrixTex = DirectX::XMMatrixTranspose(cb.matrixTex);

	// 頂点カラーのデータを作成
	cb.color = color;

	// **マッピング解除の前に更新データをコピー**
	*pData = cb;

	// マッピング解除
	g_pDeviceContext->Unmap(g_pConstantBuffer, 0);

	// 定数バッファをシェーダーに設定
	g_pDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pDeviceContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	// WaveBuffer 用のデータを更新
	struct WaveBufferData {
		float time;
		float waveAmplitude;
		float waveFrequency;
		float padding;  // HLSL の 16-byte alignment を満たすためのダミー
	} waveData;

	waveData.time = time;
	waveData.waveAmplitude = 1.0f;
	waveData.waveFrequency = 1.0f;
	waveData.padding = 0.0f;

	g_pDeviceContext->UpdateSubresource(g_pWaveConstantBuffer, 0, nullptr, &waveData, 0, 0);
	g_pDeviceContext->VSSetConstantBuffers(1, 1, &g_pWaveConstantBuffer);

	// **頂点数を増やしたので、それに応じた描画処理を行う**
	g_pDeviceContext->Draw(vertexCount, 0);  // 
}

void Wave::InitWave(int divX, int divY) {
    vertexCount = divX * 2;

    waveVertexList.clear();
    float step = 1.0f / (divX - 1);

    for (int i = 0; i < divX; ++i) {
        float x = -0.5f + step * i;
        waveVertexList.push_back({ x,  0.5f, 0.5f, 1, 1, 1, 1, x + 0.5f, 0.0f });
        waveVertexList.push_back({ x, -0.5f, 0.5f, 1, 1, 1, 1, x + 0.5f, 1.0f });
    }

    // 頂点バッファを作成
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(Vertex) * waveVertexList.size();
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = waveVertexList.data();

    HRESULT hr = g_pDevice->CreateBuffer(&bufferDesc, &initData, &m_pWaveVertexBuffer);
    if (FAILED(hr)) {
        OutputDebugString(L"Wave Vertex Buffer Creation Failed\n");
    }
}