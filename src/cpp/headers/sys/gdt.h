#pragma once
#include "../types.h"

#define GDT_SIZE 6

namespace sys
{

    typedef struct
    {
        u16 limit_lo;
        u16 base_lo;
        u8 base_hi;
        u8 type;
        u8 limit_hi;
        u8 base_vhi;
    } __attribute__((packed)) seg_desc;

    typedef struct
    {
        u16 limit;
        u32 base;
    } __attribute__((packed)) gdt_ptr;

    typedef struct
    {
        u32 prev_tss;
        u32 esp0;
        u32 ss0;
        u32 esp1;
        u32 ss1;
        u32 esp2;
        u32 ss2;
        u32 cr3;
        u32 eip;
        u32 eflags;
        u32 eax;
        u32 ecx;
        u32 edx;
        u32 ebx;
        u32 esp;
        u32 ebp;
        u32 esi;
        u32 edi;
        u32 es;
        u32 cs;
        u32 ss;
        u32 ds;
        u32 fs;
        u32 gs;
        u32 ldt;
        u16 trap;
        u16 iomap_base;
    } __attribute__((packed)) tss_entry;

    seg_desc os_gdt[GDT_SIZE] = {0};
    gdt_ptr os_gdt_ptr = {0};
    tss_entry os_tss = {0};

    inline void gdt_set_gate(i32 num, u32 base, u32 limit, u8 access, u8 gran)
    {
        os_gdt[num].base_lo = (base & 0xFFFF);
        os_gdt[num].base_hi = (base >> 16) & 0xFF;
        os_gdt[num].base_vhi = (base >> 24) & 0xFF;

        os_gdt[num].limit_lo = (limit & 0xFFFF);
        os_gdt[num].limit_hi = (limit >> 16) & 0x0F;

        os_gdt[num].limit_hi |= gran & 0xF0;
        os_gdt[num].type = access;
    }

    inline void tss_write(u32 num, u16 ss0, u32 esp0)
    {
        u32 base = (u32)&os_tss;
        u32 limit = base + sizeof(os_tss);

        gdt_set_gate(num, base, limit, 0xE9, 0x00);

        os_tss.ss0 = ss0;
        os_tss.esp0 = esp0;
        os_tss.cs = 0x0b;
        os_tss.ss = os_tss.ds = os_tss.es = os_tss.fs = os_tss.gs = 0x13;
        os_tss.iomap_base = sizeof(os_tss);
    }

    inline void gdt_install()
    {
        os_gdt_ptr.limit = (sizeof(seg_desc) * GDT_SIZE) - 1;
        os_gdt_ptr.base = (u32)&os_gdt;

        gdt_set_gate(0, 0, 0, 0, 0);
        gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // kernel code
        gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // kernel data
        gdt_set_gate(3, 0, 0xFFFFFFFF, 0xF2, 0xCF); // user code
        gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // user data

        // tss

        tss_write(5, 0x10, 0x0);

        asm volatile("lgdt %0" : : "m"(os_gdt_ptr));
        asm volatile("mov $0x10, %ax\n\t"
                     "mov %ax, %ds\n\t"
                     "mov %ax, %es\n\t"
                     "mov %ax, %fs\n\t"
                     "mov %ax, %gs\n\t"
                     "mov %ax, %ss\n\t"
                     "ljmp $0x08, $next\n\t"
                     "next:");

        asm volatile("ltr %%ax" : : "a"(0x28));
    }
}