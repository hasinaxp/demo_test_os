#pragma once
#include "../types.h"

#define SYS_EVENT_QUEUE_SIZE 256

namespace sys
{
    struct raw_event
    {
        u8 type;
        u8 data;
        u8 data2;
        u8 flags;
    };

    raw_event raw_event_queue_data[SYS_EVENT_QUEUE_SIZE];
    u32 event_queue_head = 0;
    u32 event_queue_tail = 0;
    u32 event_queue_size = 0;

    inline void event_queue_push(const raw_event &e)
    {
        raw_event_queue_data[event_queue_tail] = e;
        event_queue_tail = (event_queue_tail + 1) % SYS_EVENT_QUEUE_SIZE;
        event_queue_size++;
        if (event_queue_size > SYS_EVENT_QUEUE_SIZE)
        {
            event_queue_head = (event_queue_head + 1) % SYS_EVENT_QUEUE_SIZE;
            event_queue_size--;
        }
    }

    u8 event_queue_peak(raw_event &e)
    {
        if (event_queue_size == 0)
            return FALSE;

        e = raw_event_queue_data[event_queue_head];
        return TRUE;
    }

    inline u8 event_queue_pop(raw_event &e)
    {
        if (event_queue_size == 0)
            return FALSE;

        e = raw_event_queue_data[event_queue_head];
        event_queue_head = (event_queue_head + 1) % SYS_EVENT_QUEUE_SIZE;
        event_queue_size--;
        return TRUE;
    }

};
