#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "aes_128_common.h"
#include "aes_128_encrypt.h"
#include "aes_128_decrypt.h"
#include "aes_128_key_expansion.h"

using namespace std;

void menu_encrypt();
void menu_decrypt();
void menu_key_expansion();
string get_message_from_user();
string get_key_from_user();
byte* get_byte_array_from_user(int& out_length);
int get_ciphertext_length_from_plaintext_length(int);

int main() {
    char selection{};
    while (true)
    {
        cout << "Enter E to encrypt, D to decrypt, X for key expansion, or Q to quit:\n";
        cin >> selection;
        switch (selection)
        {
        case 'e':
        case 'E':     
            menu_encrypt();
            break;
        case 'd':
        case 'D':
            menu_decrypt();
            break;
        case 'q':
        case 'Q':
            std::exit(0);
            break;
        default:
            cout << "Error: invalid selection, please try again.\n";
        }

    }
    return 0;
}

void menu_decrypt()
{
    string key_entry{get_key_from_user()};

    byte* key = string_to_byte_array(key_entry);
    int ciphertext_length{};

    cout << "Ciphertext -\n";
    byte* ciphertext_bytes = get_byte_array_from_user(ciphertext_length);
    
    byte* plaintext_bytes = decrypt_aes_128(ciphertext_bytes, key, ciphertext_length);
    string plaintext = byte_array_to_string(plaintext_bytes, ciphertext_length);

    cout << "Decrypted message plaintext:\n";
    cout << plaintext << "\n";

    delete[] key;
    delete[] ciphertext_bytes;
    delete[] plaintext_bytes;
}

void menu_encrypt()
{
    string key_entry{get_key_from_user()};

    byte* key = string_to_byte_array(key_entry);
    cout << "Plaintext -\n";
    string plaintext{ get_message_from_user() };
    byte* plaintext_bytes = string_to_byte_array(plaintext);
    int ciphertext_length = get_ciphertext_length_from_plaintext_length(plaintext.length());
    byte* ciphertext = encrypt_aes_128(plaintext_bytes, key, plaintext.length());

    std::cout << "Output ciphertext byte array:\n";
    print_byte_array(ciphertext, ciphertext_length);

    delete[] key;
    delete[] plaintext_bytes;
    delete[] ciphertext;
}

void menu_key_expansion()
{

}

string get_key_from_user()
{
    string key_entry{};
    

    cout << "Enter the case-sensitive, 16-character key to use:\n";
    cin >> key_entry;
    while (key_entry.length() != 16)
    {
        cout << "Error: key must be 16 characters long, please try again.\n";
        cin >> key_entry;
    }
    return key_entry;
}

string get_message_from_user()
{
    string message{};
    cout << "Enter a message, then enter $DONE on a separate line when finished:\n";
    string next_line{};

    getline(cin, next_line);
    while (next_line.compare("$DONE") != 0)
    {
        message += next_line;
        getline(cin, next_line);
    }
    return message;
}

byte* get_byte_array_from_user(int& out_length)
{
    byte* byte_array{};
    vector<byte> bytes{};
    byte next_byte{};
    out_length = 0;

    cout << "Enter spaced hexadecimal bytes in the format 0xBB, where BB is the hexadecimal value.\n";
    cout << "Enter 'X' when finished:\n";

    cin >> hex >> next_byte;
    while (!cin.fail())
    {
        bytes.push_back(next_byte);
        cin >> hex >> next_byte;
    }
    // Reset CIN error flags and flush the sentinal input
    cin.clear(istream::goodbit);
    cin.ignore();

    byte_array = new byte[bytes.size()];
    for (int i = 0; i < bytes.size(); i++)
    {
        byte_array[i] = bytes[i];
    }

    out_length = bytes.size();
    return byte_array;
}

int get_ciphertext_length_from_plaintext_length(int plaintext_length)
{
    int ciphertext_length = 16 * (plaintext_length / 16);
    if (plaintext_length % 16 != 0)
        ciphertext_length += 16;
    return ciphertext_length;
}