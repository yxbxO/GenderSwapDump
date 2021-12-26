#pragma once
#include <filesystem>
#include <Windows.h>

#include "pugixml/pugixml.hpp"

#include <fstream>
#include <iostream>
#include <vector>

#include <string>

#define numOfDlc 37
#define numOfComp 12

struct fitComp
{
	LPCSTR inFiles; //its named in the scriptmeta ie "PHEAD", "BERD"
	LPCSTR name; // what we call them ie "hats", "glasses", "torso 1", "torso 2"
};

struct dlcnames
{
	LPCSTR name;		//ie "Beach Bum Update"
	LPCSTR dlc_key;			//ie "DLC_MP_BEACH"
	LPCSTR dlcFileName;		//ie "mpbeach"
};

enum codename
{
	mpbeach,
	mpchristmas,
	mpvalentines,
	mpbusiness,
	mpbusiness2,
	mphipster,
	mpindependence,
	mppilot,
	mplts,
	mpchristmas2,
	mpheist,
	mpluxe,
	mpluxe2,
	mplowrider,
	mphalloween,
	mpapartment,
	mpxmas_604490,
	mpjanuary2016,
	mpvalentines2,
	mplowrider2,
	mpexecutive,
	mpstunt,
	mpbiker,
	mpimportexport,
	mpgunrunning,
	mpairraces,
	mpsmuggler,
	mpchristmas2017,
	mpassault,
	mpbattle,
	mpchristmas2018,
	mpvinewood,
	mpheist3,
	mpsum,
	mpheist4,
	mptuner,
	mpsecurity
};


struct pasta
{
	LPCSTR m_textlabel = "";
	LPCSTR m_key;
	LPCSTR m_Gender;
	LPCSTR value;
	LPCSTR DLC_name; //The Diamond Casino & Resort
	LPCSTR comp; //JBIB = torso 2
};

struct sFound
{
	std::vector<pasta> mDump;
	std::vector<pasta> fDump;
};

struct found
{
	std::vector<pasta> initital_F_keyVal; //female array of keys and values.
	std::vector<pasta> initital_M_keyVal; //male array of keys and values.
	std::vector<pasta> found_F_keyVal; //female keys and values that would unlock items in initial female array
	std::vector<pasta> found_M_keyVal; //male keys and values that would unlock items in initial male array.
};

class ScriptData
{
private:
	pugi::xml_document doc;
	LPCSTR source = "scriptmetadata.meta";
	pugi::xml_parse_result result;

