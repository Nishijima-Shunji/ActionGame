#pragma once
#include "Object.h"
class Wave : public Object
{
private:
	// ”g‚Ì‹­‚³
	float WaveAmp = 1.0f;
	// ”g‚Ì•
	float WaveFrequency = 10.0f;
public:
	void Update(float);
	void SetConstWaveVal(ID3D11DeviceContext* context, float newTime, float newWaveAmplitude, float newWaveFrequency);
};

	