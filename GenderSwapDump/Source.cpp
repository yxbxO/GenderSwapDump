#include "Helpers.h"
#include "xmlclass.h"



int main()
{
	ScriptData scriptmeta = ScriptData();

	scriptmeta.loop();

	system("pause");
	return 0;
}

/**
TODO:
read and understand the documentation for pugixml
practice some examples and implement methods that'll yield speed.

loop through all keys and values. maybe use xpath to search and find without looping?
xpath may be used for single key/ value searching.

looping necessary for dumping.

$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
[+]finish up structs, finish array of structs.
[+]work on creating vectors.
[+] think of ways that will speed up parsing ie use of xpath for single search
*/