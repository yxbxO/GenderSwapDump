#include "ScriptMetadataParser.h"
#include "../Core/Constants.h"

ScriptMetadataParser::ScriptMetadataParser(std::shared_ptr<Core::ILogger> logger)
    : logger(std::move(logger))
{
}

bool ScriptMetadataParser::parse() {
    try {
        auto result = doc.load_file(Core::FileConstants::SCRIPT_METADATA.data(),
                                  pugi::parse_default | pugi::parse_declaration);
        
        if (!result) {
            logger->error("Failed to parse script metadata: " + 
                         std::string(result.description()));
            return false;
        }

        MPApparelData = doc.document_element().child("MPApparelData");
        if (!MPApparelData) {
            logger->error("Failed to find MPApparelData node");
            return false;
        }

        logger->log("Script metadata loaded successfully");
        
        if (!parseScriptMetadata()) {
            return false;
        }

        valid = true;
        return true;
    }
    catch (const std::exception& e) {
        logger->error(std::string("Exception in ScriptMetadataParser::parse: ") + e.what());
        return false;
    }
}

bool ScriptMetadataParser::parseScriptMetadata() {
    for (pugi::xml_node gender = MPApparelData.child("MPApparelDataMale");
         gender; gender = gender.next_sibling())
    {
        for (pugi::xml_node item = gender.child("Item");
             item; item = item.next_sibling())
        {
            Core::ClothingItem clothingItem;
            
            // Parse attributes
            pugi::xml_attribute attr = item.first_attribute();
            clothingItem.m_key = attr.value();
            attr = attr.next_attribute();
            clothingItem.value = attr.value();

            // Determine component and DLC type
            determineComponentType(clothingItem);
            determineDlcType(clothingItem);

            // Categorize by gender
            categorizeItem(clothingItem);
        }
    }
    return true;
}

void ScriptMetadataParser::categorizeItem(Core::ClothingItem& item) {
    if (item.m_key.find(Core::GenderIdentifiers::MALE_PREFIX) != std::string_view::npos) {
        item.m_gender = Core::GenderIdentifiers::MALE_STR;
        parsedData.maleItems.push_back(item);
    }
    else if (item.m_key.find(Core::GenderIdentifiers::FEMALE_PREFIX) != std::string_view::npos) {
        item.m_gender = Core::GenderIdentifiers::FEMALE_STR;
        parsedData.femaleItems.push_back(item);
    }
}

void ScriptMetadataParser::determineComponentType(Core::ClothingItem& item) {
    for (const auto& comp : Core::ComponentTypes::UNIFIED_COMPONENTS) {
        if (item.m_key.find(comp.legacyInFiles) != std::string_view::npos) {
            item.comp = comp.displayName;
            break;
        }
    }
}

void ScriptMetadataParser::determineDlcType(Core::ClothingItem& item) {
    for (const auto& dlc : Core::DlcInfo::DLC_LIST) {
        if (item.m_key.find(dlc.dlc_key) != std::string_view::npos) {
            //logger->log("[determineDlcType] : " + std::string(item.m_key) + " : " + std::string(dlc.dlc_key) + " : " + std::string(dlc.name));
            item.dlcName = dlc.name;
            return;
        }
    }

    logger->log("[determineDlcType] FAILED : " + std::string(item.m_key));
} 