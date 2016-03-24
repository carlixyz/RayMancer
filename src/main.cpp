
#include <assert.h>
#include <cstdint>

//#include <string>
//#include <math.h>


#include "PixelToaster\PixelToaster.h"

using namespace std;
using namespace PixelToaster;


#include "main.h"


DWORD *MapRef;
DWORD *winRef;

AppManager MainApp;
AppManager MapView;

PixelBuffer backBuffer(SCREEN_W>>1, SCREEN_H>>1);	// Main Window PreBuffer

PixelBuffer mapBuffer(Map.width * Map.Scale, Map.height * Map.Scale);
PixelBuffer PreBuffer(Map.width * Map.Scale, Map.height * Map.Scale);

//#include	<iostream>

void main()
{
	//std::cout << backBuffer.Width << " " << backBuffer.Height << "\n";

	winRef = MainApp.OpenWindow("RayMancer", SCREEN_W, SCREEN_H);
	MapRef = MapView.OpenWindow("Top View Mini-MAP", (Map.width * Map.Scale*2), (Map.height * Map.Scale*2));

	assert(winRef != NULL && MapRef != NULL);
	gfx::rasterMap(Map, mapBuffer);

	while (true)
	{
		if (MainApp.KeyPressed(Key::Q) || MainApp.WindowClosed() || MapView.KeyPressed(Key::Q))
			break;

		PreBuffer = mapBuffer;
		Player.DrawInMap(PreBuffer);
		Player.UpdateInput(MapView.time.delta());	// MAP GRID & PLAYER DRAWING




	


		MainApp.UpdateWindowX2(backBuffer);
		MapView.UpdateWindowX2(PreBuffer);
	}


	//HKFree();
	MainApp.CloseWindow();
	MapView.CloseWindow();

}

#include	<iostream>

void Entity::DrawInMap(PixelBuffer & Dst)
{
	//Pixel ZoneColor(0, .75f, .75f);
	DWORD ZoneColor = 0x000088FF;
	DWORD DirColor = 0x00229900;
	float Pzone			= .2f;		// A little offset 2 draw a small area around Player
	float FrontLength		= 2;		// A little offset 2 draw a small area around Player
	float FrontX = (Player.posX + (cos(Player.Rot) * FrontLength)) * Map.Scale;
	float FrontY = (Player.posY + (sin(Player.Rot) * FrontLength)) * Map.Scale;
	
	//std::cout << " RADIANS: " << Player.Rot << " - DEGREES: " << DEG(Player.Rot) << "\n";

	float LeftViewX = (Player.posX + (cos(Player.Rot + RAD(-30 ) )) * FrontLength *2) * Map.Scale;
	float LeftViewY = (Player.posY + (sin(Player.Rot + RAD(-30 ) )) * FrontLength * 2) * Map.Scale;
	float RightViewX = (Player.posX + (cos(Player.Rot + RAD(+30) )) * FrontLength * 2) * Map.Scale;
	float RightViewY = (Player.posY + (sin(Player.Rot + RAD(+30) )) * FrontLength * 2) * Map.Scale;
	gfx::drawLine(ZoneColor, Player.posX * Map.Scale, Player.posY * Map.Scale, LeftViewX, LeftViewY, Dst, 1);
	gfx::drawLine(ZoneColor, Player.posX * Map.Scale, Player.posY * Map.Scale, RightViewX, RightViewY, Dst, 1);


	gfx::drawRect( ZoneColor, (Player.posX - Pzone) * Map.Scale, (Player.posY - Pzone) * Map.Scale, (Player.posX + Pzone) * Map.Scale, (Player.posY + Pzone) * Map.Scale, Dst);
	gfx::drawLine(DirColor, Player.posX * Map.Scale, Player.posY * Map.Scale, FrontX, FrontY, Dst, 2);

}

void Entity::UpdateInput(float timeStep)
{

	Move = 0;
	Dir = 0;

	if (MapView.KeyPressed(Key::W) || MapView.KeyPressed(Key::Up))
		Move = 1;

	if (MapView.KeyPressed(Key::S) || MapView.KeyPressed(Key::Down))
		Move = -1;

	float moveStep = MoveSpeed * Move; // Player will move this far along the current direction vector

	if (MapView.KeyPressed(Key::D) || MapView.KeyPressed(Key::Right))
		Dir = 1;

	if (MapView.KeyPressed(Key::A) || MapView.KeyPressed(Key::Left))
		Dir = -1;

	// Add rotation if player is rotating (player.dir != 0)
	Rot += Dir * RotSpeed;

	// Calculate new player position with simple trigonometry
	float dX = cos(Rot) * moveStep;
	float dY = sin(Rot) * moveStep;

	// Set new position smoothly (slide along the only axis we are abled to move)
	if ( !Map.isBlocked(posX + dX, posY)) // smoother 
		posX += dX;

	if ( !Map.isBlocked(posX, posY + dY ))
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

 

