/*
   Created by laazyboy13 on 11/12/15 11:12 PM.
   Credits: s0beit - original linux external glow esp
*/

#include <dirent.h>
#include <sys/stat.h>
#include "logger.hpp"

namespace log {
    bool isDebug = true;
    const char* const logDir = "logs/";

    void init() {
        DIR* pDir;
        struct dirent* pDirent;
        if (access(logDir, F_OK) != 0) {
            mkdir(logDir, S_IRWXU | S_IRWXG | S_IRWXO);
            pDir = opendir(logDir);
        } else {
            pDir = opendir(logDir);
        }
        while ((pDirent = readdir(pDir)) != NULL) {
            printf("[%s]\n", pDirent->d_name);
        }
        closedir(pDir);
    }

    Logger::Logger(std::string fileName = std::string(*logDir + "hack.log")) {

        logCounts.insert({INFO, 0});
        logCounts.insert({DEBUG, 0});
        logCounts.insert({WARNING, 0});
        logCounts.insert({ERROR, 0});
        logCounts.insert({FATAL, 0});

        if (access(fileName.c_str(), F_OK) == 0) {
            std::string newFileName = std::string(fileName);
            int i = 0;
            do {
                unsigned int index = newFileName.find_last_of(".");
                std::string extension = newFileName.substr(index);
                newFileName = newFileName.substr(0, index);
                newFileName += "_" + i + extension;
            } while(access(newFileName.c_str(), F_OK) == 0);
            std::ifstream src(fileName, std::ios::binary);
            std::ofstream dest(newFileName, std::ios::binary);
            dest << src.rdbuf();
        }

        file.open(fileName, std::ios::trunc);
        if (file.is_open()) {
            file << "";
        }
    }
}