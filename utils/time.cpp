/*
   Created by laazyboy13 on 11/14/15 1:38 AM.
   Credits: s0beit - original linux external glow esp
*/

#include "time.hpp"

using namespace std::chrono;

namespace timeutils {
    std::string currentTime() {
        auto now = system_clock::now();
        auto in_time_t = system_clock::to_time_t(now);

        std::stringstream ss;
        ss << std::put_time(localtime(&in_time_t), "%F %T");
        return ss.str();
    }
}