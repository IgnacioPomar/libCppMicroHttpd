/*********************************************************************************************
*	Name		: StackLogger.h
*	Description	: Basic implementation of a WebLogger which allows easly integration with the web services
********************************************************************************************/

#pragma once
#ifndef _STACK_LOGGER_H_
#define _STACK_LOGGER_H_


#include "WebLogger.h"
#include "StdLogger.h"

/**
* Provide type-safe way to handle log levels within the library,
* offering functionality to compare log levels and convert them
* to strings for logging purposes
*/
class LIBHTTPD_API LogLevelWrapper
{
public:
	LogLevelWrapper () = default;
	explicit operator bool () = delete; // Prevent usage: if(LogLevel)
	operator LogLevel() const;

	constexpr LogLevelWrapper (LogLevel value);
	constexpr bool IsWorseThan (LogLevelWrapper logLevel) const;
	constexpr const char* toString () const;


private:
	LogLevel value;
};


/**
* Pure virtual class wich descendants will recieve the log entries
*/
class LIBHTTPD_API StackLoggerReceiver
{
public:
	virtual void add (const char* date, const char* log, LogLevelWrapper logLevel, const char* logLevelTxt) = 0;
};

class EventContainer;
class StackLoggerPrivateData;


/**
* The class itself
*/
class LIBHTTPD_API StackLogger : public WebLogger, public StdLogger
{
private:
	StackLoggerPrivateData* pd;
	void send (EventContainer& ec);

	//Prevent illegal usage
	StackLogger (const StackLogger&) = delete; // no copies
	StackLogger& operator=(const StackLogger&) = delete; // no self-assignments
	StackLogger (StackLogger&&) = delete; // no move constructor
	StackLogger& operator=(StackLogger&&) = delete; // no move assignments

public:
	StackLogger (unsigned int maxStoredEvents = 500, LogLevel logLevel = LogLevel::DontLog, const char* logPath = nullptr, const char* fileName = nullptr);
	~StackLogger ();

	//This is the "TRACE": it'll recieve events from WebLogger
	void sendToLog (const char* log);

	//the usual ones
	void log (LogLevel logLevel, const char* event);
	void trace (const char* event);
	void debug (const char* event);
	void info (const char* event);
	void error (const char* event);
	void fatal (const char* event);

	void getAll (StackLoggerReceiver& receiver);


	void setConsoleMode (LogLevel logLevel);
	void setFileMode (LogLevel logLevel, const char* logPath, const char* fileName);
	void delLogsOltherThan (int maxLogFileDays);
};


#endif //_STACK_LOGGER_H_
