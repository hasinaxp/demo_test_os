#pragma once
#include "../types.h"
#include "port.h"

namespace sys
{
    void ata_init()
    {
        port_write8(0x1F6, 0xA0);
        port_write8(0x3F6, 0xA0);
    }

    void ata_read(u32 lba, u8 *buffer, u32 count = 0)
    {
        port_write8(0x1F2, count);
        port_write8(0x1F3, (u8)(lba & 0xFF));
        port_write8(0x1F4, (u8)(lba >> 8) & 0xFF);
        port_write8(0x1F5, (u8)(lba >> 16) & 0xFF);
        port_write8(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));
        port_write8(0x1F7, 0x20);

        for (u32 i = 0; i < count; i++)
        {
            u8 status = port_read8(0x1F7);
            while (!(status & 0x08))
                status = port_read8(0x1F7);

            for (u32 j = 0; j < 256; j++)
            {
                u16 data = port_read16(0x1F0);
                buffer[i * 256 + j] = data;
            }
        }

        port_write8(0x1F7, 0xE7);
    }

    void ata_write(u32 lba, u8 *buffer, u32 count)
    {
        port_write8(0x1F2, count);
        port_write8(0x1F3, (u8)(lba & 0xFF));
        port_write8(0x1F4, (u8)(lba >> 8) & 0xFF);
        port_write8(0x1F5, (u8)(lba >> 16) & 0xFF);
        port_write8(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));
        port_write8(0x1F7, 0x30);

        for (u32 i = 0; i < count; i++)
        {
            u8 status = port_read8(0x1F7);
            while (!(status & 0x08))
                status = port_read8(0x1F7);

            for (u32 j = 0; j < 256; j++)
            {
                u16 data = buffer[i * 256 + j];
                port_write16(0x1F0, data);
            }
        }

        port_write8(0x1F7, 0xE7);
    }

}