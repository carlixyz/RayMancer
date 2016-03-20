
#include <assert.h>
#include <cstdint>

//#include <string>
//#include <math.h>


#include "PixelToaster\PixelToaster.h"

using namespace std;
using namespace PixelToaster;
//



#include "main.h"



void main()
{
	// Check Game Init
	//assert( Game::Get().Init() );

	Display MainCanvas("RayMancer", SCREEN_W, SCREEN_H, Output::Windowed);
	Display MapCanvas("Top View", Map.width * Map.Scale, Map.height * Map.Scale, Output::Windowed);

	//vector<Pixel> pixels(SCREEN_W * SCREEN_H);
	//vector<Pixel> MiniMap(HALF_SCR_W * HALF_SCR_H);

	gfx::rasterMap(Map, MiniMap);
	//gfx::drawLine(Pixel(1, 1, 1), Player.posX, Player.posY, 4, 4, tempMap, 1);
	//gfx::scale(tempMap, MiniMap);


	//timer.delta()


	while (MainCanvas.open() && MapCanvas.open() )
	{
		unsigned int index = 0;

		for (int y = 0; y < SCREEN_H; ++y)
		{
			for (int x = 0; x < SCREEN_W; ++x)
			{
				pixels.Data[index].r = 0.1f + (x + y) * 0.0015f;
				++index;
			}
		}

		gfx::drawRect(Pixel(0.65f, 0.25f, 0.75f, 0.015f), 20, 20, 600, 400, pixels);
		Player.DrawMiniMap();


		if (MainCanvas.open())
			MainCanvas.update(pixels.Data);

		if (MapCanvas.open())
			MapCanvas.update(MiniMap.Data);

	}
}

#include <math.h>

void Entity::DrawMiniMap()
{
	Pixel ZoneColor(0, .75f, .75f);
	Pixel DirColor(1,1,1);
	float Pzone			= .2f;		// A little offset 2 draw a small area around Player
	float FrontLength		= 3;		// A little offset 2 draw a small area around Player
	float FrontX = (Player.posX + (cos(Player.Rot) * FrontLength)) * Map.Scale;
	float FrontY = (Player.posY + (sin(Player.Rot) * FrontLength)) * Map.Scale;

	gfx::drawRect(ZoneColor, (Player.posX - Pzone) * Map.Scale, (Player.posY - Pzone) * Map.Scale, (Player.posX + Pzone) * Map.Scale, (Player.posY + Pzone) * Map.Scale, MiniMap);
	gfx::drawLine(DirColor, Player.posX * Map.Scale, Player.posY * Map.Scale, FrontX, FrontY, MiniMap, 2);

}

/*
void Entity::onKeyPressed(DisplayInterface & display, Key key)
{
	Move = 0;
	Rot = 0;

	if (key == Key::W)
		Move = 1;

	if (key == Key::S)
		Move = -1;

	// Player will move this far along
	// the current direction vector
	float moveStep = MoveSpeed * Move;

	if (key == Key::D)
		Dir = 1;

	if (key == Key::A)
		Dir = -1;

	// Add rotation if player is rotating (player.dir != 0)
	Rot += Dir * RotSpeed;

	// Calculate new player position with simple trigonometry
	float newX = posX + cos(Rot) * moveStep;
	float newY = posY + sin(Rot) * moveStep;

	// Set new position
	posX = newX;
	posY = newY;
	Timer timer;

	//speed modifiers
	MoveSpeed = timer.delta() * 5.0f; //the constant value is in squares/second
	RotSpeed = timer.delta() * 3.0f; //the constant value is in radians/second
}*/

 /*
void Entity::UpdateInput(float timeStep)
{


	Move = 0;
	Rot = 0;

	if (GetAsyncKeyState(VK_UP)&KF_UP)
		Move = 1;

	if (GetAsyncKeyState(VK_DOWN)&KF_UP)
		Move = -1;

	// Player will move this far along
	// the current direction vector
	float moveStep = MoveSpeed * Move;

	if (GetAsyncKeyState(VK_RIGHT)&KF_UP)
		Dir = 1;

	if (GetAsyncKeyState(VK_LEFT)&KF_UP)
		Dir = -1;

	// Add rotation if player is rotating (player.dir != 0)
	Rot += Dir * RotSpeed;

	// Calculate new player position with simple trigonometry
	float newX = posX + cos(Rot) * moveStep;
	float newY = posY + sin(Rot) * moveStep;

	// Set new position
	posX = newX;
	posY = newY;

	//speed modifiers
	MoveSpeed = timeStep * 5.0f; //the constant value is in squares/second
	RotSpeed = timeStep * 3.0f; //the constant value is in radians/second
}

 */

