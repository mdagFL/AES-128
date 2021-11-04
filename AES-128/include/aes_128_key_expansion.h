#include "aes_128_common.h"

#ifndef AES_128_KEY_EXPANSION_H
#define AES_128_KEY_EXPANSION_H
const byte R_CON[] = { 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };
void circular_left_shift(byte *byte_word);

void add_round_constant(byte *byte_word, unsigned char round_number);

byte *g_function(const byte *byte_word, unsigned char round_number);

byte *get_round_key(const byte *key_bytes, unsigned char round_number);

#endif //AES_128_KEY_EXPANSION_H
