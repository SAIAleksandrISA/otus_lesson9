#pragma once

#include <vector>
#include <string>
#include <chrono>

namespace bulk
{
    struct Block
    {
        std::vector<std::string> m_commands;
        std::chrono::time_point<std::chrono::system_clock> m_timestamp;

        Block(const std::vector<std::string>& cmds,
            std::chrono::time_point<std::chrono::system_clock> ts)
            : m_commands(cmds), m_timestamp(ts)
        {
        }
    };

} 