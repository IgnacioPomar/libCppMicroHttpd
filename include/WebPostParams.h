/*********************************************************************************************
*	Name		: WebPostParams.h
*	Description	: Helper class to parse and store POST parameters
*	Copyright	(C) 2020
********************************************************************************************/


#pragma once
#ifndef _WEB_POST_PARAMS_H_
#define _WEB_POST_PARAMS_H_


#include <microhttpd.h>
#include "libCppMicroHttpd_cfg.h"
#include "WebParameters.h"


class LIBHTTPD_LOCAL WebPostParams
{
private:
	static int postDataIterator (void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
		const char *filename, const char *content_type,
		const char *transfer_encoding, const char *data, uint64_t off,
		size_t size);
public:
	WebParameters wp;
	MHD_PostProcessor *postProcessor;
	~WebPostParams ();
	WebPostParams (void * context, MHD_Connection * connection);
};


#endif //_WEB_POST_PARAMS_H_
