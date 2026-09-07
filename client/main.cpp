#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>

#include "async/async.h"
#include "threading/thread_manager.h"
#include "core/consoleoutputhandler.h"
#include "core/fileoutputhandler.h"

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
    catch (...)
    {
        std::cerr << "Error: Invalid block size." << std::endl;
        return 1;
    }

    auto& tm = bulk::get_thread_manager();

    tm.start(
        std::make_unique<bulk::ConsoleOutputHandler>(),
        std::make_unique<bulk::FileOutputHandler>("_f1"),
        std::make_unique<bulk::FileOutputHandler>("_f2")
    );

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    bulk::Context ctx = bulk::connect(blockSize);
    if (ctx == 0)
    {
        tm.stop();
        return 1;
    }

    std::string line;
    while (std::getline(std::cin, line))
    {
        if (line == "exit")
        {
            break;
        }

        line += "\n";
        bulk::receive(line.c_str(), line.length(), ctx);
    }

    bulk::disconnect(ctx);
    tm.stop();

    return 0;
}