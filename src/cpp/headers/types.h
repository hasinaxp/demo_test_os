#pragma once

#define NULL 0
#define TRUE 1
#define FALSE 0

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

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
