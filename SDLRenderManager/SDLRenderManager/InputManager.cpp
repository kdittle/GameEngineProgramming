#include "stdafx.h"
#include "InputManager.h"
#include "SDL2DRenderManager.h"
#include <windows.h>
#include "SDL_syswm.h"

//-------------------------------------------------------

cInputManager::cInputManager()
{
	m_OISInputManager = NULL;
	m_Keyboard = NULL;
	m_Mouse = NULL;
}

//-------------------------------------------------------

void cInputManager::init()
{
	try{
		cSDL2DRenderManager* g_RenderManager = cSDL2DRenderManager::GetSDL2DRenderManager();

		OIS::ParamList pl;
		std::ostringstream windowHndStr;

		SDL_SysWMinfo wmi;
		SDL_Window* window = g_RenderManager->m_Window;
		SDL_VERSION(&wmi.version);

		if (!SDL_GetWindowWMInfo(window, &wmi))
			return;

		size_t handle = (size_t)wmi.info.win.window;
		//g_RenderManager->m_WindowHandle;
		windowHndStr << handle;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
		m_OISInputManager = OIS::InputManager::createInputSystem(pl);

		//If there is a keyboard
		if (m_OISInputManager->getNumberOfDevices(OIS::OISKeyboard) > 0)
		{
			m_Keyboard = static_cast<OIS::Keyboard*>(m_OISInputManager->createInputObject(OIS::OISKeyboard, true));
			m_Keyboard->setEventCallback(this);
		}

		//If there is a mouse
		if (m_OISInputManager->getNumberOfDevices(OIS::OISMouse) > 0)
		{
			m_Mouse = static_cast<OIS::Mouse*>(m_OISInputManager->createInputObject(OIS::OISMouse, true));
			m_Mouse->setEventCallback(this);
		}

	}
	catch (...){ ; }
}

//-------------------------------------------------------

void cInputManager::free()
{
	if (m_OISInputManager)
	{
		if (m_Keyboard != nullptr)
			m_OISInputManager->destroyInputObject(m_Keyboard);
		if (m_Mouse != nullptr)
			m_OISInputManager->destroyInputObject(m_Mouse);

	}
}

//-------------------------------------------------------

void cInputManager::update()
{
    if(m_Mouse)
        m_Mouse->capture();

    if(m_Keyboard)
        m_Keyboard->capture();
}

//-------------------------------------------------------

void cInputManager::addListener(cInputListener* Listener)
{
	m_InputListeners.push_back(Listener);
}

//-------------------------------------------------------

bool cInputManager::keyPressed( const OIS::KeyEvent &e )
{
	for (auto* list_it : m_InputListeners)
	{
		list_it->keyPressed(e);
	}


	return true;
}

//-------------------------------------------------------

bool cInputManager::keyReleased( const OIS::KeyEvent &e )
{
	std::list<cInputListener*>::iterator list_it;

	for(list_it=m_InputListeners.begin();list_it!=m_InputListeners.end();list_it++)
		(*list_it)->keyReleased(e);

	return true;
}

//-------------------------------------------------------

bool cInputManager::mouseMoved( const OIS::MouseEvent &e )
{
	std::list<cInputListener*>::iterator list_it;

	for(list_it=m_InputListeners.begin();list_it!=m_InputListeners.end();list_it++)
		(*list_it)->mouseMoved(e);

	return true;
}

//-------------------------------------------------------

bool cInputManager::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
	std::list<cInputListener*>::iterator list_it;

	for(list_it=m_InputListeners.begin();list_it!=m_InputListeners.end();list_it++)
		(*list_it)->mousePressed(e, id);

	return true;
}

//-------------------------------------------------------

bool cInputManager::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
	std::list<cInputListener*>::iterator list_it;

	for(list_it=m_InputListeners.begin();list_it!=m_InputListeners.end();list_it++)
		(*list_it)->mouseReleased(e, id);

	return true;
}

//-------------------------------------------------------

bool cInputManager::povMoved(const OIS::JoyStickEvent &e, int pov)
{
	return true;
}

//-------------------------------------------------------

bool cInputManager::axisMoved(const OIS::JoyStickEvent &e, int axis)
{
	return true;
}

//-------------------------------------------------------

bool cInputManager::sliderMoved(const OIS::JoyStickEvent &e, int sliderID)
{
	return true;
}

//-------------------------------------------------------

bool cInputManager::buttonPressed(const OIS::JoyStickEvent &e, int button)
{
	return true;
}

//-------------------------------------------------------

bool cInputManager::buttonReleased(const OIS::JoyStickEvent &e, int button)
{
	return true;
}

//-------------------------------------------------------