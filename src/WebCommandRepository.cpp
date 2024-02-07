/*********************************************************************************************
*	Name		: WebCommandRepository.cpp
*	Description	:
*	Copyright	(C) 2020
********************************************************************************************/

#include <memory>
#include <string>
#include <unordered_map>
#include "WebCommand.h"

#include "WebCommandRepository.h"

typedef std::unordered_map <std::string, std::unique_ptr<WebCommand>> CommandMap;

class LIBHTTPD_LOCAL CommandContainer
{
public:
	CommandMap cm;
};



WebCommand* WebCommandRepository::findResource (const char* url)
{
	/*
	//YAGNI: Actually we are only searching for the string... may be in the future

	Restful architecture: It could be this:
	http://acme.com/products/					==> All products
	http://acme.com/products/?filter=abc		==>	Filtered all products
	http://acme.com/products/1234/				==>	Product 1234
	http://acme.com/products/1234/photos/		==>	Photos of the 1234 product
	http://acme.com/products/1234/photos/123	==> That exact photo of that exact product


	that means we should split the URL by / and create a classic tree
	*/

	CommandMap& cm = WebCommandRepository::staticCommands ().cm;
	CommandMap::iterator it = cm.find (url);

	if (it != cm.end ())
	{
		WebCommand* wc = it->second.get ();
		return (wc->isEnabled) ? wc : nullptr;
	}
	else
	{
		return nullptr;
	}
}


void WebCommandRepository::setCommandStatus (const char* url, bool isEnabled)
{
	CommandMap& cm = WebCommandRepository::staticCommands ().cm;
	CommandMap::iterator it = cm.find (url);

	if (it != cm.end ())
	{
		WebCommand* wc = it->second.get ();
		wc->isEnabled = isEnabled;
	}
}


void WebCommandRepository::addCommand (WebCommand* webCommand)
{
	//YAGNI: to supportr real RestFul services, split by slash and create a tree
	// (Using the ? in the path as arguments)

	CommandMap& cm = WebCommandRepository::staticCommands ().cm;
	cm [webCommand->getBaseUrl ()] = std::unique_ptr<WebCommand> (webCommand);
}

void WebCommandRepository::listWebCommands (callbackInfoCmd infoCmd, void* context)
{
	for (auto& cmdNode : staticCommands ().cm)
	{
		infoCmd (cmdNode.second.get (), context);
	}
}



CommandContainer& WebCommandRepository::staticCommands ()
{
	static CommandContainer comandosVar;
	return comandosVar;
}


//------------------------- Helper -------------------------
StaticWebCommandAutoRegister::StaticWebCommandAutoRegister (WebCommand* comando)
{
	WebCommandRepository::addCommand (comando);
}
