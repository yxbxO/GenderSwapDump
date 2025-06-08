#pragma once
#include <string_view>
#include <vector>

namespace Core {
    // File System Constants
    namespace FileConstants {
        constexpr std::string_view SCRIPT_METADATA = "scriptmetadata.meta";
        constexpr std::string_view JSON_DIR = "Jsonfiles/";
        constexpr std::string_view XML_DIR = "xmlfiles/";
        constexpr std::string_view LOG_FILE = "clothing_dump.txt";
    }

    // Default Values
    namespace Defaults {
        constexpr std::string_view NA = "[NA]";
        constexpr std::string_view NOT_IN_STORES = "[NOT IN STORES]";
        constexpr std::string_view UNKNOWN_NAME = "[NO/ UNKNOWN NAME]";
        constexpr std::string_view UNKNOWN_COMPONENT = "[UNKNOWN]";
    }

    // Gender Identifiers
    namespace GenderIdentifiers {
        constexpr std::string_view MALE_PREFIX = "_M_";
        constexpr std::string_view FEMALE_PREFIX = "_F_";
        constexpr std::string_view MALE_STR = "Male";
        constexpr std::string_view FEMALE_STR = "Female";
    }

    // XML Node Names
    namespace XmlNodes {
        constexpr std::string_view PED_OUTFITS = "pedOutfits";
        constexpr std::string_view PED_COMPONENTS = "pedComponents";
        constexpr std::string_view PED_PROPS = "pedProps";
        constexpr std::string_view ITEM = "Item";
        constexpr std::string_view TEXT_LABEL = "textLabel";
        constexpr std::string_view UNIQUE_HASH = "uniqueNameHash";
        constexpr std::string_view MP_APPAREL_DATA = "MPApparelData";
        constexpr std::string_view PED_NAME = "pedName";
        constexpr std::string_view DLC_NAME = "dlcName";
    }

    // JSON Field Names
    namespace JsonFields {
        constexpr std::string_view GXT = "GXT";
        constexpr std::string_view LOCALIZED = "Localized";
        constexpr std::string_view COMPONENT = "Component";
        constexpr std::string_view DRAWABLE_ID = "DrawableID";
        constexpr std::string_view TEXTURE_ID = "TextureID";
        constexpr std::string_view UNLOCK_TYPE = "UnlockType";
        constexpr std::string_view PRICE = "Price";
    }

    // Error Messages
    namespace ErrorMessages {
        constexpr std::string_view PARSE_ERROR = "Parse error: ";
        constexpr std::string_view FILE_NOT_FOUND = "File not found: ";
        constexpr std::string_view INVALID_JSON = "Invalid JSON structure";
        constexpr std::string_view INVALID_XML = "Invalid XML structure";
        constexpr std::string_view DIRECTORY_ERROR = "Failed to access directory: ";
    }

    // Success Messages
    namespace SuccessMessages {
        constexpr std::string_view PARSE_SUCCESS = "Successfully parsed ";
        constexpr std::string_view PROCESS_SUCCESS = "Successfully processed ";
        constexpr std::string_view MATCH_SUCCESS = "Successfully matched items";
    }

    // Component Types - Unified Mapping System
    namespace ComponentTypes {
        struct UnifiedComponentMapping {
            std::string_view xmlType;           // Full XML type (e.g., "PV_COMP_UPPR")
            std::string_view shortType;         // Short type used in hashes (e.g., "UPPR", "EYES")
            std::string_view internalType;      // Standardized internal type (e.g., "TORSO", "GLASSES")
            std::string_view displayName;       // Human-readable display name (e.g., "Torso", "Glasses")
            std::string_view legacyInFiles;     // Legacy file identifier for compatibility
            std::string_view jsonPattern;       // Pattern to match in JSON filenames
        };
        
