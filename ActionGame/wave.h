#pragma once
#include "Object.h"
class Wave : public Object
{
private:
	// �g�̋���
	float WaveAmp = 1.0f;
	// �g�̕�
	float WaveFrequency = 10.0f;
public:
	void Update(float);
	void SetConstWaveVal(ID3D11DeviceContext* context, float newTime, float newWaveAmplitude, float newWaveFrequency);
};

	