#pragma once
#include "../Core/Interfaces.h"
#include "../Parsers/JsonParser.h"
#include "../Parsers/XmlParser.h"
#include "../Parsers/ScriptMetadataParser.h"
#include "../Models/ClothingTypes.h"
#include <map>
#include <memory>
#include <unordered_map>

class ClothingProcessor {
private:
    std::shared_ptr<Core::ILogger> logger;
    std::shared_ptr<JsonParser> jsonParser;
    std::shared_ptr<XmlParser> xmlParser;
    std::shared_ptr<ScriptMetadataParser> scriptMetaParser;

    // Simplified caching structure
    struct ProcessingCache {
        std::unordered_map<std::string, std::vector<std::string>> dlcItems;
        std::unordered_map<std::string, std::vector<std::string>> componentItems;
    } cache;

    // Unified processing utility for reducing redundancy
    struct ComponentProcessor {
        static std::string extractComponentType(const std::string& rawType, const std::string& uniqueNameHash = "");
        static std::string resolveDlcName(const std::string& rawDlcName);
        static Core::ComponentDetails createComponentDetails(const std::string& eCompType, 
                                                           const std::string& uniqueNameHash,
                                                           const std::string& drawableIndex = "",
                                                           const std::string& textureIndex = "",
                                                           const std::string& dlcName = "");
        static void enrichWithJsonData(Core::ComponentDetails& details, const std::string& textLabel,
                                     const std::vector<JsonItem>& jsonItems);
    };

    // Storage for processed items
    std::unordered_map<std::string, std::shared_ptr<Core::Outfit>> outfitCache;
    std::unordered_map<std::string, Core::ComponentDetails> componentCache;
    Core::GenderSpecificData genderData;
    
    // Hash mappings from XML data
    std::unordered_map<std::string, std::string> hashToTextLabelMap;
    std::unordered_map<std::string, Core::ComponentDetails> hashToComponentDetailsMap;

    // Core processing methods
    void buildTextLabelMappings();
    void processJsonData();
    void processXmlData();
    void processScriptMetaData();
   // void updateCacheWithMetadata();
    
    // Component processing
    Core::ComponentDetails processComponentDetails(const JsonItem& jsonItem);
    void cacheComponentDetails(const std::string& key, const Core::ComponentDetails& details);
    
    // XML processing
    void processXmlNode(const XmlNode& node, bool isMale);
    std::shared_ptr<Core::Outfit> processOutfit(const XmlOutfit& outfitData, std::string_view dlcName);
    void processOutfitComponents(std::shared_ptr<Core::Outfit> outfit, const std::vector<XmlItem>& components);

    // Helper methods
    void addItemToCollection(Core::ClothingCollection& collection, const Core::ClothingItem& item);
    void addOutfitToCollection(Core::ClothingCollection& collection, std::shared_ptr<Core::Outfit> outfit);
    std::string determineComponentType(const std::string& rawType);

    Core::ClothingItem* findExistingItem(std::string_view textLabel, bool isMale);
    Core::ClothingItem* findExistingItemByGXT(std::string_view gxtLabel, bool isFemale);
    void updateItemCaches(const Core::ClothingItem& item);
    std::string getLocalizedNameFromTextLabel(const std::string& textLabel);

public:
    ClothingProcessor(
        std::shared_ptr<Core::ILogger> logger,
        std::shared_ptr<JsonParser> jsonParser,
        std::shared_ptr<XmlParser> xmlParser,
        std::shared_ptr<ScriptMetadataParser> scriptMetaParser
    );

    bool process();
    const Core::GenderSpecificData& getProcessedData() const { return genderData; }
    
    // Query methods
    std::vector<std::shared_ptr<Core::Outfit>> getOutfitsByDLC(const std::string& dlcName) const;
        std::vector<Core::ClothingItem> getItemsByComponent(const std::string& componentType) const;
    std::vector<Core::ClothingItem> getItemsByDLC(const std::string& dlcName) const;
}; 