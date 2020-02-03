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

//class that will recive the updates
class LIBHTTPD_API WebLogger
{
public:
	void logUrl (const char * url);
	void logParams (const char * url, WebParameters & requestParams);
	void logParamsContents (const char * url, WebParameters & requestParams);


	virtual void sendToLog (const char * log) = 0;
};

#endif //_WEB_LOGGER_ABSTRACT_H_
