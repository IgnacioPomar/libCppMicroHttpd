/*********************************************************************************************
*	Name		: WebProcess.cpp
*	Description	:
*	Copyright	(C) 2020
********************************************************************************************/

#include <microhttpd.h>

#include "WebProcess.h"

WebProcess::WebProcess (int portNumber, ThreadModel threadModel, void * context)
	:portNumber (portNumber), context (context)
{
	flags = (threadModel == ThreadModel::HTTP_USE_SELECT) ?
		MHD_USE_SELECT_INTERNALLY :
		MHD_USE_THREAD_PER_CONNECTION;
#if _DEBUG
	flags |= MHD_USE_DEBUG;
#endif

	httpDaemon = nullptr;
}

/**
* Launch the processDaemon
* \see	https://www.gnu.org/software/libmicrohttpd/manual/html_node/microhttpd_002dinit.html
*/
bool WebProcess::initDaemon ()
{
	httpDaemon = MHD_start_daemon (flags, portNumber, nullptr, nullptr, &httpRequestReciever, context, MHD_OPTION_END);
	return (httpDaemon != nullptr);
}


/**
* Stops the processDaemon
*/
bool WebProcess::stopDaemon ()
{
	if (httpDaemon != nullptr)
		MHD_stop_daemon (httpDaemon);

	return true;
}




/**
* Invoked in the context of a connection to answer a request from the client.
* This callback must call MHD functions to provide content to give back to the
* client and return an HTTP status code (i.e. 200 for OK, 404, etc.).
*
* \see https://www.gnu.org/software/libmicrohttpd/manual/html_node/microhttpd_002dcb.html
* \param    [in]   context			custom value selected at callback registration time
* \param    [in]   connection		- Internal use -
* \param    [in]   url				the URL requested by the client
* \param    [in]   method			the HTTP method used by the client (GET, PUT, DELETE, POST, etc.)
* \param    [in]   version			the HTTP version string (i.e. HTTP/1.1);
* \param    [in]   upload_data		the data being uploaded (excluding headers):
*                                   POST data will be made available incrementally in upload_data
* \param    [in]   upload_data_size
* \param    [in]   ptr				reference to a pointer, initially set to NULL,
*                                   that this callback can set to some address and that
*                                   will be preserved by MHD for future calls for this request;
*                                   since the access handler may be called many times (i.e., for a PUT/POST operation with plenty of upload data)
* \return	Must return MHD_YES if the connection was handled successfully, MHD_NO if the socket must be closed due to a serious error while handling the request
*/
int WebProcess::httpRequestReciever (void * context, MHD_Connection * connection, const char * url, const char * method, const char * version, const char * upload_data, size_t * upload_data_size, void ** ptr)
{
	static int dummy;
	struct MHD_Response * response;
	int ret;

	/*
	wsParams.setMethod (method);
	if (!wsParams.isSuportedMethod ())
	{
		return MHD_NO; // unexpected method
	}
	*/

	// The first time only the headers are valid,   do not respond in the first round... 
	if (&dummy != *ptr)
	{
		*ptr = &dummy;
		return MHD_YES;
	}


	//YAGNI: Implementar la subida de ficheros meduiante POST
	//  https://www.gnu.org/software/libmicrohttpd/manual/html_node/microhttpd_002dpost.html
	if (0 != *upload_data_size)
		return MHD_NO; // De momento no soportamos ficheros
	*ptr = NULL; // clear context pointer 


	// Obtenemos los parametros query
	//MHD_get_connection_values (connection, MHD_GET_ARGUMENT_KIND, WebServiceParameters::parseQueryParameter, &wsParams);



	//SrvcEjepmploServidorWeb * wc = (SrvcEjepmploServidorWeb *)srvcEjemplo;
	//const char * page = wc->getDescription ();
	const char * page = "Hello";

	/*
	if (!wc->commands.executeResource (url, wsParams, ss))
	{
		printf ("[Descartado] %s - %s\n", method, url);
		return MHD_NO; // No es un servico esperado
	}
	*/




	// Preparamos la salida final
	//const char * page = ss.c_str ();

	//Posibles valores:
	// MHD_RESPMEM_PERSISTENT  - Buffer is a persistent (static/global) buffer that won't change for at least the lifetime of the response
	// MHD_RESPMEM_MUST_FREE   - Buffer is heap-allocated with 'malloc' (or equivalent) and should be freed by MHD
	// MHD_RESPMEM_MUST_COPY   - Buffer is in transient memory and only valid during the call to 'MHD_create_response_from_buffer'. MHD must make its own private copy of the data for processing.
	response = MHD_create_response_from_buffer (strlen (page), (void*)page, MHD_RESPMEM_MUST_COPY);
	ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
	MHD_destroy_response (response);
	return ret;
}
