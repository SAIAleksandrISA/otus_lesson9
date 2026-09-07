#pragma once

#include <vector>
#include <string>
#include <chrono>

namespace bulk
{
    class IBlockOutputHandler
    {
    public:
        virtual ~IBlockOutputHandler() = default;

        virtual void handleBlock(const std::vector<std::string>& commands,
            std::chrono::time_point<std::chrono::system_clock> times) = 0;
    };
}



