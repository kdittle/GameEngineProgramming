#include "stdafx.h"
#include "SDLSceneManager.h"

//------------------------------------------------------------

bool compareLayerOrder(const c2DLayer *lhs, const c2DLayer *rhs)
{
  return lhs->m_ZOrder < rhs->m_ZOrder;
}

//------------------------------------------------------------

c2DLayer::c2DLayer() : cEngineObject()
{
    m_bVisible=true;
    m_PosX = m_PosY = 0.0f;
    m_ZOrder = 0;
}

//------------------------------------------------------------

c2DLayer* cSDL2DSceneManager::addLayer(std::string Name)
{
    c2DLayer *Layer = findLayer(Name);

    if(!Layer)
    {
        Layer = new c2DLayer();
        Layer->m_Name=Name;

        m_Layers.push_back(Layer);
    }

    return Layer;
}

//------------------------------------------------------------

c2DLayer* cSDL2DSceneManager::findLayer(std::string Name)
{
    std::list<c2DLayer*>::iterator list_it;

    //Render all assoicated render objects
    for(list_it=m_Layers.begin();list_it!=m_Layers.end();list_it++)
    {
        if((*list_it)->m_Name==Name)
            return (*list_it);
    }

    return NULL;
}

//------------------------------------------------------------

void cSDL2DSceneManager::removeLayer(std::string Name)
{
    std::list<c2DLayer*>::iterator list_it;

    //Render all assoicated render objects
    for(list_it=m_Layers.begin();list_it!=m_Layers.end();list_it++)
    {
        if((*list_it)->m_Name==Name)
           m_Layers.remove(*list_it);
    }
}

//------------------------------------------------------------

void cSDL2DSceneManager::addLayerObjects(c2DLayer *Layer, XMLElement *Element)
{
    cSceneObject* Object = new cSceneObject();
    unsigned int r=0;
    unsigned int g=0;
    unsigned int b=0;

	if(!Object)
		return;

    for(XMLAttribute* ElementAttrib = const_cast<XMLAttribute*>(Element->FirstAttribute()); ElementAttrib; ElementAttrib = const_cast<XMLAttribute*>(ElementAttrib->Next()))
    {
        std::string AttribName = ElementAttrib->Name();
        std::string AttribValue = ElementAttrib->Value();
        if(AttribName=="resourceID")
        {
            cResourceManager* ResourceManager = cResourceManager::GetResourceManager();

            Object->setResourceObject((cRenderResource*) ResourceManager->findResourcebyID(atoi(AttribValue.c_str())));
        }

        if(AttribName=="posx")
        {
            Object->m_PosX = atof(AttribValue.c_str());
        }

        if(AttribName=="posy")
        {
            Object->m_PosY = atof(AttribValue.c_str());
        }

        if(AttribName=="colorkey")
        {
            if(AttribValue=="true")
               Object->m_bColorKeyEnabled=true;
            else
               Object->m_bColorKeyEnabled=false;
        }

        if(AttribName=="r")
        {
            r = atoi(AttribValue.c_str());
        }

        if(AttribName=="g")
        {
            g = atoi(AttribValue.c_str());
        }

        if(AttribName=="b")
        {
            b = atoi(AttribValue.c_str());
        }
    }

    if(Object->m_bColorKeyEnabled)
        Object->setColorKey(r,g,b);

    Layer->m_SceneObjects.push_back(Object);
}


//------------------------------------------------------------

bool cSDL2DSceneManager::loadFromXMLFile(std::string Filename)
{
   XMLDocument doc(Filename.c_str());

   std::list<c2DLayer*> List;

    if(doc.LoadFile(Filename.c_str()))
    {
        //Find resources node
        XMLNode* ResourceTree = doc.FirstChild();

        if(ResourceTree)
        {
            //Enumerate resource objects
            for(XMLNode* child = ResourceTree->FirstChild(); child; child = child->NextSibling())
            {
                XMLElement *Element = child->ToElement();

                if(Element)
                {
                    c2DLayer *Layer = new c2DLayer();
                    Layer->m_ZOrder = m_Layers.size();

                    for(XMLAttribute* ElementAttrib = const_cast<XMLAttribute*>(Element->FirstAttribute()); ElementAttrib; ElementAttrib = const_cast<XMLAttribute*>(ElementAttrib->Next()))
					{
                        //Examine layers
                        std::string AttribName = ElementAttrib->Name();
                        std::string AttribValue = ElementAttrib->Value();

                        //Detect resource type. Graphic? Audio? Text?
                        if(AttribName=="name")
                        {
                            Layer->m_Name=AttribValue;
                            continue;
                        }

                        if(AttribName=="posx")
                        {
                            Layer->m_PosX = atof(AttribValue.c_str());
                        }

                        if(AttribName=="posy")
                        {
                            Layer->m_PosY = atof(AttribValue.c_str());
                        }

                        if(AttribName=="visible")
                        {
                            if(AttribValue=="true")
                                Layer->m_bVisible=true;
                            else
                                Layer->m_bVisible=false;
                        }
					}

					m_Layers.push_back(Layer);

					//Cycle through layer objects
					for(XMLNode* objs = child->FirstChild(); objs; objs = objs->NextSibling())
					{
					    if(std::string(objs->Value())=="objects")
					    {
					        for(XMLNode* obj = objs->FirstChild(); obj; obj = obj->NextSibling())
					        {
                                XMLElement *ObjElement = obj->ToElement();

                                addLayerObjects(Layer, ObjElement);
					        }
                        }
					}
                }
            }

             sortLayers();
             return true;
        }
    }



    return false;
}

//------------------------------------------------------------

void cSDL2DSceneManager::checkTimerExpired()
{
    std::list<cTimer*>::iterator list_it;

    //Render all assoicated render objects
    for(list_it=m_Timers.begin();list_it!=m_Timers.end();list_it++)
    {
        (*list_it)->update();

        if((*list_it)->m_Expired)
        {
            std::list<cSceneListener*>::iterator listener_it;

            for(listener_it=m_Listeners.begin();listener_it!=m_Listeners.end();listener_it++)
            {
                if((*listener_it)->m_ListenFor == SE_TIMER_EXPIRED)
                {
                    (*listener_it)->Event(this, NULL);
                    (*list_it)->start();
                }
            }
        }
    }
}

//------------------------------------------------------------

void cSDL2DSceneManager::update()
{
    checkTimerExpired();
}

//------------------------------------------------------------

void cSDL2DSceneManager::addTimer(unsigned int ID, DWORD Interval)
{
    cTimer *Timer = new cTimer();
    Timer->m_ID=ID;
    Timer->m_Interval=Interval;
    m_Timers.push_back(Timer);
    Timer->start();
}

//------------------------------------------------------------

void cSDL2DSceneManager::addListener(cSceneListener* Object)
{
    m_Listeners.push_back(Object);
}

//------------------------------------------------------------

void cSDL2DSceneManager::sortLayers()
{
    m_Layers.sort(compareLayerOrder);
}

//------------------------------------------------------------
