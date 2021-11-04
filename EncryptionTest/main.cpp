#include <iostream>
#include "aes_128_encrypt.h"

using namespace std;

int main()
{
	std::cout << "Encryption Tester:" << std::endl;
	string key_str = "FLORIDAPOLYUNIVE";
	string plaintext_str = "AES-128 is great!";
	byte *key = string_to_byte_array(key_str);
	byte *plaintext = string_to_byte_array(plaintext_str);
	int length = plaintext_str.length();
	int ciphertext_length = 16 * (length / 16);
	if (length % 16 != 0)
		ciphertext_length += 16;

	cout << "Key: " << key_str << endl;
	cout << "Plaintext: " << plaintext_str << endl;
	cout << "Performing encryption" << endl;
	byte* ciphertext = encrypt_aes_128(plaintext, key, length);
	cout << "Ciphertext: " << endl;
	print_byte_array(ciphertext, ciphertext_length);
	

	
	exit(0);
}