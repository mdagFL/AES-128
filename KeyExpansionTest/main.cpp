#include <iostream>
#include "aes_128_key_expansion.h"

using namespace std;
int main()
{
	string key_str = "FLORIDAPOLYUNIVE";
	byte *key = string_to_byte_array(key_str);
	cout << "Key Expansion Tester" << endl << endl;
	
	cout << "Key: " << key_str << endl;
	cout << "Getting round keys: " << endl;
	for (int i = 0; i < 10; i++)
	{
		byte* state;
		state = get_round_key(key, i);
		
		cout << "Round " << i + 1 << ": " << endl;
		print_state(key);
		cout << endl;
		delete state;
	}

	exit(0);
}