/*********************************************************************************************
*	Name		: CmdEnableDisable.cpp
*	Description	: Example web service command: One disabled and other service to enable/disable it
********************************************************************************************/

#include <sstream>

#include "libCppMicroHttpd.h"
//#include "ExampleContext.h"



//-------------------------------  Disabled service  -------------------------------
class CmdDisabled : public WebCommand
{
public:
	CmdDisabled () :WebCommand (true, false) {};
	const char * getBaseUrl ();
	const char * getDescription ();
	bool execute (WebParameters & wsParams, std::string & response);


	static constexpr const char * url = "/disabled/";
};


const char * CmdDisabled::getBaseUrl ()
{
	return url;
}

const char * CmdDisabled::getDescription ()
{
	return "A service disabled at lauchTime";
}


bool CmdDisabled::execute (WebParameters & wsParams, std::string & response)
{
	response = "<h1>Working!!</h1>";

	return true;
}



AUTOREGISTER_COMMAND (CmdDisabled);


//-------------------------------  Disabled service  -------------------------------
class CmdEnabler : public WebCommand
{
public:
	CmdEnabler () :WebCommand (true, true) {};
	const char * getBaseUrl ();
	const char * getDescription ();
	bool execute (WebParameters & wsParams, std::string & response);
private:
	bool currentStatus = false;
	static constexpr const char * url = "/enabler/";
};


const char * CmdEnabler::getBaseUrl ()
{
	return url;
}

const char * CmdEnabler::getDescription ()
{
	return "A service to enable/disable others";
}


bool CmdEnabler::execute (WebParameters & wsParams, std::string & response)
{
	currentStatus = !currentStatus;
	WebCommandRepository::setCommandStatus (CmdDisabled::url, currentStatus);
	response = "<h1>Changed</h1>";
	return true;
}



AUTOREGISTER_COMMAND (CmdEnabler);
