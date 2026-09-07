#pragma once

#include "common/thread_safe_queue.h"
#include "common/block.h"
#include <thread>
#include <atomic>
#include <vector>
#include <memory>
#include <string>

#include "../core/interfaces.h"

namespace bulk
{

    class ConsoleOutputHandler;
    class FileOutputHandler;

    class ThreadManager
    {
    public:
        ThreadManager();
        ~ThreadManager();

        void start(std::unique_ptr<IBlockOutputHandler> log_handler,
            std::unique_ptr<IBlockOutputHandler> file1_handler,
            std::unique_ptr<IBlockOutputHandler> file2_handler
        );
        void stop();

        ThreadSafeQueue<Block>& get_log_queue();
        ThreadSafeQueue<Block>& get_shared_file_queue();

        bool is_running() const;

    private:
        void run_log_worker();
        void run_file_worker(ThreadSafeQueue<Block>& file_queue, IBlockOutputHandler* file_handler);

        std::atomic<bool> m_running;
        std::atomic<bool> m_stop_requested;

        ThreadSafeQueue<Block> m_log_queue;
        ThreadSafeQueue<Block> m_shared_file_queue;

        std::thread m_log_thread;
        std::thread m_file1_thread;
        std::thread m_file2_thread;

        std::unique_ptr<IBlockOutputHandler> m_log_handler;
        std::unique_ptr<IBlockOutputHandler> m_file1_handler;
        std::unique_ptr<IBlockOutputHandler> m_file2_handler;

        void join_all_threads();
    };

    ThreadManager& get_thread_manager();

}