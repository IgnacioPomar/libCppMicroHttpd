/*********************************************************************************************
*	Name		: StdLogger.h
*	Description	: Clean interface for a logger
********************************************************************************************/

#pragma once
#ifndef _STD_LOGGER_H_
#define _STD_LOGGER_H_


// Define an enumeration for different log levels with specific values.
enum class LogLevel : unsigned char
{
	DontLog = 0xff, // Highest value, used to indicate no logging should be performed.
	Trace = 0x00,   // For detailed information, useful during development.
	Debug = 0x01,   // For diagnostic information, helpful in debugging.
	Info = 0x02,    // For general runtime information.
	Warn = 0x03,    // For warnings, potential issues that are not errors.
	Error = 0x04,   // For errors, issues that may affect operation but not critical.
	Fatal = 0x05    // For critical issues, may require the program to terminate.
};


// Abstract base class for a standard logger.
class StdLogger
{
public:
	// Pure virtual function for logging messages with a specified log level.
	virtual void log (LogLevel logLevel, const char* event) = 0;

	// Pure virtual functions for logging messages at specific log levels.
	virtual void trace (const char* event) = 0;
	virtual void debug (const char* event) = 0;
	virtual void info (const char* event) = 0;
	virtual void error (const char* event) = 0;
	virtual void fatal (const char* event) = 0;
}; // class StdLogger;


#endif //_STD_LOGGER_H_

