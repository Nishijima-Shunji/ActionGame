#pragma once
#include "MapObject.h"
#include "BlockFragment.h"

class Block : public MapObject
{
private:
	bool deadFlg = false;
public:
	void Update(const std::vector<Entity*>& entities);
	void Destroy(std::vector<std::unique_ptr<BlockFragment>>& fragmentList);

	bool GetFlg() { return deadFlg; };
};

