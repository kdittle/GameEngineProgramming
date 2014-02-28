// SDLOOPRewrite.cpp : Defines the entry point for the console application.

#include "Headers.h"
#include "ResourceLoader.h"

#ifdef main
# undef main
#endif


int _tmain(int argc, _TCHAR* argv[])
{
	ResourceLoader rl;

	rl.InitializeSDL();
	rl.LoadIMGFromFile("Color_image.png");

	bool exit = false;

	SDL_Event e;

	while(!exit)
	{

		SDL_PumpEvents();

		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case (SDL_QUIT) :
				exit = true;
				break;

			case(SDL_KEYDOWN) :
			{
								  if (e.key.keysym.sym == SDLK_ESCAPE)
								  {
									  exit = true;
									  break;
								  }

								  if (e.key.keysym.sym == SDLK_1 || e.key.keysym.sym == SDLK_KP_1)
								  {
									  SDL_Rect clip1;
									  clip1.x = 0;
									  clip1.y = 0;
									  clip1.w = 100;
									  clip1.h = 100;
									  rl.Render(rl.WINDOW_WIDTH / 2, rl.WINDOW_HEIGHT / 2, &clip1);
								  }

								  if (e.key.keysym.sym == SDLK_2 || e.key.keysym.sym == SDLK_KP_2)
								  {
									  SDL_Rect clip2;
									  clip2.x = 100;
									  clip2.y = 0;
									  clip2.w = 100;
									  clip2.h = 100;
									  rl.Render(rl.WINDOW_WIDTH / 2, rl.WINDOW_HEIGHT / 2, &clip2);
								  }

								  if (e.key.keysym.sym == SDLK_3 || e.key.keysym.sym == SDLK_KP_3)
								  {
									  SDL_Rect clip3;
									  clip3.x = 0;
									  clip3.y = 100;
									  clip3.w = 100;
									  clip3.h = 100;
									  rl.Render(rl.WINDOW_WIDTH / 2, rl.WINDOW_HEIGHT / 2, &clip3);
								  }

								  if (e.key.keysym.sym == SDLK_4 || e.key.keysym.sym == SDLK_KP_4)
								  {
									  SDL_Rect clip4;
									  clip4.x = 100;
									  clip4.y = 100;
									  clip4.w = 100;
									  clip4.h = 100;
									  rl.Render(rl.WINDOW_WIDTH / 2, rl.WINDOW_HEIGHT / 2, &clip4);
								  }

			}
			}
		}

		SDL_RenderPresent(rl.GetRenderer());
	}

	return 0;
}
