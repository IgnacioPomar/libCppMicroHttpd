/*********************************************************************************************
*	Name		: WebProcess.h
*	Description	:
*	Copyright	(C) 2020
********************************************************************************************/

#pragma once
#ifndef _WEB_PROCESS_H_
#define _WEB_PROCESS_H_


#include "libCppMicroHttpd_cfg.h"

//library forward declarations
struct MHD_Connection;
struct MHD_Daemon;

enum ThreadModel : char {
	HTTP_USE_THREADS = 0x1,
	HTTP_USE_SELECT = 0x2
};


//YAGNI: In the future, may be interesting implement the rest of the flags and options

class LIBHTTPD_API WebProcess
{
public:
	WebProcess (int  portNumber, ThreadModel threadModel, void * context = nullptr);
	bool initDaemon ();
	bool stopDaemon ();

private:
	unsigned int flags;
	int portNumber;
	void * context;
	MHD_Daemon * httpDaemon;

	static int httpRequestReciever (void * context, MHD_Connection * connection, const char * url, const char * method, const char * version, const char * upload_data, size_t * upload_data_size, void ** ptr);


};


#endif //_WEB_PROCESS_H_
