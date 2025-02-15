#pragma once
#include "Object.h"

using Vec3 = DirectX::SimpleMath::Vector3;

class Entity :public Object
{
protected:
	DirectX::XMFLOAT3 prevPos;
	float radius = 25.0f;
	Vec3 velocity = { 0.0f,0.0f,0.0f };
	bool OnGround = false;
public:
	float GetRadius() { return radius; };
	Vec3 GetVelocity() { return velocity; };
	DirectX::XMFLOAT3 GetPrevPos() { return prevPos; };
	float GetWidth() { return Width; };
	float GetHeight() { return Height; };

	void SetVelocity(Vec3 _velocity) { velocity = _velocity; };
	void SetOnGround(bool is) { OnGround = is; };
};

