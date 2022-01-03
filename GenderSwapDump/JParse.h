#pragma once
#include <filesystem>
#include <Windows.h>

#include "SMclass.h"
#include "Helpers.h"
#include "ParseClass.h"


#include <fstream>
#include <iostream>
#include <vector>

#include <string>

#pragma region jsonStructs

struct lastParse
{
	std::vector<pasta> toUnlock;
	std::vector<pasta> toBuy;
};

struct secregate
{
	lastParse Male;
	lastParse Female;
};
#pragma endregion

class JPClass : public Parse, public ScriptData
{
private:
	//lastParse overwit; //stores what tobuy in order to unlock
	secregate witit;
public:
	JPClass() : Parse(), ScriptData()
	{
		sortDump();
	}

	~JPClass() override { 	}

#if 0
	//unused array. not included to save space.
	const FileGender files[25]
	{
		{eClothingType::ACCESSORIES, Gender::FEMALE, this->m_jpaths[0].c_str()},
		{eClothingType::HAIR, Gender::FEMALE, this->m_jpaths[1].c_str()},
		{eClothingType::LEGS, Gender::FEMALE, this->m_jpaths[2].c_str()},
		{eClothingType::SHOES, Gender::FEMALE, this->m_jpaths[3].c_str() },
		{eClothingType::TOPS, Gender::FEMALE, this->m_jpaths[4].c_str() },
		{eClothingType::TORSOS, Gender::FEMALE, this->m_jpaths[5].c_str()},
		{eClothingType::UNDERSHIRTS, Gender::FEMALE, this->m_jpaths[6].c_str() },
		{eClothingType::MASKS, Gender::NB, this->m_jpaths[14].c_str()},
		{eClothingType::ACCESSORIES, Gender::MALE, this->m_jpaths[7].c_str() },
		{eClothingType::HAIR, Gender::MALE, this->m_jpaths[8].c_str() },
		{eClothingType::LEGS, Gender::MALE, this->m_jpaths[9].c_str() },
		{eClothingType::SHOES, Gender::MALE, this->m_jpaths[10].c_str() },
		{eClothingType::TOPS, Gender::MALE, this->m_jpaths[11].c_str() },
		{eClothingType::TORSOS, Gender::MALE, this->m_jpaths[12].c_str() },
		{eClothingType::UNDERSHIRTS, Gender::MALE, this->m_jpaths[13].c_str() },
		//props
		{eClothingType::BRACELETS, Gender::FEMALE, this->m_jpaths[15].c_str() },
		{eClothingType::EARS, Gender::FEMALE, this->m_jpaths[16].c_str() },
		{eClothingType::GLASSES, Gender::FEMALE, this->m_jpaths[17].c_str() },
		{eClothingType::HATS, Gender::FEMALE, this->m_jpaths[18].c_str() },
		{eClothingType::WATCHES, Gender::FEMALE, this->m_jpaths[19].c_str() },
		{eClothingType::BRACELETS, Gender::MALE, this->m_jpaths[20].c_str()},
		{eClothingType::EARS, Gender::MALE, this->m_jpaths[21].c_str() },
		{eClothingType::GLASSES, Gender::MALE, this->m_jpaths[22].c_str() },
		{eClothingType::HATS, Gender::MALE, this->m_jpaths[23].c_str() },
		{eClothingType::WATCHES, Gender::MALE, this->m_jpaths[24].c_str()},
	};
#endif

