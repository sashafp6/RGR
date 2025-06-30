#ifndef ALPHABET_CONSTANTS_H
#define ALPHABET_CONSTANTS_H

#include <string>
#include <vector>
#include <locale>
#include <codecvt>

using namespace std;

namespace AlphabetConstants {
    // ASCII диапазоны для английских букв
    const int ENGLISH_UPPERCASE_START = 'A';
    const int ENGLISH_UPPERCASE_END = 'Z';
    const int ENGLISH_LOWERCASE_START = 'a';
    const int ENGLISH_LOWERCASE_END = 'z';
    const int ENGLISH_ALPHABET_SIZE = 26;
    
    // ASCII диапазоны для русских букв (Windows-1251 / CP1251)
    const int RUSSIAN_UPPERCASE_START = 0xC0; // А
    const int RUSSIAN_UPPERCASE_END = 0xDF;   // Я
    const int RUSSIAN_LOWERCASE_START = 0xE0; // а
    const int RUSSIAN_LOWERCASE_END = 0xFF;   // я
    const int RUSSIAN_ALPHABET_SIZE = 32;     // без Ё
    
    // UTF-8 строки для русского алфавита (без Ё)
    const string RUSSIAN_UPPERCASE = "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    const string RUSSIAN_LOWERCASE = "абвгдежзийклмнопрстуфхцчшщъыьэюя";
    const int RUSSIAN_UTF8_ALPHABET_SIZE = 32; // без Ё
    
    // Base64 алфавит
    const string BASE64_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    
    // Функции для генерации русского алфавита в ASCII
    inline string getRussianUppercaseASCII() {
        string result;
        for (int i = RUSSIAN_UPPERCASE_START; i <= RUSSIAN_UPPERCASE_END; i++) {
            result += static_cast<char>(i);
        }
        return result;
    }
    
    inline string getRussianLowercaseASCII() {
        string result;
        for (int i = RUSSIAN_LOWERCASE_START; i <= RUSSIAN_LOWERCASE_END; i++) {
            result += static_cast<char>(i);
        }
        return result;
    }
    
    // Функция для разбиения UTF-8 строки на символы
    inline vector<string> splitUtf8String(const string& str) {
        vector<string> result;
        for (size_t i = 0; i < str.length();) {
            unsigned char c = static_cast<unsigned char>(str[i]);
            if (c < 0x80) {
                // ASCII символ
                result.push_back(str.substr(i, 1));
                i++;
            } else if ((c & 0xE0) == 0xC0) {
                // 2-байтовый UTF-8 символ
                result.push_back(str.substr(i, 2));
                i += 2;
            } else if ((c & 0xF0) == 0xE0) {
                // 3-байтовый UTF-8 символ
                result.push_back(str.substr(i, 3));
                i += 3;
            } else if ((c & 0xF8) == 0xF0) {
                // 4-байтовый UTF-8 символ
                result.push_back(str.substr(i, 4));
                i += 4;
            } else {
                // Некорректный UTF-8, пропускаем
                i++;
            }
        }
        return result;
    }
    
    // Функция для объединения UTF-8 символов в строку
    inline string joinUtf8Chars(const vector<string>& chars) {
        string result;
        for (const auto& ch : chars) {
            result += ch;
        }
        return result;
    }
    
    // Функции для проверки принадлежности к диапазонам
    inline bool isEnglishUppercase(char c) {
        return c >= ENGLISH_UPPERCASE_START && c <= ENGLISH_UPPERCASE_END;
    }
    
    inline bool isEnglishLowercase(char c) {
        return c >= ENGLISH_LOWERCASE_START && c <= ENGLISH_LOWERCASE_END;
    }
    
    inline bool isEnglishLetter(char c) {
        return isEnglishUppercase(c) || isEnglishLowercase(c);
    }
    
    // Функции для русских букв в ASCII (Windows-1251)
    inline bool isRussianUppercaseASCII(char c) {
        return c >= RUSSIAN_UPPERCASE_START && c <= RUSSIAN_UPPERCASE_END;
    }
    
    inline bool isRussianLowercaseASCII(char c) {
        return c >= RUSSIAN_LOWERCASE_START && c <= RUSSIAN_LOWERCASE_END;
    }
    
    inline bool isRussianLetterASCII(char c) {
        return isRussianUppercaseASCII(c) || isRussianLowercaseASCII(c);
    }
    
    inline char toRussianUppercaseASCII(char c) {
        if (isRussianLowercaseASCII(c)) {
            return c - (RUSSIAN_LOWERCASE_START - RUSSIAN_UPPERCASE_START);
        }
        return c;
    }
    
    inline char toRussianLowercaseASCII(char c) {
        if (isRussianUppercaseASCII(c)) {
            return c + (RUSSIAN_LOWERCASE_START - RUSSIAN_UPPERCASE_START);
        }
        return c;
    }
    
    inline int getRussianPositionASCII(char c) {
        char upper = toRussianUppercaseASCII(c);
        return upper - RUSSIAN_UPPERCASE_START;
    }
    
