#include "caesar.h"
#include "../alphabet_constants.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>
#include <filesystem>

using namespace std;

// Шифр Цезаря - сдвиг каждой буквы на фиксированное количество позиций
string caesar_encrypt(const string& text, const string& key) {
    if (key.empty()) throw runtime_error("Ключ не может быть пустым!");
    
    vector<string> chars = AlphabetConstants::splitUtf8String(text);
    vector<string> result_chars;

    int eng_shift = -1;
    int rus_shift = -1;
    for (char k : key) {
        if (AlphabetConstants::isEnglishLetter(k)) {
            eng_shift = (toupper(k) - AlphabetConstants::ENGLISH_UPPERCASE_START) % AlphabetConstants::ENGLISH_ALPHABET_SIZE;
            if (eng_shift < 0) eng_shift += AlphabetConstants::ENGLISH_ALPHABET_SIZE;
            break;
        }
    }
    for (size_t i = 0; i < key.size();) {
        unsigned char c = key[i];
        string utf8_char;
        if (c < 0x80) {
            utf8_char = key.substr(i, 1);
            i++;
        } else if ((c & 0xE0) == 0xC0) {
            utf8_char = key.substr(i, 2);
            i += 2;
        } else if ((c & 0xF0) == 0xE0) {
            utf8_char = key.substr(i, 3);
            i += 3;
        } else if ((c & 0xF8) == 0xF0) {
            utf8_char = key.substr(i, 4);
            i += 4;
        } else {
            i++;
            continue;
        }
        if (AlphabetConstants::isRussianLetter(utf8_char)) {
            rus_shift = AlphabetConstants::getRussianPosition(utf8_char);
            break;
        }
    }
    // Если не найдено ни одной буквы нужного алфавита, сдвиг = 1
    if (eng_shift == -1) eng_shift = 1;
    if (rus_shift == -1) rus_shift = 1;
    
    for (const string& utf8_char : chars) {
        if (AlphabetConstants::isEnglishLetter(utf8_char[0])) {
            char c = utf8_char[0];
            char base = AlphabetConstants::isEnglishUppercase(c) ? AlphabetConstants::ENGLISH_UPPERCASE_START : AlphabetConstants::ENGLISH_LOWERCASE_START;
            char encrypted = (c - base + eng_shift) % AlphabetConstants::ENGLISH_ALPHABET_SIZE + base;
            result_chars.push_back(string(1, encrypted));
        } else if (AlphabetConstants::isRussianLetter(utf8_char)) {
            int pos = AlphabetConstants::getRussianPosition(utf8_char);
            if (pos >= 0) {
                int new_pos = (pos + rus_shift) % AlphabetConstants::RUSSIAN_ALPHABET_SIZE;
                string new_char = AlphabetConstants::getRussianChar(new_pos);
                if (AlphabetConstants::isRussianLowercase(utf8_char)) {
                    new_char = AlphabetConstants::toRussianLowercase(new_char);
                }
                result_chars.push_back(new_char);
            } else {
                result_chars.push_back(utf8_char);
            }
        } else {
            result_chars.push_back(utf8_char);
        }
    }
    return AlphabetConstants::joinUtf8Chars(result_chars);
}

