#include "Object.h"
#include <iostream>
#include "Camera.h"

TextureManager* Object::textureManager = nullptr; // �������������ōs��

Object::~Object() {

}


void Object::Init(const wchar_t* imgname, ID3D11VertexShader* vs, ID3D11PixelShader* ps, int splitx, int splity) {
	splitX = splitx;
	splitY = splity;
	vertexList[1].u = 1.0f / splitX;
	vertexList[2].v = 1.0f / splitY;
	vertexList[3].u = 1.0f / splitX;
	vertexList[3].v = 1.0f / splitY;

	// ���_�o�b�t�@���쐬
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(vertexList);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pSysMem = vertexList;

	HRESULT hr = g_pDevice->CreateBuffer(&bufferDesc, &subResourceData, &m_pVertexBuffer);
	if (FAILED(hr)) {
		OutputDebugStringA("���_�o�b�t�@�̍쐬�Ɏ��s\n");
	}

	// �e�N�X�`�����Z�b�g
	SetTexture(textureManager, imgname);

	// �V�F�[�_�[���Z�b�g
	m_pVertexShader = vs;
	m_pPixelShader = ps;
}

//void Object::Draw() {
//	UINT strides = sizeof(Vertex);
//	UINT offsets = 0;
//	g_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &strides, &offsets);
//
//	// �I�u�W�F�N�g���Ƃ̃V�F�[�_�[���Z�b�g
//	if (m_pVertexShader && m_pPixelShader) {
//		g_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
//		g_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
//	}
//
//	// �e�N�X�`�����s�N�Z���V�F�[�_�[�ɓn��
//	g_pDeviceContext->PSSetShaderResources(0, 1, &m_pTextureView);
//
//	// �萔�o�b�t�@���X�V
//	ConstBuffer cb;
//
//	// �v���W�F�N�V�����ϊ��s����쐬
//	cb.matrixProj = DirectX::XMMatrixOrthographicLH(SCREEN_WIDTH / g_Camera.Camera_Pos.z, SCREEN_HEIGHT / g_Camera.Camera_Pos.z, 0.0f, 3.0f);
//	cb.matrixProj = DirectX::XMMatrixTranspose(cb.matrixProj);
//
//	// ���[���h�ϊ��s��̍쐬
//	cb.matrixWorld = DirectX::XMMatrixScaling(size.x, size.y, size.z);
//	cb.matrixWorld *= DirectX::XMMatrixRotationZ(angle * 3.14f / 180);
//	cb.matrixWorld *= DirectX::XMMatrixTranslation(pos.x - g_Camera.Camera_Pos.x, pos.y - g_Camera.Camera_Pos.y, pos.z);
//	cb.matrixWorld = DirectX::XMMatrixTranspose(cb.matrixWorld);
//
//	// UV�A�j���[�V�����̍s��쐬
//	float flipU = (direction == 1) ? -1.0f : 1.0f;  // �E�����Ȃ�U���𔽓]
//	float offsetU = (direction == 1) ? 1.0f : 0.0f;  // ���]���ɉE������J�n���邽�߂ɃI�t�Z�b�g��K�p
//	cb.matrixTex = DirectX::XMMatrixScaling(flipU, 1.0f, 1.0f);
//	cb.matrixTex *= DirectX::XMMatrixTranslation((offsetU + (float)numU) / splitX, (float)numV / splitY, 0.0f);
//	cb.matrixTex = DirectX::XMMatrixTranspose(cb.matrixTex);
//
//	// ���_�J���[�̃f�[�^���쐬
//	cb.color = color;
//
//	// �萔�o�b�t�@��CPU�Ƀ}�b�v���čX�V
//	D3D11_MAPPED_SUBRESOURCE mappedResource;
//	HRESULT hr = g_pDeviceContext->Map(g_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//	if (FAILED(hr)) {
//		return; // �}�b�s���O���s���̃G���[�n���h�����O
//	}
//
//	// �X�V�����f�[�^��萔�o�b�t�@�ɃR�s�[
//	memcpy(mappedResource.pData, &cb, sizeof(ConstBuffer));
//
//	// �}�b�s���O����
//	g_pDeviceContext->Unmap(g_pConstantBuffer, 0);
//
//	// �萔�o�b�t�@���V�F�[�_�[�ɐݒ�
//	g_pDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
//	g_pDeviceContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
//
//	// �`�施��
//	g_pDeviceContext->Draw(4, 0);
//}

