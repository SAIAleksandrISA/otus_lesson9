#include "fileoutputhandler.h"

#include <vector>
#include <string>
#include <chrono>
#include <iostream>
#include <fstream>

namespace bulk
{

    FileOutputHandler::FileOutputHandler(std::string suffix)
        : m_suffix(std::move(suffix))
    {
    }

    void FileOutputHandler::handleBlock(const std::vector<std::string>& commands,
        std::chrono::time_point<std::chrono::system_clock> timestamp)
    {
        if (commands.empty())
        {
            return;
        }

        auto epoch_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(timestamp.time_since_epoch()).count();
        std::string filename = "bulk_" + std::to_string(epoch_time_ms) + m_suffix + ".log";

        std::ofstream ofs(filename);
        if (ofs.is_open())
        {
            for (const auto& cmd : commands)
            {
                ofs << cmd << std::endl;
            }
        }
        else
        {
            std::cerr << "Error: Could not open file for logging: " << filename << std::endl;
        }
    }

}