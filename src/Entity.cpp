
#include "Entity.h"
#include "Main.h"
#include <math.h>

//#include	<iostream>

void Entity::DrawInMap(PixelBuffer & Dst)
{
	//Pixel ZoneColor(0, .75f, .75f);
	DWORD ZoneColor = 0x008888FF;
	DWORD DirColor = 0x00007777;
	float Pzone = .2f;		// A little offset 2 draw a small area around Player
	float FrontLength = 2;		// A little offset 2 draw a small area around Player
	float FrontX = (Player.posX + (cos(Player.Rot) * FrontLength)) * Map.Scale;
	float FrontY = (Player.posY + (sin(Player.Rot) * FrontLength)) * Map.Scale;

	//std::cout << " RADIANS: " << Player.Rot << " - DEGREES: " << DEG(Player.Rot) << "\n";

	float LeftViewX = (Player.posX + (cos(Player.Rot + RAD(-30))) * FrontLength * 2) * Map.Scale;
	float LeftViewY = (Player.posY + (sin(Player.Rot + RAD(-30))) * FrontLength * 2) * Map.Scale;
	float RightViewX = (Player.posX + (cos(Player.Rot + RAD(+30))) * FrontLength * 2) * Map.Scale;
	float RightViewY = (Player.posY + (sin(Player.Rot + RAD(+30))) * FrontLength * 2) * Map.Scale;
	gfx::drawLine(ZoneColor, Player.posX * Map.Scale, Player.posY * Map.Scale, LeftViewX, LeftViewY, Dst, 1);
	gfx::drawLine(ZoneColor, Player.posX * Map.Scale, Player.posY * Map.Scale, RightViewX, RightViewY, Dst, 1);


	gfx::drawRect(ZoneColor, (Player.posX - Pzone) * Map.Scale, (Player.posY - Pzone) * Map.Scale, (Player.posX + Pzone) * Map.Scale, (Player.posY + Pzone) * Map.Scale, Dst);
	gfx::drawLine(DirColor, Player.posX * Map.Scale, Player.posY * Map.Scale, FrontX, FrontY, Dst, 2);

}

void Entity::UpdateInput(AppManager & App, float timeStep)
{

	Move = 0;
	Dir = 0;

	if (App.KeyPressed(Key::W) || App.KeyPressed(Key::Up))
		Move = 1;

	if (App.KeyPressed(Key::S) || App.KeyPressed(Key::Down))
		Move = -1;

	float moveStep = MoveSpeed * Move; // Player will move this far along the current direction vector

	if (App.KeyPressed(Key::D) || App.KeyPressed(Key::Right))
		Dir = 1;

	if (App.KeyPressed(Key::A) || App.KeyPressed(Key::Left))
		Dir = -1;

	// Add rotation if player is rotating (player.dir != 0)
	Rot += Dir * RotSpeed;

	// Calculate new player position with simple trigonometry
	float dX = cos(Rot) * moveStep;
	float dY = sin(Rot) * moveStep;

	// Set new position smoothly (slide along the only axis we are abled to move)
	if (!Map.isBlocked(posX + dX, posY)) // smoother 
		posX += dX;

	if (!Map.isBlocked(posX, posY + dY))
		posY += dY;

	// Calculate new player position with simple trigonometry	// It Hungs the move
	//float newX = posX + cos(Rot) * moveStep;
	//float newY = posY + sin(Rot) * moveStep;
	//if ( Map.isBlocked(newX, newY) )
	//	return;
	//posX = newX;
	//posY = newY;

	//speed modifiers
	MoveSpeed = timeStep * 5.0f; //the constant value is in squares/second
	RotSpeed = timeStep * 3.0f; //the constant value is in radians/second
}

void Entity::Render(World &map, PixelBuffer &buffer)
{
	// Draw Columns
	for (int column = 0, resolution = (SCREEN_W >> 1); column < resolution; column++)
	{
		float xCoord = column / resolution - 0.5f;
		float angle = atan2(xCoord, 0.8f);
		//Ray ray = map.cast(player, player.direction + angle, this.range);
		//this.drawColumn(column, ray, angle, map);
	}
}

void Entity::DrawColumn(World &map, PixelBuffer &buffer, Ray &ray, float angle, int column)
{
}

