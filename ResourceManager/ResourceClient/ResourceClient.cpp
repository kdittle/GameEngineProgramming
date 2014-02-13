// ResourceClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <cstdlib>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include "TinyXML.h"
#include "ErrorLogManager.h"
#include "ResourceManager.h"


int _tmain(int argc, _TCHAR* argv[])
{
	cResourceManager* ResourceManager = cResourceManager::GetResourceManager();
	std::string filename = "ResourceTree.xml";
	ResourceManager->loadFromXMLFile(filename);
	ResourceManager->setCurrentScope(0);

	std::list<cResource*> curItems = ResourceManager->m_Resources[0];
	std::list<cResource*>::iterator list_it;

	for (list_it = curItems.begin(); list_it != curItems.end(); list_it++)
		std::cout << (*list_it)->m_FileName << std::endl;

	std::cout << "End of program" << std::endl;

	system("PAUSE");

	return 0;
}

