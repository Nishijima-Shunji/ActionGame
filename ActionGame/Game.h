#pragma once
#include "Object.h"
#include "input.h"
#include "sound.h"
#include "SceneManager.h"

class Game
{
private:
	Input input;
	Sound sound;
	SceneManager sm;
	
public:

	void Init(HWND hWnd);
	void Draw();
	void Uninit();
	void Update();

};

extern Game* _pGame;
