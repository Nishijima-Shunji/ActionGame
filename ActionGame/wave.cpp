#include "Wave.h"

void Wave::Update(float timeCount) {
    time = timeCount;
	//SetConstWaveVal(g_pDeviceContext, timeCount, WaveAmp, WaveFrequency);
}

void Wave::SetConstWaveVal(ID3D11DeviceContext* context, float newTime, float newWaveAmplitude, float newWaveFrequency) {
    if (!g_pWaveConstantBuffer) {
        OutputDebugString(L"Wave constant buffer is not created.\n");
        return;
    }
    if (m_pVertexShader && m_pPixelShader) {
        g_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
        g_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
    }

    // WaveBuffer 用のデータを更新
    struct WaveBufferData {
        float time;
        float waveAmplitude;
        float waveFrequency;
        float padding;  // HLSL の 16-byte alignment を満たすためのダミー
    } waveData;

    waveData.time = newTime;
    waveData.waveAmplitude = newWaveAmplitude;
    waveData.waveFrequency = newWaveFrequency;
    waveData.padding = 0.0f;

    context->UpdateSubresource(g_pWaveConstantBuffer, 0, nullptr, &waveData, 0, 0);

    //D3D11_MAPPED_SUBRESOURCE mappedResource;
    //HRESULT hr = g_pDeviceContext->Map(g_pWaveConstantBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);
    //if (FAILED(hr)) {
    //    return; // マッピング失敗時のエラーハンドリング
    //}
    //WaveConstBuffer* pData = reinterpret_cast<WaveConstBuffer*>(mappedResource.pData);
    //WaveConstBuffer cb = *pData;  // 既存データをコピー
    //g_pDeviceContext->Unmap(g_pConstantBuffer, 0);



    // スロット b1 に WaveBuffer をセット
    context->VSSetConstantBuffers(1, 1, &g_pWaveConstantBuffer);
}
