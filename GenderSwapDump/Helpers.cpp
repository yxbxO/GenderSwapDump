#include "ParseClass.h"
#include "Helpers.h"

#pragma region helpers
void Log_(const char* fmt, ...) {
	char		text[4096];
	va_list		ap;
	va_start(ap, fmt);
	vsprintf_s(text, fmt, ap);
	va_end(ap);

	std::ofstream logfile("clothingDump.txt", std::ios::app);
	if (logfile.is_open() && text)	logfile << text << std::endl;
	logfile.close();
}
#pragma endregion

#pragma region directory_looping_test

void LoopDir(const char* Dir, std::vector<std::string> &pathArray)
{
	for (auto const &entry : std::filesystem::directory_iterator(Dir))
	{
		pathArray.push_back(entry.path().string().c_str());
	}
}
#pragma endregion
