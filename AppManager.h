#ifndef APP_MAN_H
#define APP_MAN_H

#include "PixelToaster\PixelToaster.h"
using namespace PixelToaster;

#define SCREEN_W 640
#define SCREEN_H 480
#define SIZE SCREEN_W*SCREEN_H

#define HALF_SCR_W SCREEN_W/2
#define HALF_SCR_H SCREEN_H/2

#define DWORD unsigned int

//#include "Singleton.h"
struct PixelBuffer // PixelBuffer
{
	std::vector<TrueColorPixel> Data;
	int Width;
	int Height;
	int Area;

	PixelBuffer(){ ; }
	inline PixelBuffer(int width, int height)
	{
		Width = width;
		Height = height;
		Area = Width * Height;
		Data.resize(Area);
	}
	inline void Resize(int width, int height)
	{
		Width = width;
		Height = height;
		Area = Width * Height;
		Data.resize(Area);
	}
};


class AppManager : public Listener //, public cSingleton<AppManager>
{
public:
	//friend class cSingleton<AppManager>;

	bool KeyPressed(Key key);

	bool KeyHit(Key key);

	void ForgetKeys();

	DWORD *OpenWindow(char* title, int width, int height);

	bool WindowClosed();

	void CloseWindow();

	void UpdateWindow();

	void SetPixel(int i, DWORD color);

	Display* window;
	PixelBuffer canvas;
	//EventHandler* ptwrap_events;

	AppManager()
	{
		// Protected constructor
		for (int i = 0; i < 256; i++)
		{
			keyPressed[i] = false;
			keyHit[i] = false;
		}
	}

protected:
	//AppManager()
	//{
	//	// Protected constructor
	//	for (int i = 0; i < 256; i++)
	//	{
	//		keyPressed[i] = false;
	//		keyHit[i] = false;
	//	}
	//}
	void onKeyDown(DisplayInterface& display, Key key)
	{
		int ikey = (int)key;
		// set state of pressed key
		keyPressed[ikey] = true;
		// mark as new key hit
		keyHit[ikey] = true;
		// note id of key for quick removal from keyhit list
		for (int i = 0; i<16; i++)
		if (newKeys[i] == -1)
		{
			newKeys[i] = ikey;
			break;
		}
	}
	void onKeyUp(DisplayInterface& display, Key key)
	{
		// clear state of released key
		keyPressed[(int)key] = false;
	}


private:
	bool keyPressed[256];
	bool keyHit[256];
	int newKeys[8];
};


#endif