#pragma once
#include <filesystem>
#include <Windows.h>


#include <json/value.h>
#include <json/json.h>

#include "pugixml/pugixml.hpp"
#include "Helpers.h"

#include <fstream>
#include <iostream>
#include <vector>

#include <string>

#include "JsonClass.h"
#include "SMclass.h"

struct label_hash
{
	label_hash(LPCSTR textLabel, LPCSTR uniqueHash, LPCSTR element) :m_textLabel{ textLabel }, m_uniqueHash{ uniqueHash }, m_element{ element } {};
	const char* m_textLabel;
	const char* m_uniqueHash;
	const char* m_element; //the node it is under. ie "pedOutfits", "pedComponents", "pedProps"
};

struct docparse
{
	pugi::xml_document document;
	pugi::xml_parse_result parse_result;
	LPCSTR xmlpath;
};

struct xmlnodes
{
	const char* pedName;
	const char* dlcName;
	const char* fullDlcName;
	std::vector<label_hash> infoArray; //stores array of label and hash
};

struct xmlFound
{
	std::vector<xmlnodes> mXML;
	std::vector<xmlnodes> fXML;
};

/**
* [+] the class parses all xml files and stores an array of "xmlnode" objects called nodes (member var)
* [+]DocResults (member var) = stores info of each parsed xml file, easily indexable.
* [+] printloop member func = prints what is stored in docsResult array.
* [+] loopnodes member func = prints waht is stored in nodes array.
* [+] the class is ready to be used.
* 
*/

class XMLPClass : public Parse, public ScriptData
{
private:
	//std::vector<xmlnodes> nodes; //stores node info and has 
	std::vector<docparse> docResults;
protected:
	xmlFound dataFound;
public:
	XMLPClass() : Parse{ "xmlfiles/" }, ScriptData() {
		if (loadfiles())
		{
			parseAll();
			sortDump();
		}
	}

	bool loadfiles()
	{
		docparse parsexml;
		for (auto const& elem : this->m_paths)
		{
			parsexml.parse_result = parsexml.document.load_file(elem.c_str(), pugi::parse_default | pugi::parse_declaration);
			parsexml.xmlpath = elem.c_str();
			this->docResults.emplace_back(std::move(parsexml));
		}
		//if succeeded
		return true;
	}
	void parseAll()
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
				//std::cout << "XML [" << elem.xmlpath << "] parsed without errors, root name: [" << root.name() << "]\n";
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
	void sortDump()
	{

		//loops through all the male found key and values
		for (auto& elem : this->dumpAll.mDump)
		{
			//loop through all the parsed xml files
			for (auto const& p : dataFound.mXML)
			{
				//loop through every xml file's texlabel and uniquehash
				for (auto const& i : p.infoArray)
				{
					if (!strcmp(elem.m_key, i.m_uniqueHash))
					{
						if (*i.m_textLabel == '\0')
						{
							elem.m_textlabel = "[NOT IN STORES]";
						}
						else
						{
							elem.m_textlabel = i.m_textLabel;
						}
					}
				}
			}
		}

		//loops through all the female found key and values
		for (auto& elem : this->dumpAll.fDump)
		{
			//loop through all the parsed xml files
			for (auto const& p : dataFound.fXML)
			{
				//loop through every xml file's texlabel and uniquehash
				for (auto const& i : p.infoArray)
				{
					if (!strcmp(elem.m_key, i.m_uniqueHash))
					{
						if (*i.m_textLabel == '\0' && *elem.m_textlabel == '\0')
						{
							elem.m_textlabel = "[NOT IN STORES] ";
						}
						else
						{
							elem.m_textlabel = i.m_textLabel;
						}
					}
				}
			}
		}
	}

	//use this to dump when json is not yet provided.
	void dumpNoJson()
	{
		LOG("\t\t\t\t\t\t--------------MALE UNLOCKS----------------");
		//loops through all the found key and values
		for (auto& t : this->dumpAll.mDump)
		{
			//loop through all the found key and values again
			for (auto& m : this->dumpAll.fDump)
			{
				if (!strcmp(t.value, m.value))
				{
					LOG("\n\t\t[+] TO UNLOCK : %s, %s, %s, %s, %s", t.m_key, t.m_textlabel, t.value, t.comp, t.DLC_name);
					LOG("\t\t[+] BUY ON FEMALE: %s, %s, %s, %s, %s", m.m_key, m.m_textlabel, m.value, m.comp, m.DLC_name);
					//std::cout << "\n[+] TO UNLOCK : " << t.m_key << " " << t.m_textlabel << " " << t.m_Gender << " " << t.value << std::endl;
					//std::cout << "[+] BUY ON FEMALE: " << m.m_key << " "  << m.m_textlabel << " " << m.m_Gender << " " << m.value << std::endl;
				
				}
			}

		}

		LOG("\n\n\n\t\t\t\t--------------FEMALE UNLOCKS----------------");
		//loops through all the found key and values
		for (auto& t : this->dumpAll.fDump)
		{
			//loop through all the found key and values again
			for (auto& m : this->dumpAll.mDump)
			{
				if (!strcmp(t.value, m.value))
				{
					LOG("\n\t\t[+] TO UNLOCK : %s, %s, %s, %s, %s", t.m_key, t.m_textlabel, t.value, t.comp, t.DLC_name);
					LOG("\t\t[+] BUY ON MALE: %s, %s, %s, %s, %s", m.m_key, m.m_textlabel, m.value, m.comp, m.DLC_name);

					//std::cout << "\n[+] TO UNLOCK: " << t.m_key << " " << t.m_textlabel << " " << t.m_Gender << " " << t.value << std::endl;
					//std::cout << "[+] BUY ON MALE : " << m.m_key << " " << m.m_textlabel << " " << m.m_Gender << " " << m.value << std::endl;
				}
			}

		}
	}

	
	~XMLPClass() {}
};
