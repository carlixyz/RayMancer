
#include <assert.h>
#include <cstdint>
#include "PixelToaster\PixelToaster.h"
#include "main.h"

//#include <string>
//#include <math.h>



using namespace std;
using namespace PixelToaster;



DWORD *MapRef;
DWORD *WinRef;

AppManager MainApp;

PixelBuffer MapBuffer(Map.width * Map.Scale, Map.height * Map.Scale);	// Map Buffer
PixelBuffer backBuffer(SCREEN_W >> 1, SCREEN_H >> 1);						// Main Window PreBuffer
//PixelBuffer screen(SCREEN_W >> 1, SCREEN_H >> 1);						// Main Window PreBuffer


#include	<iostream>
bool ShowMap = false;
bool EyeFish = false;

void main()
{
	//std::cout << backBuffer.Width << " " << backBuffer.Height << "\n";

	WinRef = MainApp.OpenWindow("RayMancer", SCREEN_W, SCREEN_H);

	assert(WinRef != NULL );
	//Map.rasterMap( MapBuffer);

	while (true)
	{
		backBuffer.Cls();

		Player.UpdateInput(MainApp.time.delta());	// PLAYER POS & DIR UPDATE

		if (MainApp.KeyPressed(Key::Q) || MainApp.WindowClosed() )
			break;
		if (MainApp.KeyHit(Key::Tab))
			ShowMap = !ShowMap;
		if (MainApp.KeyHit(Key::E))
			EyeFish = !EyeFish;

		Map.rasterMap(MapBuffer);					// BUILD MAP & DRAW PLAYER ORIENTATION
		Player.DrawInMap(MapBuffer);
		Player.CastRays();

		if (ShowMap)
			gfx::add(MapBuffer, backBuffer, 220, 140);	// ADD MINI MAP TO DOWN-RIGHT CORNER


		MainApp.UpdateWindowX2(backBuffer);
	}


	//HKFree();
	MainApp.CloseWindow();

}

void Entity::CastRays()
{
	for (int stripIdx = 0, i = 0; i < numRays; i++)
	{
		//Where on map does this ray go?
		float rayScreenPos = (-numRays / 2 + i) * stripWidth;

		// the distance from viewer to the point of the screen, using Pythagoras.
		float rayViewDist = sqrtf(rayScreenPos*rayScreenPos + viewDist * viewDist);

		// The angle of Ray, relative to the Viewing Direction, Right Triangle: a= sin(A) * c
		float rayAngle = asinf(rayScreenPos / rayViewDist);

		CastSingleRay(Rot + rayAngle, stripIdx++);

	}
}

void Entity::CastSingleRay(float rayAngle, int stripIdx)
{
	rayAngle = fmodf(rayAngle, PI2 );		// Clamp all values always between:  0 ~ twoPI
	rayAngle += (PI2 * (rayAngle < 0));		// if (rayAngle < 0) rayAngle += twoPI; 
	

	//if (stripIdx != 80)
	//	return;
	//cout << "Ray Angle: " << rayAngle << endl;

	// ismoving right/left? up/down? >> Check in which quadrant the angle is aiming
	bool right = (rayAngle > PI2 * 0.75f || rayAngle < PI2 * 0.25f);
	bool up = (rayAngle < 0 || rayAngle > PI);
	int wallType = 0;

	// only do these once
	float angleSin = sin(rayAngle);
	float angleCos = cos(rayAngle);

	float dist = 0;	// the distance to the block we hit
	float xHit = 0; 	// the x and y coord of where the ray hit the block
	float yHit = 0;

	DWORD color = 0x00FF3333;
	float textureX;	// the x-coord on the texture of the block, ie. what part of the texture are we going to render
	//float wallX;	// the (x,y) map coords of the block
	//float wallY;

	bool wallIsHorizontal = false;

	// first check against the vertical map/wall lines
	// we do this by moving to the right or left edge of the block we're standing in
	// and then moving in 1 map unit steps horizontally. The amount we have to move vertically
	// is determined by the slope of the ray, which is simply defined as sin(angle) / cos(angle).

	float slope = angleSin / angleCos;							// the slope of the straight line made by the ray
	float dxVer = right ? 1 : -1;								// we move either 1 map unit to the left or right
	float dyVer = dxVer * slope;								// how much to move up or down

	float x = right ? ceil(Player.posX) : floor(Player.posX);	// starting horizontal position, at one of the edges of the current map block
	float y = Player.posY + (x - Player.posX) * slope;			// starting vertical position. We add the small horizontal step we just made, multiplied by the slope.

	while (x >= 0 && x < Map.width && y >= 0 && y < Map.height) 
	{
		int wallX = floor(x + (right ? 0 : -1));
		int wallY = floor(y);

		// is this point inside a wall block?
		if (Map.data[wallY * Map.width + wallX] )
		{
			float distX = x - Player.posX;
			float distY = y - Player.posY;
			dist = distX*distX + distY*distY;	// the distance from the player to this point, squared.

			wallType = Map.data[wallY * Map.width + wallX]; // we'll remember the type of wall we hit for later
			textureX = fmodf(y, 1);	// where exactly are we on the wall? textureX is the x coordinate on the texture that we'll use later when texturing the wall.
			if (!right) textureX = 1 - textureX; // if we're looking to the left side of the map, the texture should be reversed

			xHit = x;	// save the coordinates of the hit. We only really use these to draw the rays on minimap.
			yHit = y;

			wallIsHorizontal = true;

			break;
		}
		x += dxVer;
		y += dyVer;
	}

	// now check against horizontal lines. It's basically the same, just "turned around".
	// the only difference here is that once we hit a map block, 
	// we check if there we also found one in the earlier, vertical run. We'll know that if dist != 0.
	// If so, we only register this hit if this distance is smaller.

	slope = angleCos / angleSin;
	float dYHor = up ? -1 : 1;
	float dXHor = dYHor * slope;
	y = up ? floor(Player.posY) : ceil(Player.posY);
	x = Player.posX + (y - Player.posY) * slope;

	int side = 0;


	while (x >= 0 && x < Map.width && y >= 0 && y < Map.height)
	{
		int wallY = floor(y + (up ? -1 : 0));
		int wallX = floor(x);
		if (Map.data[wallY * Map.width + wallX] > 0)
		{
			float distX = x - Player.posX;
			float distY = y - Player.posY;
			float blockDist = distX*distX + distY*distY;
			if (!dist || blockDist < dist) 
			{
				dist = blockDist;
				xHit = x;
				yHit = y;
				side = 1;

				wallType = Map.data[wallY * Map.width + wallX];
				textureX = fmodf(x, 1);
				if (up) textureX = 1 - textureX;
			}
			break;
		}
		x += dXHor;
		y += dYHor;
	}

	if (dist)
	{
		if (ShowMap)
			gfx::drawLine(Player.posX *Map.Scale, Player.posY * Map.Scale,	xHit * Map.Scale, yHit * Map.Scale, MapBuffer, 0x0033BB66, 1);

		//int strip = screenStrips

		dist = sqrt(dist);

		// use perpendicular distance to adjust for fish eye
		// distorted_dist = correct_dist / cos(relative_angle_of_ray)		// FISHEYE
		if (!EyeFish)
			dist = dist * cos(Player.Rot - rayAngle);

		// now calc the position, height and width of the wall strip

		// "real" wall height in the game world is 1 unit, the distance from the player to the screen is viewDist,
		// thus the height on the screen is equal to wall_height_real * viewDist / dist
		int height = (int)(viewDist / dist);	//(Column)

		// width is the same, but we have to stretch the texture to a factor of stripWidth to make it fill the strip correctly
		float width = height * stripWidth;

		// top placement is easy since everything is centered on the x-axis, so we simply move
		// it half way down the screen and then half the wall height back up.
		// screenHeight = 200
		//float top = round((HALF_SCR_H - height) / 2);


		//calculate lowest and highest pixel to fill in current stripe
		int drawStart = -height / 2 + HALF_SCR_H / 2;
		if (drawStart < 0)
			drawStart = 0;
		int drawEnd = height / 2 + HALF_SCR_H / 2;
		if (drawEnd >= HALF_SCR_H)
			drawEnd = HALF_SCR_H - 1;

		//gfx::verLine(stripIdx, drawStart, drawEnd, backBuffer, 0x00FF3333);
	 
		//give x and y sides different brightness
		if (side == 1) { color <<= 1; }

		gfx::verLine((stripIdx*stripWidth) - 1, drawStart, drawEnd, backBuffer, color);
		gfx::verLine(stripIdx*stripWidth, drawStart, drawEnd, backBuffer, color);
	}

}

