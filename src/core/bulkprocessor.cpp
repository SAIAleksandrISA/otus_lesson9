#include "bulkprocessor.h"
#include <sstream>
#include <memory>

namespace bulk
{

    BulkProcessor::BulkProcessor(int nSize,
        ThreadSafeQueue<Block>& log_queue,
        ThreadSafeQueue<Block>& file_queue)
        : m_nSize(nSize)
        , m_depth(0)
        , m_isTimeSet(false)
        , m_log_queue(log_queue)
        , m_file_queue(file_queue)
    {
    }

    void BulkProcessor::processBuffer(const char* data, size_t size)
    {
        std::string input = m_bufferRemainder + std::string(data, size);

        size_t last_newline = input.find_last_of('\n');

        if (last_newline == std::string::npos)
        {
            m_bufferRemainder = input;
            return;
        }

        m_bufferRemainder = input.substr(last_newline + 1);
        std::string processable = input.substr(0, last_newline);
        std::stringstream ss(processable);
        std::string command;

        while (std::getline(ss, command))
        {
            if (!command.empty() && command.back() == '\r')
            {
                command.pop_back();
            }
            if (!command.empty())
            {
                processCommand(command);
            }
        }
    }

    void BulkProcessor::commitBlock(bool ignore)
    {
        if (m_currentcommands.empty())
            return;

        if (ignore && m_depth > 0)
        {
        
        }
        else
        {
            Block block{ m_currentcommands, m_currenttimes };

            m_log_queue.push(block);
            m_file_queue.push(block);
        }

        resetBlockState();
    }

    void BulkProcessor::processCommand(const std::string& command)
    {
        if (command == "{")
        {
            if (m_depth == 0)
            {
                commitBlock(false);
            }
            m_depth++;
        }
        else if (command == "}")
        {
            if (m_depth > 0)
            {
                m_depth--;
                if (m_depth == 0)
                {
                    commitBlock(false);
                }
            }
            else
            {
                if (!m_isTimeSet)
                {
                    m_currenttimes = std::chrono::system_clock::now();
                    m_isTimeSet = true;
                }
                m_currentcommands.push_back(command);
            }
        }
        else
        {
            if (!m_isTimeSet)
            {
                m_currenttimes = std::chrono::system_clock::now();
                m_isTimeSet = true;
            }
            m_currentcommands.push_back(command);

            if (m_depth == 0 && (int)m_currentcommands.size() == m_nSize)
            {
                commitBlock(false);
            }
        }
    }

    void BulkProcessor::processEOF()
    {
        if (!m_currentcommands.empty())
            commitBlock(true);
    }

    void BulkProcessor::resetBlockState()
    {
        m_currentcommands.clear();
        m_currenttimes = std::chrono::time_point<std::chrono::system_clock>();
        m_isTimeSet = false;
    }

}