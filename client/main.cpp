#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <memory>

#include "async/async.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: bulk <block_size>" << std::endl;
        return 1;
    }

    size_t blockSize = 0;
    try
    {
        blockSize = std::stoul(argv[1]);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: Invalid block size '" << argv[1] << "'. Please provide a number." << std::endl;
        return 1;
    }

    std::cout << "--- Bulk Processor Client: Starting with block size " << blockSize << " ---" << std::endl;

    bulk::Context ctx = bulk::connect(blockSize);
    if (ctx == 0)
    {
        std::cerr << "Error: Failed to connect to async library." << std::endl;
        return 1;
    }

    std::cout << "--- Bulk Processor Started. Enter commands (type 'exit' to quit) ---" << std::endl;
    std::cout << "> ";

    std::string line;
    while (std::getline(std::cin, line))
    {
        if (line == "exit")
        {
            break;
        }

        line += "\n";
        bulk::receive(line.c_str(), line.length(), ctx);

        std::cout << "> ";
    }

    std::cout << "Disconnecting..." << std::endl;
    bulk::disconnect(ctx);

    std::cout << "Exiting." << std::endl;

    return 0;
}