    inline char getRussianCharASCII(int position) {
        if (position >= 0 && position < RUSSIAN_ALPHABET_SIZE) {
            position = position % RUSSIAN_ALPHABET_SIZE;
            return RUSSIAN_UPPERCASE_START + position;
        }
        return 0;
    }
    
    inline char toEnglishUppercase(char c) {
        if (isEnglishLowercase(c)) {
            return c - (ENGLISH_LOWERCASE_START - ENGLISH_UPPERCASE_START);
        }
        return c;
    }
    
    inline char toEnglishLowercase(char c) {
        if (isEnglishUppercase(c)) {
            return c + (ENGLISH_LOWERCASE_START - ENGLISH_UPPERCASE_START);
        }
        return c;
    }
    
    inline int getEnglishPosition(char c) {
        char upper = toEnglishUppercase(c);
        return upper - ENGLISH_UPPERCASE_START;
    }
    
    inline char getEnglishChar(int position) {
        return ENGLISH_UPPERCASE_START + (position % ENGLISH_ALPHABET_SIZE);
    }
    
    // Функции для русского алфавита (UTF-8)
    inline bool isRussianUppercase(const string& utf8_char) {
        // Проверяем, есть ли символ в строке русских заглавных букв
        vector<string> russian_chars = splitUtf8String(RUSSIAN_UPPERCASE);
        for (const auto& ch : russian_chars) {
            if (ch == utf8_char) return true;
        }
        return false;
    }
    
    inline bool isRussianLowercase(const string& utf8_char) {
        // Проверяем, есть ли символ в строке русских строчных букв
        vector<string> russian_chars = splitUtf8String(RUSSIAN_LOWERCASE);
        for (const auto& ch : russian_chars) {
            if (ch == utf8_char) return true;
        }
        return false;
    }
    
    inline bool isRussianLetter(const string& utf8_char) {
        return isRussianUppercase(utf8_char) || isRussianLowercase(utf8_char);
    }
    
    inline string toRussianUppercase(const string& utf8_char) {
        vector<string> lower_chars = splitUtf8String(RUSSIAN_LOWERCASE);
        vector<string> upper_chars = splitUtf8String(RUSSIAN_UPPERCASE);
        
        for (size_t i = 0; i < lower_chars.size() && i < upper_chars.size(); i++) {
            if (lower_chars[i] == utf8_char) {
                return upper_chars[i];
            }
        }
        return utf8_char;
    }
    
    inline string toRussianLowercase(const string& utf8_char) {
        vector<string> lower_chars = splitUtf8String(RUSSIAN_LOWERCASE);
        vector<string> upper_chars = splitUtf8String(RUSSIAN_UPPERCASE);
        
        for (size_t i = 0; i < upper_chars.size() && i < lower_chars.size(); i++) {
            if (upper_chars[i] == utf8_char) {
                return lower_chars[i];
            }
        }
        return utf8_char;
    }
    
    inline int getRussianPosition(const string& utf8_char) {
        string upper = toRussianUppercase(utf8_char);
        vector<string> upper_chars = splitUtf8String(RUSSIAN_UPPERCASE);
        
        for (size_t i = 0; i < upper_chars.size(); i++) {
            if (upper_chars[i] == upper) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }
    
    inline string getRussianChar(int position) {
        if (position >= 0 && position < RUSSIAN_UTF8_ALPHABET_SIZE) {
            position = position % RUSSIAN_UTF8_ALPHABET_SIZE;
            vector<string> upper_chars = splitUtf8String(RUSSIAN_UPPERCASE);
            if (position < static_cast<int>(upper_chars.size())) {
                return upper_chars[position];
            }
        }
        return "";
    }
    
    // Универсальные функции для работы с UTF-8
    inline bool isLetter(const string& utf8_char) {
        return isEnglishLetter(utf8_char[0]) || isRussianLetter(utf8_char);
    }
    
    inline string getOppositeEnglishChar(char c) {
        int pos = getEnglishPosition(c);
        int opposite_pos = ENGLISH_ALPHABET_SIZE - 1 - pos;
        return string(1, getEnglishChar(opposite_pos));
    }
    
    inline string getOppositeRussianChar(const string& utf8_char) {
        int pos = getRussianPosition(utf8_char);
        if (pos >= 0) {
            int opposite_pos = RUSSIAN_UTF8_ALPHABET_SIZE - 1 - pos;
            string opposite = getRussianChar(opposite_pos);
            // Сохраняем оригинальный регистр
            return isRussianLowercase(utf8_char) ? toRussianLowercase(opposite) : opposite;
        }
        return utf8_char;
    }
    
    // Функция для получения противоположной русской буквы в ASCII
    inline char getOppositeRussianCharASCII(char c) {
        int pos = getRussianPositionASCII(c);
        if (pos >= 0) {
            int opposite_pos = RUSSIAN_ALPHABET_SIZE - 1 - pos;
            char opposite = getRussianCharASCII(opposite_pos);
            // Сохраняем оригинальный регистр
            return isRussianLowercaseASCII(c) ? toRussianLowercaseASCII(opposite) : opposite;
        }
        return c;
    }
}

#endif // ALPHABET_CONSTANTS_H 
