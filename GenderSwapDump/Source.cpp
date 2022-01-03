#pragma once
#include "ParseClass.h"
#include "SMclass.h"
#include "JParse.h"



int main()
{
	JPClass jp = JPClass();
	jp.sortjsonxml();

	system("pause");
	return 0;

#if 0

	!!!use rapid json cause json cpp sucks ass. slow asf!!!


TODO:
[+] clean up and fix inheritence issue with classes. ###DONE
[+] start parsing json using parsed xml info. #started. need to work on this to get faster and more accurate parsing resutls.
[+] get rid of dlc specific stuff for now. (will add on a later date)
[+] add checks for when things fail ( when strings are null, vectors return 0, etc
[+] get rid of current log func and replace with log class and timer (ie from gta glitt)

long term todo :
[x] get rid of all vectors(the are allocated on the stack, slows down execution)
[x]add algorithm for specific dlc.
[X]add a gui(directx12 great for learning purposes)

#endif
}



/**

*/