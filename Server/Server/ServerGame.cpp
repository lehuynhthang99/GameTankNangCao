#include "ServerGame.h"

//id's to assign clients for our table
unsigned int ServerGame::client_id = 0;

ServerGame::ServerGame()
{
	UINT sleep_granularity_ms = 1;
	sleep_granularity_was_set = timeBeginPeriod(sleep_granularity_ms) == TIMERR_NOERROR;

	QueryPerformanceFrequency(&clock_frequency);
}


ServerGame::~ServerGame()
{
}

int ServerGame::Game_Init()
{
	virtualClock.Start();
	ListObjectInGame* objList = ListObjectInGame::GetInstance();
	tank = Tank(13, 13, 0, 0, UP, 8);
	camera = Camera(GAME_WIDTH);
	map = Map(8, 8, 7);
	return 1;
}

void ServerGame::Game_Run()
{
	//reacquire input
	dikeyboard->Acquire();
	dimouse->Acquire();
	Poll_Keyboard();
	Poll_Mouse();

	//Get start time of the beginning period
	//For FPS
	virtualClock.setStartTickTime();

	//Update
	tank.UpdateInput();

	Update();
	tank.Update(&map);

	//Render
	//start render
	if (d3ddev == NULL)
		return;

	if (d3ddev->BeginScene())
	{
		d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

		//spritehandler render
		spritehandler->Begin(D3DXSPRITE_ALPHABLEND);

		//begin

		tank.Render(camera);
		map.Render(camera);

		//end

		spritehandler->End();
		d3ddev->EndScene();
	}

	d3ddev->Present(NULL, NULL, NULL, NULL);

	//Sleep for FPS
	float time_taken_s = virtualClock.getTimeSince_miliSec() / 1000.0f;
	while (time_taken_s < SECONDS_PER_TICK)
	{
		if (virtualClock.sleep_granularity_was_set)
		{
			DWORD time_to_wait_ms = DWORD((SECONDS_PER_TICK - time_taken_s) * 1000);
			if (time_to_wait_ms > 0)
			{
				Sleep(time_to_wait_ms);
			}
		}
		time_taken_s = virtualClock.getTimeSince_miliSec() / 1000.0f;
	}
}

void ServerGame::Update()
{

}

void ServerGame::Game_End()
{
	if (d3d != NULL) d3d->Release();
	if (d3ddev != NULL) d3ddev->Release();
	if (backbuffer != NULL) backbuffer->Release();
	if (spritehandler != NULL) spritehandler->Release();
	if (dinput != NULL) dinput->Release();
	if (font != NULL) font->Release();
	Kill_Keyboard();
	Kill_Mouse();
}
