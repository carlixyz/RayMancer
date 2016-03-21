
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

PixelBuffer backBuffer(SCREEN_W, SCREEN_H);	// Main Window PreBuffer

PixelBuffer mapBuffer(Map.width * Map.Scale, Map.height * Map.Scale);
PixelBuffer PreBuffer(Map.width * Map.Scale, Map.height * Map.Scale);

void main()
{
	winRef = MainApp.OpenWindow("RayMancer", SCREEN_W, SCREEN_H);
	MapRef = MapView.OpenWindow("Top View Mini-MAP", (Map.width * Map.Scale*2), (Map.height * Map.Scale*2));

	assert(winRef != NULL && MapRef != NULL);
	gfx::rasterMap(Map, mapBuffer);

	while (true)
	{
		if (MainApp.KeyPressed(Key::Q) || MainApp.WindowClosed() || MapView.KeyPressed(Key::Q))
			break;

		PreBuffer = mapBuffer;

		//Player.Draw1UP(MapView.canvas);
		Player.Draw1UP(PreBuffer);
		Player.UpdateInput(MapView.time.delta());

		MainApp.ForgetKeys();
		MapView.ForgetKeys();
	
		MainApp.UpdateWindow();
		//MapView.UpdateWindow();

		MapView.UpdateWindow(PreBuffer);
	}


	//HKFree();
	MainApp.CloseWindow();
	MapView.CloseWindow();

}

#include <math.h>

void Entity::Draw1UP(PixelBuffer & Dst)
{
	//Pixel ZoneColor(0, .75f, .75f);
	DWORD ZoneColor = 0x008888FF;
	DWORD DirColor = 0x00007777;
	float Pzone			= .2f;		// A little offset 2 draw a small area around Player
	float FrontLength		= 2;		// A little offset 2 draw a small area around Player
	float FrontX = (Player.posX + (cos(Player.Rot) * FrontLength)) * Map.Scale;
	float FrontY = (Player.posY + (sin(Player.Rot) * FrontLength)) * Map.Scale;

	gfx::drawRect( ZoneColor, (Player.posX - Pzone) * Map.Scale, (Player.posY - Pzone) * Map.Scale, (Player.posX + Pzone) * Map.Scale, (Player.posY + Pzone) * Map.Scale, Dst);
	gfx::drawLine(DirColor, Player.posX * Map.Scale, Player.posY * Map.Scale, FrontX, FrontY, Dst, 2);

}

void Entity::UpdateInput(float timeStep)
{

	Move = 0;
	Dir = 0;

	if (MapView.KeyPressed(Key::W))
		Move = 1;

	if (MapView.KeyPressed(Key::S))
		Move = -1;

	// Player will move this far along
	// the current direction vector
	float moveStep = MoveSpeed * Move;

	if (MapView.KeyPressed(Key::D))
		Dir = 1;

	if (MapView.KeyPressed(Key::A))
		Dir = -1;

	// Add rotation if player is rotating (player.dir != 0)
	Rot += Dir * RotSpeed;

	// Calculate new player position with simple trigonometry
	float newX = posX + cos(Rot) * moveStep;
	float newY = posY + sin(Rot) * moveStep;

	if ( Map.isBlocked(newX, newY) )
		return;

	// Set new position
	posX = newX;
	posY = newY;

	//speed modifiers
	MoveSpeed = timeStep * 5.0f; //the constant value is in squares/second
	RotSpeed = timeStep * 3.0f; //the constant value is in radians/second
}

 

