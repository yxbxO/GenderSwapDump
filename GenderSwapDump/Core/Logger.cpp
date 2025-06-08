#include "Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>

namespace Core {
    FileLogger::FileLogger(const std::string& path) 
        : logPath(path) 
    {
        logFile.open(path, std::ios::app);
    }

    FileLogger::~FileLogger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void FileLogger::log(const std::string& message) {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        
        std::tm timeInfo;
#ifdef _WIN32
        localtime_s(&timeInfo, &time);
#else
        localtime_r(&time, &timeInfo);
#endif
        
        logFile << std::put_time(&timeInfo, "%Y-%m-%d %H:%M:%S")
                << " [INFO] " << message << std::endl;
        
        std::cout << message << std::endl;
    }

    void FileLogger::error(const std::string& message) {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        
        std::tm timeInfo;
#ifdef _WIN32
        localtime_s(&timeInfo, &time);
#else
        localtime_r(&time, &timeInfo);
#endif
        
        logFile << std::put_time(&timeInfo, "%Y-%m-%d %H:%M:%S")
                << " [ERROR] " << message << std::endl;
        
        std::cerr << "ERROR: " << message << std::endl;
    }
} 