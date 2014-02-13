// EngineStart.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <cstdlib>
#include <stdio.h>
#include <tchar.h>
#include "TinyXML.h"
#include "ErrorLogManager.h"
#include "ResourceManager.h"


int _tmain(int argc, _TCHAR* argv[])
{
    cResourceManager* ResourceManager = cResourceManager::GetResourceManager();
    ResourceManager->loadFromXMLFile("ResourceTree.xml");
    ResourceManager->setCurrentScope(0);

    return 0;
}

