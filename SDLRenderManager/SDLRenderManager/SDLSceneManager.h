#ifndef SDL2DSCENEMANAGER_H_INCLUDED
#define SDL2DSCENEMANAGER_H_INCLUDED

#include <string>
#include <sstream>
#include <iostream>
#include <time.h>
#include <iomanip>
#include <windows.h>
#include <math.h>
#include "ResourceManager.h"
#include "tinyxml2.h"
#include "RenderData.h"

//------------------------------------------------------------

typedef enum {SE_TIMER_EXPIRED=0}SCENE_EVENT_TYPE;

class cSDL2DSceneManager;

//------------------------------------------------------------

class cSceneListener : public cEngineObject
{
    public:
        SCENE_EVENT_TYPE m_ListenFor;

        virtual void Event(cSDL2DSceneManager* Manager, void* customData) const = 0;
};

//------------------------------------------------------------

class cTimer : public cEngineObject
{
    private:
    protected:
    public:

        unsigned int m_ID;
        DWORD m_StartTime;
        DWORD m_Interval;
        bool m_Expired;

        cTimer()
        {
            m_ID = 0;
            m_StartTime = 0;
            m_Interval = 0;
            m_Expired = false;
        }

        void start()
        {
            m_StartTime = SDL_GetTicks();
            m_Expired = false;
        }

        void update()
        {
            if(m_Expired)
                return;

            DWORD ElapsedTime = SDL_GetTicks() - m_StartTime;

            if(ElapsedTime>=m_Interval)
            {
                m_Expired = true;
            }
        }
};


//------------------------------------------------------------

class cSceneObject : public cSDLRenderObject
{
    private:
    protected:
    public:
};


//------------------------------------------------------------

class c2DLayer : public cEngineObject
{
    private:
    protected:
    public:
        //Render objects for the layer
        c2DLayer();
        bool m_bVisible;
        unsigned int m_ZOrder;
        float m_PosX;
        float m_PosY;
        std::string m_Name;
        std::list<cSceneObject*> m_SceneObjects;
        void update();
};

//------------------------------------------------------------

class cSDL2DSceneManager : public cEngineObject
{
    private:
    protected:
        void addLayerObjects(c2DLayer *Layer, XMLElement *Element);
        void checkTimerExpired();
    public:

        std::list<c2DLayer*> m_Layers;
        std::list<cTimer*> m_Timers;
        std::list<cSceneListener*> m_Listeners;

        c2DLayer* addLayer(std::string Name);
        c2DLayer* findLayer(std::string Name);
        void removeLayer(std::string Name);
        void sortLayers();

        bool loadFromXMLFile(std::string Filename);

        void addTimer(unsigned int ID, DWORD Interval);
        void addListener(cSceneListener* Object);

        void update();
};

//------------------------------------------------------------

class cTestListener : public cSceneListener
{
    private:
    protected:
    public:

        cTestListener()
        {
            m_ListenFor = SE_TIMER_EXPIRED;
        }

        void Event(cSDL2DSceneManager* Manager, void* customData) const
        {
            c2DLayer* Layer = Manager->findLayer("layer2");
            //Layer->m_bVisible = !Layer->m_bVisible;
			Layer->m_bVisible != Layer->m_bVisible;
        }
};

//------------------------------------------------------------

#endif // 2DSDLSCENEMANAGER_H_INCLUDED
