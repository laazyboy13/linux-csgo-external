/*
   Created by laazyboy13 on 11/12/15 11:12 PM.
   Credits: s0beit - original linux external glow esp
*/


#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <iostream>
#include <fstream>
#include <map>
#include "../utils/timeutils.hpp"
#include <unistd.h>

namespace log {
    extern bool isDebug;

    extern const char* const logDir;
    extern const std::string defaultFile;

    extern const std::string dateTimeTag;
    extern const std::string logStart;
    extern const std::string logEnd;

    enum LogLevel {
        INFO,
        DEBUG,
        WARNING,
        ERROR,
        FATAL
    };

    extern void init();

    class Logger {
    private:
        std::map<LogLevel, int> logCounts;
        std::ofstream file;

    public:
        Logger(const Logger&) = delete;
        Logger& operator =(const Logger&) = delete;

        explicit Logger(std::string str = defaultFile);
        ~Logger();

        std::string insertTimeStamp(std::string str);
        void setFileName(std::string str);

        friend Logger& operator <<(Logger& logger, const LogLevel level) {
            switch (level) {
                case INFO:
                    logger.logCounts[INFO]++;
                    return logger << "INFO";
                case DEBUG:
                    logger.logCounts[DEBUG]++;
                    return logger << "DEBUG";
                case WARNING:
                    logger.logCounts[WARNING]++;
                    return logger << "WARNING";
                case ERROR:
                    logger.logCounts[ERROR]++;
                    return logger << "ERROR";
                case FATAL:
                    logger.logCounts[FATAL]++;
                    return logger << "FATAL";
            }
            return logger;
        }

        friend Logger& operator <<(Logger& logger, const char* text) {
            logger.file << text << std::endl;
            return logger;
        }

        friend Logger& operator <<(Logger& logger, const std::string& text) {
            logger.file << text << std::endl;
            return logger;
        }
    };
}

#endif //__LOGGER_H__