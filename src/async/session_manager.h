#pragma once

#include "async.h"
#include "../core/bulkprocessor.h"
#include "../threading/thread_manager.h"
#include <map>
#include <mutex>
#include <atomic>
#include <memory>

namespace bulk
{

    class SessionManager
    {
    public:
        SessionManager();
        ~SessionManager();

        SessionManager(const SessionManager&) = delete;
        SessionManager& operator=(const SessionManager&) = delete;

        Context create_session(size_t blockSize);
        bool process_data(Context context, const char* data, size_t size);
        bool terminate_session(Context context);

    private:
        struct SessionContext
        {
            std::unique_ptr<BulkProcessor> processor;
        };

        std::map<Context, std::unique_ptr<SessionContext>> m_sessions;
        std::mutex m_sessions_mutex;
        std::atomic<Context> m_next_context_id;
    };

    SessionManager& get_session_manager();

}