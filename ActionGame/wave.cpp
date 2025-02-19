#include "Wave.h"

void Wave::Update(float timeCount) {
	SetConstWaveVal(g_pDeviceContext, timeCount, WaveAmp, WaveFrequency);
}

void Wave::SetConstWaveVal(ID3D11DeviceContext* context, float newTime, float newWaveAmplitude, float newWaveFrequency) {
//    ConstBuffer data = {};

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
    // **現在の定数バッファのデータを取得**
    ConstBuffer* pData = reinterpret_cast<ConstBuffer*>(mappedResource.pData);
    ConstBuffer cb = *pData;  // 既存データをコピー

    cb.time = newTime;
    cb.waveAmplitude = newWaveAmplitude;
    cb.waveFrequency = newWaveFrequency;

    // データをコピー
    *pData = cb;

    // マッピング解除
    context->Unmap(g_pConstantBuffer, 0);
    // 定数バッファをシェーダーに設定
    context->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
    context->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);

    hr = context->Map(g_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(hr)) {
        OutputDebugString(L"Failed to map constant buffer\n");
        return;
    }
    // **現在の定数バッファのデータを取得**
    pData = reinterpret_cast<ConstBuffer*>(mappedResource.pData);
    cb = *pData;  // 既存データをコピー
    context->Unmap(g_pConstantBuffer, 0);

}
