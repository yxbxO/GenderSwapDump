#pragma once
#include <filesystem>
#include <Windows.h>

#include <json/value.h>
#include <json/json.h>
#include "SMclass.h"
#include "Helpers.h"

#include "rapidjson/document.h"
#include "rapidjson//istreamwrapper.h"
#include "rapidjson//ostreamwrapper.h"
#include "rapidjson/stringbuffer.h"

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

enum class Gender : INT8
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

#pragma region jsonStructs
struct parsedJson
{
	rapidjson::Document doc{};
	LPCSTR filename = "";
};

struct jsonInfo
{
	LPCSTR comp = ""; //component name from path 
	LPCSTR DrawableID = ""; //type could be string. check later when using this struct
	LPCSTR TextureID = "";
	LPCSTR GXT = "";
	LPCSTR Localized = "";
};
#pragma endregion

#pragma region xmlstructs
struct docparse
{
	pugi::xml_document document;
	pugi::xml_parse_result parse_result;
	std::string xmlpath;
};

struct label_hash
{
	label_hash(LPCSTR textLabel, LPCSTR uniqueHash, LPCSTR element) :m_textLabel{ textLabel }, m_uniqueHash{ uniqueHash }, m_element{ element } {};
	const char* m_textLabel = "";
	const char* m_uniqueHash = "";
	const char* m_element = ""; //the node it is under. ie "pedOutfits", "pedComponents", "pedProps"
};

struct xmlnodes
{
	const char* pedName = "";
	const char* dlcName = "";
	const char* fullDlcName = "";
	std::vector<label_hash> infoArray; //stores array of label and hash
};

struct xmlFound
{
	std::vector<xmlnodes> mXML;
	std::vector<xmlnodes> fXML;
};
#pragma endregion

class Parse
{
private:
	LPCSTR m_jDir = "Jsonfiles/"; //json path
	LPCSTR m_xmlDir = "xmlfiles/"; //xml path

	//xml related
	std::vector<docparse> docResults;

	//json related
	std::vector<parsedJson> parsedDocs;
protected:
	std::vector< jsonInfo> jInfo;

	std::vector<std::string>m_jpaths;
	std::vector<std::string>m_xmlpaths;
	xmlFound dataFound;
public:
	Parse()
	{
		if (SetFilePaths(m_jDir, m_jpaths))
		{
			std::cout << "json path array created" << '\n';
			if (loadjsonfiles())
			{
				std::cout << "json path" << m_jpaths.size() << '\n';
				parseJson();
				//testStruct();
			}
		}
		if (SetFilePaths(m_xmlDir, m_xmlpaths))
		{
			std::cout << "xml path array created" << '\n';
			if (loadxmlfiles())
			{
				std::cout << "xml path" << m_xmlpaths.size() << '\n';
				parseAllxml();
			}
		}
	}

	virtual ~Parse() {}

