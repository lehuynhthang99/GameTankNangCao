#pragma once

#include <d3dx9.h>
#include "Sprite.h"
#include "Object.h"
#include "Tiles.h"

enum FACING
{
	UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3
};

class Tank : public Object
{
protected:
	FACING curFacing = UP;
	float speed = 1.2f;
	int curSprite = 0;
	int frameDelay = 4;
	int startingFrame = UP * 2;
	int countDownFrameDelay = frameDelay;
	Sprite spriteSheet;
	Tiles spriteSheetInfo;
public:
	Tank();
	Tank(string path, int width, int height, float x, float y, FACING direction, int spriteElemNumber);
	~Tank();

	void Update();
	void Render(Camera camera);

private:
	void UpdateAnimation();
};	