/*********************************************************************************************
*	Name		: WebCommand.h
*	Description	: Abstract generic Web command
********************************************************************************************/

#include <list>
#include <string>



#include "WebCommand.h"

class LIBHTTPD_LOCAL Option
{
public:
	Option (const char * opt, const char * description, bool isRequired, const char * defVal, callbackCheck checkFunc = nullptr);
	Option (const char * opt, const char * description, const char *alternativeOption, const char * defVal, callbackCheck checkFunc = nullptr);

	//definition
	std::string opt;
	std::string description;
	bool isRequired;

	callbackCheck checkFunc;
	std::string defVal;

	bool hasAlternativeOption;
	std::string alternativeOption;

};


class LIBHTTPD_LOCAL WebCommandPrivateData
{
public:
	bool hasAutomaticOpts;
	std::list<Option> opts;
};


Option::Option (const char * opt, const char * description, bool isRequired, const char * defVal, callbackCheck checkFunc) :
	opt (opt), description (description), isRequired (isRequired), checkFunc (checkFunc), defVal (defVal)
{
	hasAlternativeOption = false;
}

Option::Option (const char * opt, const char * description, const char * alternativeOption, const char * defVal, callbackCheck checkFunc) :
	opt (opt), alternativeOption (alternativeOption), description (description), checkFunc (checkFunc), defVal (defVal)
{
	isRequired = true;
	hasAlternativeOption = true;
}


WebCommand::WebCommand (bool hasAutomaticOpts)
{
	pd = new WebCommandPrivateData ();
	pd->hasAutomaticOpts = hasAutomaticOpts;
}

WebCommand::~WebCommand ()
{
	delete pd;
}


/**
* Help Message
* \param    [in]   errMsg
*/
const char * WebCommand::getHelp (const char * errMsg)
{
	std::string response;
	response.append ("<h1>").append (this->getBaseUrl ()).append ("</h1>");
	response.append ("<p>").append (this->getDescription ()).append ("</p>");

	if (errMsg != nullptr)
	{
		response.append ("<h2>Error</h2>");
		response.append (errMsg);
	}

	response.append ("<h2>Parametros Obligatorios</h2><ul>");
	for (Option &opt : pd->opts)
	{
		if (opt.isRequired)
		{
			std::string altTxt = "";
			if (opt.hasAlternativeOption)
			{
				altTxt.append ("<br /><i>Puede sustituirse por: ").append (opt.alternativeOption).append ("</i>");
			}

			response.append ("<li>" + opt.opt + ": " + opt.description + "</li>");
		}
	}
	response.append ("</ul>");
	response.append ("<h2>Parametros Optativos</h2><ul>");
	for (Option &opt : pd->opts)
	{
		if (!opt.isRequired)
		{
			response.append ("<li>" + opt.opt + ": " + opt.description + "</li>");
		}
	}
	response.append ("</ul>");

	//Componemos un ejemplo
	std::string href (this->getBaseUrl ());
	href.append ("?");
	std::string sep = "";
	for (Option &opt : pd->opts)
	{
		if (opt.isRequired)
		{
			href.append (sep + opt.opt + "=" + opt.defVal);
			sep = "&";
		}
	}

	response.append ("</ br><h2>Ejemplo</h2>");
	response.append ("<a href=\"" + href + "\">" + href + "</a>");

	return response.c_str ();
}

void WebCommand::addOption (const char * optName, const char * description, bool isRequired, const char * defVal, callbackCheck checkFunc)
{
	Option opt (optName, description, isRequired, defVal, checkFunc);
	pd->opts.push_back (opt);
}

void WebCommand::addOption (const char * optName, const char * description, const char * alternativeOption, const char * defVal, callbackCheck checkFunc)
{
	Option opt (optName, description, alternativeOption, defVal, checkFunc);
	pd->opts.push_back (opt);
}

//----------------------------------------------------------
#include "WebParametersPrivateData.h"
#include "WebParameters.h"

/**
* Comprobamos si existe un parámetro en concreto (o si su alternativa esta establecida)
* \param    [in]   wsParams	called params
*/
const char * WebCommand::checkOption (WebParameters & wsParams, Option & opt, bool & isAtItShould)
{
	std::string response;
	Parametros & prms = wsParams.getPrivateData ().parameters;
	Parametros::iterator it = prms.find (opt.opt);
	if (it == prms.end ())
	{
		if (opt.isRequired)
		{
			if (opt.hasAlternativeOption)
			{
				it = prms.find (opt.alternativeOption);
				if (it == prms.end ())
				{
					std::string errMsg = "No se ha encontrado ni el parametro " + opt.opt;
					errMsg.append ("ni " + opt.alternativeOption);
					response = this->getHelp (errMsg.c_str ());

					isAtItShould = false;
					return response.c_str ();
				}
			}
			else
			{
				std::string errMsg = "Parametro no encontrado: " + opt.opt;
				response = this->getHelp (errMsg.c_str ());

				isAtItShould = false;
				return response.c_str ();
			}
		}
		else
		{
			wsParams.addDefaultValue (opt.opt.c_str (), opt.defVal.c_str ());
		}
	}
	else if (opt.checkFunc != nullptr)
	{
		if (!opt.checkFunc (it->second.value.c_str (), wsParams.webContext))
		{
			std::string errMsg = "Parametro con valor no valido: " + opt.opt;
			response = this->getHelp (errMsg.c_str ());

			isAtItShould = false;
			return response.c_str ();
		}
	}

	isAtItShould = true;
	return response.c_str ();
}



/**
*
* \param    [in]   wsParams
*/

const char * WebCommand::checkOptsOrHelp (WebParameters & wsParams, bool & isAtItShould)
{
	isAtItShould = true;
	for (Option &opt : pd->opts)
	{
		std::string response (checkOption (wsParams, opt, isAtItShould));

		if (!isAtItShould)
		{
			return response.c_str ();
		}
	}

	return "";
}
