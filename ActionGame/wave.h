#pragma once
#include "Object.h"

class Wave : public Object {
private:
    std::vector<Vertex> waveVertexList;
    ID3D11Buffer* m_pWaveVertexBuffer = nullptr;
    int vertexCount = 10; // �������i���₵�������_���j
    float waveAmp = 0.1f;
    float waveFrequency = 0.0010f;
    float time = 0.0f;
    float upSpeed = 0.7;

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


	