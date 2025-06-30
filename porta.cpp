#include "tarabar.h"
#include "../alphabet_constants.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>
#include <filesystem>

using namespace std;

// Тарабарский шифр - замена каждой буквы на противоположную в алфавите
string tarabar_encrypt(const string& text) {
    // Разбиваем текст на UTF-8 символы
    vector<string> chars = AlphabetConstants::splitUtf8String(text);
    vector<string> result_chars;
    
    for (const string& utf8_char : chars) {
        if (AlphabetConstants::isEnglishLetter(utf8_char[0])) {
            // Находим противоположную букву в английском алфавите
            string opposite = AlphabetConstants::getOppositeEnglishChar(utf8_char[0]);
            char result_char = AlphabetConstants::isEnglishUppercase(utf8_char[0]) ? opposite[0] : AlphabetConstants::toEnglishLowercase(opposite[0]);
            result_chars.push_back(string(1, result_char));
        } else if (AlphabetConstants::isRussianLetter(utf8_char)) {
            // Находим противоположную букву в русском алфавите
            string opposite = AlphabetConstants::getOppositeRussianChar(utf8_char);
            result_chars.push_back(opposite);
        } else {
            // Оставляем символы, не являющиеся буквами
            result_chars.push_back(utf8_char);
        }
    }
    
    return AlphabetConstants::joinUtf8Chars(result_chars);
}

string tarabar_decrypt(const string& text) {
    // Тарабарский шифр симметричен - шифрование и расшифрование одинаковы
    return tarabar_encrypt(text);
}

string process_file_auto_tarabar(const string& input_path, const string& output_path) {
    // Определяем тип файла по расширению
    filesystem::path path(input_path);
    string extension = path.extension().string();
    transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    
    // Список текстовых расширений
    vector<string> text_extensions = {".txt", ".cpp", ".h", ".c", ".hpp", ".py", ".js", ".html", ".css", ".json", ".xml", ".md", ".log"};
    
    bool is_text_file = false;
    for (const auto& ext : text_extensions) {
        if (extension == ext) {
            is_text_file = true;
            break;
        }
    }
    
    if (is_text_file) {
        // Обрабатываем как текстовый файл
        ifstream input_file(input_path);
        if (!input_file.is_open()) {
            throw runtime_error("Не удалось открыть входной файл: " + input_path);
        }
        
        string content((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());
        input_file.close();
        
        string processed_content = tarabar_encrypt(content);
        
        ofstream output_file(output_path);
        if (!output_file.is_open()) {
            throw runtime_error("Не удалось создать выходной файл: " + output_path);
        }
        
        output_file << processed_content;
        output_file.close();
        
        return "Текстовый файл обработан тарабарским шифром";
    } else {
        // Обрабатываем как бинарный файл
        ifstream input_file(input_path, ios::binary);
        if (!input_file.is_open()) {
            throw runtime_error("Не удалось открыть входной файл: " + input_path);
        }
        
        vector<char> buffer((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());
        input_file.close();
        
        // Применяем тарабарский шифр к каждому байту
        for (char& byte : buffer) {
            byte = ~byte; // Инвертируем биты (аналог тарабарского шифра для бинарных данных)
        }
        
        ofstream output_file(output_path, ios::binary);
        if (!output_file.is_open()) {
            throw runtime_error("Не удалось создать выходной файл: " + output_path);
        }
        
        output_file.write(buffer.data(), buffer.size());
        output_file.close();
        
        return "Бинарный файл обработан тарабарским шифром";
    }
} 
