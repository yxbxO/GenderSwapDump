#include "ClothingProcessor.h"
#include "../Core/Constants.h"
#include <algorithm>
#include <stdexcept>
#include <cctype>

ClothingProcessor::ClothingProcessor(
    std::shared_ptr<Core::ILogger> logger,
    std::shared_ptr<JsonParser> jsonParser,
    std::shared_ptr<XmlParser> xmlParser,
    std::shared_ptr<ScriptMetadataParser> scriptMetaParser
)
    : logger(logger)
    , jsonParser(jsonParser)
    , xmlParser(xmlParser)
    , scriptMetaParser(scriptMetaParser)
{
}

bool ClothingProcessor::process() {
    try {
        logger->log("Starting enhanced clothing processing");
        
        // Clear existing data
        outfitCache.clear();
        componentCache.clear();
        genderData.clear();
        
        // Clear enhanced caches
        cache = ProcessingCache();
        
        // Process all data sources with enhanced metadata
       
        // Step 1: Build textLabel mappings from XML files
        buildTextLabelMappings();
        
        // Step 2: Process script metadata using textLabel mappings
        processScriptMetaData();
        
        // Step 3: Enrich with localized names from JSON
        processJsonData();
        
        // Step 4: Process remaining XML data (outfits, etc.)
        processXmlData();
        
        logger->log("Clothing processing complete");
        return true;
    }
    catch (const std::exception& e) {
        logger->error("Exception in process: " + std::string(e.what()));
        return false;
    }
}

void ClothingProcessor::processJsonData() {
    logger->log("Processing JSON data to enrich existing items with localized names");
    
    // Safety check: ensure gender collections are initialized
    logger->log("Gender data status - Male items: " + std::to_string(genderData.male.items.size()) + 
               ", Female items: " + std::to_string(genderData.female.items.size()));
    
    if (genderData.male.items.empty() && genderData.female.items.empty()) {
        logger->log("Warning: Gender collections are empty. Script metadata may not have been processed.");
    }
    
    for (const auto& jsonItem : jsonParser->getItems()) {
        // Determine if this is male or female based on filename
        bool isFemale = std::string(jsonItem.filename).find("female") != std::string::npos;
        
        // Try to find existing item by GXT label (textLabel)
        Core::ClothingItem* existingItem = findExistingItemByGXT(jsonItem.GXT, isFemale);
        
        if (existingItem) {
            // Enrich existing item with JSON localized name
            if (!jsonItem.localized.empty()) {
                existingItem->itemName = jsonItem.localized;
                logger->log("Enriched item " + std::string(existingItem->m_key) + 
                           " with localized name: " + std::string(jsonItem.localized));
            }
            
            // Update component details with JSON data (prioritize JSON over XML)
            if (!jsonItem.drawableId.empty()) {
                try {
                    int jsonDrawableId = std::stoi(std::string(jsonItem.drawableId));
                    if (existingItem->componentDetails.drawableId != jsonDrawableId) {
                        if (existingItem->componentDetails.drawableId != -1) {
                            logger->log("DrawableID mismatch for " + std::string(existingItem->m_key) + 
                                       ": XML=" + std::to_string(existingItem->componentDetails.drawableId) + 
                                       ", JSON=" + std::to_string(jsonDrawableId) + " - Using JSON value");
                        }
                        // Prioritize JSON data over XML data
                        existingItem->componentDetails.drawableId = jsonDrawableId;
                    }
                } catch (const std::exception& e) {
                    logger->error("Failed to parse JSON drawableId for " + std::string(jsonItem.GXT));
                }
            }
            
            if (!jsonItem.textureId.empty()) {
                try {
                    int jsonTextureId = std::stoi(std::string(jsonItem.textureId));
                    if (existingItem->componentDetails.textureId != jsonTextureId) {
                        if (existingItem->componentDetails.textureId != -1) {
                            logger->log("TextureID mismatch for " + std::string(existingItem->m_key) + 
                                       ": XML=" + std::to_string(existingItem->componentDetails.textureId) + 
                                       ", JSON=" + std::to_string(jsonTextureId) + " - Using JSON value");
                        }
                        // Prioritize JSON data over XML data
                        existingItem->componentDetails.textureId = jsonTextureId;
                    }
                } catch (const std::exception& e) {
                    logger->error("Failed to parse JSON textureId for " + std::string(jsonItem.GXT));
                }
            }
            
            logger->log("Enriched existing item: " + std::string(existingItem->m_key) + 
                       " | TextLabel: " + std::string(existingItem->m_textlabel) + 
                       " | Name: " + std::string(existingItem->itemName) +
                       " | Type: " + existingItem->componentDetails.componentType +
                       " | DrawableID: " + std::to_string(existingItem->componentDetails.drawableId) +
                       " | TextureID: " + std::to_string(existingItem->componentDetails.textureId));
        } else {
            // Create new item if no matching script metadata item found
            // This happens when JSON has items not in script metadata
            logger->log("No script metadata found for JSON item: " + std::string(jsonItem.GXT) + 
                       ", creating standalone item");
            
            auto componentDetails = processComponentDetails(jsonItem);
            
            Core::ClothingItem clothingItem;
            clothingItem.m_textlabel = jsonItem.GXT;
            clothingItem.m_key = jsonItem.GXT;  // Use GXT as key if no script metadata
            clothingItem.itemName = jsonItem.localized;
            clothingItem.componentDetails = componentDetails;
            clothingItem.comp = jsonItem.component;
            
            if (isFemale) {
                clothingItem.m_gender = Core::GenderIdentifiers::FEMALE_STR;
                addItemToCollection(genderData.female, clothingItem);
            } else {
                clothingItem.m_gender = Core::GenderIdentifiers::MALE_STR;
                addItemToCollection(genderData.male, clothingItem);
            }
            
            logger->log("Created standalone JSON item: " + std::string(jsonItem.GXT) + 
                       " | Name: " + std::string(jsonItem.localized) +
                       " | Type: " + componentDetails.componentType +
                       " | DrawableID: " + std::to_string(componentDetails.drawableId) +
                       " | TextureID: " + std::to_string(componentDetails.textureId));
        }
        
        // Store in cache with GXT key for quick lookup
        auto componentDetails = processComponentDetails(jsonItem);
        cacheComponentDetails(std::string(jsonItem.GXT), componentDetails);
        if (!jsonItem.localized.empty()) {
            cacheComponentDetails(std::string(jsonItem.localized), componentDetails);
        }
    }
    
    logger->log("JSON processing complete. Total items now - Male: " + 
               std::to_string(genderData.male.items.size()) + 
               ", Female: " + std::to_string(genderData.female.items.size()));
}

