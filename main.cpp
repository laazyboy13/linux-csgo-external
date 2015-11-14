/*
   Created by laazyboy13 on 11/12/15 9:31 PM.
   Credits: s0beit - original linux external glow esp
*/
#include "log/log.hpp"

using namespace log;

int main() {

    log::init();

    Logger logger("test.log");

    logger << "Test Log";

    if (getuid() != 0) {
        //not root
        std::cout << "Please start with root." << std::endl;
        return 0;
    }

    return 0;
}
