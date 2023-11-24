/*
    Property of Shaunak Perni 
    Offered in GNU-L2 Public Limited License 
*/
#include <string>
#pragma once
namespace terminalFormatingColorcodes
{
    const std::string ANSI_RESET = "\033[0m";
    const std::string ANSI_BLACK = "\033[30m";
    const std::string ANSI_RED = "\033[31m";
    const std::string ANSI_GREEN = "\033[32m";
    const std::string ANSI_YELLOW = "\033[33m";
    const std::string ANSI_BLUE = "\033[34m";
    const std::string ANSI_MAGENTA = "\033[35m";
    const std::string ANSI_CYAN = "\033[36m";
    const std::string ANSI_WHITE = "\033[37m";

    const std::string ANSI_BG_BLACK = "\033[40m";
    const std::string ANSI_BG_RED = "\033[41m";
    const std::string ANSI_BG_GREEN = "\033[42m";
    const std::string ANSI_BG_YELLOW = "\033[43m";
    const std::string ANSI_BG_BLUE = "\033[44m";
    const std::string ANSI_BG_MAGENTA = "\033[45m";
    const std::string ANSI_BG_CYAN = "\033[46m";
    const std::string ANSI_BG_WHITE = "\033[47m";
}

namespace terminalFunctions
{
    #define CLEARSCREENCOMMAND "cls"
    void clearScreen()
    {
        std::system(CLEARSCREENCOMMAND);
    }
}

namespace terminalFormatingMisc
{
    const std::string spacer = "        ";
}
