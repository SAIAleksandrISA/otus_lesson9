#include "async.h"
#include "connection_manager.h"

namespace bulk
{

    Context connect(size_t blockSize)
    {
        ConnectionManager::instance().initialize();

        ThreadManager& tm = ConnectionManager::instance().get_tm();
        SessionManager& sm = ConnectionManager::instance().get_sm();

        return sm.create_session(blockSize, tm.get_log_queue(), tm.get_shared_file_queue());
    }

    bool receive(const char* data, size_t size, Context context)
    {
        return ConnectionManager::instance().get_sm().process_data(context, data, size);
    }

    bool disconnect(Context context)
    {
        return ConnectionManager::instance().get_sm().terminate_session(context);
    }

}