#pragma once
#include "types.h"

#define is_digit(c) ((c) >= '0' && (c) <= '9')
#define is_alpha(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z'))
#define is_alnum(c) (is_digit(c) || is_alpha(c))
#define is_upper(c) ((c) >= 'A' && (c) <= 'Z')
#define is_lower(c) ((c) >= 'a' && (c) <= 'z')
#define is_space(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')

namespace sl
{
    inline u32 len(const ccstr str)
    {
        u32 length = 0;
        while (str[length] != '\0')
            length++;
        return length;
    }

    inline i32 to_i32(const ccstr str, const u32 &base = 10)
    {
        i32 result = 0;
        i32 sign = 1;
        u32 i = 0;

        if (str[0] == '-')
        {
            sign = -1;
            i++;
        }

        for (; str[i] != '\0'; i++)
        {
            if (is_digit(str[i]))
                result = result * base + str[i] - '0';
            else if (is_alpha(str[i]))
                result = result * base + str[i] - (is_upper(str[i]) ? 'A' : 'a') + 10;
            else
                break;
        }

        return result * sign;
    }

    inline void to_str(i32 val, cstr buf)
    {
        i32 i = 0;
        i32 j = 0;
        i32 sign = val < 0 ? -1 : 1;
        val *= sign;

        do
        {
            buf[i++] = val % 10 + '0';
            val /= 10;
        } while (val);

        if (sign < 0)
            buf[i++] = '-';

        buf[i] = '\0';

        i--;
        while (j < i)
        {
            char temp = buf[j];
            buf[j] = buf[i];
            buf[i] = temp;
            j++;
            i--;
        }
    }

    inline void to_hex_str(u32 val, cstr buf)
    {
        u32 i = 0;
        do
        {
            u32 digit = val & 0xF;
            buf[i++] = digit < 10 ? digit + '0' : digit - 10 + 'A';
            val >>= 4;
        } while (val);

        buf[i] = '\0';

        i--;
        u32 j = 0;
        while (j < i)
        {
            char temp = buf[j];
            buf[j] = buf[i];
            buf[i] = temp;
            j++;
            i--;
        }
    }

}