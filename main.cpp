#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <memory>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <filesystem>

// Подключаем новые шифры
#include "caesar/caesar.h"
#include "tarabar/tarabar.h"
#include "porta/porta.h"

using namespace std;

// Глобальная переменная для хранения последнего сгенерированного ключа
string last_generated_key = "";

void setup_windows_console() {
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif
}

void clear_cin() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int get_menu_choice(int min, int max) {
    int choice;
    while (true) {
        cout << ">> ";
        cin >> choice;
        if (!cin || choice < min || choice > max) {
            cout << "Ошибка: Пожалуйста, введите число между " << min << " и " << max << ".\n";
            cin.clear();
            clear_cin();
        } else {
            clear_cin();
            return choice;
        }
    }
}

string get_output_path(const string& input_path, const string& operation) {
    string output_path;
    
    cout << "Куда сохранить результат? (введите путь или ./ для текущей папки): ";
    getline(cin, output_path);
    
    // Если пользователь ввел ./ или пустую строку, используем текущую папку
    if (output_path.empty() || output_path == "./") {
        filesystem::path input_file(input_path);
        string filename = input_file.filename().string();
        string extension = input_file.extension().string();
        string base_name = input_file.stem().string();
        
        if (operation == "encrypt") {
            output_path = base_name + "_encrypted" + extension;
        } else if (operation == "decrypt") {
            output_path = base_name + "_decrypted" + extension;
        } else {
            output_path = base_name + "_processed" + extension;
        }
        
        cout << "Файл будет сохранен как: " << output_path << endl;
    }
    
    return output_path;
}

string get_key_choice() {
    string key;
    
    if (!last_generated_key.empty()) {
        cout << "Выберите способ получения ключа:\n";
        cout << "1. Использовать ранее сгенерированный ключ: " << last_generated_key << "\n";
        cout << "2. Сгенерировать новый ключ\n";
        cout << "3. Ввести ключ вручную\n";
        
        int choice = get_menu_choice(1, 3);
        
        switch (choice) {
            case 1:
                return last_generated_key;
            case 2:
                key = generate_key_caesar(16);
                last_generated_key = key;
                cout << "Сгенерирован новый ключ: " << key << endl;
                return key;
            case 3:
                cout << "Введите ключ: ";
                getline(cin, key);
                return key;
        }
    } else {
        cout << "Выберите способ получения ключа:\n";
        cout << "1. Сгенерировать новый ключ\n";
        cout << "2. Ввести ключ вручную\n";
        
        int choice = get_menu_choice(1, 2);
        
        switch (choice) {
            case 1:
                key = generate_key_caesar(16);
                last_generated_key = key;
                cout << "Сгенерирован новый ключ: " << key << endl;
                return key;
            case 2:
                cout << "Введите ключ: ";
                getline(cin, key);
                return key;
        }
    }
    
    return "";
}

string get_key_choice_porta() {
    string key;
    
    if (!last_generated_key.empty()) {
        cout << "Выберите способ получения ключа:\n";
        cout << "1. Использовать ранее сгенерированный ключ: " << last_generated_key << "\n";
        cout << "2. Сгенерировать новый ключ\n";
        cout << "3. Ввести ключ вручную\n";
        
        int choice = get_menu_choice(1, 3);
        
        switch (choice) {
            case 1:
                return last_generated_key;
            case 2:
                key = generate_key_porta(16);
                last_generated_key = key;
                cout << "Сгенерирован новый ключ: " << key << endl;
                return key;
            case 3:
                cout << "Введите ключ: ";
                getline(cin, key);
                return key;
        }
    } else {
        cout << "Выберите способ получения ключа:\n";
        cout << "1. Сгенерировать новый ключ\n";
        cout << "2. Ввести ключ вручную\n";
        
        int choice = get_menu_choice(1, 2);
        
        switch (choice) {
            case 1:
                key = generate_key_porta(16);
                last_generated_key = key;
                cout << "Сгенерирован новый ключ: " << key << endl;
                return key;
            case 2:
                cout << "⌨️  Введите ключ: ";
                getline(cin, key);
                return key;
        }
    }
    
    return "";
}

