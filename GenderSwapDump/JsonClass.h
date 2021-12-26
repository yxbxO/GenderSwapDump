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
using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

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
	LPCSTR m_Dir;
protected:
	std::vector<std::string>m_paths;
public:
	Parse(LPCSTR Dir) : m_Dir{Dir}
	{
		if (SetFilePaths(m_Dir))
		{
			std::cout << "path array created" << '\n';
		}
	}

	~Parse() {}

	bool SetFilePaths(LPCSTR Dir)
	{
		std::vector<std::string>temp;
		for (auto const &dirEntry : recursive_directory_iterator(Dir))
		{
			temp.push_back(dirEntry.path().string().c_str());
		}
			
		std::sort(temp.begin(), temp.end());
		this->m_paths = temp;
		return true;
	}
};

class JPClass : public Parse
{
private:
public:
	JPClass() : Parse{ "Jsonfiles/" } {}

	~JPClass() 
	{	}

	const FileGender files[25]
	{
		{eClothingType::ACCESSORIES, Gender::FEMALE, this->m_paths[0].c_str()},
		{eClothingType::HAIR, Gender::FEMALE, this->m_paths[1].c_str()},
		{eClothingType::LEGS, Gender::FEMALE, this->m_paths[2].c_str()},
		{eClothingType::SHOES, Gender::FEMALE, this->m_paths[3].c_str() },
		{eClothingType::TOPS, Gender::FEMALE, this->m_paths[4].c_str() },
		{eClothingType::TORSOS, Gender::FEMALE, this->m_paths[5].c_str()},
		{eClothingType::UNDERSHIRTS, Gender::FEMALE, this->m_paths[6].c_str() },
		{eClothingType::MASKS, Gender::NB, this->m_paths[14].c_str()},

		{eClothingType::ACCESSORIES, Gender::MALE, this->m_paths[7].c_str() },
		{eClothingType::HAIR, Gender::MALE, this->m_paths[8].c_str() },
		{eClothingType::LEGS, Gender::MALE, this->m_paths[9].c_str() },
		{eClothingType::SHOES, Gender::MALE, this->m_paths[10].c_str() },
		{eClothingType::TOPS, Gender::MALE, this->m_paths[11].c_str() },
		{eClothingType::TORSOS, Gender::MALE, this->m_paths[12].c_str() },
		{eClothingType::UNDERSHIRTS, Gender::MALE, this->m_paths[13].c_str() },

		//props
		{eClothingType::BRACELETS, Gender::FEMALE, this->m_paths[15].c_str() },
		{eClothingType::EARS, Gender::FEMALE, this->m_paths[16].c_str() },
		{eClothingType::GLASSES, Gender::FEMALE, this->m_paths[17].c_str() },
		{eClothingType::HATS, Gender::FEMALE, this->m_paths[18].c_str() },
		{eClothingType::WATCHES, Gender::FEMALE, this->m_paths[19].c_str() },

		{eClothingType::BRACELETS, Gender::MALE, this->m_paths[20].c_str()},
		{eClothingType::EARS, Gender::MALE, this->m_paths[21].c_str() },
		{eClothingType::GLASSES, Gender::MALE, this->m_paths[22].c_str() },
		{eClothingType::HATS, Gender::MALE, this->m_paths[23].c_str() },
		{eClothingType::WATCHES, Gender::MALE, this->m_paths[24].c_str()},
	};
};