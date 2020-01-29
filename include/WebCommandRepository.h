/*********************************************************************************************
*	Name		: WebCommandRepository.h
*	Description	:
*	Copyright	(C) 2020
********************************************************************************************/

#pragma once
#ifndef _WEB_COMMAND_REPOSITORY_H_
#define _WEB_COMMAND_REPOSITORY_H_


class WebCommand;
class CommandContainer;

//YAGNI: make it work with non static Commands (if working with dinamically load libraries)
class LIBHTTPD_LOCAL WebCommandRepository
{
public:
	static WebCommand *findResource (const char * url);
	static void addCommand (WebCommand* webCommand);
private:
	static CommandContainer & staticCommands ();

};

//------ Helper to statically register the services -----
class LIBHTTPD_API StaticWebCommandAutoRegister
{
public:
	StaticWebCommandAutoRegister (WebCommand * comando);
};
#define AUTOREGISTRO_COMANDO(Cmd)	StaticWebCommandAutoRegister AutoRegister_##Cmd ( new Cmd ());



#endif //_WEB_COMMAND_REPOSITORY_H_