void ClothingProcessor::processXmlData() {
    logger->log("Processing XML data for outfits");
    
    // Process male nodes
    for (const auto& node : xmlParser->getMaleNodes()) {
        processXmlNode(node, true);
    }
    
    // Process female nodes
    for (const auto& node : xmlParser->getFemaleNodes()) {
        processXmlNode(node, false);
    }
}

void ClothingProcessor::processXmlNode(const XmlNode& node, bool isMale) {
    for (const auto& outfitData : node.outfits) {
        // Process outfit
        auto outfit = processOutfit(outfitData, node.dlcName);
        
        // Process components
        processOutfitComponents(outfit, outfitData.components);
        
        // Add to cache and appropriate collection
        outfitCache[std::string(outfit->outfitHash)] = outfit;
        if (isMale) {
            addOutfitToCollection(genderData.male, outfit);
        } else {
            addOutfitToCollection(genderData.female, outfit);
        }
    }
    
    logger->log("Process outfit : Done");

    // Process individual items
    for (const auto& item : node.items) {
        // Check if we already have this item from JSON data
        auto existingItem = findExistingItem(item.textLabel, isMale);
        if (existingItem != nullptr) {
            // Update only missing information
            if (existingItem->itemName.empty() && !item.itemName.empty()) {
                existingItem->itemName = std::string(item.itemName);
            }
            if (existingItem->dlcName.empty()) {
                existingItem->dlcName = Core::ComponentUtils::resolveDlcFromHash(std::string(item.uniqueNameHash), node.dlcName);
            }
            continue;
        }

        // If not found in JSON data, create new item from XML
        Core::ClothingItem clothingItem;
        clothingItem.m_textlabel = item.textLabel;
        clothingItem.m_key = item.uniqueNameHash;
        clothingItem.dlcName = Core::ComponentUtils::resolveDlcFromHash(std::string(item.uniqueNameHash), node.dlcName);
        clothingItem.itemName = std::string(item.itemName);
        
        // Create component details using unified processor
        Core::ComponentDetails details = ComponentProcessor::createComponentDetails(
            std::string(item.eCompType),
            std::string(item.uniqueNameHash),
            std::string(item.localDrawableIndex),
            std::string(item.textureIndex),
            std::string(node.dlcName)
        );
        
        // Enhance with JSON data (JSON takes priority)
        ComponentProcessor::enrichWithJsonData(details, std::string(item.textLabel), jsonParser->getItems());
        
        clothingItem.componentDetails = details;
        
        // Add to appropriate collection
        if (isMale) {
            clothingItem.m_gender = Core::GenderIdentifiers::MALE_STR;
            addItemToCollection(genderData.male, clothingItem);
        } else {
            clothingItem.m_gender = Core::GenderIdentifiers::FEMALE_STR;
            addItemToCollection(genderData.female, clothingItem);
        }
    }

    logger->log("Process individual items : Done");
}

