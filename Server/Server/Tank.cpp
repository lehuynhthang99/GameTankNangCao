#include "Tank.h"



Tank::Tank()
{
}

Tank::Tank(string path, int width, int height, float x, float y, FACING direction, int spriteElemNumber)
{
	spriteSheet = Sprite(path + ".png");
	spriteSheetInfo = Tiles(path + ".xml", spriteElemNumber);
	objInfo.botLeftPosition = D3DXVECTOR2(x, y);
	objInfo.direction = D3DXVECTOR2(1, 1);
	objInfo.center = D3DXVECTOR2(width / 2.0f, height / 2.0f);
	objInfo.width = width;
	objInfo.height = height;

	startingFrame = direction * 2;
	curFacing = direction;
}


Tank::~Tank()
{
}


void Tank::UpdateVelocity()
{
	objInfo.velocity = D3DXVECTOR2(0, 0);
	if (Key_Down(DIK_UP))
	{
		//objInfo.botLeftPosition.y += speed * collisionTime;
		objInfo.velocity.y = speed;
		objInfo.direction.y = 1;
		curFacing = UP;
	}
	else if (Key_Down(DIK_DOWN))
	{
		//objInfo.botLeftPosition.y -= speed * collisionTime;
		objInfo.velocity.y = -speed;
		objInfo.direction.y = -1;
		curFacing = DOWN;
	}
	else if (Key_Down(DIK_LEFT))
	{
		//objInfo.botLeftPosition.x -= speed * collisionTime;
		objInfo.velocity.x = -speed;
		objInfo.direction.x = -1;
		curFacing = LEFT;
	}
	else if (Key_Down(DIK_RIGHT))
	{
		//objInfo.botLeftPosition.x += speed * collisionTime;
		objInfo.velocity.x = speed;
		objInfo.direction.x = 1;
		curFacing = RIGHT;
	}

	if (objInfo.velocity != D3DXVECTOR2(0, 0))
		UpdateAnimation();

}

void Tank::Update(Map mapInfo)
{
	mapInfo.CollisionDetect(this, collisionDetect, 3);
	if (abs(normalX) > 0.0001f)
		objInfo.velocity.x = 0;
	if (abs(normalY) > 0.0001f)
		objInfo.velocity.y = 0;
	objInfo.botLeftPosition += objInfo.velocity *collisionTime;

	collisionTime = 1;
	normalX = normalY = 0;
}

void Tank::Render(Camera camera)
{
	RECT rect;
	rect.bottom = spriteSheet.GetHeight();
	rect.right = spriteSheet.GetWidth();
	rect.left = rect.top = 0;
	spriteSheet.Render(camera, spriteSheetInfo.getRectLocation(curSprite), objInfo, 1);
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
