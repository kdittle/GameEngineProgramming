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
	//Search through scopes
	for (auto &it : m_Resources)
	{
		if (!it.second.empty())
		{
			//Search through resources of scope
			for (auto &list_it : it.second)
			{
				//If matches ID
				if (list_it->m_ResourceID == UID)
					return list_it;
			}
		}
	}

	return NULL;
}

//-------------------------------------------------------

void cResourceManager::clear()
{
	//Search through scopes
	for (auto it : m_Resources)
	{
		if (!it.second.empty())
		{
			//Search through resources of scope
			for (auto* list_it : it.second)
			{
				//Delete resource object
				list_it->unload();
				SAFE_DELETE(list_it);
			}

			it.second.clear();
		}
	}

	m_Resources.clear();
}

//-------------------------------------------------------

bool cResourceManager::loadFromXMLFile(std::string Filename)
{
    XMLDocument doc(Filename.c_str());

    if(doc.LoadFile(Filename.c_str()) == XML_NO_ERROR)
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
	if (m_CurrentScope != 0)
	{
		for (auto* list_it : m_Resources[m_CurrentScope])
			list_it->unload();
	}

	m_CurrentScope = Scope;


	for (auto* list_it : m_Resources[m_CurrentScope])
		list_it->load();
}

//-------------------------------------------------------