void run_caesar_menu() {
    cout << "\n--- Меню шифра Цезаря ---\n";
    cout << "1. Зашифровать файл\n";
    cout << "2. Расшифровать файл\n";
    cout << "3. Назад в главное меню\n";

    int choice = get_menu_choice(1, 3);
    if (choice == 3) return;

    string input_path, output_path;

    cout << "Введите путь к файлу: ";
    getline(cin, input_path);
    
    // Проверяем существование файла
    if (!filesystem::exists(input_path)) {
        cout << "Ошибка: Файл не найден!" << endl;
        return;
    }
    
    output_path = get_output_path(input_path, choice == 1 ? "encrypt" : "decrypt");

    string key = get_key_choice();
    if (key.empty()) {
        cout << "Ошибка: Ключ не может быть пустым.\n";
        return;
    }

    try {
        cout << "Обрабатываем файл..." << endl;
        process_file_auto_caesar(input_path, output_path, key, choice == 1);
        cout << "Файл успешно " << (choice == 1 ? "зашифрован" : "расшифрован") << "!" << endl;
        cout << "Результат сохранен в: " << output_path << endl;
    } catch (const exception& e) {
        cout << "Ошибка: " << e.what() << endl;
    }
}

void run_tarabar_menu() {
    cout << "\n--- Меню Тарабарского шифра ---\n";
    cout << "1. Обработать файл\n";
    cout << "2. Назад в главное меню\n";

    int choice = get_menu_choice(1, 2);
    if (choice == 2) return;

    string input_path, output_path;

    cout << "Введите путь к файлу: ";
    getline(cin, input_path);
    
    // Проверяем существование файла
    if (!filesystem::exists(input_path)) {
        cout << "Ошибка: Файл не найден!" << endl;
        return;
    }
    
    output_path = get_output_path(input_path, "process");

    try {
        cout << "Обрабатываем файл..." << endl;
        process_file_auto_tarabar(input_path, output_path);
        cout << "Файл успешно обработан!" << endl;
        cout << "Результат сохранен в: " << output_path << endl;
    } catch (const exception& e) {
        cout << "Ошибка: " << e.what() << endl;
    }
}

void run_porta_menu() {
    cout << "\n--- Меню шифра Порта ---\n";
    cout << "1. Зашифровать файл\n";
    cout << "2. Расшифровать файл\n";
    cout << "3. Назад в главное меню\n";

    int choice = get_menu_choice(1, 3);
    if (choice == 3) return;

    string input_path, output_path;

    cout << "Введите путь к файлу: ";
    getline(cin, input_path);
    
    // Проверяем существование файла
    if (!filesystem::exists(input_path)) {
        cout << "Ошибка: Файл не найден!" << endl;
        return;
    }
    
    output_path = get_output_path(input_path, choice == 1 ? "encrypt" : "decrypt");

    string key = get_key_choice_porta();
    if (key.empty()) {
        cout << "Ошибка: Ключ не может быть пустым.\n";
        return;
    }

    try {
        cout << "Обрабатываем файл..." << endl;
        process_file_auto_porta(input_path, output_path, key, choice == 1);
        cout << "Файл успешно " << (choice == 1 ? "зашифрован" : "расшифрован") << "!" << endl;
        cout << "Результат сохранен в: " << output_path << endl;
    } catch (const exception& e) {
        cout << "Ошибка: " << e.what() << endl;
    }
}

int main() {
    setup_windows_console();

    while (true) {
        cout << "\n====== Favourie crypto-app ====== \n";
        cout << "Выберите шифр:\n";
        cout << "1. Цезарь\n";
        cout << "2. Тарабарский\n";
        cout << "3. Порта\n";
        cout << "4. Выход\n";

        int choice = get_menu_choice(1, 4);

        switch (choice) {
            case 1:
                run_caesar_menu();
                break;
            case 2:
                run_tarabar_menu();
                break;
            case 3:
                run_porta_menu();
                break;
            case 4:
                cout << "👋 Выход из программы." << endl;
                return 0;
        }
    }

    return 0;
}
