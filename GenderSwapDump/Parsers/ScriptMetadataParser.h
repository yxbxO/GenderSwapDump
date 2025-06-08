#pragma once
#include "../Core/Interfaces.h"
#include "../Models/ClothingTypes.h"
#include "pugixml/pugixml.hpp"
#include <memory>

class ScriptMetadataParser : public Core::IParser {
private:
    std::shared_ptr<Core::ILogger> logger;
    pugi::xml_document doc;
    pugi::xml_node MPApparelData;
    bool valid{false};

    struct ParsedData {
        std::vector<Core::ClothingItem> maleItems;
        std::vector<Core::ClothingItem> femaleItems;
    } parsedData;

public:
    explicit ScriptMetadataParser(std::shared_ptr<Core::ILogger> logger);
    
    bool parse() override;
    bool isValid() const override { return valid; }
    
    const std::vector<Core::ClothingItem>& getMaleItems() const { return parsedData.maleItems; }
    const std::vector<Core::ClothingItem>& getFemaleItems() const { return parsedData.femaleItems; }

private:
    bool parseScriptMetadata();
    void categorizeItem(Core::ClothingItem& item);
    void determineComponentType(Core::ClothingItem& item);
    void determineDlcType(Core::ClothingItem& item);
}; 