#pragma once
#include "../types.h"

namespace sys {

  struct seg_desc {
        u16 limit_lo;
        u16 base_lo;
        u8 base_hi;
        u8 type;
        u8 limit_hi;
        u8 base_vhi;
    } __attribute__((packed));


    struct gdt_ptr {
        u16 limit;
        u32 base;
    } __attribute__((packed));

    struct seg_desc os_gdt[3];
    struct gdt_ptr os_gdt_ptr;

    void gdt_set_gate(i32 num, u32 base, u32 limit, u8 access, u8 gran) {
        os_gdt[num].base_lo = (base & 0xFFFF);
        os_gdt[num].base_hi = (base >> 16) & 0xFF;
        os_gdt[num].base_vhi = (base >> 24) & 0xFF;

        os_gdt[num].limit_lo = (limit & 0xFFFF);
        os_gdt[num].limit_hi = (limit >> 16) & 0x0F;

        os_gdt[num].limit_hi |= gran & 0xF0;
        os_gdt[num].type = access;
    }

    void gdt_install() {
        os_gdt_ptr.limit = (sizeof(seg_desc) * 3) - 1;
        os_gdt_ptr.base = (u32) &os_gdt;

        gdt_set_gate(0, 0, 0, 0, 0);
        // Code segment
        // memspace: 0x00000000 - 0xFFFFFFFF
        gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
        // Data segment
        // memspace: 0x00000000 - 0xFFFFFFFF
        gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

        asm volatile("lgdt %0" : : "m" (os_gdt_ptr));
        asm volatile("mov $0x10, %ax\n\t"
                     "mov %ax, %ds\n\t"
                     "mov %ax, %es\n\t"
                     "mov %ax, %fs\n\t"
                     "mov %ax, %gs\n\t"
                     "mov %ax, %ss\n\t"
                     "ljmp $0x08, $next\n\t"
                     "next:");
    }




}