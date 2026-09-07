#pragma once

#include "interfaces.h"
#include <vector>
#include <string>
#include <chrono>

namespace bulk
{
    class ConsoleOutputHandler : public IBlockOutputHandler
    {
    public:
        void handleBlock(const std::vector<std::string>& commands,
            std::chrono::time_point<std::chrono::system_clock> timestamp) override;
    };
}