	void sortDump()
	{
		//loops through all the male found key and values (in scriptmeta)
		for (auto& elem : this->dumpAll.mDump)
		{
			//loop through all the parsed xml files (in parsed xml files)
			for (auto const& p : dataFound.mXML)
			{
				//loop through every xml file's texlabel and uniquehash
				for (auto const& i : p.infoArray)
				{
					if (!strcmp(elem.m_key, i.m_uniqueHash))
					{
						*i.m_textLabel == '\0' ? elem.m_textlabel = "[NOT IN STORES]" : elem.m_textlabel = i.m_textLabel;
					}
				}
			}
		}

		//loops through all the female found key and values (in scriptmeta)
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
						*i.m_textLabel == '\0' ? elem.m_textlabel = "[NOT IN STORES]" : elem.m_textlabel = i.m_textLabel;
					}
				}
			}


		}
		std::cout << "DONE COMPARING UNIQUEHASHES" << std::endl;


		//loops through all the found key and values
		for (auto& t : this->dumpAll.mDump)
		{
			//loop through all the found key and values again
			for (auto& m : this->dumpAll.fDump)
			{
				if (!strcmp(t.value, m.value))
				{
					this->witit.Male.toUnlock.push_back(t);
					this->witit.Female.toBuy.push_back(m);
				}
			}
		}

		//loops through all the found key and values
		for (auto& t : this->dumpAll.fDump)
		{
			//loop through all the found key and values again
			for (auto& m : this->dumpAll.mDump)
			{
				if (!strcmp(t.value, m.value))
				{
					this->witit.Female.toUnlock.push_back(t);
					this->witit.Male.toBuy.push_back(m);
				}
			}

		}
		std::cout << "DONE COMPARING VALUES AND STORING WHAT UNLOCKS WHAT" << std::endl;

		for (const auto& elem : jInfo)
		{
			for (size_t i{}; i < this->witit.Female.toUnlock.size(); ++i)
			{
				if (!strcmp(elem.GXT, this->witit.Female.toUnlock[i].m_textlabel))
				{
					this->witit.Female.toUnlock[i].comp = elem.comp;
					this->witit.Female.toUnlock[i].DrawableID = elem.DrawableID;
					this->witit.Female.toUnlock[i].TextureID = elem.TextureID;
					this->witit.Female.toUnlock[i].itemName = elem.Localized;

				}

				if (!strcmp(elem.GXT, this->witit.Male.toBuy[i].m_textlabel))
				{
					this->witit.Male.toBuy[i].comp = elem.comp;
					this->witit.Male.toBuy[i].DrawableID = elem.DrawableID;
					this->witit.Male.toBuy[i].TextureID = elem.TextureID;
					this->witit.Male.toBuy[i].itemName = elem.Localized;
				}
			}

			for (size_t i{}; i < this->witit.Male.toUnlock.size(); ++i)
			{
				if (!strcmp(elem.GXT, this->witit.Male.toUnlock[i].m_textlabel))
				{
					this->witit.Male.toUnlock[i].comp = elem.comp;
					this->witit.Male.toUnlock[i].DrawableID = elem.DrawableID;
					this->witit.Male.toUnlock[i].TextureID = elem.TextureID;
					this->witit.Male.toUnlock[i].itemName = elem.Localized;
				}

				if (!strcmp(elem.GXT, this->witit.Female.toBuy[i].m_textlabel))
				{
					this->witit.Female.toBuy[i].comp = elem.comp;
					this->witit.Female.toBuy[i].DrawableID = elem.DrawableID;
					this->witit.Female.toBuy[i].TextureID = elem.TextureID;
					this->witit.Female.toBuy[i].itemName = elem.Localized; 
				}
			}
		}
		std::cout << "DONE SETTING EVERYTHING UP. STARTING DUMP!" << std::endl;
	}

	void sortjsonxml()
	{
		//std::cout << this->witit.Male.toUnlock.size() << std::endl;

		LOG("\t\t\t\t\t\t--------------MALE UNLOCKS----------------\n");
		for (size_t i{}; i < this->witit.Male.toUnlock.size(); ++i)//(const auto& elem : this->overwit.toUnlock)
		{
			LOG("\n[x]GENDER: %s. TO UNLOCK: %s. TEXTLABEL: %s. DLC: %s", this->witit.Male.toUnlock[i].m_Gender, this->witit.Male.toUnlock[i].m_key, this->witit.Male.toUnlock[i].m_textlabel, this->witit.Male.toUnlock[i].DLC_name);
			LOG("\t\t COMPONENT: %s. DrawableID: %s. TextureID: %s. NAME: %s.", this->witit.Male.toUnlock[i].comp, this->witit.Male.toUnlock[i].DrawableID, this->witit.Male.toUnlock[i].TextureID, this->witit.Male.toUnlock[i].itemName);
			LOG("[x]GENDER: %s. TO BUY: %s. TEXTLABEL: %s. DLC: %s", this->witit.Female.toBuy[i].m_Gender, this->witit.Female.toBuy[i].m_key, this->witit.Female.toBuy[i].m_textlabel, this->witit.Female.toBuy[i].DLC_name);
			LOG("\t\t COMPONENT: %s. DrawableID: %s. TextureID: %s. NAME: %s.", this->witit.Female.toBuy[i].comp, this->witit.Female.toBuy[i].DrawableID, this->witit.Female.toBuy[i].TextureID, this->witit.Female.toBuy[i].itemName);
		}

		LOG("\n\n\n\t\t\t\t--------------FEMALE UNLOCKS----------------\n");
		for (int i{}; i < this->witit.Female.toUnlock.size(); ++i)
		{
			LOG("\n[x]GENDER: %s. TO UNLOCK: %s. TEXTLABEL: %s. DLC: %s", this->witit.Female.toUnlock[i].m_Gender, this->witit.Female.toUnlock[i].m_key, this->witit.Female.toUnlock[i].m_textlabel, this->witit.Female.toUnlock[i].DLC_name);
			LOG("\t\t COMPONENT: %s. DrawableID: %s. TextureID: %s. NAME: %s.", this->witit.Female.toUnlock[i].comp,this->witit.Female.toUnlock[i].DrawableID, this->witit.Female.toUnlock[i].TextureID, this->witit.Female.toUnlock[i].itemName);
			LOG("[x]GENDER: %s. TO BUY: %s. TEXTLABEL: %s. DLC: %s", this->witit.Male.toBuy[i].m_Gender, this->witit.Male.toBuy[i].m_key, this->witit.Male.toBuy[i].m_textlabel, this->witit.Male.toBuy[i].DLC_name);
			LOG("\t\t COMPONENT: %s. DrawableID: %s. TextureID: %s. NAME: %s.", this->witit.Male.toBuy[i].comp, this->witit.Male.toBuy[i].DrawableID, this->witit.Male.toBuy[i].TextureID, this->witit.Male.toBuy[i].itemName);
		}
	}


