#pragma once
#include "Object.h"

class Wave : public Object {
private:
    std::vector<Vertex> waveVertexList;
    ID3D11Buffer* m_pWaveVertexBuffer = nullptr;
    int vertexCount = 10; // 分割数（増やしたい頂点数）
    float waveAmp = 0.1f;
    float waveFrequency = 1.0f;
    float time = 0.0f;

public:
    virtual ~Wave() {
        if (m_pWaveVertexBuffer) {
            m_pWaveVertexBuffer->Release();
            m_pWaveVertexBuffer = nullptr;
        }
    }
    void Update(float);
    void Draw();
    void InitWave(int divX, int divY);

};


	