#include <json/value.h>
#include <json/json.h>
#include "Helpers.h"

#include <fstream>
#include <iostream>


const FileGender files[]
{
	{eClothingType::ACCESSORIES, Gender::FEMALE, pathArray[0].c_str()},
	{eClothingType::HAIR, Gender::FEMALE, pathArray[1].c_str()},
	{eClothingType::LEGS, Gender::FEMALE, pathArray[2].c_str()},
	{eClothingType::SHOES, Gender::FEMALE, pathArray[3].c_str() },
	{eClothingType::TOPS, Gender::FEMALE, pathArray[4].c_str() },
	{eClothingType::TORSOS, Gender::FEMALE, pathArray[5].c_str()},
	{eClothingType::UNDERSHIRTS, Gender::FEMALE, pathArray[6].c_str() },
	{eClothingType::MASKS, Gender::NB, pathArray[14].c_str()},

	{eClothingType::ACCESSORIES, Gender::MALE, pathArray[7].c_str() },
	{eClothingType::HAIR, Gender::MALE, pathArray[8].c_str() },
	{eClothingType::LEGS, Gender::MALE, pathArray[9].c_str() },
	{eClothingType::SHOES, Gender::MALE, pathArray[10].c_str() },
	{eClothingType::TOPS, Gender::MALE, pathArray[11].c_str() },
	{eClothingType::TORSOS, Gender::MALE, pathArray[12].c_str() },
	{eClothingType::UNDERSHIRTS, Gender::MALE, pathArray[13].c_str() },

	//props
	{eClothingType::BRACELETS, Gender::FEMALE, pathArray[15].c_str() },
	{eClothingType::EARS, Gender::FEMALE, pathArray[16].c_str() },
	{eClothingType::GLASSES, Gender::FEMALE, pathArray[17].c_str() },
	{eClothingType::HATS, Gender::FEMALE, pathArray[18].c_str() },
	{eClothingType::WATCHES, Gender::FEMALE, pathArray[19].c_str() },

	{eClothingType::BRACELETS, Gender::MALE, pathArray[20].c_str()},
	{eClothingType::EARS, Gender::MALE, pathArray[21].c_str() },
	{eClothingType::GLASSES, Gender::MALE, pathArray[22].c_str() },
	{eClothingType::HATS, Gender::MALE, pathArray[23].c_str() },
	{eClothingType::WATCHES, Gender::MALE, pathArray[24].c_str()},

};

using namespace std;

int main()
{
	//std::ifstream people_file("Jsonfiles/props_female_bracelets.json", std::ifstream::binary);
	//Json::Value people;
	//Json::Reader reader;

	//reader.parse(people_file, people);
	////std::cout << "total json data: " << people << std::endl;

	//std::cout << people["1"]["0"]["Localized"]; //can index through the json file
	LoopDir("Jsonfiles/", pathArray);

	Parse p = Parse("xmlfiles/");

	JPClass jp = JPClass();

	for (size_t i = 0; i < jp.GetFilePaths().size(); ++i)
	{
		std::cout << jp.GetFilePaths()[i] << std::endl;
	}

	system("pause");
	return 0;
}

/**
TODO:
GET/ MAKE FUNCTOIN THAT LOOPS THROUGH ALL THE PROVIDED JSON FILES
ADD XML PARSER. xmllite
*/