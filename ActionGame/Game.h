#pragma once
#include "Object.h"
#include "input.h"
#include "sound.h"
#include "SceneManager.h"

extern ID3D11VertexShader* vertexShader1;
extern ID3D11PixelShader* pixelShader1;
extern ID3D11VertexShader* vertexShader2;
extern ID3D11PixelShader* pixelShader2;

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
