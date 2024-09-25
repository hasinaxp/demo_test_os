#include "../types.h"
#include "port.h"
#include "interrupt.h"

#define SYS_PIT_CONTROL_PORT 0x43
#define SYS_PIT_DATA_PORT 0x40
#define SYS_PIT_FREQUENCY 1193182 // Base frequency of PIT
#define SYS_DESIRED_FREQUENCY 100 // 100Hz for the OS clock

namespace sys
{
    u32 ticks = 0;

    char buffer[256];

    void show_clock()
    {
        int mins = ticks / 6000;
        int secs = (ticks / 100) % 60;
        if (ticks % 100 == 0)
        {

            sl::print(60, 0, "uptime: [");
            sl::to_str(mins, buffer);
            sl::print(69, 0, buffer);
            sl::print(72, 0, "m :");
            sl::to_str(secs, buffer);
            sl::print(75, 0, buffer);
            sl::print(78, 0, "s]");
        }
    }

    IRQ_CALLBACK clock_handler()
    {
        IRQ_BEGIN_CODE();
        ticks++;
        show_clock();
        port_write8(0x20, 0x20);
        IRQ_END_CODE();
    }

    void clock_init()
    {
        u32 divisor = SYS_PIT_FREQUENCY / SYS_DESIRED_FREQUENCY;

        sl::to_str(divisor, buffer);
        sl::print(buffer);

        port_write8(SYS_PIT_CONTROL_PORT, 0x36); // Channel 0, lobyte/hibyte, rate generator
        port_write8(SYS_PIT_DATA_PORT, (u8)(divisor & 0xFF));
        port_write8(SYS_PIT_DATA_PORT, (u8)((divisor >> 8) & 0xFF));

        set_idt_gate(IRQ_CLOCK_IRQ, (u32)clock_handler);
        u8 mask = port_read8(0x21); // Read the current mask
        mask &= ~0x01;              // Unmask IRQ0 (clock)
        port_write8(0x21, mask);
        sl::print("Clock initialized\n");
    }
};