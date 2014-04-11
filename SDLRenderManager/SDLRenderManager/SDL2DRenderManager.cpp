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

	m_Texture = IMG_LoadTexture(cSDL2DRenderManager::GetSDL2DRenderManager()->m_Renderer, m_FileName.c_str());
    
}

//-------------------------------------------------------

void cRenderResource::unload()
{
    if(m_Surface)
    {
        SDL_FreeSurface(m_Surface);
        m_Surface = NULL;
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
    Uint32 colorkey = SDL_MapRGB(m_RenderResource->m_Surface->format, m_ColorKey.r, m_ColorKey.g, m_ColorKey.b);
	SDL_SetColorKey(m_RenderResource->m_Surface, SDL_TRUE, colorkey);
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

    SDL_Surface *TmpSurface = m_RenderResource->m_Surface;
    m_FrameWidth = TmpSurface->w/m_FramsPerRow;
    m_FrameHeight = TmpSurface->h/m_FramsPerColumn;
    m_CurrentFrame = m_StartFrame;
    setFrameRect(m_CurrentFrame);
	m_TimeLastFrame = SDL_GetTicks();
	//m_TimeLastFrame = timeGetTime();
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
    //m_RenderWindow = NULL;
	m_Window = NULL;
	m_Renderer = NULL;
	m_WindowHandle = 0;
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

    //if(fullScreen)
    //    m_RenderWindow = SDL_SetVideoMode(Width, Height, 16, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
    //else
    //    m_RenderWindow = SDL_SetVideoMode(Width, Height, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);

    //if(!m_RenderWindow)
    //{
    //    //Error: could not create window
    //    return false;
    //}

    //SDL_WM_SetCaption(WindowTitle,NULL);

    //Populate video info

    //const SDL_VideoInfo *VidInfo = SDL_GetVideoInfo();

    //m_VideoInfo << "Video Info\nCan Create Hardware Surfaces: " << VidInfo->hw_available <<
    //    "\nWindow Manager Available: " << VidInfo->wm_available << "\nHardware to hardware blits accelerated: " << VidInfo->blit_hw
    //    << "\nHardware to hardware colorkey blits accelerated: " << VidInfo->blit_hw_CC
    //    << "\nHardware to hardware alpha blits accelerated: " << VidInfo->blit_hw_A
    //    << "\nSoftware to hardware blits accelerated: " << VidInfo->blit_sw
    //    << "\nSoftware to hardware colorkey blits accelerated: " << VidInfo->blit_sw_CC
    //    << "\nSoftware to hardware alpha blits accelerated: " << VidInfo->blit_sw_A
    //    << "\nColor fills accelerated: " << VidInfo->blit_fill
    //    << "\nTotal amount of video memory in Kilobytes: " << VidInfo->video_mem;

	
	/*SDL_GetWMInfo(&m_inf);*/

	/*m_WindowHandle = m_inf.window;*/

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
            case SDL_KEYDOWN:
				return checkKeys(event.key.keysym);
        } // end switch
    } // end of message processing

    // clear screen
	SDL_RenderClear(m_Renderer);

    renderScene();

    //Call frame listeners
    renderAllObjects();

	SDL_RenderPresent(m_Renderer);

    return true;
}

//-------------------------------------------------------

void cSDL2DRenderManager::renderAllObjects()
{
    std::list<cSDLRenderObject*>::iterator list_it;

	const int xincrement = 5;
	static int xpos = 0;
	static int ypos = 0;

	for (auto& list_it : m_RenderObjects)
	{
		if (list_it->m_bVisible)
		{
			list_it->update();

			SDL_Rect Pos = list_it->m_RenderRect;

			if (list_it->m_RenderResource->m_FileName == "spaceship.bmp")
				Pos.x += xpos;

			SDL_RenderCopy(this->m_Renderer, list_it->m_RenderResource->m_Texture, &(list_it->m_RenderRect), &Pos);

			if (list_it->m_RenderResource->m_FileName == "spaceship.bmp")
			{
				xpos += xincrement;
				if (xpos + Pos.w > 640 + Pos.w)
					xpos = -200;
			}
		}
	}

	SDL_RenderPresent(this->m_Renderer);

	cSceneObject* curScene = nullptr;

	for (auto& list_it : m_RenderObjects)
	{

		curScene = list_it->GetCurrentScene();

		if (list_it->m_bVisible)
		{
			list_it->update();

			SDL_Rect Pos = list_it->m_RenderRect;

			if (curScene)
				Pos.x = curScene->m_PosX;

			SDL_RenderCopy(this->m_Renderer, list_it->m_RenderResource->m_Texture, &(list_it->m_RenderRect), &Pos);
		}
	}

	SDL_Delay(25);
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

	//Implemented later with Scene Manager
	c2DLayer *Layer = this->m_SceneManager->findLayer("layer2");

	if (Layer)
	{
		for (auto *so : Layer->m_SceneObjects)
			NavigateLayer(so);
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
		so->m_PosX += positionMove;
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
