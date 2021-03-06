
#include "AppManager.h"
#include "Vector.h"

#include <math.h>
#include <assert.h> 

#define PI 3.14159265f
#define PI2 3.14159265f*2
float radians = PI / 180;
float degrees = 180 / PI;
#define RAD(ang) ang*radians
#define DEG(ang) ang*degrees


struct Ray
{
	//int stripWidth = 2;

};

struct Entity
{
	float posX =3;			// Current x, y position of the player
	float posY =2;			// 
	//Vector Pos;
	float Rot =1;			// current angle of rotation

	float Dir =0;			// Input Enabler: The direction that the player is turning, -1 for left or 1 for right
	float Move =0;			// Input Enabler: playing moving forward(speed = 1) or backwards (speed = -1).

	float MoveSpeed =0.18f;	// Move Multiplier: How far (in map units) does the player move each step/update
	float RotSpeed =0.1f;		// Rot Multiplier: How much does the player rotate each step/update (in radians)

	//float RadCollision = 0.15f;	// it's a reference to the Player Raidus for better collision detection
	int HorizontLine = 0;

	int stripWidth = 2;		// Def: 2
	int numRays = ceil(HALF_SCR_W / stripWidth);
	float FOV = RAD(60);
	float FOVHalf = FOV / 2;
	float viewDist = (HALF_SCR_W / 2) / tan((FOV / 2));	
	// Remember I'm using the half of the half width size because I'm working with 320/2 (and then rendering twice resolution for "artsy" impact)

	void DrawInMap(PixelBuffer & Dst);
	void UpdateInput(float tStep);
	void CastRays();
	void CastSingleRay(float rayAngle, int stripIdx);

} Player;


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
	inline bool isBlocked(int x, int y)	// Just input single values without width!
	{
		if (y < 0 || y >= height || x < 0 || x >= width)
			return true;			//make sure that we cannot move outside level
									// Return true if the map block is not 0, (if there is a blocking wall)
		return (data[(int)(x+y*width)] );
	}

	inline void rasterMap( PixelBuffer &dst)
	{
		double scaleWidth = (double)dst.Width / (double)width;
		double scaleHeight = (double)dst.Height / (double)height;

		for (uint32_t cy = 0; cy < dst.Height; cy++)
			for (uint32_t cx = 0; cx < dst.Width; cx++)
			{
				int pixel = (cy * (dst.Width)) + (cx);
				int nearestMatch = (((int)(cy / scaleHeight) * (width)) + ((int)(cx / scaleWidth)));

				//dst[pixel] = (src.data[nearestMatch] != 0 ? 0x00FF00FF : 0x0000FF00);
				dst[pixel] = (data[nearestMatch] != 0 ? 0x008800FF : (900 * !(cx % Scale && cy % Scale)));

				//dst[pixel] -= ~120 * (cx % Scale && cy % Scale);	// 16
			}
	}

	Ray cast(Entity &player, float angle, float range);

} Map = 
{ 64, 10, 10, 100, 16,
	{
	0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 2, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 2, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 2, 1, 0, 1, 1, 1, 1
	}
};






namespace gfx
{
	int clamp(int n, int lower, int upper)
	{
		//return std::max(lower, std::min(n, upper));
		return n + ((n < lower) * (lower - n)) + ((n > upper) * (upper - n));
		//return n <= lower ? lower : n >= upper ? upper : n;
	}

	// resize the data from a source to a destiny buffer
	void scale(PixelBuffer &src, PixelBuffer &dst)
	{
		double scaleWidth = (double)dst.Width / (double)src.Width;
		double scaleHeight = (double)dst.Height / (double)src.Height;

		for (uint32_t cy = 0; cy < dst.Height; cy++)
		{
			for (uint32_t cx = 0; cx < dst.Width; cx++)
			{
				int pixel = (cy * (dst.Width)) + (cx);
				int nearestMatch = (((int)(cy / scaleHeight) * (src.Width)) + ((int)(cx / scaleWidth)));
				dst.Data[pixel] = src.Data[nearestMatch];
			}
		}
	}

	// resize the data from a source to a destiny buffer (With a custom size)
	void scale(PixelBuffer &src, PixelBuffer &dst, uint32_t widthDst, uint32_t heightDst)
	{
		dst.Resize(widthDst, heightDst);
		scale(src, dst);
	}

	// Add info from one source Buffer onto antoher destiny inside coords input
	void add(PixelBuffer &src, PixelBuffer &dst, uint32_t xCoord = 0, uint32_t yCoord = 0)
	{
		assert(src.Area < dst.Area); // && xCoord + src.Width < dst.Width  
		//							&& ((yCoord *dst.Width) + src.Height < dst.Area) );
		xCoord = clamp(xCoord, 0, dst.Width - src.Width);
		yCoord = clamp(yCoord, 0, dst.Height - src.Height);

		for (int x = 0; x < src.Width; x++)
		for (int y = 0; y < src.Height; y++)
			dst[(y+ yCoord)*dst.Width + (x + xCoord)] |= src[src.Width * y + x]; //Add
	}

