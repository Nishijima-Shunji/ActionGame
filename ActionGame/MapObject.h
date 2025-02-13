#pragma once
#include "Object.h"
#include "Entity.h"
#include <array>


using Vector2 = DirectX::SimpleMath::Vector2;

class MapObject :
	public Object
{
public:
	float GetWidth() { return Width; };
	float GetHeight() { return Height; };
	void SetScele() { Width = size.x; Height = size.y; };

	bool CheckCollision(Entity* target);
	void ResolveCollision(Entity* target);
	bool CheckOBBHit(const Object* target);
	bool Collision_OBB_OBB(const std::array<Vector2, 4>& vertsA, const std::array<Vector2, 4>& vertsB);
};

