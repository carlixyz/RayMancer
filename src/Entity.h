#ifndef PLAYER_H
#define PLAYER_H

#include "AppManager.h"
#include "World.h"

float focalLength = 0.8f;

struct Entity
{
	float posX;			// Current x, y position of the player
	float posY;			// 
	//Vector Pos;
	float Rot;			// current angle of rotation

	float Dir;			// Input Enabler: The direction that the player is turning, -1 for left or 1 for right
	float Move;			// Input Enabler: playing moving forward(speed = 1) or backwards (speed = -1).

	float MoveSpeed;	// Move Multiplier: How far (in map units) does the player move each step/update
	float RotSpeed;		// Rot Multiplier: How much does the player rotate each step/update (in radians)

	void DrawInMap(PixelBuffer & Dst);
	void UpdateInput(AppManager & App, float tStep);
	void Render(World &map, PixelBuffer &buffer);
	void DrawColumn(World &map, PixelBuffer &buffer, Ray &ray, float angle, int column);


} Player = { 2, 2, 1, 0, 0, 0.18f, 0.105f };
//} Player = { {2,2,0}, 1, 0, 0, 0.18f, 0.105f };

#endif