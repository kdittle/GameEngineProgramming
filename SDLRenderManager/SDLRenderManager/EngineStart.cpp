// EngineStart.cpp : Defines the entry point for the console application.
//

/*

SDL Scene Manager line 144
Audio manager.h and .cpp commented out completely
Input manager, init function commented out parts.
EngineStart line 65 commented out

*/

//#include "stdafx.h"
#include <windows.h>
#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include <SDL.h>
#include <SDL_Image.h>
#include <cstdlib>
#include "tinyxml2.h"
#include <OIS.h>
#include "SDL2DRenderManager.h"
#include "ErrorLogManager.h"
#include "ResourceManager.h"
#include "SDLSceneManager.h"
//#include "AudioManager.h"
#include "InputManager.h"

class MyInputListener : public cInputListener
{	
	private:
	protected:
	public:
		
		bool keyPressed(const OIS::KeyEvent &e)
		{
			if(e.key==OIS::KC_ESCAPE)
				MessageBoxA(NULL,"Escape Key Pressed","",MB_OK);

			return true;
		}
};

int _tmain(int argc, _TCHAR* argv[])
{
	cSDL2DRenderManager* g_RenderManager = cSDL2DRenderManager::GetSDL2DRenderManager();
    cSDL2DSceneManager *g_SceneManager = new cSDL2DSceneManager();

    g_RenderManager->init(800,600,false,"My Window");

	cInputManager m_Input;
	m_Input.init();

	MyInputListener List;

	m_Input.addListener(&List);

    cResourceManager* ResourceManager = cResourceManager::GetResourceManager();
    ResourceManager->loadFromXMLFile("ResourceTree.xml");
    ResourceManager->setCurrentScope(0);

    g_SceneManager->loadFromXMLFile("SceneTree.xml");

    //g_RenderManager->m_SceneManager=g_SceneManager;

    cTestListener Tst;

    g_SceneManager->addListener(&Tst);
    g_SceneManager->addTimer(0,1000);

    while (g_RenderManager->update())
    {
        g_SceneManager->update();
		m_Input.update();
    }

	m_Input.free();

    g_RenderManager->free();

    delete g_SceneManager;

    return 0;
}

