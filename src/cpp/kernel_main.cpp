#include "headers/types.h"
#include "headers/str.h"
#include "headers/sys/gdt.h"

char *video_memory = (char *)0xb8000;

void print(const char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        *video_memory++ = str[i];
        *video_memory++ = 0x07;
    }
}

extern "C"
{

    void kernel_main()
    {
        sys::gdt_install();
        char str[512] = "A quick brown fox jumps over the lazy dog";
        i32 l = sl::len(str);
        sl::to_str(l, str);
        print(str);
        while (1)
            ;
    }
}