#pragma once
#include "MapObject.h"
class Floor :
    public MapObject
{
private:

public:
    Floor();
    void Update(const std::vector<Entity*>& entities);
};

