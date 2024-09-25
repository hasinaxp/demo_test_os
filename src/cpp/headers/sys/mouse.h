#pragma once
#include "../types.h"
#include "port.h"
#include "interrupt.h"

namespace sys
{
    static u8 mouse_cycle = 0; // Static for persistence across interrupts
    static u8 mouse_byte[3];
    static i32 mouse_x = 0; // Use signed integers to avoid overflow
    static i32 mouse_y = 0;

    char mtext[256];

    IRQ_CALLBACK mouse_handler()
    {
        IRQ_BEGIN_CODE();
        u8 status = port_read8(0x64);
        if ((status & 0x20))
        {
            mouse_byte[mouse_cycle] = port_read8(0x60);
            mouse_cycle = (mouse_cycle + 1) % 3;

            if (mouse_cycle == 0)
            {
                // Check for overflow or invalid data
                if ((mouse_byte[0] & 0x08) == 0x08) // Ensure packet is valid
                {
                    i32 dx = (i8)mouse_byte[1]; // Handle sign extension
                    i32 dy = (i8)mouse_byte[2]; // Handle sign extension

                    mouse_x += dx;
                    mouse_y -= dy;

                    sl::to_str(mouse_x, mtext);
                    sl::print(mtext);
                    sl::print(", ");
                    sl::to_str(mouse_y, mtext);
                    sl::print(mtext);
                    sl::print("\n");
                }
            }
        }

        // port_write8(0x20, 0x20); // EOI

        port_write8(0xA0, 0x20); // Send EOI to slave PIC
        port_write8(0x20, 0x20); // Send EOI to master PIC

        IRQ_END_CODE();
    }

    void mouse_wait(unsigned char type)
    {
        int time_out = 100000;

        if (type == 0)
        {
            while (time_out--)
            {
                if ((port_read8(0x64) & 1) == 1)
                    return;
            }
            return;
        }
        else
        {
            while (time_out--)
            {
                if ((port_read8(0x64) & 2) == 0)
                    return;
            }
            return;
        }
    }

    void mouse_write(unsigned char data)
    {
        mouse_wait(1);
        port_write8(0x64, 0xd4);

        mouse_wait(1);
        port_write8(0x60, data);
    }

    unsigned char mouse_read()
    {
        mouse_wait(0);
        return port_read8(0x60);
    }

    void mouse_init()
    {
        u8 response;
        if ((response = port_read8(0x60)) == 0x00)
        {
            sl::print("Mouse not connected\n");
            return; // Exit if no mouse is detected
        }
        else
        {
            sl::print("Mouse connected\n");
            mouse_wait(1);
            port_write8(0x64, 0xA8); // Enable the auxiliary mouse device (mouse) through the PS/2 controller

            mouse_wait(1);
            port_write8(0x64, 0x20); // Get current state

            mouse_wait(0);
            u8 status = (port_read8(0x60) | 2); // Enable IRQ12 (set bit 1)
            mouse_wait(1);
            port_write8(0x64, 0x60); // Set state
            port_write8(0x60, status);

            mouse_write(0xFF);
            // Tell the mouse to use default settings
            mouse_write(0xF6);
            mouse_read();

            // Enable the mouse
            mouse_write(0xF4);
            u8 statu1 = mouse_read(); // Acknowledge
            sl::print("Mouse status: ");
            sl::to_str(statu1, mtext);
            sl::print(mtext);
            sl::print("\n");

            set_idt_gate(IRQ_MOUSE_IRQ, (u32)mouse_handler);

            u8 mask = port_read8(0x21);
            mask &= ~(1 << 2); // Clear the mask for IRQ12 (bit 2)
            port_write8(0x21, mask);

            sl::print("Mouse initialized\n");
        }
    }

};
