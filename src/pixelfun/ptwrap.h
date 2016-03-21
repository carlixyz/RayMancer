#ifndef PTWRAP_H
#define PTWRAP_H

#define DWORD unsigned int

#include "PixelToaster/PixelToaster.h"
using namespace PixelToaster;

bool KeyPressed(Key key);

bool KeyHit(Key key);

void ForgetKeys();

DWORD* OpenWindow(char* title, int width, int height);

bool WindowClosed();

void CloseWindow();

void UpdateWindow();

void SetPixel(int i, DWORD color);

#endif

