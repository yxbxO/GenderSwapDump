#pragma once
#include <string>
#include <vector>
#include <memory>

namespace Core {
    // Forward declarations
    //struct ClothingItem;
    //struct GenderSpecificData;

    class IParser {
    public:
        virtual ~IParser() = default;
        virtual bool parse() = 0;
        virtual bool isValid() const = 0;
    };

    class IDataProcessor {
    public:
        virtual ~IDataProcessor() = default;
        virtual bool process() = 0;
        virtual bool isValid() const = 0;
    };

    class ILogger {
    public:
        virtual ~ILogger() = default;
        virtual void log(const std::string& message) = 0;
        virtual void error(const std::string& message) = 0;
    };
} 