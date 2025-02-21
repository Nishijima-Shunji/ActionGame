#include "Wave.h"
#include "Camera.h"

void Wave::Update(float timeCount) {
	// �V�F�[�_�[�œ������i�������j
	time = timeCount;
	float waveHeight = 0.1f;  // �g�̍���

	for (int i = 0; i < waveVertexList.size(); i += 2) {
		// ���[���h���W��X�l���l��
		float x = pos.x + waveVertexList[i].x * size.x;

		// �[�ł͐U��������������
		float attenuation = 1.0f - abs(waveVertexList[i].x / 0.5f);
		float wave = attenuation * sin(x * waveFrequency + time) * waveAmp;

		// Y���W��ύX
		waveVertexList[i].y = 0.5f + wave;   // ��̒��_
		waveVertexList[i + 1].y = -0.5f + wave; // ���̒��_
	}

	waveVertexList[10].y = 500.0f;

	// ���_�o�b�t�@���X�V
	g_pDeviceContext->UpdateSubresource(m_pWaveVertexBuffer, 0, nullptr, waveVertexList.data(), 0, 0);


	pos.y += upSpeed;
}

void Wave::Draw() {
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

	// WaveBuffer �p�̃f�[�^���X�V
	struct WaveBufferData {
		float time;
		float waveAmplitude;
		float waveFrequency;
		float padding;  // HLSL �� 16-byte alignment �𖞂������߂̃_�~�[
	} waveData;

	waveData.time = time;
	waveData.waveAmplitude = 1.0f;
	waveData.waveFrequency = 1.0f;
	waveData.padding = 0.0f;

	g_pDeviceContext->UpdateSubresource(g_pWaveConstantBuffer, 0, nullptr, &waveData, 0, 0);
	g_pDeviceContext->VSSetConstantBuffers(1, 1, &g_pWaveConstantBuffer);

	// **���_���𑝂₵���̂ŁA����ɉ������`�揈�����s��**
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

    // ���_�o�b�t�@���쐬
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