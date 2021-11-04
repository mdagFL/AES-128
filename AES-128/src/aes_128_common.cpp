#include "../include/aes_128_common.h"

using namespace std;



// Create Byte Array From String
byte *string_to_byte_array(string str) {
    size_t m = str.length();
    byte *return_array = new byte[m+1];
    for (size_t i = 0; i < m; i++) {
        return_array[i] = (byte) str.at(i);
    }
    return_array[m] = (byte)0;
    return return_array;
}

// Print Byte Array
void print_byte_array(byte *byte_array, size_t length) {
    for (size_t i = 0; i < length; i++) {
        cout << "0x" << hex << byte_array[i] << " ";
    }
    cout << endl;
}

// Print the state matrix
void print_state(byte *byte_array) {
    for (size_t i = 0; i < 4; i++) {
        cout << "0x" << hex << byte_array[i] << " " << "0x" << hex << byte_array[i + 4] << " " << "0x" << hex
             << byte_array[i + 8] << " " << "0x" << hex << byte_array[i + 12] << endl;
    }
}


// Print a new line
void newline() {
    cout << endl;
}

// Add round key
void add_round_key(byte *byte_array, const byte *key) {
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        byte_array[i] ^= key[i];
    }
}

// S-box ( Substitute a single byte )
byte substitute_byte(byte byte_to_substitute) {
    return S_BOX[byte_to_substitute];
}

// substitute the bytes of the state array
void substitute_bytes(byte *byte_array) {
    for (int i = 0; i < BLOCK_SIZE; i++)
        byte_array[i] = substitute_byte(byte_array[i]);
}

void clone_byte_array(const byte *byte_array, byte *clone, int len)
{
    for (int i = 0; i < len; i++)
        clone[i] = byte_array[i];
}

// Return the greatest power of 2 <= a
byte msb(byte a)
{
    byte result = 1;
    while (result <= a)
        result <<= 1;
    return result >> 1;
}

// Return the product of two bytes a and b in GF(2^8)
byte gf_multiply(byte a, byte b)
{
    // polynomial multiplication: multiply but use XOR for addition
    // for every 1 bit in a, add b left-shifted to that 1 bit
    unsigned int product = 0;
    for (int bit_offset = 0; bit_offset < 8; bit_offset++)
    {
        if (a & (1 << bit_offset))
        {
            product ^= (b << bit_offset);
        }
    }

    // divide by the irreducible polynomial
    // get the reaminder by performing the subtraction
    while (product > 0xFF)
    {
        int bit_offset = 0;
        int subtrahend = IRR_POLYNOMIAL;

        // left shift the divisor so that its MSB is the same as the product
        int product_msb = msb(product);
        while (subtrahend < product_msb)
            subtrahend <<= 1;

        // subtract (xor)
        product ^= subtrahend;
    }

    return product;  
}

// gf_multiply a column-major matrix A (size: n x n) with vector x, store the result in b
void matrix_vector_multiply(const byte *A, const byte *x, byte *b, int n)
{
    for (int i = 0; i < n; i++)
    { 
        int sum = 0;
        for (int j = 0; j < n; j++)
        {
            sum ^= gf_multiply(A[i + j * n], x[j]);
        }
        b[i] = sum;
    }
}

string byte_array_to_string(const byte *byte_array, int len)
{
    string str = "";
    for (int i = 0; i < len; i++)
        str += (char)byte_array[i];
    return str;
}