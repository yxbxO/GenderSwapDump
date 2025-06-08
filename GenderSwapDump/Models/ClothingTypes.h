#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <memory>
#include "../Core/Constants.h"

namespace Core {
    // Forward declarations
    struct OutfitComponent;
    struct Outfit;

    struct ComponentVariant {
        int textureId = -1;
        std::string localizedName;
        bool isDefault = false;
    };

    struct ComponentDetails {
        std::string componentType;      // e.g. "HAT", "MASK", etc.
        int drawableId = -1;
        int textureId;
        std::vector<ComponentVariant> variants;
        std::string dlcName;
    };

    struct OutfitComponent {
        std::string componentHash;
        std::string localizedName;
        std::string textLabel;      // GXT text label for localization
        ComponentDetails details;
        bool isRequired;  // Some outfit components might be optional
        int defaultVariantIndex;
    };

    struct Outfit {
        std::string outfitHash;
        std::string localizedName;
        std::string textLabel;      // GXT text label for localization
        std::string dlcName;
        std::vector<std::shared_ptr<OutfitComponent>> components;
        bool isComplete;  // Whether all components are available/unlocked
    };

    struct ClothingItem {
        // Core identifiers
        std::string_view m_textlabel{Defaults::NA};
        std::string_view m_key{Defaults::NA};
        std::string_view m_gender{Defaults::NA};
        std::string_view value{Defaults::NA};

        // Enhanced item details
        ComponentDetails componentDetails;
        std::shared_ptr<Outfit> parentOutfit;  // If this item is part of an outfit
        
        // Existing fields
        std::string_view dlcName{Defaults::NA};
        std::string_view comp{Defaults::UNKNOWN_COMPONENT};
        std::string_view drawableId{Defaults::NA};
        std::string_view textureId{Defaults::NA};
        std::string_view itemName{Defaults::UNKNOWN_NAME};

        // Utility functions
        bool isMale() const { 
            return m_gender == GenderIdentifiers::MALE_STR; 
        }
        
        bool isFemale() const { 
            return m_gender == GenderIdentifiers::FEMALE_STR; 
        }

        bool hasValidComponent() const {
            return comp != Defaults::UNKNOWN_COMPONENT;
        }

        bool isValid() const {
            return m_textlabel != Defaults::NA && 
                   m_key != Defaults::NA && 
                   m_gender != Defaults::NA;
        }

        bool isPartOfOutfit() const {
            return parentOutfit != nullptr;
        }
    };

    struct ClothingCollection {
        std::vector<ClothingItem> items;
        std::vector<std::shared_ptr<Outfit>> outfits;

        void clear() {
            items.clear();
            outfits.clear();
        }

        size_t totalItems() const {
            return items.size();
        }

        size_t totalOutfits() const {
            return outfits.size();
        }
    };

    struct GenderSpecificData {
        ClothingCollection male;
        ClothingCollection female;

        void clear() {
            male.clear();
            female.clear();
        }

        size_t totalItems() const {
            return male.totalItems() + female.totalItems();
        }

        size_t totalOutfits() const {
            return male.totalOutfits() + female.totalOutfits();
        }
    };
} 