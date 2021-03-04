

#pragma once



class WebProcess;
class StackLogger;

class ExampleContext
{
public:
	WebProcess * wp;
	StackLogger * logger;
	int i = 0;

};