#pragma jsonNotupdated
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
					LOG("\n\t\t[+] TO UNLOCK : KEY = %s,  GXT = %s, COMPOENT TYPE = %s, DLC = %s", t.m_key, t.m_textlabel, t.comp, t.DLC_name);
					LOG("\t\t[-] BUY ON FEMALE : KEY = %s, GXT = %s, COMPOENT TYPE = %s, DLC = %s", m.m_key, m.m_textlabel, m.comp, m.DLC_name);
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
					LOG("\n\t\t[+] TO UNLOCK : KEY = %s, GXT = %s, COMPOENT TYPE = %s, DLC = %s", t.m_key, t.m_textlabel, t.comp, t.DLC_name);
					LOG("\t\t[-] BUY ON MALE : KEY = %s, GXT = %s, COMPOENT TYPE = %s, DLC = %s", m.m_key, m.m_textlabel, m.comp, m.DLC_name);

					//std::cout << "\n[+] TO UNLOCK: " << t.m_key << " " << t.m_textlabel << " " << t.m_Gender << " " << t.value << std::endl;
					//std::cout << "[+] BUY ON MALE : " << m.m_key << " " << m.m_textlabel << " " << m.m_Gender << " " << m.value << std::endl;
				}
			}

		}
	}
#pragma endregion
};
