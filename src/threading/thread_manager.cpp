#include "thread_manager.h"
#include "common/block.h"
#include "common/thread_safe_queue.h"
#include "core/consoleoutputhandler.h"
#include "core/fileoutputhandler.h"
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <utility>

namespace bulk
{

    ThreadManager& get_thread_manager()
    {
        static ThreadManager instance;
        return instance;
    }

    ThreadManager::ThreadManager()
        : m_running(false), m_stop_requested(false)
    {
    }

    ThreadManager::~ThreadManager()
    {
        stop();
    }

    void ThreadManager::start(std::unique_ptr<IBlockOutputHandler> log_handler,
        std::unique_ptr<IBlockOutputHandler> file1_handler,
        std::unique_ptr<IBlockOutputHandler> file2_handler)
    {
        if (m_running.load() || m_stop_requested.load())
        {
            std::cerr << "Warning: ThreadManager is already running or stopping." << std::endl;
            return;
        }

        if (!log_handler || !file1_handler || !file2_handler)
        {
            std::cerr << "Error: Not all output handlers provided to ThreadManager. Cannot start." << std::endl;
            return;
        }
        m_log_handler = std::move(log_handler);
        m_file1_handler = std::move(file1_handler);
        m_file2_handler = std::move(file2_handler);

        m_running.store(true);
        m_stop_requested.store(false);


        m_log_thread = std::thread(&ThreadManager::run_log_worker, this);

        m_file1_thread = std::thread(&ThreadManager::run_file_worker, this, std::ref(m_shared_file_queue), m_file1_handler.get());
        m_file2_thread = std::thread(&ThreadManager::run_file_worker, this, std::ref(m_shared_file_queue), m_file2_handler.get());

    }

    void ThreadManager::stop()
    {
        if (!m_running.load()) return;

        m_stop_requested.store(true);

        m_log_queue.stop();
        m_shared_file_queue.stop();

        join_all_threads();

        m_running.store(false);
    }

    void ThreadManager::join_all_threads()
    {
        if (m_log_thread.joinable())
        {
            m_log_thread.join();
        }
        if (m_file1_thread.joinable())
        {
            m_file1_thread.join();
        }
        if (m_file2_thread.joinable())
        {
            m_file2_thread.join();
        }
    }

    ThreadSafeQueue<Block>& ThreadManager::get_log_queue()
    {
        return m_log_queue;
    }

    ThreadSafeQueue<Block>& ThreadManager::get_shared_file_queue()
    {
        return m_shared_file_queue;
    }

    bool ThreadManager::is_running() const
    {
        return m_running.load();
    }

    void ThreadManager::run_log_worker()
    {
        while (!m_stop_requested.load() || !m_log_queue.is_empty())
        {
            std::optional<Block> block_opt = m_log_queue.pop();

            if (!block_opt.has_value())
            {
                if (m_stop_requested.load()) {
                    break;
                }
                break;
            }

            Block block = std::move(block_opt.value());

            if (m_log_handler)
            {
                m_log_handler->handleBlock(block.m_commands, block.m_timestamp);
            }
            else 
                std::cerr << "Error: Log handler is null." << std::endl;
        };
    }

    void ThreadManager::run_file_worker(ThreadSafeQueue<Block>& file_queue, IBlockOutputHandler* file_handler)
    {
        std::string worker_name = "File Worker ";
        if (file_handler == m_file1_handler.get()) worker_name += "1";
        else if (file_handler == m_file2_handler.get()) worker_name += "2";
        else worker_name += "Unknown";

        while (!m_stop_requested.load() || !file_queue.is_empty())
        {
            std::optional<Block> block_opt = file_queue.pop();

            if (!block_opt.has_value())
            {
                if (m_stop_requested.load()) break;
                break;
            }

            Block block = std::move(block_opt.value());

            if (file_handler)
            {
                file_handler->handleBlock(block.m_commands, block.m_timestamp);
            }
            else 
                std::cerr << "Error: File handler is null in " << worker_name << "." << std::endl;
        }
    }

} 