#include "log.h"

std::string g_cwd;

void Log::init() {
    char cwd[PATH_MAX];

    if(getcwd(cwd, PATH_MAX) == NULL) {
        g_cwd = "~";
    }

    g_cwd = cwd;
    g_cwd.append("/");

    std::cout << "Log Path [" << g_cwd << "]" << std::endl;
}

void Log::put(std::string str) {
    std::ofstream file(g_cwd + "hack.log", std::ios::out | std::ios::app);

    str.append("\r\n");

    if(file.good()) {
        file << str;
    }

    file.close();
}