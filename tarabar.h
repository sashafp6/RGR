#ifndef TARABAR_H
#define TARABAR_H

#include <string>

using namespace std;

string tarabar_encrypt(const string& text);
string tarabar_decrypt(const string& text);
string process_file_auto_tarabar(const string& input_path, const string& output_path);

#endif 
