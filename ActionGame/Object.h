#pragma once

#include <SimpleMath.h>
#include <vector>
#include <memory>

#include "direct3d.h"
#include "WICTextureLoader.h"
#include "TextureManager.h"
#include "OBBHit.h"

using Vector2 = DirectX::SimpleMath::Vector2;

class Object
{
protected:
	static TextureManager* textureManager;

	Vertex vertexList[4] =
	{
		//=============TRIANGLESTRIP===============
		//	x      y     z     r     g     b     a     u     v
		{-0.5f,  0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f},  // ０番目の頂点座標　{ x, y, z }
		{ 0.5f,  0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f},  // １番目の頂点座標
		{-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},  // ２番目の頂点座標
		{ 0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},  // ３番目の頂点座標
	};

	//座標
	DirectX::SimpleMath::Vector3 pos = { 0.0f, 0.0f, 0.0f };
	DirectX::SimpleMath::Vector3 size = { 100.0f, 100.0f, 0.0f };
	DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	float angle = 0.0f;
	DirectX::XMFLOAT3 gravity{ 0.0f, -9.8f * 7.0f, 0.0f };

	float Width;
	float Height;

	int direction = 0;

	// 頂点バッファ
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_pTextureView = nullptr;
	// シェーダー
	ID3D11VertexShader* m_pVertexShader = nullptr;  // オブジェクトごとの頂点シェーダー
	ID3D11PixelShader* m_pPixelShader = nullptr;    // オブジェクトごとのピクセルシェーダー

	//テクスチャが縦横に何分割されているか
	int splitX = 1;
	int splitY = 1;
public:

	virtual ~Object();

	DirectX::SimpleMath::Vector3 GetPos(void);
	DirectX::SimpleMath::Vector3 GetSize(void);
	DirectX::XMFLOAT4 GetColor(void);
	float GetAngle(void);
	DirectX::BoundingBox GetBoundingBox() {
		DirectX::XMFLOAT3 halfsize;
		halfsize.x = size.x / 2;
		halfsize.y = size.y / 2;
		halfsize.z = size.z / 2;
		return DirectX::BoundingBox(pos, halfsize);
	}

	//右上から何個目を切り抜いて表示するか
	float numU = 0;
	float numV = 0;

	virtual void Init(const wchar_t* imgname, ID3D11VertexShader* vs, ID3D11PixelShader* ps, int splitX = 1, int splitY = 1);
	virtual void Draw();
	virtual void Uninit();

	virtual void SetPos(float x, float y, float z);		//座標をセット
	virtual void SetPos(DirectX::XMFLOAT3);		//座標をセット
	virtual void SetSize(float x, float y, float z);	//大きさをセット
	virtual void SetAngle(float a);						//角度をセット
	virtual void SetColor(float r, float g, float b, float a);	//色をセット
	virtual void SetWidth(float _width) { Width = _width; };
	virtual void SetHeight(float _height) { Height = _height; };

	virtual void SetTexture(TextureManager* textureManager, const wchar_t* imgname);
	virtual void SetUV(int u, int v);

	static void SetTextureManager(TextureManager* manager) {
		textureManager = manager;
	}

	std::array<Vector2, 4> GetOBBVertices() const {
		Vector2 _pos = { pos.x,pos.y };
		return ComputeOBBVertices(_pos, Width, Height, angle); 
	}
};