std::shared_ptr<Core::Outfit> ClothingProcessor::processOutfit(const XmlOutfit& outfitData, std::string_view dlcName) {
    auto outfit = std::make_shared<Core::Outfit>();
    outfit->outfitHash = std::string(outfitData.uniqueNameHash);
    outfit->localizedName = std::string(outfitData.outfitName);
    outfit->dlcName = std::string(dlcName);
    
    // Extract textLabel for the outfit itself
    std::string outfitHash = std::string(outfitData.uniqueNameHash);
    auto textLabelIt = hashToTextLabelMap.find(outfitHash);
    if (textLabelIt != hashToTextLabelMap.end()) {
        outfit->textLabel = textLabelIt->second;
        
        // Try to get localized name from JSON data using the textLabel
        std::string localizedName = getLocalizedNameFromTextLabel(outfit->textLabel);
        if (!localizedName.empty() && localizedName != "NULL" && localizedName != "NO_LABEL") {
            outfit->localizedName = localizedName;
        }
        
        logger->log("Linked outfit " + outfitHash + " with textLabel: " + outfit->textLabel + 
                   " | Localized: " + outfit->localizedName);
    } else {
        logger->log("No textLabel found for outfit hash: " + outfitHash);
    }
    
    return outfit;
}

void ClothingProcessor::processOutfitComponents(std::shared_ptr<Core::Outfit> outfit, 
                                              const std::vector<XmlItem>& components) {
    for (const auto& comp : components) {
        auto outfitComp = std::make_shared<Core::OutfitComponent>();
        outfitComp->componentHash = std::string(comp.uniqueNameHash);
        outfitComp->localizedName = std::string(comp.itemName);
        outfitComp->isRequired = true; // Default to required
        
        // Extract textLabel for this component
        std::string compHash = std::string(comp.uniqueNameHash);
        auto textLabelIt = hashToTextLabelMap.find(compHash);
        if (textLabelIt != hashToTextLabelMap.end()) {
            outfitComp->textLabel = textLabelIt->second;
            
            // Try to get localized name from JSON data
            std::string localizedName = getLocalizedNameFromTextLabel(outfitComp->textLabel);
            if (!localizedName.empty() && localizedName != "NULL" && localizedName != "NO_LABEL") {
                outfitComp->localizedName = localizedName;
            }
            
            logger->log("Linked outfit component " + compHash + " with textLabel: " + outfitComp->textLabel + 
                       " | Localized: " + outfitComp->localizedName);
        } else {
            logger->log("No textLabel found for component hash: " + compHash);
        }
        
        // Link to component details if available
        auto it = componentCache.find(std::string(comp.uniqueNameHash));
        if (it != componentCache.end()) {
            outfitComp->details = it->second;
            // Also update localized name from component details if available
            if (outfitComp->localizedName.empty() || outfitComp->localizedName == std::string(comp.itemName)) {
                for (const auto& variant : outfitComp->details.variants) {
                    if (!variant.localizedName.empty()) {
                        outfitComp->localizedName = variant.localizedName;
                        break;
                    }
                }
            }
        } else {
            // Create component details using unified processor
            Core::ComponentDetails details = ComponentProcessor::createComponentDetails(
                std::string(comp.eCompType),
                std::string(comp.uniqueNameHash),
                std::string(comp.localDrawableIndex),
                std::string(comp.textureIndex),
                outfit->dlcName
            );
            
            // Enhance with JSON data (JSON takes priority)
            ComponentProcessor::enrichWithJsonData(details, outfitComp->textLabel, jsonParser->getItems());
            
            outfitComp->details = details;
        }
        
        logger->log("[outfit components] : " + outfitComp->componentHash + 
                   " | Type: " + outfitComp->details.componentType +
                   " | TextLabel: " + outfitComp->textLabel +
                   " | Localized: " + outfitComp->localizedName);
        outfit->components.push_back(outfitComp);
    }
    
    outfit->isComplete = true;  // All outfits are considered complete
}

