#pragma once

#define NULL 0
#define TRUE 1
#define FALSE 0

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define abs(x) ((x) < 0 ? -(x) : (x))
#define swap(x, y)          \
    do                      \
    {                       \
        typeof(x) temp = x; \
        x = y;              \
        y = temp;           \
    } while (0)

#define offsetof(type, member) ((size_t) & (((type *)0)->member))

#define va_start(vl, last) __builtin_va_start(vl, last)
#define va_arg(vl, type) __builtin_va_arg(vl, type)
#define va_end(vl) __builtin_va_end(vl)
#if defined(__GNUC__) || defined(__clang__)
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif
#define HALT_CPU() __asm__ __volatile__("hlt")

typedef int i32;
typedef unsigned int u32;
typedef short i16;
typedef unsigned short u16;
typedef char i8;
typedef unsigned char u8;
typedef long long i64;
typedef unsigned long long u64;

typedef float f32;
typedef double f64;

typedef u8 b8;

typedef char *cstr;
typedef const char *ccstr;

#define high_16(address) ((u32)address >> 16)
#define low_16(address) ((u32)address & 0xFFFF)
