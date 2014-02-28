#include "ResourceLoader.h"

ResourceLoader::ResourceLoader()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

ResourceLoader::~ResourceLoader()
{
	Free();
	SDL_DestroyTexture(mTexture);
	mWidth = 0;
	mHeight = 0;
}

void ResourceLoader::InitializeSDL()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	pWindow = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

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

SDL_Renderer* ResourceLoader::GetRenderer()
{
	return pRenderer;
}

void ResourceLoader::Destory()
{
	SDL_DestroyTexture(mTexture);
}

void ResourceLoader::Free()
{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mWidth = 0;
		mHeight = 0;
	}
}

bool ResourceLoader::LoadIMGFromFile(const std::string &filePath)
{
	Free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());

	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", filePath.c_str(), SDL_GetError());
	}
	else
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));

		newTexture = SDL_CreateTextureFromSurface(pRenderer, loadedSurface);

		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL_Error %s\n", filePath.c_str(), SDL_GetError());
		}
		else
		{
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		SDL_FreeSurface(loadedSurface);
	}

	mTexture = newTexture;

	return mTexture != NULL;
}

void ResourceLoader::Render(int x, int y, SDL_Rect* clip, float angle, SDL_Point* center,
	SDL_RendererFlip flip)
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx(pRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int ResourceLoader::getHeight()
{
	return mHeight;
}

int ResourceLoader::getWidth()
{
	return mWidth;
}

int ResourceLoader::GetWindowWidth()
{
	return WINDOW_WIDTH;
}

int ResourceLoader::GetWindowHeight()
{
	return WINDOW_HEIGHT;
}