void Object::Draw() {
	UINT strides = sizeof(Vertex);
	UINT offsets = 0;
	g_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &strides, &offsets);

	// �I�u�W�F�N�g���Ƃ̃V�F�[�_�[���Z�b�g
	if (m_pVertexShader && m_pPixelShader) {
		g_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
		g_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	}

	// �e�N�X�`�����s�N�Z���V�F�[�_�[�ɓn��
	g_pDeviceContext->PSSetShaderResources(0, 1, &m_pTextureView);

	// �萔�o�b�t�@��CPU�Ƀ}�b�v���Č��݂̒l���擾
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = g_pDeviceContext->Map(g_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr)) {
		return; // �}�b�s���O���s���̃G���[�n���h�����O
	}

	// **���݂̒萔�o�b�t�@�̃f�[�^���擾**
	ConstBuffer* pData = reinterpret_cast<ConstBuffer*>(mappedResource.pData);
	ConstBuffer cb = *pData;  // �����f�[�^���R�s�[

	// �v���W�F�N�V�����ϊ��s����쐬
	cb.matrixProj = DirectX::XMMatrixOrthographicLH(SCREEN_WIDTH / g_Camera.Camera_Pos.z, SCREEN_HEIGHT / g_Camera.Camera_Pos.z, 0.0f, 3.0f);
	cb.matrixProj = DirectX::XMMatrixTranspose(cb.matrixProj);

	// ���[���h�ϊ��s��̍쐬
	cb.matrixWorld = DirectX::XMMatrixScaling(size.x, size.y, size.z);
	cb.matrixWorld *= DirectX::XMMatrixRotationZ(angle * 3.14f / 180);
	cb.matrixWorld *= DirectX::XMMatrixTranslation(pos.x - g_Camera.Camera_Pos.x, pos.y - g_Camera.Camera_Pos.y, pos.z);
	cb.matrixWorld = DirectX::XMMatrixTranspose(cb.matrixWorld);

	// UV�A�j���[�V�����̍s��쐬
	float flipU = (direction == 1) ? -1.0f : 1.0f;  // �E�����Ȃ�U���𔽓]
	float offsetU = (direction == 1) ? 1.0f : 0.0f;  // ���]���ɉE������J�n���邽�߂ɃI�t�Z�b�g��K�p
	cb.matrixTex = DirectX::XMMatrixScaling(flipU, 1.0f, 1.0f);
	cb.matrixTex *= DirectX::XMMatrixTranslation((offsetU + (float)numU) / splitX, (float)numV / splitY, 0.0f);
	cb.matrixTex = DirectX::XMMatrixTranspose(cb.matrixTex);

	// ���_�J���[�̃f�[�^���쐬
	cb.color = color;

	// **�}�b�s���O�����̑O�ɍX�V�f�[�^���R�s�[**
	*pData = cb;

	// �}�b�s���O����
	g_pDeviceContext->Unmap(g_pConstantBuffer, 0);


	// �萔�o�b�t�@���V�F�[�_�[�ɐݒ�
	g_pDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pDeviceContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	// �`�施��
	g_pDeviceContext->Draw(4, 0);
}



void Object::Uninit() {
	SAFE_RELEASE(m_pTextureView);
	SAFE_RELEASE(m_pVertexBuffer);
}

//���W���Z�b�g����
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

//�傫�����Z�b�g����
void Object::SetSize(float x, float y, float z) {
	size.x = x;
	size.y = y;
	size.z = z;
}

//�p�x���Z�b�g����
void Object::SetAngle(float a) {
	angle = a;
}

//�F���Z�b�g����
void Object::SetColor(float r, float g, float b, float a) {
	color.x = r;
	color.y = g;
	color.z = b;
	color.w = a;
}
//�ʒu���擾
DirectX::SimpleMath::Vector3 Object::GetPos(void) {
	return pos;
}
//�ʒu���Z�b�g
DirectX::SimpleMath::Vector3 Object::GetSize(void) {
	return size;
}
//�p�x���擾
float Object::GetAngle(void) {
	return angle;
}
//
DirectX::XMFLOAT4 Object::GetColor(void) {
	return color;
}

//�ʂɃe�N�X�`�����Z�b�g
void Object::SetTexture(TextureManager* textureManager, const wchar_t* imgname) {
	// ���݂̃e�N�X�`���r���[�����
	if (m_pTextureView) {
		m_pTextureView->Release();
		m_pTextureView = nullptr;
	}

	// TextureManager��ʂ��ĐV�����e�N�X�`�����擾
	m_pTextureView = textureManager->GetTexture(imgname);
	if (!m_pTextureView) {
		MessageBoxA(NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_ICONERROR | MB_OK);
		return;
	}
}

void Object::SetUV(int u, int v) {
	numU = u;
	numV = v;
}