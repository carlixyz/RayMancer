#include "ptwrap.h"

bool ptwrap_keypressed[256];
bool ptwrap_keyhit[256];
int ptwrap_newkeys[8];

bool KeyPressed(Key key)
{
	return ptwrap_keypressed[(int)key];
}

bool KeyHit(Key key)
{
	return ptwrap_keyhit[(int)key];
}

void ForgetKeys()
{
	for(int i=0;i<8;i++)
		if(ptwrap_newkeys[i]!=-1)
		{
			ptwrap_keyhit[ptwrap_newkeys[i]]=false;
			ptwrap_newkeys[i]=-1;
		}
}

class EventHandler : public Listener
{
public:
	EventHandler()
	{
		for(int i=0;i<256;i++)
		{
			ptwrap_keypressed[i]=false;
			ptwrap_keyhit[i]=false;
		}
	}
	~EventHandler()
	{
	}
protected:
	void onKeyDown(DisplayInterface& display, Key key)
	{
		int ikey=(int)key;
		// set state of pressed key
		ptwrap_keypressed[ikey]=true;
		// mark as new key hit
		ptwrap_keyhit[ikey]=true;
		// note id of key for quick removal from keyhit list
		for(int i=0;i<16;i++)
			if(ptwrap_newkeys[i]==-1)
			{
				ptwrap_newkeys[i]=ikey;
				break;
			}
	}
	void onKeyUp(DisplayInterface& display, Key key)
	{
		// clear state of released key
		ptwrap_keypressed[(int)key]=false;
	}
};

Display* ptwrap_display;
EventHandler* ptwrap_events;
vector<TrueColorPixel> ptwrap_screen;

DWORD* OpenWindow(char* title, int width, int height)
{
	ptwrap_display=new Display(title, width, height, Output::Default, Mode::TrueColor);
	ptwrap_events=new EventHandler();
	ptwrap_display->listener(ptwrap_events);
	ptwrap_screen.resize(width*height);
	if(!ptwrap_display->open())
		return NULL;
	else
		return &ptwrap_screen[0].integer; // yikes
}

bool WindowClosed()
{
	return !ptwrap_display->open();
}

void CloseWindow()
{
	ptwrap_display->close();
}

void UpdateWindow()
{
	ptwrap_display->update(ptwrap_screen);
}

void SetPixel(int i, DWORD color)
{
	ptwrap_screen[i].integer=color;
}

