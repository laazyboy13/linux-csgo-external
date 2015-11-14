/*
   Created by laazyboy13 on 11/12/15 11:12 PM.
   Credits: s0beit - original linux external glow esp
*/

#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "log.hpp"

namespace log {

    bool isDebug = true;

    const char* const logDir = "./logs/";
    const std::string defaultFile = std::string(logDir) + "test.log";

    const std::string dateTimeTag = "{datetime}";
    const std::string logStart    = "===============[ Log Start: " + dateTimeTag + " ]===============";
    const std::string logEnd      = "===============[  Log End:  " + dateTimeTag + " ]===============";

    void init() {
        DIR* pDir;
        if (access(logDir, F_OK) != 0) {
            mkdir(logDir, S_IRWXU | S_IRWXG | S_IRWXO);
            pDir = opendir(logDir);
        } else {
            pDir = opendir(logDir);
        }
        closedir(pDir);
    }

    Logger::Logger(std::string str) {
        logCounts.insert({INFO, 0});
        logCounts.insert({DEBUG, 0});
        logCounts.insert({WARNING, 0});
        logCounts.insert({ERROR, 0});
        logCounts.insert({FATAL, 0});

        setFileName(str);
    }

    Logger::~Logger() {
        if (file.is_open()) {
            file << insertTimeStamp(logEnd) << std::endl;
            file.close();
        }
    }

    void Logger::setFileName(std::string str) {
        if (strncmp(str.c_str(), logDir, strlen(logDir)) != 0) {
            str = std::string(logDir) + str;
        }

        if (access(str.c_str(), F_OK) == 0) {
            std::cout << "Old log found. Backing up." << std::endl;
            unsigned int index     = str.find_last_of(".");
            std::string  extension = str.substr(index);
            std::string  stripped  = str.substr(0, index);
            std::string  newFileName;

            int i = 0;
            do {
                newFileName = std::string(stripped) + extension + std::to_string(i++);
            } while (access(newFileName.c_str(), F_OK) == 0);

            std::ifstream src(str, std::ios::binary);
            std::ofstream dest(newFileName, std::ios::binary);
            dest << src.rdbuf();
            std::cout << "Log backed up to: " << newFileName << std::endl;
        }

        file.open(str, std::ios::trunc);
        if (file.is_open()) {
            file << insertTimeStamp(logStart) << std::endl;
            file.flush();
        }
    }


    std::string Logger::insertTimeStamp(std::string str) {
        return std::string(str).replace(logStart.find(dateTimeTag), dateTimeTag.size(), timeutils::currentTime());
    }
}