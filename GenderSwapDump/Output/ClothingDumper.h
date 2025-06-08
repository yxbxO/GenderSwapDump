#pragma once
#include "../Core/Interfaces.h"
#include "../Models/ClothingTypes.h"
#include <memory>
#include <string>
#include <fstream>
#include <map>
#include <unordered_map>

namespace Output {
    class ClothingDumper {
    private:
        std::shared_ptr<Core::ILogger> logger;
        std::ofstream outFile;

        // Component dumping methods
        void dumpComponentDetails(std::ostream& out, const Core::ComponentDetails& component, int indent = 0);
        void dumpComponentVariants(std::ostream& out, const std::vector<Core::ComponentVariant>& variants, int indent = 0);


        // Outfit dumping methods
        void dumpOutfit(std::ostream& out, const Core::Outfit& outfit, int indent = 0);
        void dumpOutfitComponents(std::ostream& out, const std::vector<std::shared_ptr<Core::OutfitComponent>>& components, int indent = 0);
        void dumpOutfits(std::ostream& out, const std::vector<std::shared_ptr<Core::Outfit>>& outfits, int indent = 0);

        // Collection dumping methods
        void dumpClothingCollection(std::ostream& out, const Core::ClothingCollection& collection, const std::string& gender, int indent = 0);
        void dumpClothingItems(std::ostream& out, const std::vector<Core::ClothingItem>& items, int indent = 0);

        // Item detail printing
        void printItemDetails(std::ostream& out, const Core::ClothingItem& item, bool isUnlock, int indent = 0);

        // Helper methods
        void writeIndent(std::ostream& out, int indent) const;
        void writeSeparator(std::ostream& out, int indent = 0) const;
        void writeHeader(std::ostream& out, const std::string& header, int indent = 0) const;
        std::string formatValue(const std::string& value) const;
        std::string cleanComponentTypeName(const std::string& componentType) const;

        // Matching functionality
        void dumpMatchedPairs(const Core::GenderSpecificData& data);
        std::string extractBaseKey(std::string_view itemKey);
        void dumpItemDetails(const Core::ClothingItem* item, int indent);

        // Statistics methods
        void dumpCollectionStats(std::ostream& out, const Core::ClothingCollection& collection, const std::string& gender);
        void dumpComponentTypeStats(std::ostream& out, const Core::ClothingCollection& collection);
        void dumpDLCStats(std::ostream& out, const Core::ClothingCollection& collection);
        
        // File validation
        bool validateFile(const std::string& filename) const;

    public:
        explicit ClothingDumper(std::shared_ptr<Core::ILogger> logger);
        ~ClothingDumper();

        // Main dump methods
        bool dumpToFile(const Core::GenderSpecificData& data, const std::string& filename);
        bool dumpCollection(const Core::ClothingCollection& collection, bool isMale);
        
        // Specialized dump methods
        bool dumpOutfitsByDLC(const std::vector<std::shared_ptr<Core::Outfit>>& outfits, 
                             const std::string& filename);
        bool dumpItemsByDLC(const std::vector<Core::ClothingItem>& items, 
                           const std::string& filename);
        bool dumpItemsByComponent(const std::vector<Core::ClothingItem>& items, 
                                const std::string& filename);
        

    };
} 