#include "Wave.h"

void Wave::Update(float timeCount) {
	SetConstWaveVal(g_pDeviceContext, timeCount, WaveAmp, WaveFrequency);
}

void Wave::SetConstWaveVal(ID3D11DeviceContext* context, float newTime, float newWaveAmplitude, float newWaveFrequency) {
    ConstBuffer data = {};
    data.time = newTime;
    data.waveAmplitude = newWaveAmplitude;
    data.waveFrequency = newWaveFrequency;

    // �萔�o�b�t�@���������쐬����Ă��邱�Ƃ��m�F
    if (!g_pConstantBuffer) {
        OutputDebugString(L"Constant buffer is not created.\n");
        return;
    }

    // �萔�o�b�t�@���X�V
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = context->Map(g_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(hr)) {
        OutputDebugString(L"Failed to map constant buffer\n");
        return;
    }

    // �f�[�^���R�s�[
    ConstBuffer* pData = reinterpret_cast<ConstBuffer*>(mappedResource.pData);
    *pData = data;

    // �}�b�s���O����
    context->Unmap(g_pConstantBuffer, 0);

    // �萔�o�b�t�@���V�F�[�_�[�ɐݒ�
    context->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
    context->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
}
