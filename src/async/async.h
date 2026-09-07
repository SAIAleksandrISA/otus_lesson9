#pragma once

#include <cstddef>

#if defined(_WIN32)
#ifdef ASYNC_EXPORTS
#define ASYNC_API __declspec(dllexport)
#else
#define ASYNC_API __declspec(dllimport)
#endif
#else
#define ASYNC_API
#endif

namespace bulk
{
    using Context = size_t;

    extern "C" 
    {
        ASYNC_API Context connect(size_t blockSize);
        ASYNC_API bool receive(const char* data, size_t size, Context context);
        ASYNC_API bool disconnect(Context context);
    }
}