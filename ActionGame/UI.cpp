#include "UI.h"

void UI::Draw() {
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
	cb.matrixProj = DirectX::XMMatrixOrthographicLH(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 3.0f);
	cb.matrixProj = DirectX::XMMatrixTranspose(cb.matrixProj);

	// ���[���h�ϊ��s��̍쐬
	cb.matrixWorld = DirectX::XMMatrixScaling(size.x, size.y, size.z);
	cb.matrixWorld *= DirectX::XMMatrixRotationZ(angle * 3.14f / 180);
	cb.matrixWorld *= DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
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