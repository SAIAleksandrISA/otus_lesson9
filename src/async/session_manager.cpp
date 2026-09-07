#include "session_manager.h"
#include <iostream>

namespace bulk
{

    SessionManager& get_session_manager()
    {
        static SessionManager instance;
        return instance;
    }

    SessionManager::SessionManager() : m_next_context_id(1)
    {
    }

    SessionManager::~SessionManager()
    {
        std::lock_guard<std::mutex> lock(m_sessions_mutex);
        for (auto& pair : m_sessions)
        {
            if (pair.second->processor)
            {
                pair.second->processor->processEOF();
            }
        }
    }

    Context SessionManager::create_session(size_t blockSize)
    {
        Context id = m_next_context_id.fetch_add(1);

        // Получаем очереди из глобального ThreadManager
        auto& log_q = get_thread_manager().get_log_queue();
        auto& file_q = get_thread_manager().get_shared_file_queue();

        // Создаем BulkProcessor для этой сессии
        auto processor = std::make_unique<BulkProcessor>(
            static_cast<int>(blockSize),
            log_q,
            file_q
        );

        auto context = std::make_unique<SessionContext>();
        context->processor = std::move(processor);

        std::lock_guard<std::mutex> lock(m_sessions_mutex);
        m_sessions[id] = std::move(context);

        return id;
    }

    bool SessionManager::process_data(Context context, const char* data, size_t size)
    {
        std::lock_guard<std::mutex> lock(m_sessions_mutex);

        auto it = m_sessions.find(context);
        if (it == m_sessions.end())
        {
            return false;
        }

        it->second->processor->processBuffer(data, size);
        return true;
    }

    bool SessionManager::terminate_session(Context context)
    {
        std::unique_ptr<SessionContext> session;

        {
            std::lock_guard<std::mutex> lock(m_sessions_mutex);
            auto it = m_sessions.find(context);
            if (it == m_sessions.end())
            {
                return false;
            }
            session = std::move(it->second);
            m_sessions.erase(it);
        }
        if (session->processor)
        {
            session->processor->processEOF();
        }

        return true;
    }

}