/*********************************************************************************************
*	Name		: CmdShutdown.cpp
*	Description	: Example web service command: shudowns the server
********************************************************************************************/


#include "libCppMicroHttpd.h"
#include "ExampleContext.h"


//-------------------------------  Shutdown service  -------------------------------
class CommandShutdown : public WebCommand
{
public:
	CommandShutdown () :WebCommand (true) {};
	const char * getBaseUrl ();
	const char * getDescription ();
	const char * execute (WebParameters & wsParams, bool & isAtItShould);
};


const char * CommandShutdown::getBaseUrl ()
{
	return "/exit/";
}

const char * CommandShutdown::getDescription ()
{
	return "Shutdowns the server";
}


const char * CommandShutdown::execute (WebParameters & wsParams, bool & isAtItShould)
{
	isAtItShould = true;

	ExampleContext* ec = (ExampleContext*)wsParams.webContext;
	ec->wp->stopDaemon ();

	return "Shuting down";
}


AUTOREGISTER_COMMAND (CommandShutdown);