void ClothingProcessor::processScriptMetaData() {
    logger->log("Processing script metadata as primary data source");
    
    logger->log("Using pre-built mappings - TextLabels: " + std::to_string(hashToTextLabelMap.size()) + 
               ", ComponentDetails: " + std::to_string(hashToComponentDetailsMap.size()));
    
    // Process male items from script metadata
    for (const auto& metaItem : scriptMetaParser->getMaleItems()) {
        Core::ClothingItem clothingItem;
        clothingItem.m_key = metaItem.m_key;
        clothingItem.value = metaItem.value;
        clothingItem.m_gender = Core::GenderIdentifiers::MALE_STR;
        clothingItem.comp = metaItem.comp;
        clothingItem.dlcName = metaItem.dlcName;
        
        std::string hashKey = std::string(metaItem.m_key);
        
        // Extract textLabel from pre-built mapping
        auto textLabelIt = hashToTextLabelMap.find(hashKey);
        if (textLabelIt != hashToTextLabelMap.end()) {
            clothingItem.m_textlabel = textLabelIt->second;
            logger->log("Linked script metadata " + hashKey + 
                       " with textLabel: " + textLabelIt->second);
        } else {
            logger->log("No textLabel found for hash: " + hashKey);
        }
        
        // Extract component details from pre-built mapping
        auto detailsIt = hashToComponentDetailsMap.find(hashKey);
        if (detailsIt != hashToComponentDetailsMap.end()) {
            clothingItem.componentDetails = detailsIt->second;
            // Override DLC name from script metadata if available
            if (!metaItem.dlcName.empty()) {
                clothingItem.componentDetails.dlcName = std::string(metaItem.dlcName);
                clothingItem.dlcName = metaItem.dlcName;
            }
        } else {
            // Create component details using unified processor
            Core::ComponentDetails details = ComponentProcessor::createComponentDetails(
                std::string(metaItem.comp),
                hashKey,
                "", // No drawable index from script
                "", // No texture index from script
                std::string(metaItem.dlcName)
            );
            
            // Enhance with JSON data if textLabel is available
            if (!clothingItem.m_textlabel.empty()) {
                ComponentProcessor::enrichWithJsonData(details, std::string(clothingItem.m_textlabel), jsonParser->getItems());
            }
            
            clothingItem.componentDetails = details;
        }
        
        addItemToCollection(genderData.male, clothingItem);
        logger->log("Added male item: " + hashKey + 
                   " | Value: " + std::string(metaItem.value) + 
                   " | TextLabel: " + std::string(clothingItem.m_textlabel) +
                   " | Type: " + clothingItem.componentDetails.componentType +
                   " | DrawableID: " + std::to_string(clothingItem.componentDetails.drawableId) +
                   " | TextureID: " + std::to_string(clothingItem.componentDetails.textureId));
    }
    
    // Process female items from script metadata
    for (const auto& metaItem : scriptMetaParser->getFemaleItems()) {
        Core::ClothingItem clothingItem;
        clothingItem.m_key = metaItem.m_key;
        clothingItem.value = metaItem.value;
        clothingItem.m_gender = Core::GenderIdentifiers::FEMALE_STR;
        clothingItem.comp = metaItem.comp;
        clothingItem.dlcName = metaItem.dlcName;
        
        std::string hashKey = std::string(metaItem.m_key);
        
        // Extract textLabel from pre-built mapping
        auto textLabelIt = hashToTextLabelMap.find(hashKey);
        if (textLabelIt != hashToTextLabelMap.end()) {
            clothingItem.m_textlabel = textLabelIt->second;
            logger->log("Linked script metadata " + hashKey + 
                       " with textLabel: " + textLabelIt->second);
        } else {
            logger->log("No textLabel found for hash: " + hashKey);
        }
        
        // Extract component details from pre-built mapping
        auto detailsIt = hashToComponentDetailsMap.find(hashKey);
        if (detailsIt != hashToComponentDetailsMap.end()) {
            clothingItem.componentDetails = detailsIt->second;
            // Override DLC name from script metadata if available
            if (!metaItem.dlcName.empty()) {
                clothingItem.componentDetails.dlcName = std::string(metaItem.dlcName);
                clothingItem.dlcName = metaItem.dlcName;
            }
        } else {
            // Create component details using unified processor
            Core::ComponentDetails details = ComponentProcessor::createComponentDetails(
                std::string(metaItem.comp),
                hashKey,
                "", // No drawable index from script
                "", // No texture index from script
                std::string(metaItem.dlcName)
            );
            
            // Enhance with JSON data if textLabel is available
            if (!clothingItem.m_textlabel.empty()) {
                ComponentProcessor::enrichWithJsonData(details, std::string(clothingItem.m_textlabel), jsonParser->getItems());
            }
            
            clothingItem.componentDetails = details;
        }
        
        addItemToCollection(genderData.female, clothingItem);
        logger->log("Added female item: " + hashKey + 
                   " | Value: " + std::string(metaItem.value) + 
                   " | TextLabel: " + std::string(clothingItem.m_textlabel) +
                   " | Type: " + clothingItem.componentDetails.componentType +
                   " | DrawableID: " + std::to_string(clothingItem.componentDetails.drawableId) +
                   " | TextureID: " + std::to_string(clothingItem.componentDetails.textureId));
    }
    
    logger->log("Script metadata processing complete. Male items: " + 
               std::to_string(genderData.male.items.size()) + 
               ", Female items: " + std::to_string(genderData.female.items.size()));
}

