/*********************************************************************************************
*	Name		: WebLogger.cpp
*	Description	: Abstract class to allow the configuration of a log class
*	Copyright	(C) 2020
********************************************************************************************/

#include <string>

#include "WebParametersPrivateData.h"
#include "WebParameters.h"
#include "WebLogger.h"


void formatUrl (std::string & s, const char * url);


void WebLogger::logUrl (const char * url)
{
	std::string s;
	formatUrl (s, url);

	this->sendToLog (s.c_str ());
}

void WebLogger::logParams (const char * url, WebParameters & requestParams)
{
	std::string s;
	formatUrl (s, url);


	WebParametersPrivateData & wpPd = requestParams.getPrivateData ();

	for (auto node : wpPd.parameters)
	{
		s.append ("\n\t");
		s.append (node.first);
	}

	this->sendToLog (s.c_str ());

}

void WebLogger::logParamsContents (const char * url, WebParameters & requestParams)
{

	std::string s;
	formatUrl (s, url);

	WebParametersPrivateData & wpPd = requestParams.getPrivateData ();

	for (auto node : wpPd.parameters)
	{
		s.append ("\n\t");
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
	}

	this->sendToLog (s.c_str ());
}


#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime


void formatUrl (std::string & s, const char * url)
{
	auto now = std::chrono::system_clock::now ();
	auto in_time_t = std::chrono::system_clock::to_time_t (now);


	struct tm buf;
	gmtime_s (&buf, &in_time_t);


	char str[26];
	strftime ((char *)str, sizeof (str), "%F %T UTC", &buf);

	s.append ("[").append (str).append ("]\t").append (url);
}
