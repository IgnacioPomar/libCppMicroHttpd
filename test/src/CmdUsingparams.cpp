/*********************************************************************************************
*	Name		: CmdIndex.cpp
*	Description	: Example web service command: show all the aviable commands
********************************************************************************************/

#include <sstream>
#include <cstdlib> //To check the parameter

#include "libCppMicroHttpd.h"


//-------------------------------  Shutdown service  -------------------------------
class CmdUsingParams : public WebCommand
{
public:
	CmdUsingParams ();
	const char* getBaseUrl ();
	const char* getDescription ();
	bool execute (WebParameters& wsParams, std::string& response);


	static bool isOddNumber (const char* paramValue, void* context);
private:

	static constexpr const char* url = "/useParams/";
};



CmdUsingParams::CmdUsingParams () : WebCommand (true, true)
{
	this->addOption ("forced", "This parameter must be filled", true, "x");
	this->addOption ("optional", "This parameter is Optional", false, "100");
	this->addOption ("odd", "This parameter, if set, must be an odd number", false, "100", CmdUsingParams::isOddNumber);

	//We must set one of thisparameters
	this->addOption ("simple1", "This parameter must be filled", "complex", "18.0");
	this->addOption ("simple2", "This parameter must be filled", "complex", "0.99");
	this->addOption ("complex", "This parameter must be filled", false, "18.99");
}


const char* CmdUsingParams::getBaseUrl ()
{
	return url;
}

const char* CmdUsingParams::getDescription ()
{
	return "Example using params";
}



bool CmdUsingParams::isOddNumber (const char* paramValue, void* context)
{
	//ExampleContext* ec = (ExampleContext*)context;
	int number = atoi (paramValue);
	if (number == 0)
	{
		//Not a number or 0
		return false;
	}
	if (number % 2 == 0)
	{
		// is even.
		return false;
	}
	else
	{
		// is odd
		return true;
	}
}



bool CmdUsingParams::execute (WebParameters& wsParams, std::string& response)
{
	std::ostringstream retVal;

	retVal << "<h1>Params used</h1> <ul>";
	bool isFilled;

	const char* string;
	double dob;
	unsigned int ui;

	string = wsParams.getStringParam ("forced", isFilled);
	retVal << "<li><b>forced</b>: was filled with " << string << "</li>";

	ui = wsParams.getUnsignedParam ("optional", isFilled);
	if (isFilled)
	{
		retVal << "<li><b>optional</b>: was filled with " << ui << "</li>";
	}
	else
	{
		retVal << "<li><b>optional</b>: was <b>*NOT*</b> filled, its default value is " << ui << "</li>";
	}

	ui = wsParams.getUnsignedParam ("odd", isFilled);
	if (isFilled)
	{
		retVal << "<li><b>odd</b>: was filled with " << ui << "</li>";
	}
	else
	{
		retVal << "<li><b>odd</b>: was <b>*NOT*</b> filled, its default value is " << ui << "</li>";
	}


	dob = wsParams.getDoubleParam ("simple1", isFilled);
	if (isFilled)
	{
		retVal << "<li><b>simple1</b>: was filled with " << dob << "</li>";
	}
	else
	{
		retVal << "<li><b>simple1</b>: was <b>*NOT*</b> filled, and we must take its value from complex</li>";
	}

	dob = wsParams.getDoubleParam ("simple2", isFilled);
	if (isFilled)
	{
		retVal << "<li><b>simple2</b>: was filled with " << dob << "</li>";
	}
	else
	{
		retVal << "<li><b>simple2</b>: was <b>*NOT*</b> filled, and we must take its value from complex</li>";
	}


	dob = wsParams.getDoubleParam ("complex", isFilled);
	if (isFilled)
	{
		retVal << "<li><b>complex</b>: was filled with " << dob << "</li>";
	}
	else
	{
		retVal << "<li><b>complex</b>: was <b>*NOT*</b> filled, its default value is " << dob << "</li>";
	}





	retVal << "</ul>";
	response = retVal.str ();


	return true;
}



AUTOREGISTER_COMMAND (CmdUsingParams);