//void ClothingProcessor::updateCacheWithMetadata() {
//    // Simplified cache update - removed unlock requirement processing since those fields were removed
//    // This method now primarily ensures cache consistency
//}

void ClothingProcessor::addItemToCollection(Core::ClothingCollection& collection, 
                                          const Core::ClothingItem& item) {
    collection.items.push_back(item);
}

void ClothingProcessor::addOutfitToCollection(Core::ClothingCollection& collection, 
                                            std::shared_ptr<Core::Outfit> outfit) {
    collection.outfits.push_back(outfit);
}

// Function removed - replaced with ComponentProcessor::extractComponentType

std::string ClothingProcessor::determineComponentType(const std::string& rawType) {
    // Use the unified component processor
    return ComponentProcessor::extractComponentType(rawType);
}



// Query methods implementation
std::vector<std::shared_ptr<Core::Outfit>> ClothingProcessor::getOutfitsByDLC(const std::string& dlcName) const {
    std::vector<std::shared_ptr<Core::Outfit>> results;
    for (const auto& [_, outfit] : outfitCache) {
        if (outfit->dlcName == dlcName) {
            results.push_back(outfit);
        }
    }
    return results;
}

std::vector<Core::ClothingItem> ClothingProcessor::getItemsByComponent(const std::string& componentType) const {
    std::vector<Core::ClothingItem> results;
    auto processCollection = [&](const Core::ClothingCollection& collection) {
        for (const auto& item : collection.items) {
            if (item.componentDetails.componentType == componentType) {
                results.push_back(item);
            }
        }
    };
    
    processCollection(genderData.male);
    processCollection(genderData.female);
    return results;
}

std::vector<Core::ClothingItem> ClothingProcessor::getItemsByDLC(const std::string& dlcName) const {
    std::vector<Core::ClothingItem> results;
    auto processCollection = [&](const Core::ClothingCollection& collection) {
        for (const auto& item : collection.items) {
            if (item.componentDetails.dlcName == dlcName) {
                results.push_back(item);
            }
        }
    };
    
    processCollection(genderData.male);
    processCollection(genderData.female);
    return results;
}



// Helper method to find existing item
Core::ClothingItem* ClothingProcessor::findExistingItem(std::string_view key, bool isMale) {
    auto& collection = isMale ? genderData.male : genderData.female;
    for (auto& item : collection.items) {
        if (item.m_key == key) {
            return &item;
        }
    }
    return nullptr;
}

