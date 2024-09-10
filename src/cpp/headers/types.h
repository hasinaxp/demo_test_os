#pragma once

#define NULL 0
#define TRUE 1
#define FALSE 0

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#define is_digit(c) ((c) >= '0' && (c) <= '9')
#define is_alpha(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z'))
#define is_alnum(c) (is_digit(c) || is_alpha(c))
#define is_upper(c) ((c) >= 'A' && (c) <= 'Z')
#define is_lower(c) ((c) >= 'a' && (c) <= 'z')
#define is_space(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')

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

typedef char* cstr;
typedef const char* ccstr;


#define high_16(address) ((u32)address >> 16)
#define low_16(address) ((u32)address & 0xFFFF)

