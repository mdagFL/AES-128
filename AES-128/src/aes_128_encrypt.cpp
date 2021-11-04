#include "../include/aes_128_encrypt.h"



// Encryption function
byte *encrypt_aes_128(const byte* plaintext, const byte* key, int len) {
    byte *ciphertext = NULL;

    // get length of the ciphertext
    int cipher_len = (len / BLOCK_SIZE) * BLOCK_SIZE;
    if (len % 16 != 0)
        cipher_len += 16;
    ciphertext = new byte[cipher_len];

    // for each block of plaintext
    for (int i = 0; i < cipher_len; i += BLOCK_SIZE)
    {
        // get the next block of plaintext
        byte ciphertext_block[BLOCK_SIZE];
        for (int j = 0; j < BLOCK_SIZE; j++)
        {
            if (j+i >= len) // for the last block, use zeroes for values beyond plaintext
                ciphertext_block[j] = 0;
            else
                ciphertext_block[j] = plaintext[i + j];
        }

        // encrypt the block
        byte *round_key;
        add_round_key(ciphertext_block, key); // initial add_round_key
        for (int round = 0; round < N_ROUNDS; round++)
        {
            substitute_bytes(ciphertext_block);
            shift_rows(ciphertext_block);
            if (round < 9)
                mix_columns(ciphertext_block);
            round_key = get_round_key(key, round); 
            add_round_key(ciphertext_block, round_key);
            delete round_key;
        }

        // add to the final ciphertext
        for (int j = 0; j < BLOCK_SIZE; j++)
            ciphertext[i+j] = ciphertext_block[j];
    }


    return ciphertext;
}

// Shift rows
void shift_rows(byte* byte_array) {
    byte original_state[BLOCK_SIZE];
    clone_byte_array(byte_array, original_state, BLOCK_SIZE);
    for (int row = 1; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            int current_index = 4 * col + row;
            int replace_index = ((4 * (col - row) + row) % BLOCK_SIZE); // index to replace with current
            replace_index = (replace_index + BLOCK_SIZE) % BLOCK_SIZE; // handle negatives
            byte_array[replace_index] = original_state[current_index];
        }
    }
}

// Mix Columns
void mix_columns(byte *byte_array) {
    byte original_state[BLOCK_SIZE];
    clone_byte_array(byte_array, original_state, BLOCK_SIZE);
    for (int i = 0; i < 4; i++)
    {
        matrix_vector_multiply(MIX_COL_MATRIX, original_state + 4*i, byte_array + 4*i, 4);
    }
}

