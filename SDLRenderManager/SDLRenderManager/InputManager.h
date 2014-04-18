#ifndef INPUTMANAGER_H_INCLUDED
#define INPUTMANAGER_H_INCLUDED

#include <OIS.h>
#include <list>

#include "EngineObject.h"
#include <map>

//-------------------------------------------------------

class cInputListener : public cEngineObject
{
private:
protected:
public:

	void Reset()
	{
		SignalBreak = false;
		GoHOME = false;
		GoEND = false;
		GoUpperBound = false;
		GoLowerBound = false;
	}

	bool CTRL_Pressed = false;
	bool SignalBreak = false;
	bool GoHOME = false;
	bool GoEND = false;
	bool GoUpperBound = false;
	bool GoLowerBound = false;

	//Input events
	virtual bool keyPressed( const OIS::KeyEvent &e ){return true;};
	virtual bool keyReleased( const OIS::KeyEvent &e ){return true;};

	//Mouse events
	virtual bool mouseMoved( const OIS::MouseEvent &e ){return true;};
    virtual bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ){return true;};
    virtual bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ){return true;};

	//Joystick events
	virtual bool povMoved( const OIS::JoyStickEvent &e, int pov ){return true;};
	virtual bool axisMoved( const OIS::JoyStickEvent &e, int axis ){return true;};
    virtual bool sliderMoved( const OIS::JoyStickEvent &e, int sliderID ){return true;};
    virtual bool buttonPressed( const OIS::JoyStickEvent &e, int button ){return true;};
	virtual bool buttonReleased( const OIS::JoyStickEvent &e, int button ){return true;};
};

//-------------------------------------------------------


class cInputManager : public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener, public cEngineObject
{
private:
protected:
public:
	OIS::InputManager* m_OISInputManager;
	OIS::Keyboard* m_Keyboard;
	OIS::Mouse* m_Mouse;

	std::list<cInputListener*> m_InputListeners;
	std::map<OIS::KeyCode, OIS::KeyCode> m_KeyMap;

	cInputManager();
	void init();
	void free();

	//Input events
	bool keyPressed( const OIS::KeyEvent &e );
    bool keyReleased( const OIS::KeyEvent &e );

	//Mouse events
	bool mouseMoved( const OIS::MouseEvent &e );
    bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );

	//Joystick events
	bool povMoved( const OIS::JoyStickEvent &e, int pov );
    bool axisMoved( const OIS::JoyStickEvent &e, int axis );
    bool sliderMoved( const OIS::JoyStickEvent &e, int sliderID );
    bool buttonPressed( const OIS::JoyStickEvent &e, int button );
	bool buttonReleased( const OIS::JoyStickEvent &e, int button );

	void update();

	void addListener(cInputListener* Listener);
};

#endif // INPUTMANAGER_H_INCLUDED

//-------------------------------------------------------
