#include "XmlParser.h"
#include <filesystem>
#include "../Core/Constants.h"
#include "iostream"
XmlParser::XmlParser(std::shared_ptr<Core::ILogger> logger)
    : logger(std::move(logger))
{
}

bool XmlParser::parse() {
    try {
        if (!scanDirectory(Core::FileConstants::XML_DIR.data())) {
            logger->error("Failed to scan XML directory");
            return false;
        }

        std::cout << "scanDirectory done" << std::endl;

        for (const auto& path : filePaths) {
            std::cout << path << std::endl;

            if (!loadXmlFile(path)) {
                logger->error("Failed to load XML file: " + path);
                return false;
            }
        }

        std::cout << " XmlParser::loadXmlFile" << std::endl;

        valid = true;
        return true;
    }
    catch (const std::exception& e) {
        logger->error(std::string("Exception in XmlParser::parse: ") + e.what());
        return false;
    }
}

bool XmlParser::loadXmlFile(const std::string& path) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.c_str(), pugi::parse_default | pugi::parse_comments);

    std::cout << " XmlParser::loadXmlFile" << std::endl;

    if (!result) {
        logger->error("XML parse error: " + std::string(result.description()));
        return false;
    }

    std::cout << " XmlParser::loadXmlFile" << std::endl;

    XmlNode node;
    if (!parseNode(doc.document_element(), node)) {
        return false;
    }

    categorizeNode(node);
    documents.push_back(std::move(doc));
    return true;
}