        const std::vector<UnifiedComponentMapping> UNIFIED_COMPONENTS = {
            {"PV_COMP_HEAD", "HEAD",  "HAT",      "Hat",         "PHEAD",   "hats"},
            {"PV_COMP_BERD", "BERD",  "MASK",     "Mask",        "BERD",    "mask"},
            {"PV_COMP_EYES", "EYES",  "GLASSES",  "Glasses",     "PEYES",   "glasses"},
            {"PV_COMP_HAIR", "HAIR",  "HAIR",     "Hair",        "HAIR",    "hair"},
            {"PV_COMP_UPPR", "UPPR",  "TORSO",    "Torso",       "TORSO",   "torsos"},
            {"PV_COMP_JBIB", "JBIB",  "TORSO_2",  "Torso 2 ( tops )",     "JBIB",    "tops"},
            {"PV_COMP_ACCS", "ACCS",  "TORSO_1",  "Torso 1 ( undershirts )",     "ACCS",    "undershirts"},
            {"PV_COMP_LOWR", "LOWR",  "LEGS",     "Legs",        "LEGS",    "legs"},
            {"PV_COMP_FEET", "FEET",  "SHOES",    "Shoes",       "FEET",    "shoes"},
            {"PV_COMP_TASK", "TASK",  "GLOVES",   "Gloves",      "TASK",    "accessories"},
            {"PV_COMP_TEEF", "TEEF",  "TEETH",    "Teeth",       "TEETH",   "ears"},
            {"PV_COMP_DECL", "DECL",  "DECAL",    "Decal",       "DECL",    ""},
            {"",             "WRIST", "WRIST",    "Wrist",       "WRIST",   "watch"},
            {"",             "OUTFIT","OUTFIT",   "Outfit",      "OUTFIT",  ""},
            {"",             "SPECIAL","UNKNOWN", "Unknown",     "SPECIAL", ""}
        };
    }
    
    // Prop Anchor Points
    namespace AnchorPoints {
        struct AnchorMapping {
            std::string_view anchorPoint;   // XML anchor point
            std::string_view displayName;   // Display name
            std::string_view description;   // Description
        };
        
        const std::vector<AnchorMapping> ANCHORS = {
            {"ANCHOR_HEAD",         "Head",         "Head-mounted props (hats, helmets, headwear)"},
            {"ANCHOR_EYES",         "Eyes",         "Eye-mounted props (glasses, goggles)"},
            {"ANCHOR_LEFT_WRIST",   "Left Wrist",   "Left wrist props (watches, bracelets)"},
            {"ANCHOR_RIGHT_WRIST",  "Right Wrist",  "Right wrist props (watches, bracelets)"},
            {"ANCHOR_LEFT_HAND",    "Left Hand",    "Left hand props (rings, gloves)"},
            {"ANCHOR_RIGHT_HAND",   "Right Hand",   "Right hand props (rings, gloves)"}
        };
    }

    // DLC Information
    namespace DlcInfo {
        struct DlcMapping {
            std::string_view name;
            std::string_view dlc_key;
            std::string_view dlcFileName;
        };

