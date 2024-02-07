/*********************************************************************************************
*	Name		: libCppMicroHttpd_cfg.h
*	Description	: Configuration of the project
********************************************************************************************/

#pragma once
#ifndef _LIB_CPP_MICRO_HTTPD_H_
#define _LIB_CPP_MICRO_HTTPD_H_


//If the solution is a dinamic library (dll), we need the next macro
#define LIBHTTPD_DLL

//IMPORTANT: the project who exports must have the preprocessor macro LIBHTTPD_EXPORTS

//see http://gcc.gnu.org/wiki/Visibility
// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
#define LIBHTTPD_HELPER_DLL_IMPORT __declspec(dllimport)
#define LIBHTTPD_HELPER_DLL_EXPORT __declspec(dllexport)
#define LIBHTTPD_HELPER_DLL_LOCAL
#else
#if __GNUC__ >= 4
#define LIBHTTPD_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
#define LIBHTTPD_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
#define LIBHTTPD_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define LIBHTTPD_HELPER_DLL_IMPORT
#define LIBHTTPD_HELPER_DLL_EXPORT
#define LIBHTTPD_HELPER_DLL_LOCAL
#endif
#endif

// Now we use the generic helper definitions above to define LIBHTTPD_API and LIBHTTPD_LOCAL.
// LIBHTTPD_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// LIBHTTPD_LOCAL is used for non-api symbols.

#ifdef LIBHTTPD_DLL // defined if LIBHTTPD is compiled as a DLL
#ifdef LIBCPPMICROHTTPD_EXPORTS // defined if we are building the DLL (instead of using it)
#define LIBHTTPD_API LIBHTTPD_HELPER_DLL_EXPORT
#else
#define LIBHTTPD_API LIBHTTPD_HELPER_DLL_IMPORT
#endif // LIBHTTPD_EXPORTS
#define LIBHTTPD_LOCAL LIBHTTPD_HELPER_DLL_LOCAL
#else // LIBHTTPD_DLL is not defined: this means LIBHTTPD is a static lib.
#define LIBHTTPD_API
#define LIBHTTPD_LOCAL
#endif // LIBHTTPD_DLL




#ifdef __GNUC__
#define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED
#endif


#endif //_LIB_CPP_MICRO_HTTPD_H_
