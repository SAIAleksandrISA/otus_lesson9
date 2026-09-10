#include "session_manager.h"
#include "core/ñonnection_manager.h"
#include "bulkprocessor.h"
#include "threading/thread_manager.h"

namespace bulk
{

    SessionManager::SessionManager() : m_next_context_id(1)
    {
        
    }

    SessionManager::~SessionManager()
    {
        std::lock_guard<std::mutex> lock(m_sessions_mutex);
        m_sessions.clear(); 
    }

    Context SessionManager::create_session(size_t blockSize, ThreadSafeQueue<Block>& log_queue, ThreadSafeQueue<Block>& file_queue)
    {
        Context ctx = m_next_context_id.fetch_add(1);

        auto session_context = std::make_shared<SessionContext>();  
        session_context->processor = std::make_unique<BulkProcessor>(blockSize, log_queue, file_queue);

        {
            std::lock_guard<std::mutex> lock(m_sessions_mutex);
            m_sessions[ctx] = std::move(session_context);
        }

        return ctx;
    }

    bool SessionManager::process_data(Context ctx, const char* data, size_t size)
    {
        std::shared_ptr<SessionContext> session;
        {
            std::lock_guard<std::mutex> lock(m_sessions_mutex);
            auto it = m_sessions.find(ctx);
            if (it == m_sessions.end())
            {
                return false;
            }
            session = it->second;
        } 

 
        session->processor->processBuffer(data, size);
        return true; 
    }

    bool SessionManager::terminate_session(Context ctx)
    {
        std::shared_ptr<SessionContext> session_to_terminate;
        {
            std::lock_guard<std::mutex> lock(m_sessions_mutex);
            auto it = m_sessions.find(ctx);
            if (it == m_sessions.end())
            {
                return false;
            }
            session_to_terminate = std::move(it->second);
            m_sessions.erase(it);
        }

        if (session_to_terminate && session_to_terminate->processor)
        {
            session_to_terminate->processor->processEOF();
        }
        return true;
    }

}