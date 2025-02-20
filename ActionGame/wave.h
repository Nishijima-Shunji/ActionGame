#pragma once
#include "Object.h"
class Wave : public Object
{
private:
	// ”g‚Ì‹­‚³
	float WaveAmp = 0.1f;
	// ”g‚Ì•
	float WaveFrequency = 1.0f;
public:
	void Update(float);
	void SetConstWaveVal(ID3D11DeviceContext* context, float newTime, float newWaveAmplitude, float newWaveFrequency);
};

	