#include "../include/aes_128_key_expansion.h"

// Circular Left Shift (Rotate Left)
void circular_left_shift(byte *byte_word) {
    byte original[4];
    clone_byte_array(byte_word, original, 4);
    for (int i = 0; i < 4; i++)
    {
        byte_word[i] = original[(i + 1) % 4];
    }
}

// Add Round Constant
void add_round_constant(byte *byte_word, unsigned char round_number) {
    byte_word[0] ^= R_CON[round_number];
}


// The g function of the AES key expansion
byte *g_function(const byte *byte_word, unsigned char round_number)
{
    byte *g_return_word=new byte[4];

    // copy the word to the result
    for (int i = 0; i < 4; i++)
        g_return_word[i] = byte_word[i];

    circular_left_shift(g_return_word);

    for (int i = 0; i < 4; i++)
        g_return_word[i] = substitute_byte(g_return_word[i]);

    add_round_constant(g_return_word, round_number);

    return g_return_word;
}


// Get round key:
byte *get_round_key(const byte *key_bytes, unsigned char round_number) {
    byte *round_key = new byte[BLOCK_SIZE];
    byte round_keys[BLOCK_SIZE * (N_ROUNDS+1)]; // array to store each round key to build the next
    clone_byte_array(key_bytes, round_keys, BLOCK_SIZE); // copy the key to the beginning
    for (int i = BLOCK_SIZE; i < (round_number+2)*BLOCK_SIZE; i+=4)
    {
        int current_round_num = i / BLOCK_SIZE - 1;
        // set the next word to the previous word
        for (int j = 0; j < 4; j++)
        {
            round_keys[i+j] = round_keys[i-4+j];
        }

        // first word of each new round key
        if (i % BLOCK_SIZE == 0)
        {
            byte* g = g_function(round_keys + i, current_round_num);     
            for (int j = 0; j < 4; j++)
                round_keys[i + j] = g[j];
            delete g;
        }

        // add with fourth last word
        for (int j = 0; j < 4; j++)
        {
            round_keys[i + j] ^= round_keys[i+j - BLOCK_SIZE];
            if (current_round_num == round_number) // if this is the highest round
            {
                // append to output
                round_key[i - (1 + round_number) * BLOCK_SIZE + j] = round_keys[i + j];
            }

        }
                  
    }
        
        
    return round_key;
}