string caesar_decrypt(const string& text, const string& key) {
    if (key.empty()) throw runtime_error("Ключ не может быть пустым!");
    
    vector<string> chars = AlphabetConstants::splitUtf8String(text);
    vector<string> result_chars;

    int eng_shift = -1;
    int rus_shift = -1;
    for (char k : key) {
        if (AlphabetConstants::isEnglishLetter(k)) {
            eng_shift = (toupper(k) - AlphabetConstants::ENGLISH_UPPERCASE_START) % AlphabetConstants::ENGLISH_ALPHABET_SIZE;
            if (eng_shift < 0) eng_shift += AlphabetConstants::ENGLISH_ALPHABET_SIZE;
            break;
        }
    }
    for (size_t i = 0; i < key.size();) {
        unsigned char c = key[i];
        string utf8_char;
        if (c < 0x80) {
            utf8_char = key.substr(i, 1);
            i++;
        } else if ((c & 0xE0) == 0xC0) {
            utf8_char = key.substr(i, 2);
            i += 2;
        } else if ((c & 0xF0) == 0xE0) {
            utf8_char = key.substr(i, 3);
            i += 3;
        } else if ((c & 0xF8) == 0xF0) {
            utf8_char = key.substr(i, 4);
            i += 4;
        } else {
            i++;
            continue;
        }
        if (AlphabetConstants::isRussianLetter(utf8_char)) {
            rus_shift = AlphabetConstants::getRussianPosition(utf8_char);
            break;
        }
    }
    if (eng_shift == -1) eng_shift = 1;
    if (rus_shift == -1) rus_shift = 1;
    
    for (const string& utf8_char : chars) {
        if (AlphabetConstants::isEnglishLetter(utf8_char[0])) {
            char c = utf8_char[0];
            char base = AlphabetConstants::isEnglishUppercase(c) ? AlphabetConstants::ENGLISH_UPPERCASE_START : AlphabetConstants::ENGLISH_LOWERCASE_START;
            char decrypted = (c - base - eng_shift + AlphabetConstants::ENGLISH_ALPHABET_SIZE) % AlphabetConstants::ENGLISH_ALPHABET_SIZE + base;
            result_chars.push_back(string(1, decrypted));
        } else if (AlphabetConstants::isRussianLetter(utf8_char)) {
            int pos = AlphabetConstants::getRussianPosition(utf8_char);
            if (pos >= 0) {
                int new_pos = (pos - rus_shift + AlphabetConstants::RUSSIAN_ALPHABET_SIZE) % AlphabetConstants::RUSSIAN_ALPHABET_SIZE;
                string new_char = AlphabetConstants::getRussianChar(new_pos);
                if (AlphabetConstants::isRussianLowercase(utf8_char)) {
                    new_char = AlphabetConstants::toRussianLowercase(new_char);
                }
                result_chars.push_back(new_char);
            } else {
                result_chars.push_back(utf8_char);
            }
        } else {
            result_chars.push_back(utf8_char);
        }
    }
    return AlphabetConstants::joinUtf8Chars(result_chars);
}

string process_file_auto_caesar(const string& input_path, const string& output_path, const string& key, bool encrypt) {
    if (key.empty()) throw runtime_error("Ключ не может быть пустым!");
    // Определяем тип файла по расширению
    filesystem::path path(input_path);
    string extension = path.extension().string();
    transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    vector<string> text_extensions = {".txt", ".cpp", ".h", ".c", ".hpp", ".py", ".js", ".html", ".css", ".json", ".xml", ".md", ".log"};
    bool is_text_file = false;
    for (const auto& ext : text_extensions) {
        if (extension == ext) {
            is_text_file = true;
            break;
        }
    }
    if (is_text_file) {
        ifstream input_file(input_path);
        if (!input_file.is_open()) throw runtime_error("Не удалось открыть входной файл: " + input_path);
        string content((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());
        input_file.close();
        string processed_content = encrypt ? caesar_encrypt(content, key) : caesar_decrypt(content, key);
        ofstream output_file(output_path);
        if (!output_file.is_open()) throw runtime_error("Не удалось создать выходной файл: " + output_path);
        output_file << processed_content;
        output_file.close();
        return string("Текстовый файл ") + (encrypt ? "зашифрован" : "расшифрован") + " шифром Цезаря";
    } else {
        ifstream input_file(input_path, ios::binary);
        if (!input_file.is_open()) throw runtime_error("Не удалось открыть входной файл: " + input_path);
        vector<char> buffer((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());
        input_file.close();
        unsigned char shift = static_cast<unsigned char>(key[0]);
        if (shift == 0) cerr << " Внимание: сдвиг = 0, шифрование не изменит бинарные данные!\n";
        for (char& byte : buffer) {
            if (encrypt)
                byte = static_cast<unsigned char>(byte + shift);
            else
                byte = static_cast<unsigned char>(byte - shift);
        }
        ofstream output_file(output_path, ios::binary);
        if (!output_file.is_open()) throw runtime_error("Не удалось создать выходной файл: " + output_path);
        output_file.write(buffer.data(), buffer.size());
        output_file.close();
        return string("Бинарный файл ") + (encrypt ? "зашифрован" : "расшифрован") + " шифром Цезаря";
    }
}

string generate_key_caesar(int length) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis('B', 'Z'); // Исключаем 'A' чтобы shift не был 0
    string key;
    for (int i = 0; i < length; ++i) {
        key += static_cast<char>(dis(gen));
    }
    return key;
} 