        const std::vector<DlcMapping> DLC_LIST = {
            {"Beach Bum Update", "DLC_MP_BEACH", "mpbeach"},
            {"Holiday Gifts DLC", "DLC_MP_XMAS", "mpchristmas"},
            {"Valentine's Day Massacre Special DLC", "DLC_MP_VAL", "mpvalentines"},
            {"Business Update", "DLC_MP_BUSI", "mpbusiness"},
            {"Business Update 2", "DLC_MP_BUSI2", "mpbusiness2"},
            {"Business Update 2", "DLC_MP_BUS2", "mpbusiness2"},
            {"I'm Not A Hipster Update", "DLC_MP_HIPS", "mphipster"},
            {"Independence Day Special DLC", "DLC_MP_IND", "mpindependence"},
            {"San Andreas Flight School Update", "DLC_MP_PILOT", "mppilot"},
            {"Last Team Standing Update", "DLC_MP_LTS", "mplts"},
            {"Festive Surprise", "DLC_MP_XMAS2", "mpchristmas2"},
            {"Heists Update", "DLC_MP_HEIST", "mpheist"},
            {"Ill-Gotten Gains Part 1", "DLC_MP_LUXE", "mpluxe"},
            {"Ill-Gotten Gains Part 1", "MP_DLC_LUXE", "mpluxe"},
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
            {"The Contract", "DLC_MP_FIXER", "mpsecurity"},

            { "Gen9EC", "DLC_MP_G9EC", "mpg9ec" },

            {"Summer 2022", "DLC_MP_SUM2", "mpSum2"},
            {"Summer 2022 G9EC", "DLC_MP_SUM2_G9EC", "mpSum2_G9EC"},

            {"Winter 2022", "DLC_MP_X22", "mpchristmas3"},
            //{"Winter 2022 G9EC", "DLC_MP_X22_G9EC", "mpchristmas3_g9ec"},

            {"Summer 2023", "DLC_MP_SUM23", "mp2023_01"},
            //{"Summer 2023 G9EC", "DLC_MP_SUM23_G9EC", "mp2023_01_g9ec"},

            {"Christmas2023", "DLC_MP_X23", "mp2023_02"},
            // {"Christmas2023 G9EC", "DLC_MP_X23_G9EC", "mp2023_02_g9ec"},

            {"Summer 2024", "DLC_MP_SUM24", "mp2024_01"},
            //
            // {"Summer 2024 G9EC", "DLC_MP_SUM24_G9EC", "mp2024_01_g9ec"},

            {"Winter 2024", "DLC_MP_X24", "mp2024_02"},
            {"Winter 2024 G9EC", "DLC_MP_X24_G9EC", "mp2024_02_g9ec"}
        };
    }

    // File Extensions
    namespace FileExtensions {
        constexpr std::string_view XML = ".xml";
        constexpr std::string_view JSON = ".json";
        constexpr std::string_view META = ".meta";
    }

    // Helper functions for component type lookups
    namespace ComponentUtils {
        // Find unified component mapping by any field
        inline const ComponentTypes::UnifiedComponentMapping* findUnifiedComponent(std::string_view type) {
            for (const auto& mapping : ComponentTypes::UNIFIED_COMPONENTS) {
                if (mapping.xmlType == type || 
                    mapping.shortType == type || 
                    mapping.internalType == type ||
                    mapping.legacyInFiles == type) {
                    return &mapping;
                }
            }
            return nullptr;
        }
        
        // Find DLC mapping by any field
        inline const DlcInfo::DlcMapping* findDlcMapping(std::string_view dlcKey) {
            for (const auto& mapping : DlcInfo::DLC_LIST) {
                if (mapping.dlc_key == dlcKey || 
                    mapping.dlcFileName == dlcKey ||
                    mapping.name == dlcKey) {
                    return &mapping;
                }
            }
            return nullptr;
        }
        
        // Resolve DLC name from various input formats
        inline std::string resolveDlcName(std::string_view input) {
            // Handle common patterns like "Female_freemode_business" -> "DLC_MP_BUSI"
            if (input.find("freemode_business") != std::string_view::npos) {
                auto* dlc = findDlcMapping("DLC_MP_BUSI");
                return dlc ? std::string(dlc->name) : std::string(input);
            }
            if (input.find("mpbiker") != std::string_view::npos || input.find("biker") != std::string_view::npos) {
                auto* dlc = findDlcMapping("DLC_MP_BIKER");
                return dlc ? std::string(dlc->name) : std::string(input);
            }
            if (input.find("mpbeach") != std::string_view::npos || input.find("beach") != std::string_view::npos) {
                auto* dlc = findDlcMapping("DLC_MP_BEACH");
                return dlc ? std::string(dlc->name) : std::string(input);
            }
            if (input.find("mpheist3") != std::string_view::npos || input.find("h3") != std::string_view::npos) {
                auto* dlc = findDlcMapping("DLC_MP_H3");
                return dlc ? std::string(dlc->name) : std::string(input);
            }
            
            // Try direct lookup first
            auto* dlc = findDlcMapping(input);
            return dlc ? std::string(dlc->name) : std::string(input);
        }
        
