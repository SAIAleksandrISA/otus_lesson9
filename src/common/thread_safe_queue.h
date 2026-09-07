#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <atomic>
#include <utility>

namespace bulk
{

    template <typename T>
    class ThreadSafeQueue
    {
    public:
        ThreadSafeQueue() : m_stop_requested(false) {}

        void push(T value)
        {
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                if (m_stop_requested.load())
                {
                    return;
                }
                m_queue.push(std::move(value));
            }
            m_cv.notify_one();
        }

        std::optional<T> pop()
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            m_cv.wait(lock, [this] { return !m_queue.empty() || m_stop_requested.load(); });

            if (m_queue.empty() && m_stop_requested.load())
            {
                return std::nullopt;
            }

            T value = std::move(m_queue.front());
            m_queue.pop();
            return value;
        }

        void stop()
        {
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_stop_requested.store(true);
            }
            m_cv.notify_all();
        }

        bool is_empty() const
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_queue.empty();
        }

        bool is_stop_requested() const
        {
            return m_stop_requested.load();
        }

    private:
        std::queue<T> m_queue;
        mutable std::mutex m_mutex;
        std::condition_variable m_cv;
        std::atomic<bool> m_stop_requested;
    };

}