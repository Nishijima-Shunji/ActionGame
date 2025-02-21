#pragma once
#include "BaseScene.h"
#include "sound.h"
#include "UI.h"

class Game;
class ResultScene : public BaseScene
{
private:
    Sound sound;
    UI* result_bg;
    std::vector<std::unique_ptr<UI>> scoreNum;
    int score;

public:
    ResultScene(int score);
    ~ResultScene();

    void Update() override;
    void Draw() override;
};

