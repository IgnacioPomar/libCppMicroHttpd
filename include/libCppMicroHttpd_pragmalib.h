/*********************************************************************************************
*	Name		: libCppMicroHttpd_pragmalib.h
*	Description	: header with the libs used in this project
********************************************************************************************/

#pragma once
#ifndef _PRAGMA_LIB_LIB_CPP_MICRO_HTTPD_H_
#define _PRAGMA_LIB_LIB_CPP_MICRO_HTTPD_H_

//true to use static, false to use dll	
#if false

#ifdef _DEBUG 
#pragma comment(lib, "libmicrohttpd_d.lib")
#else
#pragma comment(lib, "libmicrohttpd.lib")
#endif

#else

#ifdef _DEBUG
#pragma comment(lib, "libmicrohttpd-dll_d.lib")
#else
#pragma comment(lib, "libmicrohttpd-dll.lib")
#endif

#endif

#endif // _PRAGMA_LIB_LIB_CPP_MICRO_HTTPD_H_
