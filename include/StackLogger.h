/*********************************************************************************************
*	Name		: StackLogger.h
*	Description	: Basic implementation of a WebLogger which allows easly integration with the web services
********************************************************************************************/



#include "WebLogger.h"

enum class LogLevelVal : unsigned char
{
	DONT_LOG = 0xff,
	TRACE = 0x00,
	DEBUG = 0x01,
	INFO = 0x02,
	WARN = 0x03,
	ERROR = 0x04,
	FATAL = 0x05
};

/**
* Simple class enum with functions: represents the severity of the log
*/
class LIBHTTPD_API LogLevel
{
public:
	LogLevel () = default;
	explicit operator bool () = delete; // Prevent usage: if(LogLevel)
	operator LogLevelVal() const;

	constexpr LogLevel (LogLevelVal value);
	constexpr bool IsWorseThan (LogLevel logLevel) const;
	constexpr const char * toString () const;


private:
	LogLevelVal value;
};

/**
* Pure virtual class wich descendants will recieve the log entries
*/
class LIBHTTPD_API StackLoggerReceiver
{
public:
	virtual void add (const char * date, const char * log, LogLevel logLevel, const char * logLevelTxt) = 0;
};

class EventContainer;
class StackLoggerPrivateData;


/**
* The class itself
*/
class LIBHTTPD_API StackLogger : public WebLogger
{
private:
	StackLoggerPrivateData *pd;
	void send (EventContainer & ec);

	//Prevent illegal usage
	StackLogger (const StackLogger&) = delete; // no copies
	StackLogger& operator=(const StackLogger&) = delete; // no self-assignments
	StackLogger (StackLogger&&) = delete; // no move constructor
	StackLogger& operator=(StackLogger&&) = delete; // no move assignments

public:
	StackLogger (unsigned int maxStoredEvents = 500, LogLevel logLevel = LogLevelVal::DONT_LOG, const char * logPath = nullptr, const char * fileName = nullptr);
	~StackLogger ();

	//This is the "TRACE": it'll recieve events from WebLogger
	void sendToLog (const char * log);

	//the usual ones
	void log (LogLevel logLevel, const char * event);
	void trace (const char * event);
	void debug (const char * event);
	void info (const char * event);
	void error (const char * event);
	void fatal (const char * event);

	void getAll (StackLoggerReceiver & receiver);


	void setConsoleMode (LogLevel logLevel);
	void setFileMode (LogLevel logLevel, const char * logPath, const char * fileName);
	void delLogsOltherThan (int maxLogFileDays);
};
