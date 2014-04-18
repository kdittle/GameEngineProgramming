#include "stdafx.h"
#include "SDL2DRenderManager.h"
#include "RenderData.h"

cSDL2DRenderManager cSDL2DRenderManager::m_SDL2DRenderManager;

//CRenderResource

//-------------------------------------------------------

cRenderResource::cRenderResource() : cResource()
{
    m_Surface = NULL;
	m_Texture = NULL;
}

//-------------------------------------------------------

cRenderResource::~cRenderResource()
{
    unload();

}

//-------------------------------------------------------

void cRenderResource::load()
{
    unload();

	SDL_Renderer *curRend = cSDL2DRenderManager::GetSDL2DRenderManager()->m_Renderer;

	m_Texture = IMG_LoadTexture(cSDL2DRenderManager::GetSDL2DRenderManager()->m_Renderer, m_FileName.c_str());

	m_bLoaded = true;
    
}

//-------------------------------------------------------

void cRenderResource::unload()
{
	if (m_Texture)
	{
		SDL_DestroyTexture(m_Texture);
		m_Texture = NULL;
	}

	m_bLoaded = false;
}

//-------------------------------------------------------

//cSDLRenderObject

//-------------------------------------------------------

cSDLRenderObject::cSDLRenderObject()
{
    m_RenderResource = NULL;
    m_PosX = m_PosY = 0.0f;
    m_bVisible = true;
    ZeroMemory(&m_RenderRect, sizeof(SDL_Rect));
    ZeroMemory(&m_ColorKey, sizeof(SDL_Color));
    m_bColorKeyEnabled = false;
}

//-------------------------------------------------------

void cSDLRenderObject::setColorKey(unsigned int r, unsigned int g, unsigned int b)
{
	m_ColorKey.r = r;
	m_ColorKey.g = g;
	m_ColorKey.b = b;


	SDL_Window* window =
		cSDL2DRenderManager::GetSDL2DRenderManager()->m_Window;
	SDL_Surface* surface = SDL_GetWindowSurface(window);

	Uint32 colorkey = SDL_MapRGB(surface->format, m_ColorKey.r, m_ColorKey.g, m_ColorKey.b);
	int success = SDL_SetColorKey(surface, -1, colorkey);
	std::cout << "success value: " << success << "\n";
}

//-------------------------------------------------------

void cSDLRenderObject::setResourceObject(cRenderResource *RenderResource)
{
    if(RenderResource)
    {
        m_RenderResource = RenderResource;
		int w, h;
		SDL_QueryTexture(m_RenderResource->m_Texture, NULL, NULL, &w, &h);
		m_RenderRect.w = w;
		m_RenderRect.h = h;

        if(m_bColorKeyEnabled)
            setColorKey(m_ColorKey.r, m_ColorKey.g, m_ColorKey.b);
    }
}

//-------------------------------------------------------

//cSpriteObject

//-------------------------------------------------------

cSpriteObject::cSpriteObject() : cSDLRenderObject()
{
    m_TotalFrames = m_FramsPerRow = m_FramsPerColumn = m_CurrentFrame = m_StartFrame = m_FrameWidth = m_FrameHeight = 0;
    m_Speed = 0.0f;
}

//-------------------------------------------------------

void cSpriteObject::play()
{
    //calculate frame dimensions
	int w, h;
	SDL_QueryTexture(m_RenderResource->m_Texture, NULL, NULL, &w, &h);
	m_FrameHeight = h / m_FramsPerColumn;
	m_FrameWidth = w / m_FramsPerRow;
	m_CurrentFrame = m_StartFrame;
	setFrameRect(m_CurrentFrame);
}

//-------------------------------------------------------

void cSpriteObject::stop()
{
}

//-------------------------------------------------------

void cSpriteObject::update()
{
    DWORD timeSinceLastFrame = SDL_GetTicks() - m_TimeLastFrame;

    if(timeSinceLastFrame >= m_Speed)
    {
        //Increment current frame
        m_CurrentFrame++;

        if(m_CurrentFrame>=m_TotalFrames)
            m_CurrentFrame = m_StartFrame;

        setFrameRect(m_CurrentFrame);

		m_TimeLastFrame = SDL_GetTicks();
        //m_TimeLastFrame = timeGetTime();
    }
}

//-------------------------------------------------------