	//writes to an array of path to files we want parsed
	bool SetFilePaths(LPCSTR Dir, std::vector<std::string> &pathArray)
	{
		for (auto const &dirEntry : recursive_directory_iterator(Dir))
		{
			pathArray.push_back(dirEntry.path().string().c_str());
		}
			
		std::sort(pathArray.begin(), pathArray.end());
		return true;
	}

#pragma region jsonPrase
	bool loadjsonfiles() {
		//loops through all the json files
		for (auto& t : this->m_jpaths)
		{
			parsedJson p;
			p.filename = t.c_str();
			std::ifstream ifs{ t, std::ifstream::binary };
			if (!ifs.is_open())
			{
				std::cerr << "Could not open file for reading!\n";
				return false;
			}
			rapidjson::IStreamWrapper isw{ ifs };
			p.doc.ParseStream(isw);
			this->parsedDocs.emplace_back(std::move(p));
		}
		return true;
	}
	void parseJson()
	{
		for (const auto& elem : parsedDocs)
		{
			jsonInfo j;
			if (strstr(elem.filename, "accessories"))
			{
				j.comp = "accessories";
			}
			else if (strstr(elem.filename, "hair"))
			{
				j.comp = "hair";
			}
			else if (strstr(elem.filename, "legs"))
			{
				j.comp = "legs";
			}
			else if (strstr(elem.filename, "shoes"))
			{
				j.comp = "shoes";
			}
			else if (strstr(elem.filename, "tops"))
			{
				j.comp = "tops (torso 2)";
			}
			else if (strstr(elem.filename, "torsos"))
			{
				j.comp = "torsos (Gloves)";
			}
			else if (strstr(elem.filename, "undershirts"))
			{
				j.comp = "undershirts (torso 1)";
			}
			else if (strstr(elem.filename, "bracelets"))
			{
				j.comp = "bracelets";
			}
			else if (strstr(elem.filename, "ears"))
			{
				j.comp = "ears";
			}
			else if (strstr(elem.filename, "glasses"))
			{
				j.comp = "glasses";
			}
			else if (strstr(elem.filename, "hats"))
			{
				j.comp = "hats";
			}
			else if (strstr(elem.filename, "watch"))
			{
				j.comp = "watch";
			}

			for (rapidjson::Value::ConstMemberIterator D_ID = elem.doc.MemberBegin(); D_ID != elem.doc.MemberEnd(); ++D_ID)
			{
				//printf("DrawableID is %s\n", D_ID->name.GetString());
				j.DrawableID = D_ID->name.GetString();
				//loop through all child nodes/ items (TEXTUREID)
				for (rapidjson::Value::ConstMemberIterator T_ID = elem.doc[D_ID->name.GetString()].MemberBegin(); T_ID != elem.doc[D_ID->name.GetString()].MemberEnd(); ++T_ID)
				{
					//printf("TextureID is %s\n", T_ID->name.GetString());
					j.TextureID = T_ID->name.GetString();
					//printf("GXT is %s\n", elem.doc[D_ID->name.GetString()][T_ID->name.GetString()]["GXT"].GetString());
					j.GXT = elem.doc[D_ID->name.GetString()][T_ID->name.GetString()]["GXT"].GetString();
					//printf("Localized is %s\n", elem.doc[D_ID->name.GetString()][T_ID->name.GetString()]["Localized"].GetString());
					j.Localized = elem.doc[D_ID->name.GetString()][T_ID->name.GetString()]["Localized"].GetString();
					this->jInfo.push_back(j);
				}
			}
		}
	}
	void testStruct()
	{
		for (const auto& elem : jInfo)
		{
			std::cout << "COMP: " << elem.comp << " Drawable: " << elem.DrawableID << " Texture: " << elem.TextureID <<
				" GXT: " << elem.GXT << " Localized: " << elem.Localized << "\n";
		}
	}
#pragma endregion

#pragma region xmlParse
	//loads the parsed xml files to "docResults"
	bool loadxmlfiles()
	{
		docparse parsexml;
		for (auto const& elem : this->m_xmlpaths)
		{
			parsexml.parse_result = parsexml.document.load_file(elem.c_str(), pugi::parse_default | pugi::parse_declaration);
			parsexml.xmlpath = elem;
			this->docResults.emplace_back(std::move(parsexml));
		}
		//if succeeded
		return true;
	}

	void parseAllxml()
	{
		pugi::xml_node PedApparel;

		const auto fillnode = [](const auto& node) -> LPCSTR {
			if (node.child_value())
			{
				return node.child_value();
			}
		};

		const LPCSTR specNodes[3]
		{
			"pedOutfits",
			"pedComponents",
			"pedProps",
		};

		for (auto const& elem : docResults)
		{
			//std::cout << "\nROOT: " << elem.root.name() << "\n";
			pugi::xml_node root;
			if (elem.parse_result)
			{
				root = elem.document.document_element();
				std::cout << "XML [" << elem.xmlpath << "] parsed without errors, root name: [" << root.name() << "]\n";
			}
			xmlnodes nodedata; //object for user defined struct where im creating the vector for. pushback at the end of loop
			if (PedApparel = root.child("pedName")) {
				nodedata.pedName = fillnode(PedApparel);
			}

			if (PedApparel = root.child("dlcName")) {
				nodedata.dlcName = fillnode(PedApparel);
			}

			if (PedApparel = root.child("fullDlcName")) {
				nodedata.fullDlcName = fillnode(PedApparel);
			}

			for (int i{}; i < ARRAYSIZE(specNodes); ++i)
			{
				PedApparel = root.child(specNodes[i]);
				if (PedApparel)
				{
					//loops through each child node "Item"
					for (pugi::xml_node item = PedApparel.child("Item"); item; item = item.next_sibling())
					{
						//get textlabel and uniquehashes.
						pugi::xml_node lbl;
						pugi::xml_node hash;
						lbl = item.child("textLabel");

						hash = item.child("uniqueNameHash");

						//temp.push_back(label_hash(fillnode(lbl), fillnode(hash), PedApparel.name()));
						nodedata.infoArray.push_back(label_hash(fillnode(lbl), fillnode(hash), PedApparel.name()));
					}
				}
			}

			if (strstr(nodedata.pedName, "_m_"))
			{
				this->dataFound.mXML.push_back(nodedata);
			}
			else if (strstr(nodedata.pedName, "_f_"))
			{
				this->dataFound.fXML.push_back(nodedata);
			}


			//this->nodes.push_back(nodedata);
		}

	}
#pragma endregion
};