/*********************************************************************************************
*	Name		: WebLogger.cpp
*	Description	: Abstract class to allow the configuration of a log class
*	Copyright	(C) 2020
********************************************************************************************/

#include <string>

#include "WebParametersPrivateData.h"
#include "WebParameters.h"
#include "WebLogger.h"

void WebLogger::logUrl (const char * url)
{
	this->sendToLog (url);
}

void WebLogger::logParams (const char * url, WebParameters & requestParams)
{
	std::string s (url);
	s.append ("\n");


	WebParametersPrivateData & wpPd = requestParams.getPrivateData ();

	for (auto node : wpPd.parameters)
	{
		s.append ("\t");
		s.append (node.first).append ("\n");
	}

	this->sendToLog (s.c_str ());

}

void WebLogger::logParamsContents (const char * url, WebParameters & requestParams)
{
	std::string s (url);
	s.append ("\n");

	WebParametersPrivateData & wpPd = requestParams.getPrivateData ();

	for (auto node : wpPd.parameters)
	{
		s.append ("\t");
		Parameter & p = node.second;
		s.append (node.first).append (" = ");
		if (p.isSet)
		{
			s.append (p.value);
		}
		else
		{
			s.append ("(default)").append (p.defaultValue);
		}
		s.append ("\n");
	}

	this->sendToLog (s.c_str ());
}