        // Get DLC display name with filename
        inline std::string getDlcDisplayName(std::string_view dlcKey) {
            auto* dlc = findDlcMapping(dlcKey);
            if (dlc) {
                return std::string(dlc->name) + " (" + std::string(dlc->dlcFileName) + ")";
            }
            return std::string(dlcKey);
        }
        
        // Find anchor mapping by anchor point
        inline const AnchorPoints::AnchorMapping* findAnchor(std::string_view anchorPoint) {
            for (const auto& mapping : AnchorPoints::ANCHORS) {
                if (mapping.anchorPoint == anchorPoint) {
                    return &mapping;
                }
            }
            return nullptr;
        }
        
        // Check if a string is a valid unified component type
        inline bool isValidUnifiedComponentType(std::string_view type) {
            return findUnifiedComponent(type) != nullptr;
        }
        
        // Check if a string is a valid anchor point
        inline bool isValidAnchorPoint(std::string_view anchorPoint) {
            return findAnchor(anchorPoint) != nullptr;
        }
        
        // Extract DLC key from uniqueNameHash
        inline std::string extractDlcFromHash(std::string_view uniqueNameHash) {
            // Look for DLC_MP_ pattern in the hash
            size_t dlcPos = uniqueNameHash.find("DLC_MP_");
            if (dlcPos != std::string_view::npos) {
                // Find the next underscore after DLC_MP_XXX to get the complete DLC key
                size_t startPos = dlcPos;
                size_t endPos = uniqueNameHash.find('_', startPos + 7); // Skip "DLC_MP_"
                
                // Look for pattern like DLC_MP_XXX_[M|F]_ or DLC_MP_XXX_G9EC_
                if (endPos != std::string_view::npos) {
                    // Check if next part is gender (M/F) or platform specific (G9EC)
                    std::string_view nextPart = uniqueNameHash.substr(endPos + 1, 1);
                    if (nextPart == "M" || nextPart == "F") {
                        // Standard pattern: DLC_MP_XXX_M_ or DLC_MP_XXX_F_
                        return std::string(uniqueNameHash.substr(startPos, endPos - startPos));
                    } else {
                        // Could be G9EC or other platform specific
                        size_t secondEndPos = uniqueNameHash.find('_', endPos + 1);
                        if (secondEndPos != std::string_view::npos) {
                            std::string_view platformPart = uniqueNameHash.substr(endPos + 1, secondEndPos - endPos - 1);
                            if (platformPart == "G9EC") {
                                // Platform specific: DLC_MP_XXX_G9EC
                                return std::string(uniqueNameHash.substr(startPos, secondEndPos - startPos));
                            } else {
                                // Regular DLC with extra part: DLC_MP_XXX_PART
                                return std::string(uniqueNameHash.substr(startPos, endPos - startPos));
                            }
                        } else {
                            // No more underscores, take up to this point
                            return std::string(uniqueNameHash.substr(startPos, endPos - startPos));
                        }
                    }
                } else {
                    // No underscore found after DLC_MP_, unlikely but handle gracefully
                    return std::string(uniqueNameHash.substr(startPos));
                }
            }
            
            // No DLC pattern found, return empty string
            return "";
        }
        
        // Resolve DLC name from uniqueNameHash first, then fallback to input patterns
        inline std::string resolveDlcFromHash(std::string_view uniqueNameHash, std::string_view fallbackInput = "") {
            // First try to extract DLC from hash
            std::string dlcKey = extractDlcFromHash(uniqueNameHash);
            if (!dlcKey.empty()) {
                auto* dlc = findDlcMapping(dlcKey);
                if (dlc) {
                    return std::string(dlc->name);
                }
            }
            
            // Fallback to regular resolution if provided
            if (!fallbackInput.empty()) {
                return resolveDlcName(fallbackInput);
            }
            
            // Return original hash if no resolution possible
            return std::string(uniqueNameHash);
        }
    }
} 