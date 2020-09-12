#pragma once

#include "Tiles.h"
#include "Sprite.h"
#include <fstream>

class Map
{
protected:
	Sprite spriteSheet;
	Tiles spriteSheetInfo;
	int block[26][26];
	int elemWidth, elemHeight;

public:
	Map();
	Map(string path, int width, int height, int spriteElemNumber);

	void Render(Camera camera);
	~Map();
};

