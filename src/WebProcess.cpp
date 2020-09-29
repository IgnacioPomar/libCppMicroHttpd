/*********************************************************************************************
*	Name		: WebProcess.cpp
*	Description	:
*	Copyright	(C) 2020
********************************************************************************************/
#include <string>
#include <utility> //For std::move
#include <microhttpd.h>

#include "WebLogger.h"
#include "WebCommandRepository.h"
#include "WebParameters.h"
#include "WebPostParams.h"
#include "WebCommand.h"
#include "WebProcess.h"

WebProcess::WebProcess (int portNumber, ThreadModel threadModel, void * context)
	:portNumber (portNumber), context (context), launched (false),
	logger (nullptr), logMode (LogMode::NO_DEBUG)
{
	setThreadModel (threadModel);

	httpDaemon = nullptr;
}

/**
* Launch the processDaemon
* \see	https://www.gnu.org/software/libmicrohttpd/manual/html_node/microhttpd_002dinit.html
*/
bool WebProcess::initDaemon ()
{
	httpDaemon = MHD_start_daemon (flags, portNumber, nullptr, nullptr,
		&httpRequestReciever, this,
		MHD_OPTION_NOTIFY_COMPLETED, &httpRequestCompleted, nullptr,
		MHD_OPTION_END);
	launched = (httpDaemon != nullptr);
	return launched;
}


/**
* Stops the processDaemon
*/
bool WebProcess::stopDaemon ()
{
	launched = false;
	if (httpDaemon != nullptr)
		MHD_stop_daemon (httpDaemon);


	return true;
}

bool WebProcess::isOnline ()
{
	return launched;
}

bool WebProcess::setPort (int portNumber)
{
	if (!launched)
	{
		this->portNumber = portNumber;
	}
	return launched;
}


bool WebProcess::setThreadModel (ThreadModel threadModel)
{
	if (!launched)
	{
		flags = (threadModel == ThreadModel::HTTP_USE_SELECT) ?
			MHD_USE_SELECT_INTERNALLY :
			MHD_USE_THREAD_PER_CONNECTION;
#if _DEBUG
		flags |= MHD_USE_DEBUG;
#endif
	}
	return launched;
}


void WebProcess::setDebugMode (LogMode logMode, WebLogger * logger)
{
	this->logger = logger;
	this->logMode = (logger == nullptr) ? LogMode::NO_DEBUG : logMode;
}


/**
* Dummy var to haca a copnstant memory point
*/
static int dummy;

/**
* Invoked to notify the application about completed requests.
*
* \see https://www.gnu.org/software/libmicrohttpd/manual/html_node/microhttpd_002dcb.html
* \param    [in]   context			custom value selected at callback registration time
* \param    [in]   connection		Connection handle - Internal use -
* \param    [in]   ptr				Reference to a pointer, value as set by the last call of
*                                   httpRequestReciever in the current connection
* \param    [in]   toe				reason for request termination see MHD_OPTION_NOTIFY_COMPLETED.
*/
void WebProcess::httpRequestCompleted (void * context, MHD_Connection * connection, void ** ptr, MHD_RequestTerminationCode toe)
{
	if (*ptr != nullptr && *ptr != &dummy)
	{
		WebPostParams * pwp = (WebPostParams *)*ptr;
		delete pwp;
		*ptr = nullptr;
	}
}



/**
* Esta función se llama para configurar los parámetros de entrada del servidor
* \param    [in]   context		Contexto que se pasa desde la función receptora del servicio
* \param    [in]   kind		Tipo de valor
* \param    [in]   key			Clave query
* \param    [in]   value		Valor de la clave query
* \return	Siempre MHD_YES: No hay posibilidad de no aceptar un parametro
*/
int WebProcess::parseQueryParameter (void * context, MHD_ValueKind kind, const char * key, const char * value)
{
	WebParameters * wc = (WebParameters *)context;
	wc->addParam (key, value);

	return MHD_YES;
}



