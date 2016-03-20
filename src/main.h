#define SCREEN_W 640
#define SCREEN_H 480
#define SIZE SCREEN_W*SCREEN_H

#define HALF_SCR_W SCREEN_W/2
#define HALF_SCR_H SCREEN_H/2

#include <cstdio>


struct Entity
{
	float posX;			// Current x, y position of the player
	float posY;			
	float Dir;			// The direction that the player is turning, either -1 for left or 1 for right
	float Rot;			// The current angle of rotation
	float Move;		// playing moving forward(speed = 1) or backwards (speed = -1).
	float MoveSpeed;	// How far (in map units) does the player move each step/update
	float RotSpeed;		// How much does the player rotate each step/update (in radians)

	void DrawMiniMap();
	//void UpdateInput(float tStep);

} Player = { 2, 2, 1, 1, 0, 0.18f,  0.105f};


struct World
{
	uint32_t textSize;
	uint32_t width;
	uint32_t height;
	uint32_t Area;
	uint32_t Scale;	// 32
	int data[100];
} Map = 
{ 64, 10, 10, 100, 32,
	{
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	}
};

struct Sprite // PixelBuffer
{
	uint32_t Width;
	uint32_t Height;
	uint32_t Area;
	std::vector<Pixel> Data;
	Sprite(){;}
	inline Sprite(uint32_t width, uint32_t height)
	{
		Width = width;
		Height = height;
		Area = Width * Height;
		Data.resize(Area);
	}
	inline void Resize(uint32_t width, uint32_t height)
	{
		Width = width;
		Height = height;
		Area = Width * Height;
		Data.resize(Area);
	}
};

Sprite pixels(SCREEN_W, SCREEN_H);
Sprite MiniMap(Map.width * Map.Scale, Map.height * Map.Scale);

namespace gfx
{
	// resize the data from a source to a destiny buffer
	void scale(Sprite &src, Sprite &dst)
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
	void scale(Sprite &src, Sprite &dst, uint32_t widthDst, uint32_t heightDst)
	{
		dst.Resize(widthDst, heightDst);
		scale(src, dst);
	}

	void rasterMap(struct World &src, Sprite &dst)
	{
		double scaleWidth = (double)dst.Width / (double)src.width;
		double scaleHeight = (double)dst.Height / (double)src.height;

		for (uint32_t cy = 0; cy < dst.Height; cy++)
		{
			for (uint32_t cx = 0; cx < dst.Width; cx++)
			{
				int pixel = (cy * (dst.Width)) + (cx);
				int nearestMatch = (((int)(cy / scaleHeight) * (src.width)) + ((int)(cx / scaleWidth)));

				dst.Data[pixel] = (src.data[nearestMatch] != 0 ? Pixel(1, 0, 1) : Pixel(0, 0, 1));
			}
		}
	}
	
	bool drawRect(const Pixel& color, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, Sprite &dst)
	{
		if (x1 < 0 || x1 > dst.Width - 1 || x2 < 0 || x2 > dst.Width - 1 || y1 < 0 || y1 > dst.Height - 1 || y2 < 0 || y2 > dst.Height - 1)
			return 0;

		int xEnd = (x1 > x2 ? x1 : x2);
		int yEnd = (y1 > y2 ? y1 : y2);

		int xStart = (x1 < x2 ? x1 : x2);
		int yStart = (y1 < y2 ? y1 : y2);

		for (int yIndex = yStart; yIndex < yEnd; yIndex++)
		for (int xIndex = xStart; xIndex < xEnd; xIndex++)
			dst.Data[xIndex + yIndex * dst.Width] = color;

		return 1;
	}

	//Bresenham line from (x1,y1) to (x2,y2) with rgb color
	bool drawLine(const Pixel& color, int x1, int y1, int x2, int y2, Sprite &Dst, int brush = 2)
	{
		if (x1 < 0 || x1 > Dst.Width - (1 + brush) || x2 < 0 || x2 > Dst.Width - (1 + brush) ||
			y1 < 0 || y1 > Dst.Height - (1 + brush) || y2 < 0 || y2 > Dst.Height - (1 + brush)) return 0;

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
			Dst.Data[x + (y * Dst.Width)] = Pixel(1, 1, 1);

			for (int i = 0; i < brush; i++)
			{
				Dst.Data[(x + i) + (y * Dst.Width)] = color;		// Brush Broad Trough x (Kinda)
				Dst.Data[x + ((y + i) * Dst.Width)] = color;		// Brush Broad Trough y
			}
			//Dst.Data[(x + 1) + (y * Dst.Width)] = color;
			//Dst.Data[x + ((y + 1) * Dst.Width)] = color;

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

}












////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////										OLD FOOs DEPRECATED												////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//void scaleMap(struct World &src, vector<Pixel> &dst, int wDst, int hDst)
//{
//	double scaleWidth = (double)wDst / (double)src.width;
//	double scaleHeight = (double)hDst / (double)src.height;
//	for (int cy = 0; cy < hDst; cy++)
//	{
//		for (int cx = 0; cx < wDst; cx++)
//		{
//			int pixel = (cy * (wDst)) + (cx);
//			int nearestMatch = (((int)(cy / scaleHeight) * (src.width)) + ((int)(cx / scaleWidth)));
//			dst[pixel] = (src.data[nearestMatch] != 0 ? Pixel(1, 0, 1) : Pixel(0, 0, 1));
//		}
//	}
//}


//void scale(vector<Pixel> &src, int wSrc, int hSrc, vector<Pixel> &dst, int wDst, int hDst)
//{
//	double scaleWidth = (double)wDst / (double)wSrc;
//	double scaleHeight = (double)hDst / (double)hSrc;
//
//	for (int cy = 0; cy < hDst; cy++)
//	{
//		for (int cx = 0; cx < wDst; cx++)
//		{
//			int pixel = (cy * (wDst)) + (cx);
//			int nearestMatch = (((int)(cy / scaleHeight) * (wSrc)) + ((int)(cx / scaleWidth)));
//
//			dst[pixel] = src[nearestMatch];
//		}
//	}
//}


//void scaleMap(struct World &src, Sprite &dst, uint32_t wDst, uint32_t hDst)
//{
//	dst.Resize(wDst, hDst);	// Life is beautifull when you can wrap buffers like this
//
//	double scaleWidth = (double)dst.Width / (double)src.width;
//	double scaleHeight = (double)dst.Height / (double)src.height;
//
//	for (uint32_t cy = 0; cy < dst.Height; cy++)
//	{
//		for (uint32_t cx = 0; cx < dst.Width; cx++)
//		{
//			int pixel = (cy * (dst.Width)) + (cx);
//			int nearestMatch = (((int)(cy / scaleHeight) * (src.width)) + ((int)(cx / scaleWidth)));
//
//			dst.Data[pixel] = (src.data[nearestMatch] != 0 ? Pixel(1, 0, 1) : Pixel(0, 0, 1));
//		}
//	}
//}