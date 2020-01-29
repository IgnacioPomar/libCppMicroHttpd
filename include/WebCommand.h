/*********************************************************************************************
*	Name		: WebCommand.h
*	Description	: Abstract generic Web command
********************************************************************************************/

#pragma once
#ifndef _WEB_COMMAND_H_ABSTRACT
#define _WEB_COMMAND_H_ABSTRACT


#include "libCppMicroHttpd_cfg.h"


//#include <functional> typedef std::function<bool (std::string &, void *)> CallbackCheck;
typedef bool (*callbackCheck)(const char *, void *);

class WebParameters;

class Option;
class WebCommandPrivateData;




class LIBHTTPD_API WebCommand
{
private:
	WebCommandPrivateData *pd;

public:
	WebCommand (bool hasAutomaticOpts);
	~WebCommand ();
	WebCommand (WebCommand const&) = delete;
	WebCommand& operator=(WebCommand const&) = delete;

	virtual const char * getBaseUrl () = 0;
	virtual const char * getDescription () = 0;

	virtual const char * getHelp (const char * errMsg = nullptr);


	void addOption (const char * optName, const char * description, bool isRequired, const char * defVal, callbackCheck checkFunc = nullptr);
	void addOption (const char * optName, const char * description, const char *alternativeOption, const char * defVal, callbackCheck checkFunc = nullptr);

	const char * checkOptsOrHelp (WebParameters & wsParams, bool & isAtItShould);
	const char * checkOption (WebParameters & wsParams, Option &opt, bool & isAtItShould);

	virtual bool execute (WebParameters & wsParams, std::string & response) = 0;
};

#endif //_WEB_COMMAND_H_ABSTRACT
