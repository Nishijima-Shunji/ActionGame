#pragma once
#include "BaseScene.h"
#include "Sound.h"
#include "UI.h"

class Game;
class TitleScene : public BaseScene
{
private:
	TextureManager* textureManager;
	Sound sound;
	std::vector<std::unique_ptr<UI>> title_bg;
	UI* button;
	UI* text;

	int state = 0;

public:
	TitleScene();
	~TitleScene();

	void Update() override;
	void Draw() override;

	void Animation();
};