// Helper method to find item by GXT label
Core::ClothingItem* ClothingProcessor::findExistingItemByGXT(std::string_view gxtLabel, bool isFemale) {
    if (gxtLabel.empty()) {
        logger->log("findExistingItemByGXT called with empty gxtLabel");
        return nullptr;
    }
    
    auto& collection = isFemale ? genderData.female : genderData.male;
    
    logger->log("findExistingItemByGXT: Searching for '" + std::string(gxtLabel) + 
               "' in " + (isFemale ? "female" : "male") + 
               " collection with " + std::to_string(collection.items.size()) + " items");
    
    for (auto& item : collection.items) {
        // Add safety check for m_textlabel
        if (item.m_textlabel.empty()) {
            continue; // Skip items with empty textlabels
        }
        
        if (item.m_textlabel == gxtLabel) {
            logger->log("Found match: " + std::string(item.m_key) + " -> " + std::string(item.m_textlabel));
            return &item;
        }
    }
    
    logger->log("No match found for GXT: " + std::string(gxtLabel));
    return nullptr;
}

Core::ComponentDetails ClothingProcessor::processComponentDetails(const JsonItem& jsonItem) {
    Core::ComponentDetails details;
    details.componentType = determineComponentType(std::string(jsonItem.component));
    
    try {
        // Process drawable ID
        if (!jsonItem.drawableId.empty()) {
            try {
                details.drawableId = std::stoi(std::string(jsonItem.drawableId));
            }
            catch (const std::exception& e) {
                details.drawableId = -1;
                logger->error("Failed to parse drawableId for item " + std::string(jsonItem.GXT));
            }
        }

        // Process texture ID
        if (!jsonItem.textureId.empty()) {
            try {
                details.textureId = std::stoi(std::string(jsonItem.textureId));
            }
            catch (const std::exception& e) {
                details.textureId = -1;
                logger->error("Failed to parse textureId for item " + std::string(jsonItem.GXT));
            }
        }

        //// Set DLC name if available
        //if (!jsonItem.dlcName.empty()) {
        //    details.dlcName = std::string(jsonItem.dlcName);
        //}

        return details;
    }
    catch (const std::exception& e) {
        logger->error("Exception in processComponentDetails: " + std::string(e.what()));
        return details;
    }
}

void ClothingProcessor::updateItemCaches(const Core::ClothingItem& item) {
    // Update DLC cache
    if (!item.dlcName.empty()) {
        auto dlcIt = cache.dlcItems.find(std::string(item.dlcName));
        if (dlcIt == cache.dlcItems.end()) {
            cache.dlcItems[std::string(item.dlcName)] = std::vector<std::string>();
        }
        cache.dlcItems[std::string(item.dlcName)].push_back(std::string(item.m_key));
    }
    
    // Update component cache
    if (!item.comp.empty()) {
        auto compIt = cache.componentItems.find(std::string(item.comp));
        if (compIt == cache.componentItems.end()) {
            cache.componentItems[std::string(item.comp)] = std::vector<std::string>();
        }
        cache.componentItems[std::string(item.comp)].push_back(std::string(item.m_key));
    }
}

void ClothingProcessor::cacheComponentDetails(const std::string& key, const Core::ComponentDetails& details) {
    componentCache[key] = details;
}

// Removed unused metadata processing methods (pricing, unlocks, compatibility validation)

