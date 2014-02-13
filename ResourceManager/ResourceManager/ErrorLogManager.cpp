#include "ErrorLogManager.h"

cErrorLogManager cErrorLogManager::m_ErrorManager;

//------------------------------------------------------------------------

//cException

//------------------------------------------------------------------------

cException::cException(int ErrorNumber, std::string ErrorDesc, std::string SrcFileName, int LineNumber)
{
	m_ErrorNumber = ErrorNumber;
	m_ErrorDesc = ErrorDesc;
    m_SrcFileName = SrcFileName;
    m_LineNumber = LineNumber;

	std::stringstream ErrStr;

	ErrStr << "Error Num: " << m_ErrorNumber << "\nError Desc: " << m_ErrorDesc << "\nSrc File: "
		<< m_SrcFileName << "\nLine Number: " << m_LineNumber << "\n";

	m_ErrText = ErrStr.str();
}

//------------------------------------------------------------------------

const char* cException::what()
{
	return m_ErrText.c_str();
}

//------------------------------------------------------------------------

//cErrorLogManager

//------------------------------------------------------------------------

cErrorLogManager::cErrorLogManager()
{
}

//------------------------------------------------------------------------

cErrorLogManager* cErrorLogManager::GetErrorManager()
{
    return &m_ErrorManager;
}

//------------------------------------------------------------------------

//Opens log file
void cErrorLogManager::create(std::string Filename)
{
    m_LogFile.open(Filename.c_str());
}

//------------------------------------------------------------------------

//Commits information to file and clears text cache
void cErrorLogManager::flush()
{
    m_LogFile << m_LogBuffer.str();
    m_LogFile.flush();
    m_LogBuffer.str("");
}

//------------------------------------------------------------------------

//Close log file
void cErrorLogManager::close()
{
    m_LogFile.close();
}

//------------------------------------------------------------------------

void cErrorLogManager::logException(cException e)
{
    m_LogBuffer << getTimeString() << "\n" << e.what();
    flush();
}

//------------------------------------------------------------------------

//Gets current time as string in the form: hours:mins:secs
std::string cErrorLogManager::getTimeString()
{
    std::stringstream TimeStr;

    struct tm *pTime;
    time_t ctTime; time(&ctTime);
    pTime = localtime( &ctTime );

    TimeStr << std::setw(2) << std::setfill('0') <<pTime->tm_hour << ":";
    TimeStr << std::setw(2) << std::setfill('0') <<pTime->tm_min << ":";
    TimeStr << std::setw(2) << std::setfill('0') <<pTime->tm_sec;

    return TimeStr.str();
}

//------------------------------------------------------------------------