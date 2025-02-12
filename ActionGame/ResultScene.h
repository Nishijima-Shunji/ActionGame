#pragma once
#include "BaseScene.h"
#include "sound.h"

class Game;
class ResultScene : public BaseScene
{
private:
    Sound sound;
    Object* result_bg;
    std::vector<std::unique_ptr<Object>> scoreNum;
    int score;

public:
    ResultScene(int score);
    ~ResultScene();

    void Update() override;
    void Draw() override;
};

