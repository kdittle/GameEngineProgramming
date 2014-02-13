// ErrorMgrClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdlib>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include "ErrorLogManager.h"


int _tmain(int argc, _TCHAR* argv[])
{
	cErrorLogManager* Log = cErrorLogManager::GetErrorManager();

	try
	{
		Log->create("testlog.txt");
		THROW_EXCEPTION(1, "this is my error");
	}
	catch (cException& e)
	{
		MessageBoxA(NULL, e.what(), "", MB_OK);

		Log->m_LogBuffer << "*****ERROR*****\n";
		Log->flush();
		Log->logException(e);
		Log->m_LogBuffer << "***************\n";
		Log->flush();
	}

	Log->close();

	return 0;
}

