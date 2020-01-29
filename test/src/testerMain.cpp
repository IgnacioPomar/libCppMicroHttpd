

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


int main (int argc, const char * argv[])
{
	WebProcess wp (8585, ThreadModel::HTTP_USE_SELECT);

	wp.initDaemon ();

	std::this_thread::sleep_for (std::chrono::milliseconds (10000));

	wp.stopDaemon ();


	return 1;
}
