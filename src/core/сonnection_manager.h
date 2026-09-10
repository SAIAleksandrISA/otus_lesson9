#pragma once

#include "threading/thread_manager.h"
#include "session_manager.h"

#include <memory>

namespace bulk
{
    class ConnectionManager
    {
    public:
        static ConnectionManager& instance();

        ConnectionManager(const ConnectionManager&) = delete;
        ConnectionManager& operator=(const ConnectionManager&) = delete;

        void initialize();
        void shutdown();

        ThreadManager& get_tm() { return m_tm; }
        SessionManager& get_sm() { return m_sm; }

    private:
        ConnectionManager();
        ~ConnectionManager();

        ThreadManager m_tm;
        SessionManager m_sm;
        bool m_initialized = false;
    };
}