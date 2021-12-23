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
	{"PHEAD",	"hat"},
	{"BERD",	"mask"},
	{"PEYES",	"glasses"},
	{"TORSO",	"gloves"},
	{"ACCS",	"torso 1"},
	{"DECL",	"Decal"},
	{"FEET",	"Shoes"},
	{"HAIR",	"hair"},
	{"JBIB",	"torso 2"},
	{"LEGS",	"legs"},
	{"TEETH",	"accessories"},
};

struct dlcnames
{
	LPCSTR dlc_name;		//ie "Beach Bum Update"
	LPCSTR dlc_key;			//ie "DLC_MP_BEACH"
	LPCSTR dlcFileName;		//ie "mpbeach"
};

const dlcnames DLC[]
{
	{"Beach Bum Update", "DLC_MP_BEACH", "mpbeach"},
	{"Holiday Gifts DLC", "DLC_MP_XMAS", "mpchristmas"},
	{"Valentine's Day Massacre Special DLC", "DLC_MP_VAL", "mpvalentines"},
	{"Business Update", "DLC_MP_BUSI", "mpbusiness"},
	{"Business Update", "DLC_MP_BUSI2", "mpbusiness2"},
	{"I'm Not A Hipster Update", "DLC_MP_HIPS", "mphipster"},
	{"Independence Day Special DLC", "DLC_MP_IND", "mpindependence"},
	{"San Andreas Flight School Update", "DLC_MP_PILOT", "mppilot"},
	{"Last Team Standing Update", "DLC_MP_LTS", "mplts"},
	{"Festive Surprise", "DLC_MP_XMAS2", "mpchristmas2"},
	{"Heists Update", "DLC_MP_HEIST", "mpheist"},
	{"Ill-Gotten Gains Part 1", "DLC_MP_LUXE", "mpluxe"},
	{"Ill-Gotten Gains Part 2", "DLC_MP_LUXE2", "mpluxe2"},
	{"Lowriders", "DLC_MP_LOW", "mplowrider"},
	{"Halloween Surprise", "DLC_MP_HAL", "mphalloween"},
	{"Executives and Other Criminals", "DLC_MP_APA", "mpapartment"},
	{"Festive Surprise 2015", "DLC_MP_XMAS3", "mpxmas_604490"},
	{"January 2016 Update", "DLC_MP_JAN", "mpjanuary2016"},
	{"Be My Valentine", "DLC_MP_VAL2", "mpvalentines2"},
	{"Lowriders: Custom Classics", "DLC_MP_LOW2", "mplowrider2"},
	{"Finance And Felony", "DLC_MP_EXEC", "mpexecutive"},
	{"Cunning Stunts", "DLC_MP_STUNT", "mpstunt"},
	{"Bikers", "DLC_MP_BIKER", "mpbiker"},
	{"Import/Export", "DLC_MP_IE", "mpimportexport"},
	{"Gunrunning", "DLC_MP_GR", "mpgunrunning"},
	{"Air Races / Smuggler", "DLC_MP_AR", "mpairraces"},
	{"Smuggler's Run", "DLC_MP_SMUG", "mpsmuggler"},
	{"The Doomsday Heist", "DLC_MP_X17", "mpchristmas2017"},
	{"Southern San Andreas Super Sport Series", "DLC_MP_ASS", "mpassault"},
	{"After Hours", "DLC_MP_BH", "mpbattle"},
	{"Arena War", "DLC_MP_ARENA", "mpchristmas2018"},
	{"The Diamond Casino & Resort", "DLC_MP_VWD", "mpvinewood"},
	{"The Diamond Casino Heist", "DLC_MP_H3", "mpheist3"},
	{"Los Santos Summer Special", "DLC_MP_SUM", "mpsum"},
	{"The Cayo Perico Heist", "DLC_MP_H4", "mpheist4"},
	{"Tuner", "DLC_MP_TUNER", "mptuner"},
	{"The Contract", "DLC_MP_FIXER", "mpsecurity"}
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