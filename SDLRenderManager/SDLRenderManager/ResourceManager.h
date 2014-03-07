#ifndef RESOURCEMANAGER_H_INCLUDED
#define RESOURCEMANAGER_H_INCLUDED

#include "EngineObject.h"
#include <iostream>
#include <list>
#include <map>
#include <string>

//Tiny XML Header. Needs TinyXML library
//Available for free at: http://www.grinninglizard.com/tinyxml/
//#include "TinyXML.h"
#include "tinyxml2.h"

using namespace tinyxml2;

//Enum to indicate resource type
typedef enum{
    RESOURCE_NULL = 0,
    RESOURCE_GRAPHIC = 1,
    RESOURCE_MOVIE = 2,
    RESOURCE_AUDIO = 3,
    RESOURCE_TEXT = 4,
}RESOURCE_TYPE;

//-------------------------------------------------------

//Resource class. Represents a resource object.
//To be managed by a Resource Manager.
//Should be overriden by other classes
//and methods providing specific implementations
//RenderManager, for example, will implement a
//derived graphics resource class
class cResource : public cEngineObject
{
    private:
    protected:
    public:
        unsigned int m_ResourceID;
        unsigned int m_Scope;
        std::string m_FileName;
        RESOURCE_TYPE m_Type;
        bool m_bLoaded;

        //To be overloaded by derived classes
        virtual ~cResource(){};
        virtual void load(){};
        virtual void unload(){};

        inline cResource()
        {
            m_ResourceID = m_Scope = 0;
            m_Type = RESOURCE_NULL;
            m_bLoaded = false;
        }
};

//-------------------------------------------------------

//Resource Manager. Manages Resource Objects.
class cResourceManager : public cEngineObject
{
    private:
    protected:

    static cResourceManager m_ResourceManager;

    //Current resource scope
    unsigned int m_CurrentScope;

    //Total number of resources, both loaded and non-loaded
    unsigned int m_ResourceCount;

    inline cResourceManager()
    {
        m_CurrentScope = m_ResourceCount = 0;
    }

    public:

    static cResourceManager* GetResourceManager();

    //STL MAP of form <scope, resource_list>
    //Each scope element contains a list of resource for that scope
    std::map<unsigned int, std::list<cResource*> > m_Resources;

    //Finds resource by ID. Returns NULL if specified resource not found.
    cResource* findResourcebyID(unsigned int UID);

    //Clears all resources and scopes
    void clear();

    //Loads resources from XML File
    bool loadFromXMLFile(std::string Filename);

    //Sets the current scope. Depends on the scene currently loaded
    //Call this function with a valid scene scope
    //to load and unload appropriate resources
    void setCurrentScope(unsigned int Scope);

    const unsigned int getResourceCount(){return m_ResourceCount;}
};

//-------------------------------------------------------

#endif // RESOURCEMANAGER_H_INCLUDED