	bool drawRect(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, PixelBuffer &dst, const DWORD& color = 0x00888888)
	{

		if (x1 < 0 || x1 > dst.Width - 1 || x2 < 0 || x2 > dst.Width - 1 || y1 < 0 || y1 > dst.Height - 1 || y2 < 0 || y2 > dst.Height - 1)
			return 0;

		int xEnd = (x1 > x2 ? x1 : x2);
		int yEnd = (y1 > y2 ? y1 : y2);

		int xStart = (x1 < x2 ? x1 : x2);
		int yStart = (y1 < y2 ? y1 : y2);

		for (int yIndex = yStart; yIndex < yEnd; yIndex++)
		for (int xIndex = xStart; xIndex < xEnd; xIndex++)
			dst.Data[xIndex + yIndex * dst.Width].integer += color;
			//dst.Data[xIndex + yIndex * dst.Width].integer = 0x0000FFFF;

		return 1;
	}

		//Bresenham line from (x1,y1) to (x2,y2) with rgb color
	bool drawLine(int x1, int y1, int x2, int y2, PixelBuffer &Dst, const DWORD& color = 0x00888888, int brush = 2)
	{
		x1 = clamp(x1, 0, Dst.Width - (brush));
		y1 = clamp(y1, 0, Dst.Height - (brush));
		x2 = clamp(x2, 0, Dst.Width - (brush));
		y2 = clamp(y2, 0, Dst.Height - (brush));

		//if (x1 < 0 || x1 > Dst.Width - (1 + brush) || x2 < 0 || x2 > Dst.Width - (1 + brush) ||
		//	y1 < 0 || y1 > Dst.Height - (1 + brush) || y2 < 0 || y2 > Dst.Height - (1 + brush)) return 0;


		int deltax = std::abs(x2 - x1);									//The difference between the x's
		int deltay = std::abs(y2 - y1);									//The difference between the y's
		int x = x1;														//Start x off at the first pixel
		int y = y1;														//Start y off at the first pixel
		int xinc1, xinc2, yinc1, yinc2, den, num, numadd, numpixels, curpixel;

		if (x2 >= x1)													//The x-values are increasing
		{
			xinc1 = 1;
			xinc2 = 1;
		}
		else															//The x-values are decreasing
		{
			xinc1 = -1;
			xinc2 = -1;
		}
		if (y2 >= y1)													//The y-values are increasing
		{
			yinc1 = 1;
			yinc2 = 1;
		}
		else															//The y-values are decreasing
		{
			yinc1 = -1;
			yinc2 = -1;
		}
		if (deltax >= deltay)						//There is at least one x-value for every y-value
		{
			xinc1 = 0;								//Don't change the x when numerator >= denominator
			yinc2 = 0;								//Don't change the y for every iteration
			den = deltax;
			num = deltax >> 2;
			numadd = deltay;
			numpixels = deltax;						//There are more x-values than y-values
		}
		else										//There is at least one y-value for every x-value
		{
			xinc2 = 0;								//Don't change the x for every iteration
			yinc1 = 0;								//Don't change the y when numerator >= denominator
			den = deltay;
			num = deltay >> 2;
			numadd = deltax;
			numpixels = deltay;						//There are more y-values than x-values
		}
		for (curpixel = 0; curpixel <= numpixels; curpixel++)
		{
			//pset(x % Dst.Width, y % Dst.Height, color);				//Draw the current pixel
			//Dst.Data[x + (y * Dst.Width)] = Pixel(1, 1, 1);
			Dst.Data[x + (y * Dst.Width)].integer += 0x00FF0000;

			for (int i = 0; i < brush; i++)
			{
				Dst.Data[(x + i) + (y * Dst.Width)].integer += color;		// Brush Broad Trough x (Kinda)
				Dst.Data[x + ((y + i) * Dst.Width)].integer += color;		// Brush Broad Trough y
			}
	

			num += numadd;							//Increase the numerator by the top of the fraction
			if (num >= den)							//Check if numerator >= denominator
			{
				num -= den;							//Calculate the new numerator value
				x += xinc1;							//Change the x as appropriate
				y += yinc1;							//Change the y as appropriate
			}
			x += xinc2;								//Change the x as appropriate
			y += yinc2;								//Change the y as appropriate
		}

		return 1;
	}

	//Fast vertical line from (x,y1) to (x,y2), with rgb color
	bool verLine(int sx, int sy1, int sy2, PixelBuffer &Dst, const DWORD& color = 0x00888888)
	{
		if (sy2 < sy1)
			{ sy1 += sy2; sy2 = sy1 - sy2; sy1 -= sy2; } //swap y1 and y2
		if (sy2 < 0 || sy1 >= Dst.Height || sx < 0 || sx >= Dst.Width)
			return 0; //no single point of the line is on screen

		//if(sy1 < 0)  sy1 = 0;					//clip top
		//if(sy2 >= WIDTH) sy2 = HEIGHT - 1;	//clip bottom

		for (int y = sy1; y < sy2; y++)
		{
			if (y < 0) continue;
			if (y >= Dst.Height) break;

			if (sx >= 0 && sx <= Dst.Width)
				Dst.Data[y*Dst.Width + sx].integer = color;
		}

		return 1;
	}

	/*void rotate_texture(int id)
	{
		DWORD temp[64 * 64];
		for (int y = 0; y<64; y++)
		for (int x = 0; x<64; x++)
			temp[x * 64 + y] = textures[id].data[y * 64 + x];
		for (int i = 0; i<64 * 64; i++)
			textures[id].data[i] = temp[i];
	}*/


	//void Render();
	

}




