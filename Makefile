CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS = -ldl

# Директории
CAESAR_DIR = caesar
TARABAR_DIR = tarabar
PORTA_DIR = porta

# Исходные файлы
MAIN_SRC = main.cpp
CAESAR_SRC = $(CAESAR_DIR)/caesar.cpp
TARABAR_SRC = $(TARABAR_DIR)/tarabar.cpp
PORTA_SRC = $(PORTA_DIR)/porta.cpp

# Объектные файлы
MAIN_OBJ = main.o
CAESAR_OBJ = $(CAESAR_DIR)/caesar.o
TARABAR_OBJ = $(TARABAR_DIR)/tarabar.o
PORTA_OBJ = $(PORTA_DIR)/porta.o

# Динамические библиотеки
CAESAR_LIB = $(CAESAR_DIR)/libcaesar.so
TARABAR_LIB = $(TARABAR_DIR)/libtarabar.so
PORTA_LIB = $(PORTA_DIR)/libporta.so

# Исполняемый файл
TARGET = favourite_app

# Цель по умолчанию
all: $(TARGET) $(CAESAR_LIB) $(TARABAR_LIB) $(PORTA_LIB)

# Основная программа
$(TARGET): $(MAIN_OBJ) $(CAESAR_OBJ) $(TARABAR_OBJ) $(PORTA_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Объектные файлы
$(MAIN_OBJ): $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(CAESAR_OBJ): $(CAESAR_SRC)
	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@

$(TARABAR_OBJ): $(TARABAR_SRC)
	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@

$(PORTA_OBJ): $(PORTA_SRC)
	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@

# Динамические библиотеки
$(CAESAR_LIB): $(CAESAR_OBJ)
	$(CXX) $(CXXFLAGS) -shared -o $@ $<

$(TARABAR_LIB): $(TARABAR_OBJ)
	$(CXX) $(CXXFLAGS) -shared -o $@ $<

$(PORTA_LIB): $(PORTA_OBJ)
	$(CXX) $(CXXFLAGS) -shared -o $@ $<

# Очистка
clean:
	rm -f $(MAIN_OBJ) $(CAESAR_OBJ) $(TARABAR_OBJ) $(PORTA_OBJ)
	rm -f $(CAESAR_LIB) $(TARABAR_LIB) $(PORTA_LIB)
	rm -f $(TARGET)

# Пересборка
rebuild: clean all

# Запуск
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run 
