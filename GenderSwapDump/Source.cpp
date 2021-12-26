#include "JsonClass.h"
#include "SMclass.h"
#include "XmlClass.h"



int main()
{
	XMLPClass xmlparse = XMLPClass();
	xmlparse.dumpNoJson();
	//JPClass jparse = JPClass();
	
	system("pause");
	return 0;
}

/**
TODO:
[+] clean up and fix inheritence issue with classes.
[+] start parsing json using parsed xml info.
[+] add log class and timer (from gta glitt)
*/