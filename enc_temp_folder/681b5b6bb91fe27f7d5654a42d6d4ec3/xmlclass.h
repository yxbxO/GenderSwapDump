#pragma once
#include <filesystem>
#include <Windows.h>

#include "pugixml/pugixml.hpp"

#include <fstream>
#include <iostream>
#include <vector>

#include <string>

struct fitComp
{
	LPCSTR inFiles; //its named in the scriptmeta ie "PHEAD", "BERD"
	LPCSTR name; // what we call them ie "hats", "glasses", "torso 1", "torso 2"
};

const fitComp components[]
{
	{"PHEAD", "hat"},
	{"BERD","mask"},
	{"PEYES", "glasses"},
	{"TORSO", "gloves"},
	{"ACCS", "torso 1"},
	{"DECL", "Decal"},
	{"FEET", "Shoes"},
	{"HAIR", "hair"},
	{"JBIB", "torso 2"},
	{"LEGS", "legs"},
	{"TEETH", "accessories"},
};

struct dlcnames
{
	LPCSTR dlc_name;		//ie "Beach Bum Update"
	LPCSTR dlc_key;			//ie "DLC_MP_BEACH"
	LPCSTR dlcFileName;		//ie "mpbeach"
};

const dlcnames DLC[]
{
	{"Beach Bum Update", "DLC_MP_BEACH"},
	{"Holiday Gifts DLC"},
	{"Valentine's Day Massacre Special DLC"},
	{"Business Update"},
	{"Business Update"},
	{"Business Update"},
	{"I'm Not A Hipster Update"},
	{"I'm Not A Hipster Update"},
	{"Independence Day Special DLC"},
	{"San Andreas Flight School Update"},
	{"Last Team Standing Update"},
	{"Festive Surprise"},
	{"Heists Update"},
	{"Ill-Gotten Gains Part 1"}
};

class ScriptData
{
private:
	pugi::xml_document doc;
	LPCSTR source = "scriptmetadata.meta";
	pugi::xml_parse_result result;

	// A valid XML document must have a single root node
	pugi::xml_node MPApparelData;
protected:
	std::vector<std::string> dlc_specific;
	std::vector<std::string> dumpAll;

public:
	ScriptData() {
		// Load XML file into memory
		// Remark: to fully read declaration entries you have to specify
		// "pugi::parse_declaration"
		this->result = doc.load_file(this->source, pugi::parse_default | pugi::parse_declaration);

		MPApparelData = doc.document_element().child("MPApparelData");
		if (!result)
		{
			std::cout << "Parse error: " << result.description()
				<< ", character pos= " << result.offset;
		}
		else if (result)
		{
			std::cout << "XML [" << source << "] parsed without errors, attr value: [" << this->MPApparelData.name() << "]\n\n";
		}
	}

	void loop()
	{
		//loops through both male and female nodes
		for (pugi::xml_node Gender = MPApparelData.child("MPApparelDataMale"); Gender; Gender = Gender.next_sibling())
		{
			std::cout << "Gender: " << Gender.name() << "\n";
			//loops through all "Item" nodes within the male and female nodes
			for (pugi::xml_node item = Gender.child("Item"); item; item = item.next_sibling())
			{
				//loops through all atributes within "item" childnodes
				for (pugi::xml_attribute attr = item.first_attribute(); attr; attr = attr.next_attribute())
				{
					if(strstr(attr.value(), "DLC_MP_FIXER"))
						std::cout << " " << attr.name() << "=" << attr.value() << '\n';
				}
			}
		}
	}

	~ScriptData() {}

};