#pragma once
#include "Object.h"
class Wave : public Object
{
private:
	// �g�̋���
	float WaveAmp = 0.1f;
	// �g�̕�
	float WaveFrequency = 1.0f;
public:
	void Update(float);
	void SetConstWaveVal(ID3D11DeviceContext* context, float newTime, float newWaveAmplitude, float newWaveFrequency);
};

	