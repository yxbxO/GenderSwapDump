#pragma once
#include "Interfaces.h"
#include <fstream>

namespace Core {
    class FileLogger : public ILogger {
    private:
        std::ofstream logFile;
        const std::string logPath;

    public:
        explicit FileLogger(const std::string& path);
        ~FileLogger() override;

        void log(const std::string& message) override;
        void error(const std::string& message) override;
    };
} 