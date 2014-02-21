#ifndef SDLENTITY_H
#define SDLENTITY_H

#include "Headers.h"

class SDLEntity
{
public:
	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;

	SDLEntity();
	~SDLEntity();

	void InitializeSDL();

private:


protected:
	SDL_Window* pWindow;
	SDL_Renderer* pRenderer;
	SDL_Surface* pSurface;

};



#endif