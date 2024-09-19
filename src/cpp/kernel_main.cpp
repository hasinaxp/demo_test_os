#include "headers/types.h"
#include "headers/str.h"
#include "headers/sys/gdt.h"
#include "headers/sys/port.h"
#include "headers/sys/interrupt.h"
#include "headers/sys/keyboard.h"

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
    sys::gdt_install();
    sys::idt_init();
    sys::keyboard_init();

    sys::idt_install();
    sys::activate_interrupts();

    sl::print("System initialized\n");
}

extern "C"
{

    void kernel_main()
    {
        init_systems();
        while (1)
            ;
    }
}