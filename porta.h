#ifndef PORTA_H
#define PORTA_H

#include <string>
using namespace std;

string porta_encrypt(const string& text, const string& key);
string porta_decrypt(const string& text, const string& key);
string process_file_auto_porta(const string& input_path, const string& output_path, const string& key, bool encrypt);
string generate_key_porta(int length = 16);

string base64_encode(const string &in);
string base64_decode(const string &in);

#endif 
