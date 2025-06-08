#pragma once
#include "../Core/Interfaces.h"
#include "rapidjson/document.h"
#include <vector>
#include <memory>

struct JsonItem {
    std::string_view filename;
    //std::string_view dlcName;
    std::string_view drawableId;
    std::string_view textureId;
    std::string_view GXT;
    std::string_view localized;
    std::string_view component;  // Type of item (accessories, hair, legs, etc.)
};

class JsonParser : public Core::IParser {
private:
    std::shared_ptr<Core::ILogger> logger;
    std::vector<std::string> filePaths;
    std::vector<rapidjson::Document> documents;
    std::vector<JsonItem> items;
    bool valid{false};

    // Helper method to determine component type from filename
    std::string_view getComponentType(const std::string_view& filename);

public:
    explicit JsonParser(std::shared_ptr<Core::ILogger> logger);
    
    bool parse() override;
    bool isValid() const override { return valid; }
    
    const std::vector<JsonItem>& getItems() const { return items; }
    const std::vector<rapidjson::Document>& getDocuments() const { return documents; }

private:
    bool loadJsonFile(const std::string& path);
    bool scanDirectory(const std::string& directory);
    bool parseDocuments();
}; 