/*********************************************************************************************
*	Name		: WebParameters.h
*	Description	:
********************************************************************************************/

#pragma once
#ifndef _WEB_PARAMETERS_H_
#define _WEB_PARAMETERS_H_

#include "libCppMicroHttpd_cfg.h"

enum Methods { GET, PUT, DEL, POST, UNKNOWN };

class WebParametersPrivateData;

class LIBHTTPD_API WebParameters
{
private:
	void addParam (const char * param, const char * data);
public:
	Methods method;
	void * webContext;

	WebParametersPrivateData *pd;



	WebParameters (WebParameters const&) = delete;
	WebParameters& operator=(WebParameters const&) = delete;
	WebParameters (void * webContext);
	~WebParameters ();



	double getDoubleParam (const char * param, bool & isFilled);
	unsigned int getUnsignedParam (const char * param, bool & isFilled);
	const char * getStringParam (const char * param, bool & isFilled);

	void append (const char * param, const char * data);
	void addDefaultValue (const char * param, const char * defaultValue);

	void setMethod (const char * strMethod);

	bool isSuportedMethod ();

	//static int parseQueryParameter (void *context, enum MHD_ValueKind kind, const char *key, const char *value);

};



#endif //_WEB_PARAMETERS_H_
