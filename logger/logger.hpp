/*
   Created by laazyboy13 on 11/12/15 11:12 PM.
   Credits: s0beit - original linux external glow esp
*/


#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <iostream>
#include <fstream>
#include <map>
#include "../utils/time.hpp"
#include <unistd.h>

namespace log {
    extern bool isDebug;
    extern const char* const logDir;

    enum LogLevel {
        INFO,
        DEBUG,
        WARNING,
        ERROR,
        FATAL
    };

    extern void init();

    extern void stripCarriage(std::string& str);

    class Logger {
    private:
        std::map<LogLevel, int> logCounts;
        std::ofstream file;

    public:
        explicit Logger(std::string fileName);

        friend Logger& operator <<(Logger& logger, const log::LogLevel level) {
            switch (level) {
                case log::LogLevel::INFO:
                    return logger << "INFO";
                case log::LogLevel::DEBUG:
                    return logger << "DEBUG";
                case log::LogLevel::WARNING:
                    return logger << "WARNING";
                case log::LogLevel::ERROR:
                    return logger << "ERROR";
                case log::LogLevel::FATAL:
                    return logger << "FATAL";
            }
            return logger;
        }

        friend Logger& operator <<(Logger& logger, const char* const text) {
            logger << text;
            return logger;
        }

        friend Logger& operator <<(Logger& logger, const std::string& text) {
            logger << text;
            return logger;
        }
    };
}

#endif //__LOGGER_H__