/*
void UpdateInput(Vector2D & pos, Vector2D & dir, Vector2D & plane, float lfTimestep)
{
	GetCursorPos(&MousePos);

	//speed modifiers
	double moveSpeed = lfTimestep * 5.0f; //the constant value is in squares/second
	double rotSpeed = lfTimestep * 3.0f; //the constant value is in radians/second

	//if(GetAsyncKeyState(VK_RIGHT)&KF_UP)
	if (GetAsyncKeyState(VK_UP)&KF_UP || GetAsyncKeyState(0x57)&KF_UP)
	{
		if (worldMap[int(pos.x + dir.x * moveSpeed)][int(pos.y)] == false) pos.x += dir.x * moveSpeed;
		if (worldMap[int(pos.x)][int(pos.y + dir.y * moveSpeed)] == false) pos.y += dir.y * moveSpeed;
	}
	if (GetAsyncKeyState(VK_DOWN)&KF_UP || GetAsyncKeyState(0x53)&KF_UP)		//move backwards if no wall behind you
	{
		if (worldMap[int(pos.x - dir.x * moveSpeed)][int(pos.y)] == false) pos.x -= dir.x * moveSpeed;
		if (worldMap[int(pos.x)][int(pos.y - dir.y * moveSpeed)] == false) pos.y -= dir.y * moveSpeed;
	}

	if (GetAsyncKeyState(VK_RSHIFT)&KF_UP || GetAsyncKeyState(0x44)&KF_UP)		//move rightside if no wall behind you
	{
		if (worldMap[int(pos.x + plane.x * moveSpeed)][int(pos.y)] == false) pos.x += plane.x * moveSpeed;
		if (worldMap[int(pos.x)][int(pos.y + plane.y * moveSpeed)] == false) pos.y += plane.y * moveSpeed;
	}
	if (GetAsyncKeyState(VK_LSHIFT)&KF_UP || GetAsyncKeyState(0x41)&KF_UP)		//move leftside if no wall behind you
	{
		if (worldMap[int(pos.x - plane.x * moveSpeed)][int(pos.y)] == false) pos.x -= plane.x * moveSpeed;
		if (worldMap[int(pos.x)][int(pos.y - plane.y * moveSpeed)] == false) pos.y -= plane.y * moveSpeed;
	}

	if (MousePos.x != ScreenW >> 2)
	{

		double oldDirX = dir.x;

		double tempRot = (double)((ScreenW >> 2) - MousePos.x) * lfTimestep * .5;

		dir.x = dir.x * cos(tempRot) - dir.y * sin(tempRot);
		dir.y = oldDirX * sin(tempRot) + dir.y * cos(tempRot);
		double oldPlaneX = plane.x;
		plane.x = plane.x * cos(tempRot) - plane.y * sin(tempRot);
		plane.y = oldPlaneX * sin(tempRot) + plane.y * cos(tempRot);

		SetCursorPos(ScreenW >> 2, ScreenH >> 2);
	}

	if ((GetAsyncKeyState(VK_RIGHT)&KF_UP))		//rotate to the right 		
	{
		//both camera direction and camera plane must be rotated
		double oldDirX = dir.x;
		dir.x = dir.x * cos(-rotSpeed) - dir.y * sin(-rotSpeed);
		dir.y = oldDirX * sin(-rotSpeed) + dir.y * cos(-rotSpeed);
		double oldPlaneX = plane.x;
		plane.x = plane.x * cos(-rotSpeed) - plane.y * sin(-rotSpeed);
		plane.y = oldPlaneX * sin(-rotSpeed) + plane.y * cos(-rotSpeed);
	}

	if (GetAsyncKeyState(VK_LEFT)&KF_UP)		//rotate to the left	
	{
		//both camera direction and camera plane must be rotated
		double oldDirX = dir.x;
		dir.x = dir.x * cos(rotSpeed) - dir.y * sin(rotSpeed);
		dir.y = oldDirX * sin(rotSpeed) + dir.y * cos(rotSpeed);
		double oldPlaneX = plane.x;
		plane.x = plane.x * cos(rotSpeed) - plane.y * sin(rotSpeed);
		plane.y = oldPlaneX * sin(rotSpeed) + plane.y * cos(rotSpeed);
	}

}

*/
