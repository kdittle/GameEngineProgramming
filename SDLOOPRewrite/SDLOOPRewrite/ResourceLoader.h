#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include "Headers.h"
#include "SDLEntity.h"

class ResourceLoader : public SDLEntity
{
public:
	ResourceLoader();
	~ResourceLoader();

	void Destory();
	void Free();

	bool LoadIMGFromFile(const std::string &filePath);
	void Render(int x, int y, SDL_Rect* clip = NULL, float angle = 0.0f, SDL_Point* center = NULL,
		SDL_RendererFlip = SDL_FLIP_NONE);

	int getHeight();
	int getWidth();

private:
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;

protected:

	
};

#endif