/*********************************************************************************************
*	Name		: WebCommand.h
*	Description	: Abstract generic Web command
********************************************************************************************/

#pragma once
#ifndef _WEB_COMMAND_H_ABSTRACT
#define _WEB_COMMAND_H_ABSTRACT


#include "libCppMicroHttpd_cfg.h"


//#include <functional> typedef std::function<bool (std::string &, void *)> CallbackCheck;
typedef void (*callbackCheck)(const char *, void *);

class WebServiceParameters;

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

	const char * checkOptsOrHelp (WebServiceParameters & wsParams, bool & isAtItShould);
	const char * checkOption (WebServiceParameters & wsParams, Option &opt, bool & isAtItShould);

	virtual bool execute (WebServiceParameters & wsParams, std::string & response) = 0;
};


//------ Helper to statically register the services -----
class LIBHTTPD_API StaticWebCommandAutoRegister
{
public:
	StaticWebCommandAutoRegister (WebCommand * comando);
};
#define AUTOREGISTRO_COMANDO(Cmd)	StaticWebCommandAutoRegister AutoRegister_##Cmd ( new Cmd ());

#endif //_WEB_COMMAND_H_ABSTRACT
