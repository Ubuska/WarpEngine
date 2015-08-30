#include "Logger.h"
#include <map>
#include <list>
#include <string>

#pragma region Constants, Statics, and Globals

using namespace std;

class LogManager
{
public:
	enum ErrorDialogResult
	{
		LOGMGR_ERROR_ABORT,
		LOGMGR_ERROR_RETRY,
		LOGMGR_ERROR_IGNORE
	};

	typedef std::map<string, unsigned char> Tags;
	//typedef std::list<LogManager::ErrorMessenger*> ErrorMessengerList;
	
	Tags m_tags;
	//ErrorMessengerList m_errorMessengers;
	
	// thread safety
	//CriticalSection m_tagCriticalSection;
	//CriticalSection m_messengerCriticalSection;

public:
	// construction
	LogManager(void);
	~LogManager(void);
    void Init(const char* loggingConfigFilename);

	// logs
	void Log(const string& tag, const string& message, const char* funcName, const char* sourceFile, unsigned int lineNum);
	void SetDisplayFlags(const std::string& tag, unsigned char flags);

	// error messengers
	//void AddErrorMessenger(Logger::ErrorMessenger* pMessenger);
	//LogMgr::ErrorDialogResult Error(const std::string& errorMessage, bool isFatal, const char* funcName, const char* sourceFile, unsigned int lineNum);

private:
	// log helpers
	void OutputFinalBufferToLogs(const string& finalBuffer, unsigned char flags);
	void WriteToLogFile(const string& data);
	void GetOutputBuffer(std::string& outOutputBuffer, const string& tag, const string& message, const char* funcName, const char* sourceFile, unsigned int lineNum);
};
#pragma endregion

#pragma region LogMgr class definition
//------------------------------------------------------------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------------------------------------------------------------
LogManager::LogManager(void)
{
	// set up the default log tags
	//SetDisplayFlags("ERROR", ERRORFLAG_DEFAULT);
	//SetDisplayFlags("WARNING", WARNINGFLAG_DEFAULT);
	//SetDisplayFlags("INFO", LOGFLAG_DEFAULT);
}


//------------------------------------------------------------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------------------------------------------------------------
LogManager::~LogManager(void)
{
	/*m_messengerCriticalSection.Lock();
    for (auto it = m_errorMessengers.begin(); it != m_errorMessengers.end(); ++it)
	{
		Logger::ErrorMessenger* pMessenger = (*it);
		delete pMessenger;
	}
	m_errorMessengers.clear();
	m_messengerCriticalSection.Unlock();*/
}


//------------------------------------------------------------------------------------------------------------------------------------
// This function builds up the log string and outputs it to various places based on the display flags (m_displayFlags).
//------------------------------------------------------------------------------------------------------------------------------------
/*
void LogManager::Log(const string& tag, const string& message, const char* funcName, const char* sourceFile, unsigned int lineNum)
{
	//m_tagCriticalSection.Lock();
	Tags::iterator findIt = m_tags.find(tag);
	if (findIt != m_tags.end())
	{
		//m_tagCriticalSection.Unlock();
		
		string buffer;
		GetOutputBuffer(buffer, tag, message, funcName, sourceFile, lineNum);
		OutputFinalBufferToLogs(buffer, findIt->second);
	}
	else
	{
		// Critical section is exited in the if statement above, so we need to exit it here if that didn't 
		// get executed.
        //m_tagCriticalSection.Unlock();
	}
	
}  // end LogMgr::Log()
*/