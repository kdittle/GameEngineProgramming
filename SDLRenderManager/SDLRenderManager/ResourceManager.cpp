#include "stdafx.h"
#include "ResourceManager.h"
#include "SDL2DRenderManager.h"
#include <windows.h>

cResourceManager cResourceManager::m_ResourceManager;

//-------------------------------------------------------

cResourceManager* cResourceManager::GetResourceManager()
{
    return &m_ResourceManager;
}

//-------------------------------------------------------

cResource* cResourceManager::findResourcebyID(unsigned int UID)
{
    std::map<unsigned int, std::list<cResource*> >::iterator it;

    //Search through scopes
    for(it=m_Resources.begin();it!=m_Resources.end();it++)
    {
        if(!(*it).second.empty())
        {
            std::list<cResource*>::iterator list_it;

            //Search through resources of scope
            for(list_it=(*it).second.begin();list_it!=(*it).second.end();list_it++)
            {
                //If matches ID
                if((*list_it)->m_ResourceID == UID)
                    return (*list_it);
            }
        }
    }

    return NULL;
}

//-------------------------------------------------------

void cResourceManager::clear()
{
    std::map<unsigned int, std::list<cResource*> >::iterator it;

    //Search through scopes
    for(it=m_Resources.begin();it!=m_Resources.end();it++)
    {
        if(!(*it).second.empty())
        {
            std::list<cResource*>::iterator list_it;

            //Search through resources of scope
            for(list_it=(*it).second.begin();list_it!=(*it).second.end();list_it++)
            {
                //Delete resource object
                (*list_it)->unload();
                SAFE_DELETE(*list_it);
            }

            (*it).second.clear();
        }
    }

    m_Resources.clear();
}

//-------------------------------------------------------

bool cResourceManager::loadFromXMLFile(std::string Filename)
{
    XMLDocument doc(Filename.c_str());

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
                    cResource *Resource = NULL;

                    for(XMLAttribute* ElementAttrib = const_cast<XMLAttribute*>(Element->FirstAttribute()); ElementAttrib; ElementAttrib = const_cast<XMLAttribute*>(ElementAttrib->Next()))
					{
                        //Examine resource object
                        std::string AttribName = ElementAttrib->Name();
                        std::string AttribValue = ElementAttrib->Value();

                        //Detect resource type. Graphic? Audio? Text?
                        if(AttribName=="type")
                        {
                            if(AttribValue=="graphic")
                            {
                                //Get singelton render manager

                                cSDL2DRenderManager* g_RenderManager = cSDL2DRenderManager::GetSDL2DRenderManager();
                                Resource = g_RenderManager->loadResourceFromXML(Element);

                                //Resource = new cResource();
                            }

                            if(AttribValue=="audio")
                            {
								//cAudioManager* g_AudioManager = cAudioManager::GetAudioManager();
                                //Resource = g_AudioManager->loadResourceFromXML(Element);

								Resource = new cResource();
                            }

                            if(AttribValue=="text")
                            {
                                //Resource = g_ConfigManager->loadResourceFromXML(Element);

                                Resource = new cResource();
                            }
                        }
					}

                    if(Resource)
                    {
                        m_Resources[Resource->m_Scope].push_back(Resource);
                        m_ResourceCount++;
                    }
                }
            }

            return true;
        }
    }

    return false;
}

//-------------------------------------------------------

void cResourceManager::setCurrentScope(unsigned int Scope)
{
    //Unload old scope, if not global scope.
    if(m_CurrentScope!=0)
    {
        std::list<cResource*>::iterator list_it;

        for(list_it=m_Resources[m_CurrentScope].begin();list_it!=m_Resources[m_CurrentScope].end();list_it++)
            (*list_it)->unload();
    }

    m_CurrentScope = Scope;

    //Load new scope.
    std::list<cResource*>::iterator list_it;

    for(list_it=m_Resources[m_CurrentScope].begin();list_it!=m_Resources[m_CurrentScope].end();list_it++)
        (*list_it)->load();
}

//-------------------------------------------------------