bool XmlParser::parseNode(const pugi::xml_node& xmlNode, XmlNode& outNode) {
    try {
        logger->log("Parsing XML node...");

        // Parse basic node info
        if (auto pedName = xmlNode.child("pedName")) {
            outNode.pedName = pedName.child_value();
            logger->log("Found pedName: " + std::string(outNode.pedName));
        }

        if (auto dlcName = xmlNode.child("dlcName")) {
            outNode.dlcName = dlcName.child_value();
            logger->log("Found dlcName: " + std::string(outNode.dlcName));
        }

        if (auto fullDlcName = xmlNode.child("fullDlcName")) {
            outNode.fullDlcName = fullDlcName.child_value();
            logger->log("Found fullDlcName: " + std::string(outNode.fullDlcName));
        }

        // Parse outfits
        logger->log("Parsing pedOutfits...");
        auto outfitsNode = xmlNode.child("pedOutfits");
        if (outfitsNode) {
            std::string_view currentOutfitName = Core::Defaults::NA;

            // Iterate through all nodes including comments
            for (auto node = outfitsNode.first_child(); node; node = node.next_sibling()) {
                // Check if this is a comment node
                if (node.type() == pugi::node_comment) {
                    // Extract outfit name from comment
                    currentOutfitName = node.value();

                    logger->log("Found outfit comment: " + std::string(currentOutfitName));
                }
                // Process outfit Item node
                else if (std::string(node.name()) == "Item") {
                    XmlOutfit outfit;
                    outfit.outfitName = currentOutfitName;
                    outfit.lockHash = node.child("lockHash").child_value();
                    outfit.textLabel = node.child("textLabel").child_value();
                    outfit.uniqueNameHash = node.child("uniqueNameHash").child_value();

                    // Parse included components
                    for (auto comp : node.child("includedPedComponents").children("Item")) {
                        XmlItem item;
                        item.uniqueNameHash = comp.child("nameHash").child_value();
                        //item.enumValue = comp.child("enumValue").attribute("value").as_string();
                        item.eCompType = comp.child("eCompType").child_value();
                        outfit.components.push_back(std::move(item));
                    }

                    // Parse included props
                    for (auto prop : node.child("includedPedProps").children("Item")) {
                        XmlItem item;
                        item.uniqueNameHash = prop.child("nameHash").child_value();
                        //item.enumValue = prop.child("enumValue").attribute("value").as_string();
                        item.eAnchorPoint = prop.child("eAnchorPoint").child_value();
                        outfit.props.push_back(std::move(item));
                    }

                    logger->log("Found outfit: " + std::string(outfit.outfitName) + 
                                ", Hash: " + std::string(outfit.uniqueNameHash) + 
                                ", Components: " + std::to_string(outfit.components.size()) +
                                ", Props: " + std::to_string(outfit.props.size()));

                    outNode.outfits.push_back(std::move(outfit));
                    currentOutfitName = Core::Defaults::NA;  // Reset outfit name after use
                }
            }
        }

        // Parse pedComponents
        logger->log("Parsing pedComponents...");
        auto componentsNode = xmlNode.child("pedComponents");
        if (componentsNode) {
            std::string_view currentComponentName = Core::Defaults::NA;

            for (auto node = componentsNode.first_child(); node; node = node.next_sibling()) {
                if (node.type() == pugi::node_comment) {
                    currentComponentName = node.value();
                    logger->log("Found component comment: " + std::string(currentComponentName));
                }
                else if (std::string(node.name()) == "Item") {
                    XmlItem itemData;
                    itemData.itemName = currentComponentName;
                    itemData.lockHash = node.child("lockHash").child_value();
                    itemData.textLabel = node.child("textLabel").child_value();
                    itemData.uniqueNameHash = node.child("uniqueNameHash").child_value();
                    itemData.localDrawableIndex = node.child("localDrawableIndex").attribute("value").as_string();
                    itemData.textureIndex = node.child("textureIndex").child_value();
                    itemData.eCompType = node.child("eCompType").child_value();

                    logger->log("Found item - Name: " + std::string(itemData.itemName) +
                                ", Hash: " + std::string(itemData.uniqueNameHash) +
                                ", Type: " + std::string(itemData.eCompType));

                    outNode.items.push_back(std::move(itemData));
                    currentComponentName = Core::Defaults::NA;
                }
            }
        }

        // Parse pedProps
        logger->log("Parsing pedProps...");
        auto propsNode = xmlNode.child("pedProps");
        if (propsNode) {
            std::string_view currentPropName = Core::Defaults::NA;

            for (auto node = propsNode.first_child(); node; node = node.next_sibling()) {
                if (node.type() == pugi::node_comment) {
                    currentPropName = node.value();
                    logger->log("Found prop comment: " + std::string(currentPropName));
                }
                else if (std::string(node.name()) == "Item") {
                    XmlItem itemData;
                    itemData.itemName = currentPropName;
                    itemData.lockHash = node.child("lockHash").child_value();
                    itemData.textLabel = node.child("textLabel").child_value();
                    itemData.uniqueNameHash = node.child("uniqueNameHash").child_value();
                    itemData.propIndex = node.child("propIndex").attribute("value").as_string();
                    itemData.textureIndex = node.child("textureIndex").child_value();
                    itemData.eAnchorPoint = node.child("eAnchorPoint").child_value();

                    logger->log("Found prop - Name: " + std::string(itemData.itemName) +
                                ", Hash: " + std::string(itemData.uniqueNameHash) +
                                ", Anchor: " + std::string(itemData.eAnchorPoint));

                    outNode.items.push_back(std::move(itemData));
                    currentPropName = Core::Defaults::NA;
                }
            }
        }

        logger->log("Successfully parsed XML node with " + 
                    std::to_string(outNode.outfits.size()) + " outfits and " +
                    std::to_string(outNode.items.size()) + " individual items");
        return true;
    }
    catch (const std::exception& e) {
        logger->error("Exception in parseNode: " + std::string(e.what()));
        return false;
    }
}

void XmlParser::categorizeNode(XmlNode& node) {
    if (node.pedName.find("_m_") != std::string::npos) {
        maleNodes.push_back(std::move(node));
    }
    else if (node.pedName.find("_f_") != std::string::npos) {
        femaleNodes.push_back(std::move(node));
    }
}

bool XmlParser::scanDirectory(const std::string& directory) {
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
            if (entry.path().extension() == ".meta") {
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