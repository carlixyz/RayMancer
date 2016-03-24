
#ifndef WORLD_H
#define WORLD_H

#include <cstdint>


struct Entity;	// foward decl

struct Ray
{
	Ray(){ ; }
	float pos;

};

struct World
{
	uint32_t textSize;
	uint32_t width;
	uint32_t height;
	uint32_t Area;
	uint32_t Scale;	// 32
	int data[100];

	//inline int Get(int x, int y)
	//{  return (x < 0 ? -1 : ((int)(x + y*width) >= Area ? -1 : data[(int)(x + y*width)]));  }
	bool isBlocked(int x, int y);
	Ray cast(Entity &player, float angle, float range);

} Map =
{ 64, 10, 10, 100, 32,
{
	0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 0, 1, 1, 1, 1
}
};


#endif