/*********************************************************************************************
*	Name		: StackLogger.h
*	Description	: Basic implementation of a WebLogger which allows easly integration with the web services
********************************************************************************************/

#include <filesystem>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <list>

#include "StackLogger.h"


static constexpr int MAX_DATE_SIZE = 26;
namespace fs = std::filesystem;


/**
* Local class it'll contain the info for a single event
*/
class LIBHTTPD_LOCAL EventContainer
{
public:
	std::string date;
	std::string event;
	LogLevelWrapper logLevel;
};

/**
* Local class: data for the StackLogger
*/
class LIBHTTPD_LOCAL StackLoggerPrivateData
{
public:
	std::list<EventContainer> events;


	std::string logName;
	std::string logPath;
	std::ofstream logfile;

	LogLevelWrapper fileLogLevel;
	LogLevelWrapper consoleLogLevel;
	unsigned int maxStoredEvents;
};


//------------------------------------------------
//------------------- LogLevel -------------------
//------------------------------------------------

/**
* Constructor
*/
constexpr  LogLevelWrapper::LogLevelWrapper (LogLevel value) : value (value)
{
}


/**
* Allow switch and comparisons
*/
LogLevelWrapper::operator LogLevel() const
{
	return value;
}

/**
* Tells if the current logLevel is Worse than the specified one
*/
constexpr bool LogLevelWrapper::IsWorseThan (LogLevelWrapper logLevel) const
{
	return (this->value >= logLevel.value);
}

/**
* Converts the log level into its sting
*/
constexpr const char* LogLevelWrapper::toString () const
{
	switch (this->value)
	{
	case LogLevel::Trace: return "TRACE"; break;
	case LogLevel::Debug: return "DEBUG"; break;
	case LogLevel::Info: return "INFO"; break;
	case LogLevel::Warn: return "WARN"; break;
	case LogLevel::Error: return "ERROR"; break;
	case LogLevel::Fatal: return "FATAL"; break;
	default: return "";
	}
}


//---------------------------------------------------
//------------------- StackLogger -------------------
//---------------------------------------------------

/**
* Constructor: creates the private data and sets the initial logs
*/
StackLogger::StackLogger (unsigned int maxStoredEvents, LogLevel logLevel, const char* logPath, const char* fileName)
{
	this->pd = new StackLoggerPrivateData ();

	this->setConsoleMode (logLevel);
	this->setFileMode (logLevel, logPath, fileName);
	pd->maxStoredEvents = maxStoredEvents;
}


/**
* Destructor: flush the logFile (if any), and destrys the private data
*/
StackLogger::~StackLogger ()
{
	if (pd->logfile.is_open ())
	{
		pd->logfile.flush ();
		pd->logfile.close ();
	}

	delete pd;
}


/**
* send to the corresponding out streams the formatted mesage
*/
void StackLogger::send (EventContainer& ec)
{
	std::string retVal = ec.logLevel.toString ();
	retVal.append ("\t\t").append (ec.date).append ("\t\t").append (ec.event);


	if (ec.logLevel.IsWorseThan (pd->consoleLogLevel))
	{
		std::cout << retVal << std::endl;
	}

	if (ec.logLevel.IsWorseThan (pd->fileLogLevel) && pd->logfile.is_open ())
	{
		pd->logfile << retVal << std::endl;
	}
}


/**
* Recieves the log from the WebLogger. Mark them as trace
*/
void StackLogger::sendToLog (const char* event)
{
	this->log (LogLevel::Trace, event);
}

/**
* Send the log with TRACE LogLevel
*/

void StackLogger::trace (const char* event)
{
	this->log (LogLevel::Trace, event);
}

/**
* Send the log with DEBUG LogLevel
*/
void StackLogger::debug (const char* event)
{
	this->log (LogLevel::Debug, event);
}

/**
* Send the log with INFO LogLevel
*/
void StackLogger::info (const char* event)
{
	this->log (LogLevel::Info, event);
}


