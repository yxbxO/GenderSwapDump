#include "ClothingDumper.h"
#include "../Core/Constants.h"
#include <fstream>
#include <map>
#include <unordered_map>
#include <iomanip>
#include <sstream>
#include <algorithm>

namespace Output {

    ClothingDumper::ClothingDumper(std::shared_ptr<Core::ILogger> logger)
        : logger(logger)
    {
    }

    ClothingDumper::~ClothingDumper() {
        if (outFile.is_open()) {
            outFile.close();
        }
    }

    void ClothingDumper::printItemDetails(std::ostream& out, const Core::ClothingItem& item, bool isUnlock, int indent) {
        try {
            writeIndent(out, indent);
            out << (isUnlock ? "UNLOCK" : "BUY") << " DETAILS:\n";
        
            writeIndent(out, indent + 4);
            out << "+- Item Name: " << std::string(item.itemName) << "\n";
        
            writeIndent(out, indent + 4);
            out << "+- Store Label: " << std::string(item.m_textlabel) << "\n";
        
            writeIndent(out, indent + 4);
            out << "+- Gender: " << std::string(item.m_gender) << "\n";
        
            writeIndent(out, indent + 4);
            out << "+- Component Type: " << cleanComponentTypeName(item.componentDetails.componentType) << "\n";
        
            writeIndent(out, indent + 4);
            out << "+- DLC Pack: " << Core::ComponentUtils::getDlcDisplayName(item.componentDetails.dlcName) << "\n";
        
            writeIndent(out, indent + 4);
            out << "+- Technical Info:\n";
        
            writeIndent(out, indent + 4);
            out << "|  +- Unique Key: " << std::string(item.m_key) << "\n";
        
            writeIndent(out, indent + 4);
            out << "|  +- Value Hash: " << std::string(item.value) << "\n";
        
            writeIndent(out, indent + 4);
            out << "|  +- Drawable ID: " << item.componentDetails.drawableId << "\n";
        }
        catch (const std::exception& e) {
            logger->error("Exception in printItemDetails: " + std::string(e.what()));
        }
    }

    // Removed unused printMatchGroup method

