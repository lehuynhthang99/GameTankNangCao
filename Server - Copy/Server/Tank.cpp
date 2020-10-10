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
		SendPack('w');
	}
	else if (Key_Down(DIK_DOWN))
	{
		//objInfo.botLeftPosition.y -= speed * collisionTime;
		objInfo.velocity.y = -speed;
		objInfo.direction.y = -1;
		curFacing = DOWN;
		SendPack('s');
	}
	else if (Key_Down(DIK_LEFT))
	{
		//objInfo.botLeftPosition.x -= speed * collisionTime;
		objInfo.velocity.x = -speed;
		objInfo.direction.x = -1;
		curFacing = LEFT;
		SendPack('a');
	}
	else if (Key_Down(DIK_RIGHT))
	{
		//objInfo.botLeftPosition.x += speed * collisionTime;
		objInfo.velocity.x = speed;
		objInfo.direction.x = 1;
		curFacing = RIGHT;
		SendPack('d');
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

	ReceivPack();
}

void Tank::Render(Camera camera)
{
	RECT rect;
	rect.bottom = spriteSheet.GetHeight();
	rect.right = spriteSheet.GetWidth();
	rect.left = rect.top = 0;
	spriteSheet.Render(camera, spriteSheetInfo.getRectLocation(curSprite), objInfo, 1);
}

package* Tank::GetPackage()
{
	package* out = new package(objInfo.botLeftPosition.x, objInfo.botLeftPosition.y);
	return out;
}

void Tank::UsePackage(package * pak)
{
	objInfo.botLeftPosition.x = pak->x;
	objInfo.botLeftPosition.y = pak->y;
}

int Tank::SendPack(char command)
{
	const int SOCKET_BUFFER_SIZE = 2;
	__int8 buffer[SOCKET_BUFFER_SIZE];
	__int32 player_x;
	__int32 player_y;

	//scanf_s("\n%c", &buffer[0], 1);
	buffer[0] = command;

	// send to server
	int buffer_length = 1;
	int flags = 0;
	SOCKADDR* to = (SOCKADDR*)&server_address;
	int to_length = sizeof(server_address);
	if (sendto(sock, buffer, buffer_length, flags, to, to_length) == SOCKET_ERROR)
	{
		printf("sendto failed: %d", WSAGetLastError());
		auto err = WSAGetLastError();
		return 1;
	}
}

void Tank::ReceivPack()
{
	const int SOCKET_BUFFER_SIZE = 8;
	__int8 buffer[SOCKET_BUFFER_SIZE];

	__int32 player_x;
	__int32 player_y;

	int flags = 0;
	SOCKADDR_IN from;
	int from_size = sizeof(from);
	int bytes_received = recvfrom(sock, buffer, SOCKET_BUFFER_SIZE, flags, (SOCKADDR*)&from, &from_size);

	if (bytes_received == SOCKET_ERROR)
	{
		printf("recvfrom returned SOCKET_ERROR, WSAGetLastError() %d", WSAGetLastError());
		auto err = WSAGetLastError();
		auto err2 = err;
	}
	else
	{

		// grab data from packet
		__int32 read_index = 0;

		memcpy(&player_x, &buffer[read_index], sizeof(player_x));
		read_index += sizeof(player_x);

		memcpy(&player_y, &buffer[read_index], sizeof(player_y));
		read_index += sizeof(player_y);
		objInfo.botLeftPosition.x = player_x;
		objInfo.botLeftPosition.y = player_y;
	}
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
