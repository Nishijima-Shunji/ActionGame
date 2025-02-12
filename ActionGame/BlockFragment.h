#pragma once
#include "Object.h"
class BlockFragment : public Object
{
private:
	DirectX::XMFLOAT3 velocity;  // ”ò‚ÑU‚é‘¬“x
	float lifetime; // ”j•Ğ‚Ìõ–½ (ŠÔŒo‰ß‚ÅÁ‚¦‚é)
public:
	BlockFragment(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 vel);
	void Update(float deltatime);

	float GetLifeTime() { return lifetime; };
};

