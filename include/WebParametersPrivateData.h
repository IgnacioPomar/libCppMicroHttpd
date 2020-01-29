/*********************************************************************************************
*	Name		: WebParametersPrivateData.h
*	Description	:
********************************************************************************************/

#pragma once
#ifndef _WEB_PARAMETERS_PRIVATE_DATA_H_
#define _WEB_PARAMETERS_PRIVATE_DATA_H_

#include <string>
#include <unordered_map>

#include "libCppMicroHttpd_cfg.h"


class LIBHTTPD_LOCAL Parameter
{
public:
	std::string value;
	std::string defaultValue;
	bool isSet;
};


typedef std::unordered_map <std::string, Parameter> Parametros;


class LIBHTTPD_LOCAL WebParametersPrivateData
{
public:
	Parametros parameters;

};




#endif //_WEB_PARAMETERS_PRIVATE_DATA_H_
