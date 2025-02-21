#include "UI.h"

void UI::Draw() {
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
	cb.matrixProj = DirectX::XMMatrixOrthographicLH(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 3.0f);
	cb.matrixProj = DirectX::XMMatrixTranspose(cb.matrixProj);

	// ワールド変換行列の作成
	cb.matrixWorld = DirectX::XMMatrixScaling(size.x, size.y, size.z);
	cb.matrixWorld *= DirectX::XMMatrixRotationZ(angle * 3.14f / 180);
	cb.matrixWorld *= DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
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

	// 描画命令
	g_pDeviceContext->Draw(4, 0);
}