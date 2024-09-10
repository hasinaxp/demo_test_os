#pragma once
#include "../types.h"

namespace sys
{
    inline void port_write8(u16 port, u8 data)
    {
        asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
    }

    inline void port_write16(u16 port, u16 data)
    {
        asm volatile("outw %0, %1" : : "a"(data), "Nd"(port));
    }

    inline void port_write32(u16 port, u32 data)
    {
        asm volatile("outl %0, %1" : : "a"(data), "Nd"(port));
    }

    inline u8 port_read8(u16 port)
    {
        u8 data;
        asm volatile("inb %1, %0" : "=a"(data) : "Nd"(port));
        return data;
    }

    inline u16 port_read16(u16 port)
    {
        u16 data;
        asm volatile("inw %1, %0" : "=a"(data) : "Nd"(port));
        return data;
    }

    inline u32 port_read32(u16 port)
    {
        u32 data;
        asm volatile("inl %1, %0" : "=a"(data) : "Nd"(port));
        return data;
    }

}
