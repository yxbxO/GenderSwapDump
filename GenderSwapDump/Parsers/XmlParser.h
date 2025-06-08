#pragma once
#include "../Core/Interfaces.h"
#include "pugixml/pugixml.hpp"
#include <memory>
#include <vector>
#include <string>

struct XmlItem {
    std::string_view itemName;
    std::string_view lockHash;
    std::string_view textLabel;
    std::string_view uniqueNameHash;
    std::string_view localDrawableIndex;
    std::string_view propIndex;
    std::string_view textureIndex;
    std::string_view eCompType;
    std::string_view eAnchorPoint;
};

struct XmlOutfit {
    std::string_view outfitName = "NA";  // From comment or NA
    std::string_view lockHash;
    std::string_view textLabel;
    std::string_view uniqueNameHash;
    std::vector<XmlItem> components;
    std::vector<XmlItem> props;
};

struct XmlNode {
    std::string_view pedName;
    std::string_view dlcName;
    std::string_view fullDlcName;
    std::vector<XmlOutfit> outfits;
    std::vector<XmlItem> items;
};

class XmlParser : public Core::IParser {
private:
    std::shared_ptr<Core::ILogger> logger;
    std::vector<std::string> filePaths;
    std::vector<pugi::xml_document> documents;
    std::vector<XmlNode> maleNodes;
    std::vector<XmlNode> femaleNodes;
    bool valid{false};

public:
    explicit XmlParser(std::shared_ptr<Core::ILogger> logger);
    
    bool parse() override;
    bool isValid() const override { return valid; }
    
    const std::vector<XmlNode>& getMaleNodes() const { return maleNodes; }
    const std::vector<XmlNode>& getFemaleNodes() const { return femaleNodes; }

private:
    bool loadXmlFile(const std::string& path);
    bool scanDirectory(const std::string& directory);
    bool parseNode(const pugi::xml_node& node, XmlNode& outNode);
    void categorizeNode(XmlNode& node);
}; 