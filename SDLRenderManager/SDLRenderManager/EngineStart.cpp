// EngineStart.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
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

//class cMySoundEffect: public cAudioPlayer
//{
//	private:
//	protected:
//	public:
//		
//		cMySoundEffect()
//		{
//			m_NumRepeats = 2;
//		}
//	
//		void onMediaComplete()
//		{
//			MessageBoxA(NULL,"media completed","", MB_OK);
//		}
//};

class MyInputListener : public cInputListener
{
private:
protected:
public:

	bool keyPressed(const OIS::KeyEvent &e)
	{
		if (e.key == OIS::KC_ESCAPE)
			MessageBoxA(NULL, "Escape Key Pressed", "", MB_OK);

		return true;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	cSDL2DRenderManager* g_RenderManager = cSDL2DRenderManager::GetSDL2DRenderManager();
    cSDL2DSceneManager* g_SceneManager = new cSDL2DSceneManager();
	//cAudioManager* g_AudioManager = cAudioManager::GetAudioManager();

    g_RenderManager->init(800,600,false,"My Window");
	//g_AudioManager->init();

	cInputManager m_Input;
	m_Input.init();

	MyInputListener List;

	m_Input.addListener(&List);

    cResourceManager* ResourceManager = cResourceManager::GetResourceManager();
    ResourceManager->loadFromXMLFile("ResourceTree.xml");
    ResourceManager->setCurrentScope(0);

    g_SceneManager->loadFromXMLFile("SceneTree.xml");

    g_RenderManager->m_SceneManager = g_SceneManager;

	//cMySoundEffect MySound;
	//MySound.m_AudioResource = (cAudioResource*)ResourceManager->findResourcebyID(5);

	//g_AudioManager->addAudioPlayer(&MySound);

    cTestListener Tst;

    g_SceneManager->addListener(&Tst);
    g_SceneManager->addTimer(0,1000);

	/*for (size_t i = 0; i < ResourceManager->getResourceCount(); i++)
	{
		cSDLRenderObject* RenderObject = new cSDLRenderObject();
		RenderObject->setResourceObject((cRenderResource*)ResourceManager->findResourcebyID(i + 1));
		g_RenderManager->m_RenderObjects.push_back(RenderObject);
	}*/

    while (g_RenderManager->update())
    {
		g_SceneManager->update();
		m_Input.update();
		//g_AudioManager->update();
    }

	g_RenderManager->m_RenderObjects.clear();

	m_Input.free();

    g_RenderManager->free();

    delete g_SceneManager;

	//g_AudioManager->free();

    return 0;
}