void Entity::DrawInMap(PixelBuffer & Dst)
{
	//Pixel ZoneColor(0, .75f, .75f);
	DWORD ZoneColor = 0x000088FF;
	DWORD DirColor = 0x0033BB66;
	float Pzone			= .2f;		// A little offset 2 draw a small area around Player
	float FrontLength		= 2;		// The offset 2 draw the foward Vector  in front of Player view
	float FrontX = (Player.posX + (cos(Player.Rot) * FrontLength)) * Map.Scale;
	float FrontY = (Player.posY + (sin(Player.Rot) * FrontLength)) * Map.Scale;
	
	//std::cout << " RADIANS: " << Player.Rot << " - DEGREES: " << DEG(Player.Rot) << "\n";

	//float LeftViewX = (Player.posX + (cos(Player.Rot + RAD(-30 ) )) * FrontLength *2) * Map.Scale;
	//float LeftViewY = (Player.posY + (sin(Player.Rot + RAD(-30 ) )) * FrontLength * 2) * Map.Scale;
	//float RightViewX = (Player.posX + (cos(Player.Rot + RAD(+30) )) * FrontLength * 2) * Map.Scale;
	//float RightViewY = (Player.posY + (sin(Player.Rot + RAD(+30) )) * FrontLength * 2) * Map.Scale;
	//gfx::drawLine(Player.posX * Map.Scale, Player.posY * Map.Scale, LeftViewX, LeftViewY, Dst, DirColor,  1);
	//gfx::drawLine(Player.posX * Map.Scale, Player.posY * Map.Scale, RightViewX, RightViewY, Dst, DirColor, 1);


	gfx::drawRect((Player.posX - Pzone) * Map.Scale, (Player.posY - Pzone) * Map.Scale, (Player.posX + Pzone) * Map.Scale, (Player.posY + Pzone) * Map.Scale, Dst,  ZoneColor);
	gfx::drawLine(Player.posX * Map.Scale, Player.posY * Map.Scale, FrontX, FrontY, Dst, DirColor, 2);

}

void Entity::UpdateInput(float timeStep)
{

	Move = 0;
	Dir = 0;

	if (MainApp.KeyPressed(Key::W) || MainApp.KeyPressed(Key::Up))
		Move = 1;

	if (MainApp.KeyPressed(Key::S) || MainApp.KeyPressed(Key::Down))
		Move = -1;

	float moveStep = MoveSpeed * Move; // Player will move this far along the current direction vector

	if (MainApp.KeyPressed(Key::D) || MainApp.KeyPressed(Key::Right))
		Dir = 1;

	if (MainApp.KeyPressed(Key::A) || MainApp.KeyPressed(Key::Left))
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

 

