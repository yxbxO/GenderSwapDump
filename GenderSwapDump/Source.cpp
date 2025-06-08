#pragma once
#include "Core/Logger.h"
#include "Core/Constants.h"
#include "Parsers/JsonParser.h"
#include "Parsers/XmlParser.h"
#include "Parsers/ScriptMetadataParser.h"
#include "Processors/ClothingProcessor.h"
#include "Output/ClothingDumper.h"
#include <memory>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

// Enhanced configuration
struct Config {
	fs::path outputDir{"output"};
	std::string mainDumpFile{"clothing_dump.txt"};
	std::string logFile{"clothing_dump.log"};
	
	void ensureOutputDir() {
		if (!fs::exists(outputDir)) {
			fs::create_directories(outputDir);
		}
	}
	
	fs::path getOutputPath(const std::string& filename) const {
		return outputDir / filename;
	}
};

// Helper functions
bool validateParsers(
	std::shared_ptr<ScriptMetadataParser> scriptParser,
	std::shared_ptr<JsonParser> jsonParser,
	std::shared_ptr<XmlParser> xmlParser,
	std::shared_ptr<Core::ILogger> logger) {
	
	bool valid = true;
	
	logger->log("Validating parsers...");
	
	if (!scriptParser->parse()) {
		logger->error("Failed to parse script metadata");
		valid = false;
	} else {
		logger->log("Script metadata parsed successfully");
	}
	
	if (!jsonParser->parse()) {
		logger->error("Failed to parse JSON files");
		valid = false;
	} else {
		logger->log("JSON files parsed successfully");
	}
	
	if (!xmlParser->parse()) {
		logger->error("Failed to parse XML files");
		valid = false;
	} else {
		logger->log("XML files parsed successfully");
	}
	
	return valid;
}

void dumpDlcSpecificData(
	std::shared_ptr<Output::ClothingDumper> dumper,
	std::shared_ptr<ClothingProcessor> processor,
	const Core::DlcInfo::DlcMapping& dlc,
	const Config& config,
	std::shared_ptr<Core::ILogger> logger) {
	
	logger->log("Processing DLC: " + std::string(dlc.name));
	
	// Dump outfits by DLC
	auto outfits = processor->getOutfitsByDLC(std::string(dlc.name));
	if (!outfits.empty()) {
		std::string filename = "outfits_" + std::string(dlc.dlcFileName) + ".txt";
		if (!dumper->dumpOutfitsByDLC(outfits, config.getOutputPath(filename).string())) {
			logger->error("Failed to dump outfits for DLC: " + std::string(dlc.name));
		}
	}
	
	// Dump items by DLC with enhanced formatting
	auto items = processor->getItemsByDLC(std::string(dlc.name));
	if (!items.empty()) {
		std::string filename = "items_" + std::string(dlc.dlcFileName) + ".txt";
		if (!dumper->dumpItemsByDLC(items, config.getOutputPath(filename).string())) {
			logger->error("Failed to dump items for DLC: " + std::string(dlc.name));
		}
	}
}

void dumpComponentSpecificData(
	std::shared_ptr<Output::ClothingDumper> dumper,
	std::shared_ptr<ClothingProcessor> processor,
	const Core::ComponentTypes::UnifiedComponentMapping& comp,
	const Config& config,
	std::shared_ptr<Core::ILogger> logger) {
	
	logger->log("Processing component type: " + std::string(comp.displayName));
	
	auto items = processor->getItemsByComponent(std::string(comp.internalType));
	if (!items.empty()) {
		std::string filename = "items_" + std::string(comp.internalType) + ".txt";
		if (!dumper->dumpItemsByComponent(items, config.getOutputPath(filename).string())) {
			logger->error("Failed to dump items for component: " + std::string(comp.displayName));
		}
	}
}

int main()
{
	try {
		Config config;
		config.ensureOutputDir();

		// Initialize logger with enhanced formatting
		auto logger = std::make_shared<Core::FileLogger>(
			config.getOutputPath(config.logFile).string());
		logger->log("Application started");

		// Initialize parsers
		auto scriptParser = std::make_shared<ScriptMetadataParser>(logger);
		auto jsonParser = std::make_shared<JsonParser>(logger);
		auto xmlParser = std::make_shared<XmlParser>(logger);

		// Validate all parsers
		if (!validateParsers(scriptParser, jsonParser, xmlParser, logger)) {
			return 1;
		}

		// Initialize processor
		auto processor = std::make_shared<ClothingProcessor>(
			logger, jsonParser, xmlParser, scriptParser);
			
		logger->log("Starting enhanced processing pipeline...");
		
		// Process with detailed progress tracking
		if (!processor->process()) {
			logger->error("Processing failed");
			return 1;
		}
		
		// Initialize dumper
		auto dumper = std::make_shared<Output::ClothingDumper>(logger);
		
		// Main dump file
		logger->log("Creating main dump file...");
		if (!dumper->dumpToFile(processor->getProcessedData(), 
							   config.getOutputPath(config.mainDumpFile).string())) {
			logger->error("Failed to create main dump");
			return 1;
		}
		
		// Additional specialized dumps
		logger->log("Creating specialized reports...");
		

		
		// Pricing report - disabled as pricing information is not relevant
		// if (!dumper->dumpPricingReport(
		//		config.getOutputPath(config.pricingFile).string(),
		//		processor->getItemPricing("all"))) {
		//	logger->error("Failed to create pricing report");
		// }
		
		// Unlock conditions - removed as data is not useful (all items show "Unlocked/None")
		// if (!dumper->dumpUnlockConditions(
		//		config.getOutputPath(config.unlockFile).string(),
		//		processor->getUnlockConditions("all"))) {
		//	logger->error("Failed to create unlock conditions report");
		// }
		
		// Process DLC-specific data
		logger->log("Processing DLC-specific data...");
		for (const auto& dlc : Core::DlcInfo::DLC_LIST) {
			dumpDlcSpecificData(dumper, processor, dlc, config, logger);
		}
		
		// Process component-specific data
		logger->log("Processing component-specific data...");
		    for (const auto& comp : Core::ComponentTypes::UNIFIED_COMPONENTS) {
			dumpComponentSpecificData(dumper, processor, comp, config, logger);
		}
		
		logger->log("Successfully completed all data processing and dumps");
		return 0;
	}
	catch (const std::exception& e) {
		std::cerr << "Fatal error: " << e.what() << std::endl;
		return 1;
	}
}