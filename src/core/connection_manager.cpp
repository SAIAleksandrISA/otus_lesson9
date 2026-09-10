#include "core/ñonnection_manager.h"
#include "consoleoutputhandler.h"
#include "fileoutputhandler.h"
#include <iostream>

namespace bulk
{
    ConnectionManager& ConnectionManager::instance()
    {
        static ConnectionManager inst;
        return inst;
    }

    ConnectionManager::ConnectionManager() = default;

    ConnectionManager::~ConnectionManager()
    {
        shutdown();
    }

    void ConnectionManager::initialize()
    {
        if (m_initialized) return;

        m_tm.start(
            std::make_unique<ConsoleOutputHandler>(),
            std::make_unique<FileOutputHandler>("_f1"),
            std::make_unique<FileOutputHandler>("_f2")
        );

        m_initialized = true;
    }

    void ConnectionManager::shutdown()
    {
        if (!m_initialized) return;
        m_tm.stop();
        m_initialized = false;
    }
}