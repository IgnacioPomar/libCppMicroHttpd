/*********************************************************************************************
*	Name		: CmdIndex.cpp
*	Description	: Example web service command: show all the aviable commands
********************************************************************************************/

#include <sstream>

#include "libCppMicroHttpd.h"
//#include "ExampleContext.h"



//-------------------------------  Shutdown service  -------------------------------
class CmdIndex : public WebCommand
{
public:
	CmdIndex () :WebCommand (true, true) {};
	const char* getBaseUrl ();
	const char* getDescription ();
	bool execute (WebParameters& wsParams, std::string& response);
private:
	static void addCmdInfo (WebCommand* webCmd, void* context);

	static constexpr const char* url = "/";
};


const char* CmdIndex::getBaseUrl ()
{
	return url;
}

const char* CmdIndex::getDescription ()
{
	return "Show the aviable commands";
}


bool CmdIndex::execute (WebParameters& wsParams, std::string& response)
{
	std::ostringstream retVal;

	retVal << "<h1>Aviable commands</h1>";;

	retVal << "<li><b>" << getBaseUrl () << "</b>: (Este servicio)" << getDescription () << " </li>";


	WebCommandRepository::listWebCommands (&addCmdInfo, &retVal);



	response = retVal.str ();

	return true;
}


void CmdIndex::addCmdInfo (WebCommand* webCmd, void* context)
{
	std::ostringstream& retVal = *(std::ostringstream*) context;

	//we are comparing the pointer direction.... as static it should be the same
	if (CmdIndex::url != webCmd->getBaseUrl ())
	{
		if (webCmd->isEnabled)
		{
			retVal << "<li><a href=\"" << webCmd->getBaseUrl () << "\">" << webCmd->getBaseUrl () << "</a>: " << webCmd->getDescription () << " </li>";
		}
		else
		{
			retVal << "<li style=\"color:#afafaf;\">" << webCmd->getBaseUrl () << ": " << webCmd->getDescription () << " </li>";
		}

	}
}


AUTOREGISTER_COMMAND (CmdIndex);
