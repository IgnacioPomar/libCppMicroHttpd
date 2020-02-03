/*********************************************************************************************
*	Name		: WebLogger.cpp
*	Description	: Abstract class to allow the configuration of a log class
*	Copyright	(C) 2020
********************************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include <string>

#include "WebParametersPrivateData.h"
#include "WebParameters.h"
#include "WebLogger.h"


void formatUrl (std::string & s, const char * url, const char * method, MHD_Connection * connection);


void WebLogger::logUrl (const char * url, const char * method, MHD_Connection * connection)
{
	std::string s;
	formatUrl (s, url, method, connection);

	this->sendToLog (s.c_str ());
}

void WebLogger::logParams (const char * url, const char * method, MHD_Connection * connection, WebParameters & requestParams)
{
	std::string s;
	formatUrl (s, url, method, connection);


	WebParametersPrivateData & wpPd = requestParams.getPrivateData ();

	for (auto node : wpPd.parameters)
	{
		s.append ("\n\t");
		s.append (node.first);
	}

	this->sendToLog (s.c_str ());

}

void WebLogger::logParamsContents (const char * url, const char * method, MHD_Connection * connection, WebParameters & requestParams)
{

	std::string s;
	formatUrl (s, url, method, connection);

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


//----------------------------------------------------------------------------------


#include <microhttpd.h>
#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime



void formatUrl (std::string & s, const char * url, const char * method, MHD_Connection * conn)
{
	//TODO: When C++20 arrives.. use std::format and std::chrono::format
	//TODO: Support ipV6

	//Current time to CString
	auto now = std::chrono::system_clock::now ();
	auto in_time_t = std::chrono::system_clock::to_time_t (now);
	struct tm buf;
	gmtime_s (&buf, &in_time_t);
	char str[26];
	strftime ((char *)str, sizeof (str), "%F %T UTC", &buf);



	//Get origin IP
	char ipBuff[17];
	struct sockaddr *addr = MHD_get_connection_info (conn, MHD_CONNECTION_INFO_CLIENT_ADDRESS)->client_addr;
	if (addr != nullptr)
	{
		snprintf (ipBuff, sizeof (ipBuff), "%d.%d.%d.%d",
			addr->sa_data[2] & 0xFF,
			addr->sa_data[3] & 0xFF,
			addr->sa_data[4] & 0xFF,
			addr->sa_data[5] & 0xFF);
	}
	else
	{
		strncpy (ipBuff, "-.-.-.-", 8);
	}

	//Compose the final line
	s.append (ipBuff).append ("\t");
	s.append ("[").append (str).append ("]\t");
	s.append ("\"").append (method).append (" ").append (url).append ("\"");
}
