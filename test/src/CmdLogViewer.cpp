/*********************************************************************************************
*	Name		: CmdLogViewer.cpp
*	Description	: Example web service command: show the log contents
********************************************************************************************/

#include <sstream>

#include "libCppMicroHttpd.h"
#include "ExampleContext.h"


/**
* this class will recieve the log lines ando format it
*/
class LogFormatter : public StackLoggerReceiver
{
private:
	std::ostringstream retVal;
public:
	void addHeader ();
	void add (const char* text);
	void add (const char* date, const char* log, LogLevel logLevel, const char* logLevelTxt);
	std::string getRetVal ();
};

/**
* Adds the header  (style and column titles) of the log
*/
void LogFormatter::addHeader ()
{
	retVal << "<style type = 'text/css'>span, pre {display: inline-block;} pre {vertical-align:top;margin: 0px;} .date{width: 195px;} .logLevel{width: 90px;} .head{font-size: large;font-weight:bolder;}</style>";
	retVal << "<div class='head'><span class='date'>Date</span><span class='logLevel'>Log level</span><span>Msg</span></div>";

}

/**
* Adds a log line
*/
void LogFormatter::add (const char* date, const char* log, LogLevel logLevel, const char* logLevelTxt)
{
	retVal << "<div><span class='date'>" << date << "</span><span class='logLevel'>" << logLevelTxt << "</span><pre>" << log << "</pre></div>";
}

/**
* Adds arbitrary text
*/
void LogFormatter::add (const char* text)
{
	retVal << text;
}

/**
* Returns a string with the full log
*/
std::string LogFormatter::getRetVal ()
{
	return retVal.str ();
}

//-------------------------------  Show logger service  -------------------------------
class CmdLogViewer : public WebCommand
{
public:
	CmdLogViewer () :WebCommand (true, true) {};
	const char* getBaseUrl ();
	const char* getDescription ();
	bool execute (WebParameters& wsParams, std::string& response);
private:

	static constexpr const char* url = "/LogViewer/";
};


const char* CmdLogViewer::getBaseUrl ()
{
	return url;
}

const char* CmdLogViewer::getDescription ()
{
	return "Show the aviable commands";
}


bool CmdLogViewer::execute (WebParameters& wsParams, std::string& response)
{
	ExampleContext* ec = (ExampleContext*) wsParams.webContext;
	LogFormatter  logFormatter;


	logFormatter.add ("<h1>LastEntries From the log</h1>");
	logFormatter.addHeader ();

	ec->logger->getAll (logFormatter);


	response = logFormatter.getRetVal ();

	return true;
}



AUTOREGISTER_COMMAND (CmdLogViewer);
