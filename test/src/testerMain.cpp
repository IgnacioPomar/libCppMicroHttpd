﻿

#include "libCppMicroHttpd.h"


#ifdef _DEBUG 
#define END_LIB_STD "d.lib"
#else
#define END_LIB_STD ".lib"
#endif


// Libraries the project needs
#pragma comment(lib, "libCppMicroHttpd" END_LIB_STD)



#include <chrono>
#include <thread>


#include "ExampleContext.h"

ExampleContext ec;

int main (int argc, const char * argv[])
{


	WebProcess wp (8585, ThreadModel::HTTP_USE_SELECT, &ec);
	ec.wp = &wp;

	wp.initDaemon ();


	while (wp.isOnline ())
	{
		std::this_thread::sleep_for (std::chrono::milliseconds (100));
	}



	//wp.stopDaemon ();


	return 1;
}
