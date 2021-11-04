#include "aes_128_common.h"
#include "aes_128_key_expansion.h"

#ifndef AES_128_ENCRYPT_H
#define AES_128_ENCRYPT_H

const byte MIX_COL_MATRIX[] = { 2, 1, 1, 3,
                                3, 2, 1, 1,
                                1, 3, 2, 1,
                                1, 1, 3, 2 };

byte *encrypt_aes_128(const byte *plaintext, const byte *key, int len);

void shift_rows(byte *byte_array);

void mix_columns(byte *byte_array);

#endif //AES_128_ENCRYPT_H
