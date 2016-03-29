#include "AppManager.h"
#include <assert.h> 

bool AppManager::KeyPressed(Key key)
{
	return keyPressed[(int)key];
}

bool AppManager::KeyHit(Key key)
{
	return keyHit[(int)key];
}

void AppManager::ForgetKeys()
{
	for (int i = 0; i<8; i++)
	if (newKeys[i] != -1)
	{
		keyHit[newKeys[i]] = false;
		newKeys[i] = -1;
	}
}

DWORD *AppManager::OpenWindow(char* title, int width, int height)
{
	window = new Display(title, width, height, Output::Windowed, Mode::TrueColor);
	//ptwrap_events = new EventHandler();
	window->listener(this);
	canvas.Resize(width, height);
	if (!window->open())
		return NULL;
	else
		return &canvas[0]; // return &canvas.Data[0].integer; // yikes
}

bool AppManager::WindowClosed()
{
	return !window->open();
}

void AppManager::CloseWindow()
{
	window->close();
}

void AppManager::UpdateWindow()
{
	ForgetKeys();

	window->update(this->canvas.Data);
}


void AppManager::UpdateWindow(PixelBuffer  & pBuffer)
{
	ForgetKeys();

	assert(pBuffer.Area < canvas.Area);

	//for (int x = 0; x < pBuffer.Width; x++)
	//for (int y = 0; y < pBuffer.Height; y++)
	//	canvas[y*canvas.Width + x] = pBuffer[pBuffer.Width * y + x]; //clear the buffer instead of cls()

	canvas.Data = pBuffer.Data;
	window->update(this->canvas.Data);
}


void AppManager::UpdateWindowX2(PixelBuffer & pBuffer) // Take a source buffer & duplicates its size in windows
{
	ForgetKeys();

	int HalfWidth = pBuffer.Width;	// These needs to be HALF_SIZE of Destiny Buffer always
	int HalfHeight = pBuffer.Height;
	//int HalfWidth = (canvas.Width>>1);
	//int HalfHeight = (canvas.Height>>1);

	DWORD * dp = &canvas.Data[0].integer;;
	DWORD * sp = &pBuffer.Data[0].integer;
	for (int y = 0; y < HalfHeight; y++)
	{
		for (int x = 0; x < HalfWidth; x++)
		{
			*dp++ = *sp;
			*dp++ = *sp++;
		}
		sp -= HalfWidth;
		for (int x = 0; x < HalfWidth; x++)
		{
			*dp++ = *sp;
			*dp++ = *sp++;
		}
	}

	window->update(canvas.Data);
}

