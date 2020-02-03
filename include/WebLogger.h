/*********************************************************************************************
*	Name		: WebLogger.h
*	Description	:
*	Copyright	(C) 2020
********************************************************************************************/

#pragma once
#ifndef _WEB_LOGGER_ABSTRACT_H_
#define _WEB_LOGGER_ABSTRACT_H_


#include "libCppMicroHttpd_cfg.h"

class WebParameters;
struct MHD_Connection;

//class that will recive the updates
class LIBHTTPD_API WebLogger
{
public:
	void logUrl (const char * url, const char * method, MHD_Connection * connection);
	void logParams (const char * url, const char * method, MHD_Connection * connection, WebParameters & requestParams);
	void logParamsContents (const char * url, const char * method, MHD_Connection * connection, WebParameters & requestParams);


	virtual void sendToLog (const char * log) = 0;
};

#endif //_WEB_LOGGER_ABSTRACT_H_