void ClothingProcessor::buildTextLabelMappings() {
    logger->log("Building textLabel mappings from XML files");
    
    // Ensure XML parser has data
    if (!xmlParser->isValid()) {
        logger->error("XML parser is not valid. Cannot build textLabel mappings.");
        return;
    }
    
    // Clear any existing mappings
    hashToTextLabelMap.clear();
    hashToComponentDetailsMap.clear();
    
    // Process male XML nodes
    for (const auto& node : xmlParser->getMaleNodes()) {
        for (const auto& item : node.items) {
            if (!item.uniqueNameHash.empty()) {
                std::string hashKey = std::string(item.uniqueNameHash);
                
                // Map textLabel
                if (!item.textLabel.empty()) {
                    hashToTextLabelMap[hashKey] = std::string(item.textLabel);
                    logger->log("Mapped hash to textLabel: " + hashKey + 
                               " -> " + std::string(item.textLabel));
                }
                
                // Map component details
                Core::ComponentDetails details;
                details.componentType = determineComponentType(std::string(item.eCompType));
                
                // If component type is unknown, try to extract from uniqueNameHash
                if (details.componentType == "UNKNOWN" || details.componentType.empty()) {
                    std::string extractedType = ComponentProcessor::extractComponentType("", hashKey);
                    if (extractedType != "UNKNOWN") {
                        details.componentType = extractedType;
                        logger->log("Enhanced mapping component type detection for " + hashKey + 
                                   ": " + std::string(item.eCompType) + " -> " + extractedType + 
                                   " (from hash: " + hashKey + ")");
                    }
                }
                
                // Extract drawable and texture IDs from XML
                if (!item.localDrawableIndex.empty()) {
                    try {
                        details.drawableId = std::stoi(std::string(item.localDrawableIndex));
                    } catch (const std::exception& e) {
                        details.drawableId = -1;
                        logger->error("Failed to parse localDrawableIndex for " + hashKey + ": " + e.what());
                    }
                } else {
                    details.drawableId = -1;
                }
                
                if (!item.textureIndex.empty()) {
                    try {
                        details.textureId = std::stoi(std::string(item.textureIndex));
                    } catch (const std::exception& e) {
                        details.textureId = -1;
                        logger->error("Failed to parse textureIndex for " + hashKey + ": " + e.what());
                    }
                } else {
                    details.textureId = -1;
                }
                
                // Extract DLC name from uniqueNameHash, fallback to node dlcName
                details.dlcName = Core::ComponentUtils::resolveDlcFromHash(hashKey, node.dlcName);
                
                hashToComponentDetailsMap[hashKey] = details;
                logger->log("Mapped hash to component details: " + hashKey + 
                           " -> Type: " + details.componentType + 
                           ", DrawableID: " + std::to_string(details.drawableId) + 
                           ", TextureID: " + std::to_string(details.textureId));
            }
        }
    }
    
    // Process female XML nodes
    for (const auto& node : xmlParser->getFemaleNodes()) {
        for (const auto& item : node.items) {
            if (!item.uniqueNameHash.empty()) {
                std::string hashKey = std::string(item.uniqueNameHash);
                
                // Map textLabel
                if (!item.textLabel.empty()) {
                    hashToTextLabelMap[hashKey] = std::string(item.textLabel);
                    logger->log("Mapped hash to textLabel: " + hashKey + 
                               " -> " + std::string(item.textLabel));
                }
                
                // Map component details
                Core::ComponentDetails details;
                details.componentType = determineComponentType(std::string(item.eCompType));
                
                // If component type is unknown, try to extract from uniqueNameHash
                if (details.componentType == "UNKNOWN" || details.componentType.empty()) {
                    std::string extractedType = ComponentProcessor::extractComponentType("", hashKey);
                    if (extractedType != "UNKNOWN") {
                        details.componentType = extractedType;
                        logger->log("Enhanced mapping component type detection for " + hashKey + 
                                   ": " + std::string(item.eCompType) + " -> " + extractedType + 
                                   " (from hash: " + hashKey + ")");
                    }
                }
                
                // Extract drawable and texture IDs from XML
                if (!item.localDrawableIndex.empty()) {
                    try {
                        details.drawableId = std::stoi(std::string(item.localDrawableIndex));
                    } catch (const std::exception& e) {
                        details.drawableId = -1;
                        logger->error("Failed to parse localDrawableIndex for " + hashKey + ": " + e.what());
                    }
                } else {
                    details.drawableId = -1;
                }
                
                if (!item.textureIndex.empty()) {
                    try {
                        details.textureId = std::stoi(std::string(item.textureIndex));
                    } catch (const std::exception& e) {
                        details.textureId = -1;
                        logger->error("Failed to parse textureIndex for " + hashKey + ": " + e.what());
                    }
                } else {
                    details.textureId = -1;
                }
                
                // Extract DLC name from uniqueNameHash, fallback to node dlcName
                details.dlcName = Core::ComponentUtils::resolveDlcFromHash(hashKey, node.dlcName);
                
                hashToComponentDetailsMap[hashKey] = details;
                logger->log("Mapped hash to component details: " + hashKey + 
                           " -> Type: " + details.componentType + 
                           ", DrawableID: " + std::to_string(details.drawableId) + 
                           ", TextureID: " + std::to_string(details.textureId));
            }
        }
    }
    
    logger->log("Created hash->textLabel mapping with " + std::to_string(hashToTextLabelMap.size()) + " entries");
    logger->log("Created hash->componentDetails mapping with " + std::to_string(hashToComponentDetailsMap.size()) + " entries");
}

std::string ClothingProcessor::getLocalizedNameFromTextLabel(const std::string& textLabel) {
    if (textLabel.empty() || textLabel == "NO_LABEL") {
        return "";
    }
    
    // Search through all JSON items to find matching GXT text label
    const auto& jsonItems = jsonParser->getItems();
    for (const auto& item : jsonItems) {
        if (std::string(item.GXT) == textLabel) {
            std::string localized = std::string(item.localized);
            if (!localized.empty() && localized != "NULL" && localized != "NO_LABEL") {
                return localized;
            }
        }
    }
    
    // If not found in JSON items, return empty string
    return "";
}

