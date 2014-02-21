// SDLOOPRewrite.cpp : Defines the entry point for the console application.

#include "Headers.h"
#include "SDLEntity.h"
#include "ResourceLoader.h"

#ifdef main
# undef main
#endif


int _tmain(int argc, _TCHAR* argv[])
{
	SDLEntity sdlEntity;

	sdlEntity.InitializeSDL();


	SDL_Delay(2000);

	return 0;
}
