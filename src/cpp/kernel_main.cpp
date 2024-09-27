#include "headers/types.h"
#include "headers/str.h"
#include "headers/sys/gdt.h"
#include "headers/sys/port.h"
#include "headers/sys/interrupt.h"
#include "headers/sys/keyboard.h"
#include "headers/sys/events.h"
#include "headers/sys/clock.h"
#include "headers/sys/mouse.h"
#include "headers/sys/memory.h"
#include "headers/sys/ata_io.h"

char *video_memory = (char *)0xb8000;

void print(const char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        *video_memory++ = str[i];
        *video_memory++ = 0x07;
    }
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void call_constructors()
{
    for (constructor *i = &start_ctors; i != &end_ctors; i++)
    {
        (*i)();
    }
}

void init_systems()
{
    sl::clear_screen();
    sys::gdt_install();
    sys::idt_init();

    sys::keyboard_init();
    sys::clock_init();
    sys::mouse_init();

    sys::idt_install();

    sl::print("System initialized\n");

    char mtext[256];
}

void update_systems()
{
}
char sector_buffer[512];

void write_dummy_sector(char *sb, char *data)
{
    u32 data_len = sl::len(data);
    for (u32 i = 0; i < data_len; i++)
    {
        sb[i] = data[i];
    }
    for (u32 i = data_len; i < 512; i++)
    {
        sb[i] = 0;
    }
}

extern "C"
{

    void kernel_main(u32 magic, sys::multiboot_info *mbi)
    {
        init_systems();
        sys::get_memory_map(mbi);
        sys::print_memory_map();
        while (1)
        {
            update_systems();
        }
    }
}