/*
   Created by laazyboy13 on 11/12/15 11:12 PM.
   Credits: s0beit - original linux external glow esp
*/


#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include "../utils/time.hpp"
#include <unistd.h>

namespace log {

    extern bool isDebug;

    enum LogLevel {
        INFO,
        DEBUG,
        WARNING,
        ERROR,
        FATAL
    };

    class Logger {
    private:
        std::map<LogLevel, int> logCounts;
        std::ofstream file;

    public:
        explicit Logger(const char* c, const char* fileName = "hack.log") {

            if(access(fileName, 0) == 0) {
                //file exists

            }

            file.open(fileName);
            if(file.is_open()) {
                file << "";
            }
        }

        friend Logger& operator<< (Logger& logger, const log::LogLevel level) {
            switch (level) {
                case log::LogLevel::INFO: return logger << "INFO";
                case log::LogLevel::DEBUG: return logger << "DEBUG";
                case log::LogLevel::WARNING: return logger << "WARNING";
                case log::LogLevel::ERROR: return logger << "ERROR";
                case log::LogLevel::FATAL: return logger << "FATAL";
            }
            return logger;
        }

        friend Logger& operator<< (Logger& logger, const char* const text) {
            logger << text;
            return logger;
        }

        friend Logger& operator<< (Logger& logger, const std::string& text) {
            logger << text;
            return logger;
        }
    };
}