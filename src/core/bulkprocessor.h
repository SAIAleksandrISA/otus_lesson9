#pragma once

#include "common/thread_safe_queue.h"
#include "common/block.h"
#include <vector>
#include <string>
#include <chrono>

namespace bulk
{

    class BulkProcessor
    {
    public:
        BulkProcessor(int nSize,
            ThreadSafeQueue<Block>& log_queue,
            ThreadSafeQueue<Block>& file_queue);

        void processBuffer(const char* data, size_t size);
        void processEOF();

    private:
        void commitBlock(bool ignore);
        void processCommand(const std::string& command);
        void resetBlockState();

        int m_nSize;
        int m_depth;
        std::vector<std::string> m_currentcommands;
        std::chrono::time_point<std::chrono::system_clock> m_currenttimes;
        bool m_isTimeSet;

        std::string m_bufferRemainder;
        ThreadSafeQueue<Block>& m_log_queue;
        ThreadSafeQueue<Block>& m_file_queue;
    };

}