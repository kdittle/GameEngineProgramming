#include "SDLEntity.h"

SDLEntity::SDLEntity()
{
	pWindow = NULL;
	pRenderer = NULL;
	pSurface = NULL;
}

SDLEntity::~SDLEntity()
{
	SDL_FreeSurface(pSurface);
	SDL_DestroyRenderer(pRenderer);
	SDL_DestroyWindow(pWindow);
}

void SDLEntity::InitializeSDL()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	pWindow = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

	if (pWindow == NULL)
	{
		std::printf("Window is Null %s! SDL_Window Error: %s\n", SDL_GetError());
	}

	pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);

	if (pRenderer == NULL)
	{
		std::printf("Renderer is Null %s! SDL_Renderer Error: %s\n", SDL_GetError());
	}

	pSurface = SDL_GetWindowSurface(pWindow);

	if (pSurface == NULL)
	{
		std::printf("Surface is Null %s! SDL_Surface Error: %s\n", SDL_GetError());
	}
}