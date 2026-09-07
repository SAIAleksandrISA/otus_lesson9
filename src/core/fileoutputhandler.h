#pragma once

#include "interfaces.h"
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <memory>

namespace bulk
{
    class FileOutputHandler : public IBlockOutputHandler
    {
    public:
        explicit FileOutputHandler(std::string suffix);

        void handleBlock(const std::vector<std::string>& commands,
            std::chrono::time_point<std::chrono::system_clock> timestamp) override;

    private:
        std::string m_suffix;
    };
}