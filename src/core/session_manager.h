#pragma once

#include "async/async.h"
#include "bulkprocessor.h"
#include "common/thread_safe_queue.h"
#include "common/block.h"

#include <map>
#include <mutex>
#include <atomic>
#include <memory>
#include <vector>
#include <utility>

namespace bulk
{
    class SessionManager
    {
    public:
        SessionManager();
        ~SessionManager();

        SessionManager(const SessionManager&) = delete;
        SessionManager& operator=(const SessionManager&) = delete;

        Context create_session(size_t blockSize, ThreadSafeQueue<Block>& log_queue, ThreadSafeQueue<Block>& file_queue);
        bool process_data(Context context, const char* data, size_t size);
        bool terminate_session(Context context);

    private:
        struct SessionContext
        {
            std::unique_ptr<BulkProcessor> processor;
        };
        std::map<Context, std::shared_ptr<SessionContext>> m_sessions;
        mutable std::mutex m_sessions_mutex;
        std::atomic<Context> m_next_context_id;
    };
}