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



		switch (e.key)
		{
		case OIS::KC_ESCAPE:
			SignalBreak = true;

			break;
		case OIS::KC_HOME:
			GoHOME = true;

			if (CTRL_Pressed)
			{
				GoUpperBound = true;
				GoHOME = false;
				CTRL_Pressed = false;
			}
			break;
		case OIS::KC_END:
			GoEND = true;
			if (CTRL_Pressed)
			{
				GoLowerBound = true;
				GoEND = false;
				CTRL_Pressed = false;
			}
			break;
		case OIS::KC_UP:
			GoUpperBound = true;
			break;
		case OIS::KC_DOWN:
			GoLowerBound = true;
			break;
		case OIS::KC_LCONTROL:
		case OIS::KC_RCONTROL:
			CTRL_Pressed = true;
			break;
		}


		return true;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	cSDL2DRenderManager* g_RenderManager = cSDL2DRenderManager::GetSDL2DRenderManager();
	cSDL2DSceneManager *g_SceneManager = new cSDL2DSceneManager();

	g_RenderManager->init(800, 600, false, "My Window");

	cInputManager m_Input;
	m_Input.init();

	MyInputListener List;

	m_Input.addListener(&List);

	cResourceManager* ResourceManager = cResourceManager::GetResourceManager();
	ResourceManager->loadFromXMLFile("ResourceTree.xml");
	ResourceManager->setCurrentScope(0);

	for (size_t i = 0; i < ResourceManager->getResourceCount(); i++)
	{

		cSDLRenderObject* RenderObject = new cSDLRenderObject();
		RenderObject->setResourceObject((cRenderResource*)ResourceManager->findResourcebyID(i + 1));
		g_RenderManager->m_RenderObjects.push_back(RenderObject);
	}

	g_SceneManager->loadFromXMLFile("SceneTree.xml");

	g_RenderManager->m_SceneManager = g_SceneManager;


	cTestListener Tst;

	g_SceneManager->addListener(&Tst);
	g_SceneManager->addTimer(0, 2000);

	SDL_RenderClear(g_RenderManager->m_Renderer);
	g_RenderManager->InputListener = &List;

	while (g_RenderManager->update())
	{
		g_SceneManager->update();
		m_Input.update();
		if (List.SignalBreak)
		{
			SDL_Event event;
			SDL_zero(event);
			event.type = SDL_EventType::SDL_QUIT;
			SDL_PushEvent(&event);
		}


	}


	m_Input.free();
	g_RenderManager->m_RenderObjects.clear();



	g_RenderManager->free();

	delete g_SceneManager;

	return 0;
}

