#include "Wave.h"

void Wave::Update(float timeCount) {
	SetConstWaveVal(g_pDeviceContext, timeCount, WaveAmp, WaveFrequency);
}

void Wave::SetConstWaveVal(ID3D11DeviceContext* context, float newTime, float newWaveAmplitude, float newWaveFrequency) {
    ConstBuffer data = {};
    data.time = newTime;
    data.waveAmplitude = newWaveAmplitude;
    data.waveFrequency = newWaveFrequency;

    // 定数バッファが正しく作成されていることを確認
    if (!g_pConstantBuffer) {
        OutputDebugString(L"Constant buffer is not created.\n");
        return;
    }

    // 定数バッファを更新
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = context->Map(g_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(hr)) {
        OutputDebugString(L"Failed to map constant buffer\n");
        return;
    }

    // データをコピー
    ConstBuffer* pData = reinterpret_cast<ConstBuffer*>(mappedResource.pData);
    *pData = data;

    // マッピング解除
    context->Unmap(g_pConstantBuffer, 0);

    // 定数バッファをシェーダーに設定
    context->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
    context->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
}
