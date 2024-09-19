#pragma once
#include "../types.h"
#include "port.h"
#include "interrupt.h"
#include "events.h"

#define SYS_EVENT_KEYDOWN 10
#define SYS_EVENT_KEYUP 11

namespace sys
{

    const u8 VK_BACKSPACE = 0x0E;
    const u8 VK_TAB = 0x0F;
    const u8 VK_ENTER = 0x1C;
    const u8 VK_LCTRL = 0x1D;
    const u8 VK_LSHIFT = 0x2A;
    const u8 VK_RSHIFT = 0x36;
    const u8 VK_LALT = 0x38;
    const u8 VK_CAPSLOCK = 0x3A;
    const u8 VK_ESC = 0x01;
    const u8 VK_SPACE = 0x39;
    const u8 VK_F1 = 0x3B;

    struct
    {
        u8 shift = 0;
        u8 ctrl = 0;
        u8 alt = 0;
        u8 caps = 0;
        u8 keys[128] = {0};
        char text_buffer[256] = {0};
    } keyboard_state = {0};

    void process_scancode(u8 scancode)
    {
        raw_event e;
        if (scancode & 0x80)
        {
            e.type = SYS_EVENT_KEYUP;
            e.data = scancode & 0x7F;
            keyboard_state.keys[e.data] = 1;
        }
        else
        {
            e.type = SYS_EVENT_KEYDOWN;
            e.data = scancode;
            keyboard_state.keys[e.data] = 0;
        }

        if (e.data == 0x2A || e.data == 0x36)
            keyboard_state.shift = e.type == SYS_EVENT_KEYDOWN;
        else if (e.data == 0x1D)
            keyboard_state.ctrl = e.type == SYS_EVENT_KEYDOWN;
        else if (e.data == 0x38)
            keyboard_state.alt = e.type == SYS_EVENT_KEYDOWN;
        else if (e.data == 0x3A && e.type == SYS_EVENT_KEYDOWN)
            keyboard_state.caps = !keyboard_state.caps;

        sl::to_str(e.data, keyboard_state.text_buffer);
        int l = sl::len(keyboard_state.text_buffer);
        keyboard_state.text_buffer[l] = ' ';
        sl::to_str(e.type, keyboard_state.text_buffer + l + 1);
        keyboard_state.text_buffer[sl::len(keyboard_state.text_buffer)] = '\0';

        sl::to_str(keyboard_state.shift, keyboard_state.text_buffer + sl::len(keyboard_state.text_buffer));
        keyboard_state.text_buffer[sl::len(keyboard_state.text_buffer)] = 0;
        sl::to_str(keyboard_state.ctrl, keyboard_state.text_buffer + sl::len(keyboard_state.text_buffer));
        keyboard_state.text_buffer[sl::len(keyboard_state.text_buffer)] = 0;
        sl::to_str(keyboard_state.alt, keyboard_state.text_buffer + sl::len(keyboard_state.text_buffer));
        keyboard_state.text_buffer[sl::len(keyboard_state.text_buffer)] = 0;
        sl::to_str(keyboard_state.caps, keyboard_state.text_buffer + sl::len(keyboard_state.text_buffer));
        keyboard_state.text_buffer[sl::len(keyboard_state.text_buffer)] = '\n';
        keyboard_state.text_buffer[sl::len(keyboard_state.text_buffer) + 1] = '\0';
        sl::print(keyboard_state.text_buffer);

        event_queue_push(e);
    }

    IRQ_CALLBACK handle_keyboard()
    {
        IRQ_BEGIN_CODE();

        u8 scancode = sys::port_read8(0x60);
        process_scancode(scancode);

        sys::port_write8(0x20, 0x20);

        IRQ_END_CODE();
    }

    void keyboard_init()
    {
        sys::set_idt_gate(IRQ_KEYBOARD_IRQ, (u32)sys::handle_keyboard);

        u8 mask = sys::port_read8(0x21); // Read the current mask
        mask &= ~0x02;                   // Unmask IRQ1 (keyboard)
        sys::port_write8(0x21, mask);
    }
}