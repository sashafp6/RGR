#ifndef CAESAR_H
#define CAESAR_H

#include <string>

using namespace std;

string caesar_encrypt(const string& text, const string& key);
string caesar_decrypt(const string& text, const string& key);
string process_file_auto_caesar(const string& input_path, const string& output_path, const string& key, bool encrypt);
string generate_key_caesar(int length = 16);

#endif 
