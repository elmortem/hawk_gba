#include "fixed.h"

const u16 sin_table[ 256 ] = {
0x0000, 0x0064, 0x00c8, 0x012d, 0x0191, 0x01f5, 0x0259, 0x02bc, 
0x031f, 0x0381, 0x03e3, 0x0444, 0x04a5, 0x0504, 0x0563, 0x05c2, 
0x061f, 0x067b, 0x06d7, 0x0731, 0x078a, 0x07e2, 0x0839, 0x088f, 
0x08e3, 0x0936, 0x0987, 0x09d7, 0x0a26, 0x0a73, 0x0abe, 0x0b08, 
0x0b50, 0x0b96, 0x0bda, 0x0c1d, 0x0c5e, 0x0c9d, 0x0cd9, 0x0d14, 
0x0d4d, 0x0d84, 0x0db9, 0x0deb, 0x0e1c, 0x0e4a, 0x0e76, 0x0ea0, 
0x0ec8, 0x0eed, 0x0f10, 0x0f31, 0x0f4f, 0x0f6b, 0x0f85, 0x0f9c, 
0x0fb1, 0x0fc3, 0x0fd3, 0x0fe1, 0x0fec, 0x0ff4, 0x0ffb, 0x0ffe, 
0x1000, 0x0ffe, 0x0ffb, 0x0ff4, 0x0fec, 0x0fe1, 0x0fd3, 0x0fc3, 
0x0fb1, 0x0f9c, 0x0f85, 0x0f6b, 0x0f4f, 0x0f31, 0x0f10, 0x0eed, 
0x0ec8, 0x0ea0, 0x0e76, 0x0e4a, 0x0e1c, 0x0deb, 0x0db9, 0x0d84, 
0x0d4d, 0x0d14, 0x0cd9, 0x0c9d, 0x0c5e, 0x0c1d, 0x0bda, 0x0b96, 
0x0b50, 0x0b08, 0x0abe, 0x0a73, 0x0a26, 0x09d7, 0x0987, 0x0936, 
0x08e3, 0x088f, 0x0839, 0x07e2, 0x078a, 0x0731, 0x06d7, 0x067b, 
0x061f, 0x05c2, 0x0563, 0x0504, 0x04a5, 0x0444, 0x03e3, 0x0381, 
0x031f, 0x02bc, 0x0259, 0x01f5, 0x0191, 0x012d, 0x00c8, 0x0064, 
0x0000, 0xff9c, 0xff38, 0xfed3, 0xfe6f, 0xfe0b, 0xfda7, 0xfd44, 
0xfce1, 0xfc7f, 0xfc1d, 0xfbbc, 0xfb5b, 0xfafc, 0xfa9d, 0xfa3e, 
0xf9e1, 0xf985, 0xf929, 0xf8cf, 0xf876, 0xf81e, 0xf7c7, 0xf771, 
0xf71d, 0xf6ca, 0xf679, 0xf629, 0xf5da, 0xf58d, 0xf542, 0xf4f8, 
0xf4b0, 0xf46a, 0xf426, 0xf3e3, 0xf3a2, 0xf363, 0xf327, 0xf2ec, 
0xf2b3, 0xf27c, 0xf247, 0xf215, 0xf1e4, 0xf1b6, 0xf18a, 0xf160, 
0xf138, 0xf113, 0xf0f0, 0xf0cf, 0xf0b1, 0xf095, 0xf07b, 0xf064, 
0xf04f, 0xf03d, 0xf02d, 0xf01f, 0xf014, 0xf00c, 0xf005, 0xf002, 
0xf000, 0xf002, 0xf005, 0xf00c, 0xf014, 0xf01f, 0xf02d, 0xf03d, 
0xf04f, 0xf064, 0xf07b, 0xf095, 0xf0b1, 0xf0cf, 0xf0f0, 0xf113, 
0xf138, 0xf160, 0xf18a, 0xf1b6, 0xf1e4, 0xf215, 0xf247, 0xf27c, 
0xf2b3, 0xf2ec, 0xf327, 0xf363, 0xf3a2, 0xf3e3, 0xf426, 0xf46a, 
0xf4b0, 0xf4f8, 0xf542, 0xf58d, 0xf5da, 0xf629, 0xf679, 0xf6ca, 
0xf71d, 0xf771, 0xf7c7, 0xf81e, 0xf876, 0xf8cf, 0xf929, 0xf985, 
0xf9e1, 0xfa3e, 0xfa9d, 0xfafc, 0xfb5b, 0xfbbc, 0xfc1d, 0xfc7f,
0xfce1, 0xfd44, 0xfda7, 0xfe0b, 0xfe6f, 0xfed3, 0xff38, 0xff9c };