void cSpriteObject::setFrameRect(unsigned int FrameNumber)
{
    unsigned int RowNumber = floor((double)FrameNumber/(double)m_FramsPerRow);
    unsigned int ColumnNumber = FrameNumber;

    if(RowNumber>0)
        ColumnNumber = FrameNumber - (RowNumber * m_FramsPerRow);

    m_RenderRect.x = ColumnNumber * m_FrameWidth;
    m_RenderRect.y = RowNumber * m_FrameHeight;
    m_RenderRect.w = m_FrameWidth;
    m_RenderRect.h = m_FrameHeight;
}

//-------------------------------------------------------

//CRenderManager

//-------------------------------------------------------

cSDL2DRenderManager::cSDL2DRenderManager() : c2DRenderManager()
{
	m_Window = NULL;
	m_SceneManager = NULL;
	m_WindowHandle = 0;
	InputListener = NULL;
}

//-------------------------------------------------------

bool cSDL2DRenderManager::init(unsigned int Width, unsigned int Height, bool fullScreen, char* WindowTitle)
{
    if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        //Error: could not initialise SDL Renderer
        return false;
    }

	m_Window = SDL_CreateWindow("SDLRenderManager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);

	SDL_RendererInfo Video, *VidInfo;
	SDL_GetRenderDriverInfo(0, &Video);

	VidInfo = &Video;

	bool hw_available = false;
	if ((VidInfo->flags & SDL_RENDERER_ACCELERATED) == SDL_RENDERER_ACCELERATED)
		hw_available = true;

	m_VideoInfo << "Video Info\nCan Create Hardware Surfaces: " << hw_available << "\n";

	std::cout << m_VideoInfo.str();

    return true;
}

//-------------------------------------------------------

void cSDL2DRenderManager::free()
{
     SDL_Quit();
}

//-------------------------------------------------------

bool cSDL2DRenderManager::checkKeys(SDL_Keysym keysym)
{
	// exit if ESCAPE is pressed
	switch (keysym.sym)
	{
	case SDLK_ESCAPE:
		return false;

	case SDLK_HOME:
		// reset to X origin
		if (keysym.mod & KMOD_CTRL)
			bGoOrigin = true;
		else
			bGoHome = true;
		break;

	case SDLK_END:
		// reset to X origin
		bGoEnd = true;
		break;

	}
	return true;
}

bool cSDL2DRenderManager::update()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // check for messages
        switch (event.type)
        {
            // exit if the window is closed
            case SDL_QUIT:
                return false;

			case SDL_WINDOWEVENT:
				
				SDL_GetWindowSize(this->m_Window, &current_width, &current_height);
				break;

            // check for keypresses
            /*case SDL_KEYDOWN:
				return checkKeys(event.key.keysym);*/
        } // end switch
    } // end of message processing

    // clear screen
	//SDL_RenderClear(m_Renderer);

    renderScene();

    //Call frame listeners
    renderAllObjects();

	//SDL_RenderPresent(m_Renderer);

    return true;
}

//-------------------------------------------------------

void cSDL2DRenderManager::renderAllObjects()
{
    std::list<cSDLRenderObject*>::iterator list_it;

	for (auto& list_it : m_RenderObjects)
	{
		if (list_it->m_bVisible)
		{
			list_it->update();

			SDL_Rect Pos;
			Pos.x = list_it->m_PosX;
			Pos.y = list_it->m_PosY;

			SDL_RenderCopy(this->m_Renderer, list_it->m_RenderResource->m_Texture, &(list_it->m_RenderRect), &Pos);
		}
	}

	SDL_Delay(25);
	SDL_RenderPresent(this->m_Renderer);
}

//-------------------------------------------------------

void cSDL2DRenderManager::toggleFullScreen()
{
    //SDL_WM_ToggleFullScreen(m_RenderWindow);
	SDL_SetWindowFullscreen(m_Window, 0);
}

 //-------------------------------------------------------

cResource* cSDL2DRenderManager::loadResourceFromXML(XMLElement *Element)
{
    if(Element)
    {
        cResource* Resource = new cRenderResource();

        for(XMLAttribute* ElementAttrib = const_cast<XMLAttribute*>(Element->FirstAttribute()); ElementAttrib; ElementAttrib = const_cast<XMLAttribute*>(ElementAttrib->Next()))
        {
            std::string AttribName = ElementAttrib->Name();
            std::string AttribValue = ElementAttrib->Value();

            if(AttribName=="UID")
            {
                Resource->m_ResourceID = atoi(AttribValue.c_str());
            }

            if(AttribName=="filename")
            {
                Resource->m_FileName = AttribValue;
            }

            if(AttribName=="scenescope")
            {
                Resource->m_Scope = atoi(AttribValue.c_str());
            }
        }

        return Resource;
    }

    return NULL;
}

