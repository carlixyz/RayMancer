#include "AppManager.h"

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
	//window = new Display(title, width, height, Output::Windowed, Mode::FloatingPoint);
	//ptwrap_events = new EventHandler();
	window->listener(this);
	canvas.Resize(width, height);
	if (!window->open())
		return NULL;
	else
		return &canvas.Data[0].integer; // yikes
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
	window->update(this->canvas.Data);

	ForgetKeys();

}

void AppManager::UpdateWindow(PixelBuffer & pBuffer) // Take a source buffer & duplicates its size in windows
{

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
	ForgetKeys();
}

