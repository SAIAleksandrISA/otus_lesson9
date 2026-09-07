#include "consoleoutputhandler.h"
#include <iostream>

namespace bulk
{

    void ConsoleOutputHandler::handleBlock(const std::vector<std::string>& commands,
        [[maybe_unused]] std::chrono::time_point<std::chrono::system_clock> timestamp)
    {
        if (commands.empty())
        {
            return;
        }

        std::cout << "bulk: ";
        for (size_t i = 0; i < commands.size(); ++i)
        {
            std::cout << commands[i] << (i == commands.size() - 1 ? "" : ", ");
        }
        std::cout << std::endl;
    }
}