#pragma once
#include "MapObject.h"
class Floor :
    public MapObject
{
private:

public:
    void Update(const std::vector<Entity*>& entities);
};

