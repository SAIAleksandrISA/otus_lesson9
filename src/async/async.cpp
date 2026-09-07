#include "async.h"
#include "session_manager.h"
#include "threading/thread_manager.h"
#include "core/consoleoutputhandler.h"
#include "core/fileoutputhandler.h"
#include <iostream>

namespace bulk
{
    void ensure_system_started()
    {
        static bool started = false;
        if (!started)
        {
            auto& tm = get_thread_manager();
            tm.start(
                std::make_unique<ConsoleOutputHandler>(),
                std::make_unique<FileOutputHandler>("_f1"),
                std::make_unique<FileOutputHandler>("_f2")
            );
            started = true;
        }
    }

    Context connect(size_t blockSize)
    {
        ensure_system_started();
        return get_session_manager().create_session(blockSize);
    }

    bool receive(const char* data, size_t size, Context context)
    {
        return get_session_manager().process_data(context, data, size);
    }

    bool disconnect(Context context)
    {
        return get_session_manager().terminate_session(context);
    }
}