/*********************************************************************************************
*	Name		: WebPostParams.h
*	Description	:
*	Copyright	(C) 2020
********************************************************************************************/

#include "WebPostParams.h"

constexpr size_t HTTP_BUFF_SIZE = 512;



WebPostParams::WebPostParams (void * context, MHD_Connection * connection) : wp (context)
{
	//YAGNI: suportt extra configuration: files in memory or in file
	postProcessor = MHD_create_post_processor (connection, HTTP_BUFF_SIZE, &postDataIterator, (void *)&wp);
}


WebPostParams::~WebPostParams ()
{
	MHD_destroy_post_processor (postProcessor);
}



/**
* Used for processing POST data.
* Iterator over key-value pairs where the value maybe made available in increments and/or may not be zero-terminated.
* \see		https://www.gnu.org/software/libmicrohttpd/manual/html_node/microhttpd_002dcb.html#index-_002aMHD_005fPostDataIterator
* \param    [in]	cls						custom value selected at callback registration time.
* \param    [in]	kind					type of the value
* \param    [in]	key						zero-terminated key for the value.
* \param    [in]	filename				name of the uploaded file, NULL if not known.
* \param    [in]	content_type			mime-type of the data, NULL if not known.
* \param    [in]	transfer_encoding		encoding of the data, NULL if not known.
* \param    [in]	data					pointer to size bytes of data at the specified offset.
* \param    [in]	off						offset of data in the overall value.
* \param    [in]	size					number of bytes in data available.
*/
int WebPostParams::postDataIterator (void * coninfo_cls, MHD_ValueKind kind, const char * key, const char * filename, const char * content_type, const char * transfer_encoding, const char * data, uint64_t off, size_t size)
{
	WebParameters *wp = (WebParameters *)coninfo_cls;

	//TODO: Support binary data and 
	wp->append (key, data);
	return MHD_YES;
}