/**
* Send the log with ERROR LogLevel
*/
void StackLogger::error (const char* event)
{
	this->log (LogLevel::Error, event);
}


/**
* Send the log with FATAL LogLevel
*/
void StackLogger::fatal (const char* event)
{
	this->log (LogLevel::Fatal, event);
}



/**
* Send all the events to the receiver
*/
void StackLogger::getAll (StackLoggerReceiver& receiver)
{
	for (EventContainer const& e : pd->events)
	{
		receiver.add (e.date.c_str (), e.event.c_str (), e.logLevel, e.logLevel.toString ());
	}
}

/**
* generate a EventContainer and send to wherever is needed
*/
void StackLogger::log (LogLevel logLevel, const char* event)
{
	//TODO: When C++20 arrives.. use std::format and std::chrono::format
	auto now = std::chrono::system_clock::now ();
	auto in_time_t = std::chrono::system_clock::to_time_t (now);
	struct tm buf;
	gmtime_s (&buf, &in_time_t);
	char str [MAX_DATE_SIZE];
	strftime ((char*) str, sizeof (str), "%F %T UTC", &buf);

	EventContainer ec;
	ec.date = str;
	ec.event = event;
	ec.logLevel = logLevel;

	pd->events.push_front (ec);
	if (pd->events.size () > pd->maxStoredEvents)
	{
		pd->events.pop_back ();
	}

	send (ec);
}


/**
* Set the LogLevel in wich the console will show info
*/
void StackLogger::setConsoleMode (LogLevel logLevel)
{
	pd->consoleLogLevel = logLevel;
}


/**
* Set the LogLevel in wich the logfile will show info and prepares the file
*/
void StackLogger::setFileMode (LogLevel logLevel, const char* logPath, const char* fileName)
{
	if (pd->fileLogLevel != LogLevel::DontLog)
	{
		pd->logfile.flush ();
		pd->logfile.close ();
	}


	pd->logPath = (logPath != nullptr) ? logPath : "";
	pd->logName = (fileName != nullptr) ? fileName : "";

	if ((pd->logPath.size () > 0) && (pd->logName.size () > 0))
	{
		pd->fileLogLevel = logLevel;
	}
	else
	{
		pd->fileLogLevel = LogLevel::DontLog;
	}

	//open the file if we must
	if (logLevel != LogLevel::DontLog)
	{
		std::string logFileName;

		fs::path p = logPath;

		//Make sure the path exists
		try
		{
			fs::create_directories (p);
		}
		catch (std::exception&)
		{
		}

		//Generamos el fichero de log
		logFileName = fs::absolute (p).u8string () + pd->logName;

		auto now = std::chrono::system_clock::now ();
		auto in_time_t = std::chrono::system_clock::to_time_t (now);
		struct tm buf;
		gmtime_s (&buf, &in_time_t);
		char str [MAX_DATE_SIZE];
		strftime ((char*) str, sizeof (str), "_%F.log", &buf);

		logFileName.append (str);

		//Abrimos el fichero
		pd->logfile.open (logFileName, std::ofstream::out | std::ofstream::app);


	}
}


/**
* When called will delete the older log files
*/
void StackLogger::delLogsOltherThan (int maxLogFileDays)
{
	if (maxLogFileDays > 0)
	{
		std::vector<fs::path> fileOlder;

		auto now = fs::file_time_type::clock::now ();
		int hours = maxLogFileDays * 24;

		for (const auto& p : fs::recursive_directory_iterator (pd->logPath))
		{
			if (fs::is_regular_file (p))
			{
				if (std::chrono::duration_cast<std::chrono::hours>(now - fs::last_write_time (p)).count () > hours)
				{
					fileOlder.push_back (p);
				}
			}
		}

		for (const auto& p : fileOlder)
		{
			fs::remove (p);
		}
	}
}
