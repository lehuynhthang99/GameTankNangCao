#include "Map.h"



Map::Map()
{
}

Map::Map(string path, int width, int height, int spriteElemNumber)
{
	spriteSheet = Sprite(path + ".png");
	spriteSheetInfo = Tiles(path + ".xml", spriteElemNumber);
	elemWidth = width;
	elemHeight = height;
	ifstream map;
	map.open(path + ".txt");
	for (int i = 25; i > -1; i--)
		for (int j = 0; j < 26; j++)
			map >> block[i][j];
	map.close();
}


void Map::Render(Camera camera)
{
	D3DXVECTOR3 center = D3DXVECTOR3(elemWidth / 2.0f, elemHeight / 2.0f, 0);
	D3DXVECTOR2 offset = D3DXVECTOR2(elemWidth / 2.0f, elemHeight / 2.0f);
	for (int i = 0; i < 26; i++)
		for (int j = 0; j < 26; j++)
		{
			if (block[i][j] == 0)
				continue;
			spriteSheet.Render(camera, spriteSheetInfo.getRectLocation(block[i][j] - 1), center,
				offset.x + elemWidth * j, offset.y + elemHeight * i);
		}
}

Map::~Map()
{
}