    bool ClothingDumper::validateFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) {
            logger->error("Failed to open file: " + filename);
            return false;
        }
        file.close();
        return true;
    }

    bool ClothingDumper::dumpToFile(const Core::GenderSpecificData& data, const std::string& filename) {
        try {
            if (!validateFile(filename)) {
                return false;
            }
        
            outFile.open(filename);
            if (!outFile) {
                logger->error("Failed to open output file: " + filename);
                return false;
            }

            // NEW: Dump matched pairs first (this is what the user expects)
            writeHeader(outFile, "GENDER SWAP CLOTHING PAIRS (VALUE-BASED MATCHING)");
            outFile << "This section shows clothing items that can be gender-swapped.\n";
            outFile << "Items with the same 'value' from script metadata are gender swap pairs.\n";
            outFile << "Example: DLC_MP_GR_F_FEET_1_7 matches DLC_MP_GR_M_BERD_2_9 if they share the same value.\n\n";
            dumpMatchedPairs(data);
            writeSeparator(outFile, 0);

            writeHeader(outFile, "MALE CLOTHING ITEMS");
            dumpCollection(data.male, true);
            writeSeparator(outFile, 0);

            writeHeader(outFile, "FEMALE CLOTHING ITEMS");
            dumpCollection(data.female, false);
        
            outFile.close();
            return true;
        }
        catch (const std::exception& e) {
            logger->error("Exception in dumpToFile: " + std::string(e.what()));
            return false;
        }
    }

    void ClothingDumper::dumpMatchedPairs(const Core::GenderSpecificData& data) {
        try {
            logger->log("Dumping matched clothing pairs based on script metadata values");
        
            // Group items by their VALUE field (this is the correct matching logic)
            std::map<std::string, std::pair<const Core::ClothingItem*, const Core::ClothingItem*>> pairs;
        
            // Process male items
            for (const auto& maleItem : data.male.items) {
                std::string value(maleItem.value);
                if (!value.empty() && value != "[NA]") {
                    pairs[value].first = &maleItem;
                }
            }
        
            // Process female items
            for (const auto& femaleItem : data.female.items) {
                std::string value(femaleItem.value);
                if (!value.empty() && value != "[NA]") {
                    pairs[value].second = &femaleItem;
                }
            }
        
            // Dump matched pairs
            size_t matchedCount = 0;
            size_t maleOnlyCount = 0;
            size_t femaleOnlyCount = 0;
        
            outFile << "GENDER SWAP PAIRS (Based on Script Metadata Values):\n";
            outFile << "===================================================\n\n";
        
            for (const auto& [valueKey, pair] : pairs) {
                const auto& [maleItem, femaleItem] = pair;
            
                if (maleItem && femaleItem) {
                    matchedCount++;
                    outFile << "GENDER SWAP PAIR - Value: " << valueKey << "\n";
                    outFile << "===============================================\n";
                
                    outFile << "MALE VERSION:\n";
                    dumpItemDetails(maleItem, 2);
                
                    outFile << "\nFEMALE VERSION:\n";
                    dumpItemDetails(femaleItem, 2);
                
                    // Show if they're correctly different genders but same value
                    outFile << "\n  [OK] VALID GENDER SWAP: Same value (" << valueKey 
                           << ") but opposite genders\n";
                
                    outFile << "\n" << std::string(50, '-') << "\n\n";
                }
                else if (maleItem) {
                    maleOnlyCount++;
                    // Optional: List male-only items
                    // outFile << "MALE ONLY - Value: " << valueKey 
                    //        << " Key: " << maleItem->m_key << "\n";
                }
                else if (femaleItem) {
                    femaleOnlyCount++;
                    // Optional: List female-only items
                    // outFile << "FEMALE ONLY - Value: " << valueKey 
                    //        << " Key: " << femaleItem->m_key << "\n";
                }
            }
        
            // Summary statistics
            outFile << "\nGENDER SWAP STATISTICS:\n";
            outFile << "======================\n";
            outFile << "Matched Gender Swap Pairs: " << matchedCount << "\n";
            outFile << "Male Only Items: " << maleOnlyCount << "\n";
            outFile << "Female Only Items: " << femaleOnlyCount << "\n";
            outFile << "Total Unique Values: " << pairs.size() << "\n\n";
        
            logger->log("Gender swap pairs dump complete: " + std::to_string(matchedCount) + " pairs found");
        }
        catch (const std::exception& e) {
            logger->error("Exception in dumpMatchedPairs: " + std::string(e.what()));
        }
    }

    std::string ClothingDumper::extractBaseKey(std::string_view itemKey) {
        std::string key(itemKey);
    
        // Remove gender-specific prefixes to find the base key
        // CLO_X7F_U_3_11 -> CLO_X7_U_3_11 (remove F)
        // CLO_X7M_U_3_11 -> CLO_X7_U_3_11 (remove M)
    
        if (key.length() < 8) return "";
    
        // Look for patterns like CLO_X7F_ or CLO_X7M_
        size_t pos = key.find("_F_");
        if (pos != std::string::npos) {
            key.erase(pos + 1, 1); // Remove the 'F'
            return key;
        }
    
        pos = key.find("_M_");
        if (pos != std::string::npos) {
            key.erase(pos + 1, 1); // Remove the 'M'
            return key;
        }
    
        // Try other patterns like DLC_MP_*_F_ or DLC_MP_*_M_
        pos = key.rfind("_F_");
        if (pos != std::string::npos) {
            key.erase(pos + 1, 1);
            return key;
        }
    
        pos = key.rfind("_M_");
        if (pos != std::string::npos) {
            key.erase(pos + 1, 1);
            return key;
        }
    
        return key; // Return as-is if no gender pattern found
    }

    void ClothingDumper::dumpItemDetails(const Core::ClothingItem* item, int indent) {
        if (!item) return;
    
        writeIndent(outFile, indent);
        outFile << "Name: " << std::string(item->itemName) << "\n";
    
        writeIndent(outFile, indent);
        outFile << "GXT Label: " << std::string(item->m_textlabel) << "\n";
    
        writeIndent(outFile, indent);
        outFile << "Key: " << std::string(item->m_key) << "\n";
    
        writeIndent(outFile, indent);
        outFile << "Component Type: " << item->componentDetails.componentType << "\n";
    
        writeIndent(outFile, indent);
        outFile << "Drawable ID: " << item->componentDetails.drawableId << "\n";
    
        writeIndent(outFile, indent);
        outFile << "Texture ID: " << item->componentDetails.textureId << "\n";
    
        writeIndent(outFile, indent);
                outFile << "DLC: " << Core::ComponentUtils::getDlcDisplayName(item->componentDetails.dlcName) << "\n";
    
        // Pricing information removed as requested
    }

    bool ClothingDumper::dumpCollection(const Core::ClothingCollection& collection, bool isMale) {
        try {
            std::string gender = isMale ? "Male" : "Female";
            dumpCollectionStats(outFile, collection, gender);
            writeSeparator(outFile);

            writeHeader(outFile, "ITEMS BY COMPONENT TYPE");
            dumpComponentTypeStats(outFile, collection);
            dumpClothingItems(outFile, collection.items, 2);
            writeSeparator(outFile);

            writeHeader(outFile, "OUTFITS");
            dumpOutfits(outFile, collection.outfits);
            writeSeparator(outFile);

            writeHeader(outFile, "DLC BREAKDOWN");
            dumpDLCStats(outFile, collection);
        
            return true;
        }
        catch (const std::exception& e) {
            logger->error("Exception in dumpCollection: " + std::string(e.what()));
            return false;
        }
    }

    void ClothingDumper::dumpComponentDetails(std::ostream& out, const Core::ComponentDetails& component, int indent) {
        writeIndent(out, indent);
        out << "Component Type: " << cleanComponentTypeName(component.componentType) << "\n";
        writeIndent(out, indent);
        out << "Drawable ID: " << (component.drawableId != -1 ? std::to_string(component.drawableId) : "Not Available") << "\n";
        writeIndent(out, indent);
        out << "Texture ID: " << (component.textureId != -1 ? std::to_string(component.textureId) : "Not Available") << "\n";
        writeIndent(out, indent);
        out << "DLC: " << Core::ComponentUtils::getDlcDisplayName(component.dlcName) << "\n";

    
        // Unlock requirements removed - data is not useful (always empty or "None")

        // Dump variants
        if (!component.variants.empty()) {
            writeIndent(out, indent);
            out << "Variants:\n";
            dumpComponentVariants(out, component.variants, indent + 2);
        }

    
    }

    void ClothingDumper::dumpComponentVariants(std::ostream& out,
                                             const std::vector<Core::ComponentVariant>& variants,
                                             int indent) {
        for (const auto& variant : variants) {
            writeIndent(out, indent);
            out << "- " << variant.localizedName << "\n";
            writeIndent(out, indent + 2);
            out << "Texture ID: " << variant.textureId << "\n";
        
            // Unlock conditions removed - data is not useful
        
                writeIndent(out, indent + 2);
            out << (variant.isDefault ? "Default Variant" : "Alternative Variant") << "\n";
        }
    }



    void ClothingDumper::dumpOutfit(std::ostream& out, const Core::Outfit& outfit, int indent) {
        writeIndent(out, indent);
        out << "Outfit: " << outfit.localizedName << "\n";
        writeIndent(out, indent);
        out << "Hash: " << outfit.outfitHash << "\n";
    
        if (!outfit.textLabel.empty()) {
        writeIndent(out, indent);
            out << "Text Label: " << outfit.textLabel << "\n";
        }
    
        writeIndent(out, indent);
        out << "DLC: " << Core::ComponentUtils::getDlcDisplayName(outfit.dlcName) << "\n";
        writeIndent(out, indent);
        out << "Status: " << (outfit.isComplete ? "Complete" : "Incomplete") << "\n";

        // Unlock requirements removed - data is not useful

        writeIndent(out, indent);
        out << "Components:\n";
        dumpOutfitComponents(out, outfit.components, indent + 2);
    }

    void ClothingDumper::dumpOutfitComponents(std::ostream& out,
                                            const std::vector<std::shared_ptr<Core::OutfitComponent>>& components,
                                            int indent) {
        for (const auto& component : components) {
            writeIndent(out, indent);
            out << "- " << component->localizedName << "\n";
            writeIndent(out, indent + 2);
            out << "Hash: " << component->componentHash << "\n";
        
            if (!component->textLabel.empty()) {
                writeIndent(out, indent + 2);
                out << "Text Label: " << component->textLabel << "\n";
            }
        
            writeIndent(out, indent + 2);
            out << "Required: " << (component->isRequired ? "Yes" : "Optional") << "\n";
        
            // Dump component details
            dumpComponentDetails(out, component->details, indent + 4);
        }
    }

    void ClothingDumper::dumpClothingItems(std::ostream& out, const std::vector<Core::ClothingItem>& items, int indent) {
        for (const auto& item : items) {
            writeIndent(out, indent);
        
            // Enhanced item name display with character filtering
            std::string displayName = std::string(item.m_textlabel);
            std::string localizedName = "";
        
            // Filter out non-printable characters and replace with safe alternatives
            auto cleanString = [](const std::string& input) -> std::string {
                std::string cleaned;
                for (char c : input) {
                    if (c >= 32 && c <= 126) { // Printable ASCII range
                        cleaned += c;
                    } else if (c == 0 || c < 0) {
                        // Skip null bytes and negative values
                        continue;
                    } else {
                        cleaned += '?'; // Replace non-printable with ?
                    }
                }
                return cleaned;
            };
        
            displayName = cleanString(displayName);
        
            // Try to get the actual item name
            if (displayName == "NA" || displayName.empty() || displayName == "?" || displayName.find('?') != std::string::npos) {
                std::string itemNameStr = cleanString(std::string(item.itemName));
                if (itemNameStr != "UNKNOWN_NAME" && !itemNameStr.empty() && itemNameStr.find('?') == std::string::npos) {
                    displayName = itemNameStr;
                } else {
                    displayName = "[No Name Available]";
                }
            } else {
                // We have a clean text label
                localizedName = displayName;
            }
        
            out << "Item: " << displayName << "\n";
        
            // Show text label if different from display name
            if (!localizedName.empty() && localizedName != displayName) {
                writeIndent(out, indent + 2);
                out << "Text Label: " << localizedName << "\n";
            }
        
            // Show unique hash/key information
            writeIndent(out, indent + 2);
            out << "Key: " << std::string(item.m_key) << "\n";
        
            if (!item.value.empty() && item.value != "NA") {
                writeIndent(out, indent + 2);
                out << "Unique Hash: " << std::string(item.value) << "\n";
            }
        
            // Show gender
            writeIndent(out, indent + 2);
            out << "Gender: " << std::string(item.m_gender) << "\n";
        
            // Note: Texture ID will be shown in component details below
        
            if (item.isPartOfOutfit()) {
                writeIndent(out, indent + 2);
                out << "Part of Outfit: " << item.parentOutfit->localizedName;
                if (!item.parentOutfit->textLabel.empty()) {
                    out << " (Label: " << item.parentOutfit->textLabel << ")";
                }
                out << "\n";
            }

            dumpComponentDetails(out, item.componentDetails, indent + 2);
            writeSeparator(out, indent);
        }
    }

    void ClothingDumper::dumpCollectionStats(std::ostream& out,
                                           const Core::ClothingCollection& collection,
                                           const std::string& gender) {
        out << gender << " Collection Statistics:\n";
        out << "Total Items: " << collection.totalItems() << "\n";
        out << "Total Outfits: " << collection.totalOutfits() << "\n";
    }

    void ClothingDumper::dumpComponentTypeStats(std::ostream& out,
                                             const Core::ClothingCollection& collection) {
        std::map<std::string, int> componentCounts;
        for (const auto& item : collection.items) {
            componentCounts[item.componentDetails.componentType]++;
        }

        out << "Items by Component Type:\n";
        for (const auto& [type, count] : componentCounts) {
            out << "- " << type << ": " << count << " items\n";
        }
    }

    void ClothingDumper::dumpDLCStats(std::ostream& out,
                                    const Core::ClothingCollection& collection) {
        std::map<std::string, std::pair<int, int>> dlcCounts; // items, outfits
    
        // Count items
        for (const auto& item : collection.items) {
            std::string displayName = Core::ComponentUtils::getDlcDisplayName(item.componentDetails.dlcName);
            dlcCounts[displayName].first++;
        }
    
        // Count outfits
        for (const auto& outfit : collection.outfits) {
            std::string displayName = Core::ComponentUtils::getDlcDisplayName(outfit->dlcName);
            dlcCounts[displayName].second++;
        }

        out << "Content by DLC:\n";
        for (const auto& [dlc, counts] : dlcCounts) {
            out << "- " << dlc << ":\n";
            out << "  Items: " << counts.first << "\n";
            out << "  Outfits: " << counts.second << "\n";
        }
    }

    void ClothingDumper::writeIndent(std::ostream& out, int indent) const {
        out << std::string(indent, ' ');
    }

    // Removed unused pricing formatting methods

    std::string ClothingDumper::cleanComponentTypeName(const std::string& componentType) const {
        if (componentType.empty()) {
            return "Unknown";
        }
    
        std::string cleaned = componentType;
    
        // Remove trailing numbers and underscores (e.g., "TORSO_2" -> "TORSO")
        size_t lastAlpha = cleaned.find_last_not_of("_0123456789");
        if (lastAlpha != std::string::npos) {
            cleaned = cleaned.substr(0, lastAlpha + 1);
        }
    
        // Convert to proper case (first letter uppercase, rest lowercase)
        if (!cleaned.empty()) {
            std::transform(cleaned.begin(), cleaned.end(), cleaned.begin(), ::tolower);
            cleaned[0] = std::toupper(cleaned[0]);
        }
    
        // Handle special cases for better readability
        if (cleaned == "Torso") return "Torso";
        if (cleaned == "Legs") return "Legs";
        if (cleaned == "Feet") return "Shoes";
        if (cleaned == "Berd") return "Mask";
        if (cleaned == "Hair") return "Hair";
        if (cleaned == "Head") return "Hat";
        if (cleaned == "Eyes") return "Glasses";
        if (cleaned == "Ears") return "Earrings";
        if (cleaned == "Hand") return "Gloves";
        if (cleaned == "Wrist") return "Watch";
        if (cleaned == "Jbib") return "Accessory";
        if (cleaned == "Accs") return "Accessory";
        if (cleaned == "Task") return "Gear";
        if (cleaned == "Decl") return "Badge";
        if (cleaned == "Outfit") return "Outfit";
    
        return cleaned;
    }

    void ClothingDumper::writeSeparator(std::ostream& out, int indent) const {
        writeIndent(out, indent);
        out << std::string(80 - indent, '-') << "\n";
    }

    void ClothingDumper::writeHeader(std::ostream& out, const std::string& header, int indent) const {
        writeSeparator(out, indent);
        writeIndent(out, indent);
        out << header << "\n";
        writeSeparator(out, indent);
    }

    std::string ClothingDumper::formatValue(const std::string& value) const {
        return value.empty() ? "[NA]" : value;
    }

    bool ClothingDumper::dumpOutfitsByDLC(
        const std::vector<std::shared_ptr<Core::Outfit>>& outfits,
                                        const std::string& filename) {
        try {
            if (!validateFile(filename)) {
                return false;
            }

            std::ofstream file(filename);
            writeHeader(file, "OUTFITS BY DLC");
            dumpOutfits(file, outfits);
        
            return true;
        }
        catch (const std::exception& e) {
            logger->error("Exception in dumpOutfitsByDLC: " + std::string(e.what()));
            return false;
        }
    }

    bool ClothingDumper::dumpItemsByComponent(
        const std::vector<Core::ClothingItem>& items,
                                           const std::string& filename) {
        try {
            if (!validateFile(filename)) {
                return false;
            }

            std::ofstream file(filename);
            writeHeader(file, "ITEMS BY COMPONENT");
            dumpClothingItems(file, items, 2);
        
            return true;
        }
        catch (const std::exception& e) {
            logger->error("Exception in dumpItemsByComponent: " + std::string(e.what()));
            return false;
        }
    }

    bool ClothingDumper::dumpItemsByDLC(
        const std::vector<Core::ClothingItem>& items,
                                      const std::string& filename) {
        try {
            if (!validateFile(filename)) {
                return false;
            }

            std::ofstream file(filename);
            writeHeader(file, "ITEMS BY DLC");
            dumpClothingItems(file, items, 2);
        
            return true;
        }
        catch (const std::exception& e) {
            logger->error("Exception in dumpItemsByDLC: " + std::string(e.what()));
            return false;
        }
    }

    void ClothingDumper::dumpOutfits(std::ostream& out, const std::vector<std::shared_ptr<Core::Outfit>>& outfits, int indent) {
        for (const auto& outfit : outfits) {
            dumpOutfit(out, *outfit, indent);
            writeSeparator(out, indent);
        }
    }

    void ClothingDumper::dumpClothingCollection(std::ostream& out, const Core::ClothingCollection& collection, const std::string& gender, int indent) {
        writeHeader(out, gender + " Collection", indent);
        dumpCollectionStats(out, collection, gender);
        writeSeparator(out, indent);
    
        writeHeader(out, "Items", indent);
        dumpClothingItems(out, collection.items, indent);
        writeSeparator(out, indent);
    
        writeHeader(out, "Outfits", indent);
        dumpOutfits(out, collection.outfits, indent);
    }

} // namespace Output 