// Unified ComponentProcessor implementation
std::string ClothingProcessor::ComponentProcessor::extractComponentType(const std::string& rawType, const std::string& uniqueNameHash) {
    // First, try the unified component mapping with rawType
    auto* mapping = Core::ComponentUtils::findUnifiedComponent(rawType);
    if (mapping) {
        return std::string(mapping->internalType);
    }
    
    // If no mapping found and we have a hash, try hash extraction
    if (!uniqueNameHash.empty()) {
        std::string hash = uniqueNameHash;
        std::transform(hash.begin(), hash.end(), hash.begin(), ::toupper);
        
        // Look for known component patterns in the hash
        for (const auto& comp : Core::ComponentTypes::UNIFIED_COMPONENTS) {
            if (!comp.shortType.empty()) {
                std::string pattern = "_" + std::string(comp.shortType);
                size_t pos = hash.find(pattern);
                if (pos != std::string::npos) {
                    // Verify boundary
                    size_t nextPos = pos + pattern.length();
                    if (nextPos >= hash.length() || hash[nextPos] == '_' || 
                        hash[nextPos] == '0' || std::isdigit(hash[nextPos])) {
                        return std::string(comp.internalType);
                    }
                }
            }
        }
        
        // Check for outfit patterns
        if (hash.find("_OUTFIT") != std::string::npos || hash.find("OUTFIT_") != std::string::npos) {
            return "OUTFIT";
        }
    }
    
    // Fallback to uppercase rawType if not empty
    if (!rawType.empty()) {
        std::string result = rawType;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }
    
    return "UNKNOWN";
}

std::string ClothingProcessor::ComponentProcessor::resolveDlcName(const std::string& rawDlcName) {
    return Core::ComponentUtils::resolveDlcName(rawDlcName);
}

Core::ComponentDetails ClothingProcessor::ComponentProcessor::createComponentDetails(
    const std::string& eCompType, 
    const std::string& uniqueNameHash,
    const std::string& drawableIndex,
    const std::string& textureIndex,
    const std::string& dlcName) {
    
    Core::ComponentDetails details;
    details.componentType = extractComponentType(eCompType, uniqueNameHash);
    details.dlcName = Core::ComponentUtils::resolveDlcFromHash(uniqueNameHash, dlcName);
    
    // Parse drawable ID
    if (!drawableIndex.empty()) {
        try {
            details.drawableId = std::stoi(drawableIndex);
        } catch (const std::exception&) {
            details.drawableId = -1;
        }
    } else {
        details.drawableId = -1;
    }
    
    // Parse texture ID  
    if (!textureIndex.empty()) {
        try {
            details.textureId = std::stoi(textureIndex);
        } catch (const std::exception&) {
            details.textureId = -1;
        }
    } else {
        details.textureId = -1;
    }
    
    return details;
}

void ClothingProcessor::ComponentProcessor::enrichWithJsonData(Core::ComponentDetails& details, 
                                                             const std::string& textLabel,
                                                             const std::vector<JsonItem>& jsonItems) {
    if (textLabel.empty() || textLabel == "NO_LABEL") return;
    
    // Find matching JSON data and prioritize it over XML data
    for (const auto& item : jsonItems) {
        if (std::string(item.GXT) == textLabel) {
            // JSON data takes priority over XML data
            if (!item.drawableId.empty()) {
                try {
                    int jsonDrawableId = std::stoi(std::string(item.drawableId));
                    if (jsonDrawableId >= 0) {
                        details.drawableId = jsonDrawableId;
                    }
                } catch (const std::exception&) {}
            }
            
            if (!item.textureId.empty()) {
                try {
                    int jsonTextureId = std::stoi(std::string(item.textureId));
                    if (jsonTextureId >= 0) {
                        details.textureId = jsonTextureId;
                    }
                } catch (const std::exception&) {}
            }
            
            // Add localized variant
            std::string localized = std::string(item.localized);
            if (!localized.empty() && localized != "NULL" && localized != "NO_LABEL") {
                Core::ComponentVariant variant;
                variant.textureId = details.textureId;
                variant.localizedName = localized;
                variant.isDefault = true;
                details.variants.push_back(variant);
            }
            break;
        }
    }
} 