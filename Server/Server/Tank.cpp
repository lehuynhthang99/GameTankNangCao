#include "Tank.h"

unsigned int Tank::idInit = 0;
const string Tank::pathToResource = "Resources/Tank";

Tank::Tank()
{
	objType = TankObj;
}

Tank::Tank(int width, int height, float x, float y, FACING direction, int spriteElemNumber)
{
	id = idInit;
	string tankName = "//Tank" + to_string(id + 1);
	idInit = (idInit + 1) % TANK_MAX_RANGE;
	string path = pathToResource + tankName + tankName;
	spriteSheet = Sprite(path + ".png");
	spriteSheetInfo = Tiles(path + ".xml", spriteElemNumber);

	respawnPos = D3DXVECTOR2(x, y);

	objInfo.botLeftPosition = respawnPos;
	objInfo.direction = D3DXVECTOR2(1, 1);
	objInfo.center = D3DXVECTOR2(width / 2.0f, height / 2.0f);
	objInfo.width = width;
	objInfo.height = height;
	objType = TankObj;

	startingFrame = direction * 2;
	curFacing = direction;
}


Tank::~Tank()
{
}


Bullet* Tank::UpdateInput()
{
	objInfo.velocity = D3DXVECTOR2(0, 0);
	FACING prevFace = curFacing;
	if (Key_Down(DIK_UP) && id == 0 ||
		Key_Down(DIK_W) && id == 1)
	{
		//objInfo.botLeftPosition.y += speed * collisionTime;
		objInfo.velocity.y = speed;
		objInfo.direction.x = 0;
		objInfo.direction.y = 1;
		curFacing = UP;
	}
	else if (Key_Down(DIK_DOWN) && id == 0 ||
		Key_Down(DIK_S) && id == 1)
	{
		//objInfo.botLeftPosition.y -= speed * collisionTime;
		objInfo.velocity.y = -speed;
		objInfo.direction.x = 0;
		objInfo.direction.y = -1;
		curFacing = DOWN;
	}
	else if (Key_Down(DIK_LEFT) && id == 0 ||
		Key_Down(DIK_A) && id == 1)
	{
		//objInfo.botLeftPosition.x -= speed * collisionTime;
		objInfo.velocity.x = -speed;
		objInfo.direction.x = -1;
		objInfo.direction.y = 0;
		curFacing = LEFT;
	}
	else if (Key_Down(DIK_RIGHT) && id == 0 ||
		Key_Down(DIK_D) && id == 1)
	{
		//objInfo.botLeftPosition.x += speed * collisionTime;
		objInfo.velocity.x = speed;
		objInfo.direction.x = 1;
		objInfo.direction.y = 0;
		curFacing = RIGHT;
	}

	bool isCreateBullet = false;
	if (bullet == NULL)
	{
		if (Key_Down(DIK_SPACE) && id == 0 ||
			Key_Down(DIK_LCONTROL) && id == 1)
		{
			isCreateBullet = true;
			D3DXVECTOR2 firingPos = objInfo.GetCenterPos();
			//firingPos += objInfo.direction * (objInfo.width / 2 - 7);
			bullet = new Bullet(id, firingPos.x, firingPos.y, curFacing);
		}
	}

	if (prevFace != curFacing)
		countDownFrameDelay = 0;

	if (objInfo.velocity != D3DXVECTOR2(0, 0))
		UpdateAnimation();

	if (isCreateBullet)
		return bullet;
	return NULL;

}

void Tank::Update(Map* mapInfo, Tank* tanks, int numberOfTanks)
{
	mapInfo->CollisionDetect(this, collisionDetect, 3);
	TankCollideDetect(tanks, numberOfTanks);
	
	
	if (bullet != NULL)
	{
		if (!bullet->isDestroy)
			bullet->UpdateBullet(mapInfo);
		//if (bullet->isDestroy)
		//{
		//	//delete bullet;
		//	bullet = nullptr;
		//}
	}
}



void Tank::Render(Camera camera)
{
	if (!isRespawn)
	{
		RECT rect;
		rect.bottom = spriteSheet.GetHeight();
		rect.right = spriteSheet.GetWidth();
		rect.left = rect.top = 0;
		spriteSheet.Render(camera, spriteSheetInfo.getRectLocation(curSprite), objInfo, 1);
	}
	if (bullet)
		bullet->Render(camera);
}

void Tank::UpdateAnimation()
{
	startingFrame = curFacing * 2;
	countDownFrameDelay--;
	if (countDownFrameDelay <= 0)
	{
		countDownFrameDelay = frameDelay;
		curSprite = curSprite % 2 + startingFrame;
	}
}

void Tank::TankCollideDetect(Tank * tanks, int numberOfTanks)
{
	for (int i = 0; i < numberOfTanks; i++)
	{
		/*if (tanks[i].id == id)
			continue;*/
		float normalX, normalY;
		float collisionTime = SweptAABB(this->objInfo, tanks[i].objInfo, normalX, normalY);
		if (collisionTime < 0.0f)
			collisionTime = 0;
		if (collisionTime < this->collisionTime && (normalX != 0 || normalY != 0))
		{
			this->collisionTime = collisionTime;
			this->normalX = normalX;
			this->normalY = normalY;
		}

	}
}

void Tank::TankCollideBullet(Bullet * bullet)
{
	if (isRespawn || bullet->isDestroy)
		return;
	float normalX, normalY;
	float collisionTime = SweptAABB(this->objInfo, bullet->objInfo, normalX, normalY);
	if (collisionTime < 0.0f)
		collisionTime = 0;
	if (collisionTime < this->collisionTime && (normalX != 0 || normalY != 0))
	{
		this->isRespawn = true;
		bullet->isDestroy = true;
	}
}

void Tank::UpdateVelocity()
{
	if (abs(normalX) > 0.0001f)
		objInfo.velocity.x = 0;
	if (abs(normalY) > 0.0001f)
		objInfo.velocity.y = 0;
	objInfo.botLeftPosition += objInfo.velocity *collisionTime;

	collisionTime = 1;
	normalX = normalY = 0;
}

void Tank::BulletReset()
{
	if (bullet && bullet->isDestroy)
		bullet = NULL;
}

void Tank::Respawn()
{
	if (!isRespawn)
		return;
	objInfo.botLeftPosition = respawnPos;
	isRespawn = false;
}
