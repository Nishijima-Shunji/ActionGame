#pragma once
#include "Object.h"
#include "MapObject.h"
#include "Entity.h"
#include "BlockFragment.h"

class Blade :
    public Object
{
public:
    void Update(std::vector<Entity*>& eneties, std::vector<std::unique_ptr<MapObject>>& blocks, std::vector<std::unique_ptr<BlockFragment>>&fragmentList);
};

