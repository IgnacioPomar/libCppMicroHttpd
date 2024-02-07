/*********************************************************************************************
*	Name		: WebParameters.h
*	Description	:
********************************************************************************************/

#pragma once
#ifndef _WEB_PARAMETERS_H_
#define _WEB_PARAMETERS_H_

#include "libCppMicroHttpd_cfg.h"

enum Method { GET, PUT, DEL, POST, UNKNOWN };

class WebParametersPrivateData;

class LIBHTTPD_API WebParameters
{
public:
	Method method;
	void* webContext;

	WebParametersPrivateData* pd;



	WebParameters (WebParameters const&) = delete;
	WebParameters& operator=(WebParameters const&) = delete;
	WebParameters (void* webContext);
	~WebParameters ();

	void addParam (const char* param, const char* data);

	//move constructor and assignement
	WebParameters& operator=(WebParameters&& other);
	WebParameters (WebParameters&& other);



	double getDoubleParam (const char* param, bool& isFilled);
	unsigned int getUnsignedParam (const char* param, bool& isFilled);
	const char* getStringParam (const char* param, bool& isFilled);

	void append (const char* param, const char* data);
	void addDefaultValue (const char* param, const char* defaultValue);

	static Method getMethod (const char* strMethod);

	static bool isMethodWithFiles (Method method);

	WebParametersPrivateData& LIBHTTPD_LOCAL getPrivateData ();

};



#endif //_WEB_PARAMETERS_H_
