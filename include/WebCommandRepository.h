/*********************************************************************************************
*	Name		: WebCommandRepository.h
*	Description	:
*	Copyright	(C) 2020
********************************************************************************************/

#pragma once
#ifndef _WEB_COMMAND_REPOSITORY_H_
#define _WEB_COMMAND_REPOSITORY_H_

#include "libCppMicroHttpd_cfg.h"

class WebCommand;
class CommandContainer;

//Callback to inform about the commands aviable
typedef void (*callbackInfoCmd)(WebCommand *, void *);

//YAGNI: make it work with non static Commands (if working with dinamically load libraries)
class LIBHTTPD_API WebCommandRepository
{
public:
	static WebCommand *findResource (const char * url);
	static void addCommand (WebCommand* webCommand);
	static void setCommandStatus (const char * url, bool isEnabled);

	static void listWebCommands (callbackInfoCmd infoCmd, void * context);
private:
	static CommandContainer & staticCommands ();

};

//------ Helper to statically register the services -----
class LIBHTTPD_API StaticWebCommandAutoRegister
{
public:
	StaticWebCommandAutoRegister (WebCommand * comando);
};
#define AUTOREGISTER_COMMAND(Cmd)	StaticWebCommandAutoRegister AutoRegister_##Cmd ( new Cmd ());



#endif //_WEB_COMMAND_REPOSITORY_H_
