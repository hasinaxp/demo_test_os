#pragma once
#include "../types.h"

namespace sys
{
    extern char *_heap_start;
    struct memory_block
    {
        memory_block *next;
        memory_block *prev;
        bool free;
        u32 size;
    };

}
