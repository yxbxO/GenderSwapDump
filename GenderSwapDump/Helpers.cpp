#include "Helpers.h"


#pragma region directory_looping_test

void LoopDir(const char* Dir, std::vector<std::string> &pathArray)
{
	//std::string path = "/path/to/directory";
	for (const auto& entry : fs::directory_iterator(Dir))
	{
		pathArray.push_back((std::string)entry.path().string().c_str());
	}
}
#pragma endregion
