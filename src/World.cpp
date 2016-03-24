
#include "World.h"


bool World::isBlocked(int x, int y)
{
	if (y < 0 || y >= height || x < 0 || x >= width)
		return true;			//make sure that we cannot move outside level
	// Return true if the map block is not 0, (if there is a blocking wall)
	return (data[(int)(x + y*width)]);
}

Ray World::cast(Entity &player, float angle, float range)
{

	return Ray();
}