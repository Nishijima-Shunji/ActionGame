#pragma once
#include "Object.h"
#include "Entity.h"

class MapObject :
	public Object
{
protected:
	float Width;
	float Height;

public:
	float GetWidth() { return Width; };
	float GetHeight() { return Height; };
	void SetScele() { Width = size.x; Height = size.y; };

	bool CheckCollisionForDestruction(Entity* target);
	bool CheckCollision(Entity* target);
};

