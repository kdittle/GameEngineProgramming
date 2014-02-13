#ifndef ERRORLOGMANAGER_H_INCLUDED
#define ERRORLOGMANAGER_H_INCLUDED

//------------------------------------------------------------------------

#include "EngineObject.h"
#include <string>
#include <sstream>
#include <iostream>
#include <time.h>
#include <iomanip>
#include <fstream>
#include <exception>

#ifndef THROW_EXCEPTION
#define THROW_EXCEPTION(ErrorNum, ErrorDesc) throw cException(ErrorNum, ErrorDesc, __FILE__, __LINE__ );
#endif

//------------------------------------------------------------------------

//Custom exception class

class cException : public std::exception
{
    private:
    protected:
    public:
    int m_ErrorNumber;
    std::string m_ErrorDesc;
    std::string m_SrcFileName;
    int m_LineNumber;
	std::string m_ErrText;

    // Override std::exception::what
    const char* what();

    cException(int ErrorNumber, std::string ErrorDesc, std::string SrcFileName, int LineNumber);
    ~cException() throw() {}
};

//------------------------------------------------------------------------

//Singleton Logging Object

class cErrorLogManager : public cEngineObject
{
    public:
        static cErrorLogManager * GetErrorManager();

    protected:
        cErrorLogManager();
        virtual ~cErrorLogManager(){}
        static cErrorLogManager m_ErrorManager;

    public:
        std::stringstream m_LogBuffer;
        void create(std::string Filename);
        void flush();
        void close();
        void logException(cException e);
        std::string getTimeString();
        std::ofstream m_LogFile;
};

//------------------------------------------------------------------------

#endif // ERRORLOGMANAGER_H_INCLUDED
