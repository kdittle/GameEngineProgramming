#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include "Headers.h"

static SDL_Window* pWindow;
static SDL_Renderer* pRenderer;
static SDL_Surface* pSurface;

class ResourceLoader
{
public:
	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;

	ResourceLoader();
	~ResourceLoader();

	void InitializeSDL();

	void Destory();
	void Free();

	bool LoadIMGFromFile(const std::string &filePath);
	void Render(int x, int y, SDL_Rect* clip = NULL, float angle = 0.0f, SDL_Point* center = NULL,
		SDL_RendererFlip = SDL_FLIP_NONE);

	int getHeight();
	int getWidth();

	int GetWindowWidth();
	int GetWindowHeight();
	SDL_Renderer* GetRenderer();

private:
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;

protected:
	
	
};

#endif