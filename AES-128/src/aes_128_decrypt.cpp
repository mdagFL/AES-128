#include "../include/aes_128_decrypt.h"

// Decryption function
byte *decrypt_aes_128(const byte *ciphertext, const byte *key, int len) {
    byte* plaintext = new byte[len];
       
    for (int i = 0; i < len; i += BLOCK_SIZE)
    {
        // get the next block of ciphertext
        byte plaintext_block[BLOCK_SIZE];        
        clone_byte_array(ciphertext + i, plaintext_block, BLOCK_SIZE);

        // initial key addition, use last round key
        byte* init_key = get_round_key(key, 9);
        add_round_key(plaintext_block, init_key);
        delete init_key;

        // decrypt the block
        for (int round = 9; round >= 0; round--)
        {
            inverse_substitute_bytes(plaintext_block);
            inverse_shift_rows(plaintext_block);
            if (round != 0)
            {
                inverse_mix_columns(plaintext_block);
                byte* round_key = get_round_key(key, round - 1);
                inverse_mix_columns(round_key);
                add_round_key(plaintext_block, round_key);
                delete round_key;
            }                
            else // last round, just add original key
            {
                add_round_key(plaintext_block, key);
            }
            
        }

        // add decrypted block to the plaintext
        for (int j = 0; j < BLOCK_SIZE; j++)
            plaintext[i + j] = plaintext_block[j];
    }

    return plaintext;
}

// IS-box ( Inverse substitute a single byte )
byte inverse_substitute_byte(byte byte_to_substitute) {
    return INV_S_BOX[byte_to_substitute];
}

// Inverse substitute the bytes of the state array
void inverse_substitute_bytes(byte *byte_array) {
    for (int i = 0; i < BLOCK_SIZE; i++)
        byte_array[i] = INV_S_BOX[byte_array[i]];
}

// Inverse Shift rows
void inverse_shift_rows(byte *byte_array) {
    byte original_state[BLOCK_SIZE];
    clone_byte_array(byte_array, original_state, BLOCK_SIZE);
    for (int row = 1; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            int current_index = 4 * col + row;
            int replace_index = ((4 * (col - (4-row)) + row) % BLOCK_SIZE); // index to replace with current
            replace_index = (replace_index + BLOCK_SIZE) % BLOCK_SIZE; // handle negatives
            byte_array[replace_index] = original_state[current_index];
        }
    }
}

// Inverse Mix Columns
void inverse_mix_columns(byte *byte_array) {
    byte original_state[BLOCK_SIZE];
    clone_byte_array(byte_array, original_state, BLOCK_SIZE);
    for (int i = 0; i < 4; i++)
    {
        matrix_vector_multiply(INV_MIX_COL_MATRIX, original_state + 4 * i, byte_array + 4 * i, 4);
    }
}
