#ifndef SDL2DRENDERMANAGER_H_INCLUDED
#define SDL2DRENDERMANAGER_H_INCLUDED

#include "2DRenderManager.h"
#include "ResourceManager.h"
//#include "SDLSceneManager.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <string>
#include <sstream>
#include <iostream>
#include <time.h>
#include <iomanip>
#include <windows.h>
#include <math.h>
#include "RenderData.h"
#include "SDL_syswm.h"

//Tiny XML Header. Needs TinyXML library
//Available for free at: http://www.grinninglizard.com/tinyxml/
//#include "TinyXML.h"

#include "tinyxml2.h"

//2D Sprite Object
//Derived from Render Object. This class features extra properties
//for creating animated tile sets. By including all frames in a single image,
//and by specifiying the dimensions of rach frame, this class can
//animate through the frames at a specified speed

class cSpriteObject : public cSDLRenderObject
{
    private:
    protected:
    DWORD m_TimeLastFrame;

    public:
    unsigned int m_TotalFrames;
    unsigned int m_FramsPerRow;
    unsigned int m_FramsPerColumn;
    unsigned int m_CurrentFrame;
    unsigned int m_StartFrame;
    float m_Speed;
    unsigned int m_FrameWidth;
    unsigned int m_FrameHeight;

    void update();
    void play();
    void stop();
    void setFrameRect(unsigned int FrameNumber);
    cSpriteObject();
};

//------------------------------------------------------------

//2D Render Manager class
//This is the 2D SDL Render Manager component, and it works with all
//classes listed above.
//Its purpose is to create a window, initialise hardware,
//create, load and unload graphics resources, and render graphics

class cSDL2DRenderManager : public c2DRenderManager
{
    private:
    protected:
    cSDL2DRenderManager();
    static cSDL2DRenderManager m_SDL2DRenderManager;
    void renderScene();
	SDL_SysWMinfo m_inf;

    public:
    static cSDL2DRenderManager* GetSDL2DRenderManager();
	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;
	SDL_Texture* m_Texture;
    std::stringstream m_VideoInfo;
    bool init(unsigned int Width=800, unsigned int Height=600, bool fullScreen=false, char* WindowTitle=0);
    void free();
    bool update();
    void toggleFullScreen();
    cResource* loadResourceFromXML(XMLElement *Element);
    void renderAllObjects();
	HWND m_WindowHandle;

    std::list<cSDLRenderObject*> m_RenderObjects;
};

//------------------------------------------------------------

#endif // SDL2DRENDERMANAGER_H_INCLUDED
