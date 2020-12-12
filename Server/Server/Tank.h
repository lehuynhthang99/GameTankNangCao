#pragma once

class Tank;

#include <d3dx9.h>
#include "Sprite.h"
#include "Object.h"
#include "Tiles.h"
#include "Map.h"
#include "Bullet.h"



class Tank : public Object
{
protected:
	static unsigned int idInit;
	static const string pathToResource;
	FACING curFacing = UP;
	float speed = 1.2f;
	int curSprite = 0;
	int frameDelay = 4;
	int startingFrame = UP * 2;
	int countDownFrameDelay = frameDelay;
	Sprite spriteSheet;
	Tiles spriteSheetInfo;
	MapElement collisionDetect[3] = { BRICK, STONE, WATER };
	Bullet* bullet = NULL;

	D3DXVECTOR2 respawnPos;

	

public:
	Tank();
	Tank(int width, int height, float x, float y, FACING direction, int spriteElemNumber);
	~Tank();

	Bullet* UpdateInput();
	void Update(Map* mapInfo, Tank* tanks, int numberOfTanks);
	void Render(Camera camera);
	void TankCollideDetect(Tank* tanks, int numberOfTanks);
	void TankCollideBullet(Bullet* bullet);
	void UpdateVelocity();
	void BulletReset();
	void Respawn();
private:
	void UpdateAnimation();
};	