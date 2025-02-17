#include "Object.h"
#include <iostream>
#include "Camera.h"

TextureManager* Object::textureManager = nullptr; // 初期化もここで行う

Object::~Object() {

}


void Object::Init(const wchar_t* imgname, int splitx, int splity) {
	splitX = splitx;
	splitY = splity;
	vertexList[1].u = 1.0f / splitX;
	vertexList[2].v = 1.0f / splitY;
	vertexList[3].u = 1.0f / splitX;
	vertexList[3].v = 1.0f / splitY;

	// 頂点バッファを作成する
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(vertexList);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subResourceData;
	subResourceData.pSysMem = vertexList;
	subResourceData.SysMemPitch = 0;
	subResourceData.SysMemSlicePitch = 0;

	HRESULT hr = g_pDevice->CreateBuffer(&bufferDesc, &subResourceData, &m_pVertexBuffer);


	//テクスチャをセット
	SetTexture(textureManager, imgname);
}

	void Object::Draw() {
		UINT strides = sizeof(Vertex);
		UINT offsets = 0;
		g_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &strides, &offsets);

		//テクスチャをピクセルシェーダーに渡す
		g_pDeviceContext->PSSetShaderResources(0, 1, &m_pTextureView);

		//定数バッファを更新
		ConstBuffer cb;

		//プロジェクション変換行列を作成
		cb.matrixProj = DirectX::XMMatrixOrthographicLH(SCREEN_WIDTH / g_Camera.Camera_Pos.z, SCREEN_HEIGHT / g_Camera.Camera_Pos.z, 0.0f, 3.0f);
		cb.matrixProj - DirectX::XMMatrixTranspose(cb.matrixProj);

		//ワールド変換行列の作成
		//→オブジェクトの位置・大きさ・向きを指定
		cb.matrixWorld = DirectX::XMMatrixScaling(size.x, size.y, size.z);
		cb.matrixWorld *= DirectX::XMMatrixRotationZ(angle * 3.14f / 180);
		cb.matrixWorld *= DirectX::XMMatrixTranslation(pos.x - g_Camera.Camera_Pos.x, pos.y - g_Camera.Camera_Pos.y, pos.z);
		cb.matrixWorld = DirectX::XMMatrixTranspose(cb.matrixWorld);

		//UVアニメーションの行列作成
		float flipU = (direction == 1) ? -1.0f : 1.0f;  // 右向きならU軸を反転
		float offsetU = (direction == 1) ? 1.0f : 0.0f;  // 反転時に右側から開始するためにオフセットを適用
		cb.matrixTex = DirectX::XMMatrixScaling(flipU, 1.0f, 1.0f);
		cb.matrixTex *= DirectX::XMMatrixTranslation((offsetU + (float)numU) / splitX, (float)numV / splitY, 0.0f);
		cb.matrixTex = DirectX::XMMatrixTranspose(cb.matrixTex);

		//頂点カラーのデータを作成
		cb.color = color;

		//行列をシェーダーに渡す
		g_pDeviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

		g_pDeviceContext->Draw(4, 0); // 描画命令
	}

void Object::Uninit() {
	SAFE_RELEASE(m_pTextureView);
	SAFE_RELEASE(m_pVertexBuffer);
}

	//座標をセットする
void Object::SetPos(float x, float y, float z) {
	pos.x = x;
	pos.y = y;
	pos.z = z;
}
void Object::SetPos(DirectX::XMFLOAT3 _pos) {
	pos.x = _pos.x;
	pos.y = _pos.y;
	pos.z = _pos.z;
}

	//大きさをセットする
void Object::SetSize(float x, float y, float z) {
	size.x = x;
	size.y = y;
	size.z = z;
}

	//角度をセットする
void Object::SetAngle(float a) {
	angle = a;
}

	//色をセットする
void Object::SetColor(float r, float g, float b, float a) {
	color.x = r;
	color.y = g;
	color.z = b;
	color.w = a;
}
	//位置を取得
DirectX::SimpleMath::Vector3 Object::GetPos(void) {
	return pos;
}
	//位置をセット
DirectX::SimpleMath::Vector3 Object::GetSize(void) {
	return size;
}
	//角度を取得
float Object::GetAngle(void) {
	return angle;
}
	//
DirectX::XMFLOAT4 Object::GetColor(void) {
	return color;
}

	//個別にテクスチャをセット
void Object::SetTexture(TextureManager* textureManager, const wchar_t* imgname) {
	// 現在のテクスチャビューを解放
	if (m_pTextureView) {
		m_pTextureView->Release();
		m_pTextureView = nullptr;
	}

	// TextureManagerを通じて新しいテクスチャを取得
	m_pTextureView = textureManager->GetTexture(imgname);
	if (!m_pTextureView) {
		MessageBoxA(NULL, "テクスチャ読み込み失敗", "エラー", MB_ICONERROR | MB_OK);
		return;
	}
}

void Object::SetUV(int u, int v) {
	numU = u;
	numV = v;
}