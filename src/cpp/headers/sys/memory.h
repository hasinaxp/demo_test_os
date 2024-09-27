#pragma once
#include "../types.h"
#include "../console.h"

extern "C" char _end_of_kernel;

#define SYS_MEMORY_REGION_USABLE 1
#define SYS_MEMORY_REGION_RESERVED 2
#define SYS_MEMORY_REGION_ACPI 3
#define SYS_MEMORY_REGION_NVS 4
#define SYS_MEMORY_REGION_BAD 5

namespace sys
{

    typedef struct
    {
        u32 size;
        u64 base_addr;
        u64 length;
        u32 type;
    } __attribute__((packed)) mem_map_entry;

    typedef struct
    {
        u32 flags;
        u32 mem_lower;
        u32 mem_upper;
        u32 boot_device;
        u32 cmdline;
        u32 mods_count;
        u32 mods_addr;
        u32 syms[4];
        u32 mmap_length;
        u32 mmap_addr;
    } __attribute__((packed)) multiboot_info;

    typedef struct
    {
        u32 base;
        u32 length;
        u32 type;
    } memory_region;

    memory_region memory_regions[16] = {0};
    memory_region runtime_memory = {0};
    u32 n_memory_regions = 0;

    inline void get_memory_map(multiboot_info *mbi)
    {
        if (mbi->flags & (1 << 6))
        {
            mem_map_entry *mmap = (mem_map_entry *)mbi->mmap_addr;
            u32 mmap_end = mbi->mmap_addr + mbi->mmap_length;
            while ((u32)mmap < mmap_end)
            {
                memory_regions[n_memory_regions].base = mmap->base_addr;
                memory_regions[n_memory_regions].length = mmap->length;
                memory_regions[n_memory_regions].type = mmap->type;
                n_memory_regions++;

                mmap = (mem_map_entry *)((u32)mmap + mmap->size + 4);
            }
        }

        u8 highest_usable_region = 0;

        for (u8 i = 0; i < n_memory_regions; i++)
        {
            if (memory_regions[i].type == SYS_MEMORY_REGION_USABLE)
            {
                if (memory_regions[i].base + memory_regions[i].length > runtime_memory.base + runtime_memory.length)
                {
                    runtime_memory.base = memory_regions[i].base;
                    runtime_memory.length = memory_regions[i].length;
                    highest_usable_region = i;
                }
            }
        }
    }

    inline void print_memory_map()
    {
        char buff[256];

        for (u8 i = 0; i < n_memory_regions; i++)
        {

            sl::print("Memory region ");
            sl::to_str(i, buff);
            sl::print(buff);
            sl::print(": ");
            sl::to_str(memory_regions[i].base, buff);
            sl::print(buff);
            sl::print(" - ");
            sl::to_str(memory_regions[i].base + memory_regions[i].length, buff);
            sl::print(buff);
            sl::print(" (");
            sl::to_str(memory_regions[i].length, buff);
            sl::print(buff);
            sl::print(" bytes) ");
            sl::print("Type: ");
            sl::to_str(memory_regions[i].type, buff);
            sl::print(buff);
            sl::print("\n");
        }

        sl::print("runtime memory: \n");
        sl::print("base: ");
        sl::to_str(runtime_memory.base, buff);
        sl::print(buff);
        sl::print("\nlength: ");
        sl::to_str(runtime_memory.length / (1024 * 1024), buff);
        sl::print(buff);
        sl::print(" MB\n");
    }

}
