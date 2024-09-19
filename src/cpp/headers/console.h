#pragma once
#define CNSL_H 25
#define CNSL_W 80

namespace sl
{
    int console_cursor = 0;
    char *video_memory = (char *)0xb8000;

    void print(const char *str)
    {
        for (int i = 0; str[i] != '\0'; i++)
        {
            if (str[i] == '\n')
            {
                console_cursor += CNSL_W - console_cursor % CNSL_W;
            }
            else if (str[i] == '\t')
            {
                console_cursor += 4 - console_cursor % 4;
            }
            else
            {
                video_memory[console_cursor * 2] = str[i];
                video_memory[console_cursor * 2 + 1] = 0x07;
                console_cursor++;
            }

            if (console_cursor >= CNSL_W * CNSL_H)
            {
                for (int j = 0; j < CNSL_W * (CNSL_H - 1) * 2; j++)
                {
                    video_memory[j] = video_memory[j + CNSL_W * 2];
                }

                for (int j = CNSL_W * (CNSL_H - 1) * 2; j < CNSL_W * CNSL_H * 2; j++)
                {
                    video_memory[j] = 0;
                }

                console_cursor -= CNSL_W;
            }
        }
    }
}