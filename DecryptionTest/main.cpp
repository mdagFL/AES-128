#include <iostream>
#include "aes_128_decrypt.h"

using namespace std;
int main()
{
	std::cout << "Decryption Tester:" << std::endl;
	string key_str = "FLORIDAPOLYUNIVE";
	byte ciphertext[] = { 0xe6, 0x8c, 0xa4, 0x1e, 0x47, 0x55, 0x2a, 0xd4, 0x50, 0x26, 0x32, 0x1, 0x78, 0xb, 0x51, 0xad,
						 0x92, 0xff, 0x4f, 0x21, 0x6, 0x42, 0xf4, 0xe8, 0xb4, 0xec, 0xdc, 0x64, 0x51, 0xab, 0x25, 0x76 };
	byte* key = string_to_byte_array(key_str);	
	int ciphertext_length = 32;

	cout << "Key: " << key_str << endl;
	cout << "Ciphertext: " << endl;
	print_byte_array(ciphertext, ciphertext_length);
	cout << "Performing Decryption" << endl;
	byte* plaintext = decrypt_aes_128(ciphertext, key, ciphertext_length);
	string plaintext_str = byte_array_to_string(plaintext, ciphertext_length);
	cout << "Plaintext: " << plaintext_str << endl;

	exit(0);
}