	#pragma once
	#include "Object.h"

	class BlockFragment : public Object
	{
	private:
		DirectX::XMFLOAT3 velocity;  // 飛び散る速度
		float lifetime; // 破片の寿命 (時間経過で消える)
	public:
		BlockFragment(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 vel);
		void Update(float deltatime);

		float GetLifeTime() { return lifetime; };
	};

