#pragma once
#include "BaseScene.h"
#include "Sound.h"

class Game;
class TitleScene : public BaseScene
{
private:
	Sound sound;
	Object* title_bg;
	Object* button;
	Object* health;
	std::vector<std::unique_ptr<Object>> scoreNum;

	int state = 0;

	int max = 30;
	int maxhp = 1;

	float time = 0.0f;
	float frequency = 0.8f;     // U“®‚Ì‘¬‚³ (1.0f = 1•b‚Å1üŠú)
	float amplitude = 10.0f;    // U• (Šp“x‚Ì”ÍˆÍ: }“x)
	float offset = 0.0f;       // Šî€Šp“x (’†SˆÊ’u)

public:
	TitleScene();
	~TitleScene();

	void Update() override;
	void Draw() override;
};