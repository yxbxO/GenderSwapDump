#pragma once
#include <filesystem>
#include <Windows.h>

#include <json/value.h>
#include <json/json.h>

#include <fstream>
#include <iostream>
#include <vector>



#include <string>

//https://stackoverflow.com/a/32889434
//using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
namespace fs = std::filesystem;

static std::vector<std::string>pathArray;

enum class eClothingType //lists all clothing types
{
	ACCESSORIES,
	HAIR,
	LEGS,
	SHOES,
	TOPS,
	TORSOS,
	UNDERSHIRTS,
	MASKS,
	//PROPS BELOW
	BRACELETS,
	EARS,
	GLASSES,
	HATS,
	WATCHES,
};

enum class Gender
{
	MALE,
	FEMALE,
	NB, //NON BINARY (SHARED BETWEEN BOTH GENDERS)
};

struct FileGender //distinguishes male and female clothing types.
{
	eClothingType type;
	Gender gender;
	const char* path;
};

void LoopDir(const char* Dir, std::vector<std::string> &pathArray);

class Parse
{
private:
	std::vector<std::string>m_paths;
	LPCSTR m_Dir;
public:

	Parse(LPCSTR Dir) : m_Dir{Dir}
	{
		SetFilePaths(this->m_Dir);
	}

	~Parse() {}

	void SetFilePaths(LPCSTR Dir)
	{
		//std::string path = "/path/to/directory";
		for (const auto& entry : fs::directory_iterator(Dir))
		{
			this->m_paths.push_back((std::string)entry.path().string().c_str());
		}
	}

	std::vector<std::string> GetFilePaths()
	{
		return this->m_paths;
	}
};

class JPClass : public Parse
{
private:
public:
	JPClass() : Parse{ "Jsonfiles/" } {}
	~JPClass() {}
};

class XMLPClass : public Parse
{
private:
public:

	XMLPClass() : Parse{ "xmlfiles/" } {}
	~XMLPClass() {}
};