	//root node
	pugi::xml_node MPApparelData;


protected:
	//std::vector<pasta> dumpAll;
	sFound dumpAll;
	found keyVal;
public:
	ScriptData() {
		// Load XML file into memory
		// Remark: to fully read declaration entries you have to specify
		// "pugi::parse_declaration"
		this->result = doc.load_file(this->source, pugi::parse_default | pugi::parse_declaration);

		this->MPApparelData = doc.document_element().child("MPApparelData");
		if (!result)
		{
			std::cout << "Parse error: " << result.description()
				<< ", character pos= " << result.offset;
		}
		else if (result)
		{
			std::cout << "XML [" << source << "] parsed without errors, root name: [" << this->MPApparelData.name() << "]\n\n";
			dump();
		}
	}

#pragma region dlc_specific
	bool dlc_specific(dlcnames dlc)
	{
		pugi::xml_node Gender;
		//loops through both male and female nodes
		for (Gender = MPApparelData.child("MPApparelDataMale"); Gender; Gender = Gender.next_sibling())
		{
			std::cout << "Gender: " << Gender.name() << "\n";
			//loops through all "Item" nodes within the male and female nodes
			for (pugi::xml_node item = Gender.child("Item"); item; item = item.next_sibling())
			{
				//loops through all atributes within "item" childnodes
				pugi::xml_attribute attr = item.first_attribute();
				if (strstr(attr.value(), dlc.dlc_key/*"DLC_MP_FIXER"*/))
				{
//START BY MODIFYING HERE
					pasta key_val;
					key_val.m_key = attr.value();

					if (strstr(key_val.m_key, "_M_"))
					{
						key_val.m_Gender = "M";
					}
					else if (strstr(key_val.m_key, "_F_"))
					{
						key_val.m_Gender = "F";
					}

					attr = attr.next_attribute();
					key_val.value = attr.value();

					if (key_val.m_Gender == "M")
					{
						this->keyVal.initital_M_keyVal.push_back(key_val);
					}
					if (key_val.m_Gender == "F")
					{
						this->keyVal.initital_F_keyVal.push_back(key_val);
					}
					
				}
			}
		}

		//checks if we successfully backed up key and val to array
		if (this->keyVal.initital_M_keyVal.empty() || this->keyVal.initital_F_keyVal.empty())
		{
			return false;
		}

		for (size_t i = 0; i < this->keyVal.initital_M_keyVal.size() - 1; ++i)//(auto const& elem : this->keyVal.initital_keyVal)
		{
			for (Gender = MPApparelData.child("MPApparelDataFemale"); Gender; Gender = Gender.child("TRAP"))
			{
				//loops through all "Item" nodes within the female node
				for (pugi::xml_node item = Gender.child("Item"); item; item = item.next_sibling())
				{
					//loops through all atributes within "item" childnodes
					pugi::xml_attribute attr = item.first_attribute();
					pasta key_val;
					key_val.m_key = attr.value();

					if (strstr(key_val.m_key, "_M_"))
					{
						key_val.m_Gender = "M";
					}
					else if (strstr(key_val.m_key, "_F_"))
					{
						key_val.m_Gender = "F";
					}

					//std::cout << " " << attr.name() << "=" << attr.value() << '\n';
					attr = attr.next_attribute();
					key_val.value = attr.value();

					//if value from loop is same as value from initial array
					if (strstr(key_val.value, this->keyVal.initital_M_keyVal[i].value) /* && !strstr(key_val.key, elem.key)*/)
					{
						//if key from loop is same as key from loop.
						//if (strstr(key_val.key, this->keyVal.initital_M_keyVal[i].key))
						//{
						//	continue;
						//}
						this->keyVal.found_M_keyVal.push_back(key_val);
						//i++;
					}
				}
			}	
		}

		for (size_t i = 0; i < this->keyVal.initital_F_keyVal.size() - 1; ++i)//(auto const& elem : this->keyVal.initital_keyVal)
		{
			for (Gender = MPApparelData.child("MPApparelDataMale"); Gender; Gender = Gender.child("TRAP"))
			{
				//loops through all "Item" nodes within the female node
				for (pugi::xml_node item = Gender.child("Item"); item; item = item.next_sibling())
				{
					//loops through all atributes within "item" childnodes
					pugi::xml_attribute attr = item.first_attribute();
					pasta key_val;
					key_val.m_key = attr.value();

					if (strstr(key_val.m_key, "_M_"))
					{
						key_val.m_Gender = "M";
					}
					else if (strstr(key_val.m_key, "_F_"))
					{
						key_val.m_Gender = "F";
					}

					//std::cout << " " << attr.name() << "=" << attr.value() << '\n';
					attr = attr.next_attribute();
					key_val.value = attr.value();

					//if value from loop is same as value from initial array
					if (strstr(key_val.value, this->keyVal.initital_F_keyVal[i].value) /* && !strstr(key_val.key, elem.key)*/)
					{
						this->keyVal.found_F_keyVal.push_back(key_val);
						i++;
					}
				}
			}
		}
		if (this->keyVal.found_M_keyVal.empty() || this->keyVal.found_F_keyVal.empty())
		{
			return false;
		}
		return true;
	}
	void print_dlc_specific()
	{

		std::cout << "[+]SIZE OF INITIAL MALE: " << this->keyVal.initital_M_keyVal.size() << std::endl;
		std::cout << "[+]SIZE OF INITIAL FEMALE: " << this->keyVal.initital_F_keyVal.size() << std::endl;
		std::cout << "[+]SIZE OF FOUND MALE: " << this->keyVal.found_M_keyVal.size() << std::endl;
		std::cout << "[+]SIZE OF FOUND FEMALE: " << this->keyVal.found_F_keyVal.size() << std::endl;

		for (auto const& elem : this->keyVal.initital_M_keyVal)
		{
			std::cout << "[+]INITIAL MALE: " << elem.m_key << ", " << elem.value << " GENDER: " << elem.m_Gender << std::endl;
		}

		//for (auto const& elem : this->keyVal.initital_F_keyVal)
		//{
		//	std::cout << "[+]INITIAL FEMALE: " << elem.key << ", " << elem.value << " GENDER: " << elem.Gender << std::endl;
		//}

		for (auto const& elem : this->keyVal.found_M_keyVal)
		{
			std::cout << "[+]FOUND MALE: " << elem.m_key << ", " << elem.value << " GENDER: " << elem.m_Gender << std::endl;
		}

		//for (auto const& elem : this->keyVal.found_F_keyVal)
		//{
		//	std::cout << "[+]FOUND FEMALE: " << elem.key << ", " << elem.value << " GENDER: " << elem.Gender << std::endl;
		//}
	}
#pragma endregion

	bool dump()
	{
		pasta key_val;
		//loops through both male and female nodes
		for (pugi::xml_node Gender = MPApparelData.child("MPApparelDataMale"); Gender; Gender = Gender.next_sibling())
		{
			//loops through all "Item" nodes within the male and female nodes
			for (pugi::xml_node item = Gender.child("Item"); item; item = item.next_sibling())
			{
				//loops through all atributes within "item" childnodes
				pugi::xml_attribute attr = item.first_attribute();
				key_val.m_key = attr.value();
				attr = attr.next_attribute();
				key_val.value = attr.value();

				//finds the component it belongs to 
				for (const auto& i : components)
				{
					if (strstr(key_val.m_key, i.inFiles))
					{
						key_val.comp = i.name;
					}
				}

				//finds the dlc it belongs to 
				for (const auto& m : DLC)
				{
					if (strstr(key_val.m_key, m.dlc_key))
					{
						key_val.DLC_name = m.name;
					}
				}
				if (strstr(key_val.m_key, "_M_"))
				{
					key_val.m_Gender = "Male Ped";
					this->dumpAll.mDump.push_back(key_val);
				}
				else if (strstr(key_val.m_key, "_F_"))
				{
					key_val.m_Gender = "Female Ped";
					this->dumpAll.fDump.push_back(key_val);
				}
			}
		}
		if (this->dumpAll.fDump.empty() && this->dumpAll.mDump.empty()) return false;

		return true;
	}

	void print_dump()
	{
		for (auto const& elem : dumpAll.mDump)
		{
			std::cout << elem.m_key << ", " << elem.value << std::endl;
		}
	}

	const std::array<dlcnames, numOfDlc> DLC {
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
	} };

	const std::array<fitComp, numOfComp> components {
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
		{"OUTFIT",	"outfit"}
	} };

	~ScriptData() {}
};