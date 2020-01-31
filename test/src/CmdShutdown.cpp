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
	bool execute (WebParameters & wsParams, std::string & response);
private:
	static void closeDaemon (ExampleContext* ec);
};


const char * CommandShutdown::getBaseUrl ()
{
	return "/exit/";
}

const char * CommandShutdown::getDescription ()
{
	return "Shutdowns the server";
}


#include <thread>
bool CommandShutdown::execute (WebParameters & wsParams, std::string & response)
{

	ExampleContext* ec = (ExampleContext*)wsParams.webContext;
	std::thread t1 (closeDaemon, ec);
	t1.detach ();
	//closeDaemon (ec);
	response = "Shuting down";
	return true;
}


#include <chrono>
void CommandShutdown::closeDaemon (ExampleContext * ec)
{
	std::this_thread::sleep_for (std::chrono::milliseconds (10000));
	ec->wp->stopDaemon ();
}


AUTOREGISTER_COMMAND (CommandShutdown);
