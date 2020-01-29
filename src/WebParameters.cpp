/*********************************************************************************************
*	Name		: WebParameters.h
*	Description	:
********************************************************************************************/

#include <microhttpd.h>


#include  "WebParametersPrivateData.h"

#include "WebParameters.h"

/*
#include <map>
static const std::map<Methods, std::string> HTTP_METHODS_STR
{
	{GET, "GET"},
	{PUT, "PUT"},
	{DEL, "DELETE"},
	{POST, "POST"},
	{UNKNOWN, "UNKNOWN"}
};
*/

WebParameters::WebParameters (void * webContext) :webContext (webContext)
{
	pd = new WebParametersPrivateData ();
}

WebParameters::~WebParameters ()
{
	delete pd;
}


/**
*
* \param    [in]   chMethod
*/
void WebParameters::setMethod (const char * chMethod)
{
	std::string strMethod (chMethod);

	if (strMethod == "GET")
	{
		method = Methods::GET;
	}
	else if (strMethod == "POST")
	{
		method = Methods::POST;
	}
	else if (strMethod == "PUT")
	{
		method = Methods::PUT;
	}
	else if (strMethod == "DELETE")
	{
		method = Methods::DEL;
	}
	else
	{
		method = Methods::UNKNOWN;
	}
}


/**
* Check if is one of the supported Methods
*/
bool WebParameters::isSuportedMethod ()
{
	return ((method == Methods::GET) || (method == Methods::POST));
}


/**
*
* \param    [in]	param
* \param    [in]	data
*/
void WebParameters::append (const char * param, const char * data)
{
	//TODO: posible Bug si se trata de fichero binario
	Parametros::iterator it = pd->parameters.find (param);
	if (it == pd->parameters.end ())
	{
		addParam (param, data);
	}
	else
	{
		it->second.value.append (data);
	}
}


/**
*
* \param    [in]	param
* \param    [in]	data
*/
void WebParameters::addParam (const char * param, const char * data)
{
	Parameter parameter;
	parameter.isSet = true;
	parameter.value = (data == nullptr) ? "" : data;
	pd->parameters[param] = parameter;
}



/**
*
* \param    [in]	param
* \param    [out]	isFilled
*/
const char * WebParameters::getStringParam (const char * param, bool & isFilled)
{
	Parametros::iterator it = pd->parameters.find (param);

	if (it != pd->parameters.end ())
	{
		Parameter & param = it->second;
		isFilled = param.isSet;
		return (param.isSet) ? param.value.c_str () : param.defaultValue.c_str ();
	}
	else
	{
		isFilled = false;
		return "";
	}
}


/**
*
* \param    [in]	param
* \param    [out]	isFilled
* \return
*/
double WebParameters::getDoubleParam (const char * param, bool & isFilled)
{
	Parametros::iterator it = pd->parameters.find (param);

	if (it != pd->parameters.end ())
	{
		Parameter & param = it->second;
		std::string & val = (param.isSet) ? param.value : param.defaultValue;
		try
		{
			double retVal = std::stod (val);
			isFilled = param.isSet;
			return retVal;
		}
		catch (...)
		{
			//const std::exception& e 
			//Puede ser o bien  std::out_of_range, o bien std::invalid_argument
			//En cualquier caso, ignoraremos el parámetro
			//(void)e;

			isFilled = false;
			return 0.0;
		}
	}
	else
	{
		isFilled = false;
		return 0.0;
	}
}

/**
*
* \param    [in]	param
* \param    [out]	isFilled
* \return
*/
unsigned int WebParameters::getUnsignedParam (const char * param, bool & isFilled)
{
	Parametros::iterator it = pd->parameters.find (param);

	if (it != pd->parameters.end ())
	{
		Parameter & param = it->second;
		std::string & val = (param.isSet) ? param.value : param.defaultValue;
		try
		{
			unsigned int retVal = std::stoi (val);
			isFilled = param.isSet;
			return retVal;
		}
		catch (...)
		{
			isFilled = false;
			return 0;
		}
	}
	else
	{
		isFilled = false;
		return 0;
	}
}

/**
* Añadimos los el parametro existente. De existir, se sobreescribe
* \param    [in]	param	Nombre que se uso en el servicio para este parámetro
* \param    [out]	data	Valor del parametro
*/
void WebParameters::addDefaultValue (const char * param, const char * defaultValue)
{
	//YAGNI: Comprobar si existe previamente
	//a dia de hoy, se comprueba si no existe, y de no existir, se añade el valor por defecto
	Parameter parameter;
	parameter.isSet = false;
	parameter.defaultValue = defaultValue;
	pd->parameters[param] = parameter;
}




WebParametersPrivateData &LIBHTTPD_LOCAL WebParameters::getPrivateData ()
{
	return *pd;
}