/**
* Invoked in the context of a connection to answer a request from the client.
* This callback must call MHD functions to provide content to give back to the
* client and return an HTTP status code (i.e. 200 for OK, 404, etc.).
*
* \see https://www.gnu.org/software/libmicrohttpd/manual/html_node/microhttpd_002dcb.html
* \param    [in]   context			custom value selected at callback registration time
* \param    [in]   connection		Connection handle - Internal use -
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
int WebProcess::httpRequestReciever (void * webProcess, MHD_Connection * connection, const char * url, const char * methodCStr, const char * version, const char * upload_data, size_t * upload_data_size, void ** ptr)
{
	WebProcess * wp = (WebProcess *)webProcess;
	//--------------- first fase: parse the parameters ---------------

	// The first time only the headers are valid,   do not respond in the first round... 
	if (*ptr == nullptr)
	{
		*ptr = &dummy;
		if (wp->logMode == LogMode::DEBUG_URL)
		{
			wp->logger->logUrl (url, methodCStr, connection);
		}

		return MHD_YES;
	}

	Method method = Method::UNKNOWN;


	//Process post uploads
	if (*upload_data_size)
	{
		WebPostParams * postParams;
		if (*ptr == &dummy)
		{
			method = WebParameters::getMethod (methodCStr);

			if (!WebParameters::isMethodWithFiles (method))
			{
				//Not suported Method for file uploading
				return MHD_NO;
			}


			postParams = new WebPostParams (wp->context, connection);
			*ptr = postParams;

			postParams->wp.method = method;

			return MHD_YES;
		}
		else
		{
			postParams = (WebPostParams *)*ptr;
		}

		MHD_post_process (postParams->postProcessor, upload_data, *upload_data_size);
		*upload_data_size = 0;
		return MHD_YES;
	}

	// Now, query parameters
	WebParameters requestParams (wp->context);
	if (*ptr != &dummy)
	{
		//we have parsed POST parameters
		WebPostParams * pwp = (WebPostParams *)*ptr;
		requestParams = std::move (pwp->wp);
	}
	else
	{
		requestParams.method = WebParameters::getMethod (methodCStr);
	}
	MHD_get_connection_values (connection, MHD_GET_ARGUMENT_KIND, parseQueryParameter, &requestParams);


	if (wp->logMode == LogMode::DEBUG_CONTENT_PARAMS)
	{
		wp->logger->logParamsContents (url, methodCStr, connection, requestParams);

	}
	else if (wp->logMode == LogMode::DEBUG_USED_PARAMS)
	{
		wp->logger->logParams (url, methodCStr, connection, requestParams);
	}


	//--------------- Second fase: call the service ---------------
	bool isOk = true;
	std::string responseBody = "";
	unsigned int httpCode = MHD_HTTP_OK;
	WebCommand *comando = WebCommandRepository::findResource (url);
	if (comando == nullptr)
	{
		httpCode = MHD_HTTP_NOT_FOUND;
	}
	else if (!comando->checkOptsOrHelp (requestParams, responseBody))
	{
		httpCode = MHD_HTTP_UNPROCESSABLE_ENTITY; // Incoorrect parametters
	}
	else if (!comando->execute (requestParams, responseBody))
	{
		// error during execution
		httpCode = MHD_HTTP_INTERNAL_SERVER_ERROR;
	}



	// Prepare oputput 
	const char * page = responseBody.c_str ();

	//--------------- final fase: the response ---------------
	int ret;
	struct MHD_Response * response;
	//Posibles valores:
	// MHD_RESPMEM_PERSISTENT  - Buffer is a persistent (static/global) buffer that won't change for at least the lifetime of the response
	// MHD_RESPMEM_MUST_FREE   - Buffer is heap-allocated with 'malloc' (or equivalent) and should be freed by MHD
	// MHD_RESPMEM_MUST_COPY   - Buffer is in transient memory and only valid during the call to 'MHD_create_response_from_buffer'. MHD must make its own private copy of the data for processing.
	response = MHD_create_response_from_buffer (strlen (page), (void*)page, MHD_RESPMEM_MUST_COPY);
	ret = MHD_queue_response (connection, httpCode, response);
	MHD_destroy_response (response);
	return ret;
}
