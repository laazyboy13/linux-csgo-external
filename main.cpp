/*
   Created by laazyboy13 on 11/12/15 9:31 PM.
   Credits: s0beit - original linux external glow esp
*/
#include <unistd.h>
#include "logger/logger.hpp"

int main() {

    if(getuid() != 0) {
        //not root
        std::cout << "Please start with root." << std::endl;
        return 0;
    }
}
