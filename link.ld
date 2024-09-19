ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

SECTIONS
{
    . = 0x100000;
    .text : {
        *(.text*)
        *(.rodata)
    }
    .data : {
        start_ctors = .;
        KEEP(*(.init_array));
        KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));
        end_ctors = .;
        *(.data)
    }
    .bss : {
        *(.bss) 
        _end = .;
    }
    .heap_start = _end + 0x800;
    PROVIDE(_heap_start = .heap_start);
}
