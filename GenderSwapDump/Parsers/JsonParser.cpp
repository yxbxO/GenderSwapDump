#include "JsonParser.h"
#include "rapidjson/istreamwrapper.h"
#include <filesystem>
#include <fstream>
#include "../Core/Constants.h"

JsonParser::JsonParser(std::shared_ptr<Core::ILogger> logger)
    : logger(std::move(logger))
{
}

bool JsonParser::parse() {
    try {
        if (!scanDirectory(Core::FileConstants::JSON_DIR.data())) {
            logger->error("Failed to scan JSON directory");
            return false;
        }

        for (const auto& path : filePaths) {
            if (!loadJsonFile(path)) {
                logger->error("Failed to load JSON file: " + path);
                return false;
            }
        }

        if (!parseDocuments()) {
            logger->error("Failed to parse JSON documents");
            return false;
        }

        valid = true;
        return true;
    }
    catch (const std::exception& e) {
        logger->error(std::string("Exception in JsonParser::parse: ") + e.what());
        return false;
    }
}

bool JsonParser::loadJsonFile(const std::string& path) {
    std::ifstream ifs(path);
    if (!ifs) {
        return false;
    }

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    if (doc.HasParseError()) {
        return false;
    }

    documents.push_back(std::move(doc));
    return true;
}

bool JsonParser::scanDirectory(const std::string& directory) {
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
            if (entry.path().extension() == ".json") {
                filePaths.push_back(entry.path().string());
            }
        }
        return true;
    }
    catch (const std::filesystem::filesystem_error& e) {
        logger->error(std::string("Filesystem error: ") + e.what());
        return false;
    }
}

std::string_view JsonParser::getComponentType(const std::string_view& filename) {
    // Return the short component types that match our constants
    if (filename.find("accessories") != std::string_view::npos) return "ACCS";
    if (filename.find("hair") != std::string_view::npos) return "HAIR";
    if (filename.find("legs") != std::string_view::npos) return "LEGS";
    if (filename.find("shoes") != std::string_view::npos) return "FEET";
    if (filename.find("tops") != std::string_view::npos) return "JBIB";        // Torso 2
    if (filename.find("torsos") != std::string_view::npos) return "TORSO";     // Gloves/Arms
    if (filename.find("undershirts") != std::string_view::npos) return "ACCS"; // Torso 1
    if (filename.find("bracelets") != std::string_view::npos) return "WRIST";
    if (filename.find("ears") != std::string_view::npos) return "TEETH";       // Accessories
    if (filename.find("glasses") != std::string_view::npos) return "PEYES";
    if (filename.find("hats") != std::string_view::npos) return "PHEAD";
    if (filename.find("watch") != std::string_view::npos) return "WRIST";
    if (filename.find("mask") != std::string_view::npos) return "BERD";
    return "UNKNOWN";
}

bool JsonParser::parseDocuments() {
    try {
        logger->log("Starting JSON document parsing");
        
        for (size_t i = 0; i < documents.size(); ++i) {
            const auto& doc = documents[i];
            std::string_view filename = filePaths[i];
            std::string_view componentType = getComponentType(filename);
            
            logger->log("Parsing file: " + std::string(filename));
            logger->log("Component type: " + std::string(componentType));

            // Iterate through drawable IDs
            for (auto drawableIt = doc.MemberBegin(); drawableIt != doc.MemberEnd(); ++drawableIt) {
                std::string drawableId = drawableIt->name.GetString();
                
                // Iterate through texture IDs
                for (auto textureIt = drawableIt->value.MemberBegin(); 
                     textureIt != drawableIt->value.MemberEnd(); 
                     ++textureIt) {
                    
                    JsonItem item;
                    item.filename = filename;
                    item.component = componentType;
                    //item.drawableId = drawableId;
                    item.drawableId = drawableIt->name.GetString();
                    item.textureId = textureIt->name.GetString();
                    
                    // Get GXT and Localized values
                    if (textureIt->value.HasMember("GXT")) {
                        item.GXT = textureIt->value["GXT"].GetString();
                    }
                    if (textureIt->value.HasMember("Localized")) {
                        item.localized = textureIt->value["Localized"].GetString();
                    }

                    logger->log("Found item - DrawableID: " + std::string(item.drawableId) +
                                ", TextureID: " + std::string(item.textureId) + 
                                ", GXT: " + std::string(item.GXT) + 
                                ", Localized: " + std::string(item.localized));

                    items.push_back(std::move(item));
                }
            }
        }

        logger->log("Successfully parsed " + std::to_string(items.size()) + " items from JSON documents");
        return true;
    }
    catch (const std::exception& e) {
        logger->error("Exception in parseDocuments: " + std::string(e.what()));
        return false;
    }
} 