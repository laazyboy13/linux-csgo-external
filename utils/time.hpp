/*
   Created by laazyboy13 on 11/13/15 10:23 AM.
   Credits: s0beit - original linux external glow esp
*/

#pragma once

#include <chrono>
#include <iomanip>
#include <sstream>

namespace utils {


    namespace time {
        std::string currentTime(){
            auto now = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);

            std::stringstream ss;
            ss << std::put_time(std::localtime(&in_time_t), "%F %T");
            return ss.str();
        }
    }
}