//-------------------------------------------------------

void cSDL2DRenderManager::renderScene()
{

	if (!m_SceneManager)
		return;

	for (auto* Layer : m_SceneManager->m_Layers)
	{
		if (Layer->m_Name == "layer2")
			NavigateLayer(Layer);

		if (!Layer->m_bVisible)
			continue;

		for (auto* Object : Layer->m_SceneObjects)
		{
			if (Object->m_bVisible)
			{
				Object->update();
				SDL_Rect Pos;

				Pos.x = int(Layer->m_PosX) + int(Object->m_PosX);
				Pos.y = int(Layer->m_PosY) + int(Object->m_PosY);
				Pos.w = int(Object->m_RenderRect.w);
				Pos.h = int(Object->m_RenderRect.h);


				SDL_RenderCopy(this->m_Renderer,
					Object->m_RenderResource->m_Texture,
					&Object->m_RenderRect, &Pos);
			}
		}
	}

}

void cSDL2DRenderManager::NavigateLayer(c2DLayer* Layer)
{

	SDL_Rect& m_RenderRect = Layer->m_SceneObjects.front()->m_RenderRect;
	bGoHome = InputListener->GoHOME;
	bGoEnd = InputListener->GoEND;
	bGoOrigin = InputListener->GoUpperBound;
	bGoAxis = InputListener->GoLowerBound;
	InputListener->Reset();


	if (bGoHome)
	{
		bGoHome = false;
		MoveAcross = true;
		Layer->m_PosX = 0;
	}
	else if (bGoOrigin)
	{
		bGoOrigin = false;
		MoveAcross = true;
		Layer->m_PosX = Layer->m_PosY = 0;
	}
	else if (bGoEnd)
	{
		bGoEnd = false;
		MoveBack = true;
		Layer->m_PosX = current_width - m_RenderRect.w;
	}
	else if (bGoAxis)
	{
		bGoEnd = false;
		MoveBack = true;
		Layer->m_PosX = current_width - m_RenderRect.w;
		Layer->m_PosY =
			current_height - m_RenderRect.h;
	}

	if (MoveAcross)
	{
		Layer->m_PosX += positionMove;
		if (Layer->m_PosX + m_RenderRect.w > current_width)
		{
			MoveAcross = false;
			MoveDown = true;
		}
	}
	else if (MoveDown)
	{
		Layer->m_PosY += positionMove;
		if (Layer->m_PosY + m_RenderRect.h > current_height)
		{
			MoveDown = false;
			MoveBack = true;
		}
	}
	else if (MoveBack)
	{
		Layer->m_PosX -= positionMove;
		if (Layer->m_PosX < 0)
		{
			MoveBack = false;
			MoveUp = true;
		}
	}
	else if (MoveUp)
	{
		Layer->m_PosY -= positionMove;
		if (Layer->m_PosY < 0)
		{
			MoveUp = false;
			MoveAcross = true;
		}
	}
}

void cSDL2DRenderManager::NavigateLayer(cSceneObject* so)
{

	if (bGoHome)
	{
		bGoHome = false;
		MoveAcross = true;
		so->m_PosX = 0;
	}
	else if (bGoOrigin)
	{
		bGoOrigin = false;
		MoveAcross = true;
		so->m_PosX = so->m_PosY = 0;
	}
	else if (bGoEnd)
	{
		bGoEnd = false;
		MoveBack = true;
		so->m_PosX = current_width - so->m_RenderRect.w;
	}

	if (MoveAcross)
	{
		so->m_PosX += positionMove;
		if (so->m_PosX + so->m_RenderRect.w > current_width)
		{
			MoveAcross = false;
			MoveDown = true;
		}
	}
	else if (MoveDown)
	{
		so->m_PosY += positionMove;
		if (so->m_PosY + so->m_RenderRect.h > current_height)
		{
			MoveDown = false;
			MoveBack = true;
		}
	}
	else if (MoveBack)
	{
		so->m_PosX -= positionMove;
		if (so->m_PosX < 0)
		{
			MoveBack = false;
			MoveUp = true;
		}
	}
	else if (MoveUp)
	{
		so->m_PosY -= positionMove;
		if (so->m_PosY < 0)
		{
			MoveUp = false;
			MoveAcross = true;
		}
	}
}

//-------------------------------------------------------

cSDL2DRenderManager* cSDL2DRenderManager::GetSDL2DRenderManager()
{
    return &m_SDL2DRenderManager;
}

//-------------------------------------------------------
