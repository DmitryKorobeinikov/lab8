#include <iostream>
#include <iomanip>
#include <cstdio>   // Для работы с файлами
#include <cstring>
#include <windows.h>

using namespace std;

// Структура "Видеокассета"
struct Videotape {
    char title[50];       // Название фильма
    char director[50];    // Режиссер
    int duration;         // Продолжительность (мин)
    double price;         // Цена
};

// Функция для ввода одной структуры
void inputVideotape(Videotape &v) {
    cout << "Название фильма: ";
    cin.ignore(); 
    cin.getline(v.title, 50);
    cout << "Режиссер: ";
    cin.getline(v.director, 50);
    cout << "Продолжительность (мин): ";
    cin >> v.duration;
    cout << "Цена: ";
    cin >> v.price;
}

// 1. Функция формирования файла
void createFile(const char* filename, int count) {
    FILE* f = fopen(filename, "wb");
    if (!f) {
        cout << "Ошибка: не удалось открыть файл для записи!\n";
        return;
    }
    Videotape v;
    for (int i = 0; i < count; i++) {
        cout << "\n--- Ввод записи №" << i + 1 << " ---" << endl;
        inputVideotape(v);
        fwrite(&v, sizeof(Videotape), 1, f);
    }
    fclose(f);
    cout << "Файл успешно сформирован.\n";
}

// 2. Функция печати файла
void printFile(const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) {
        cout << "Ошибка: не удалось открыть файл для чтения!\n";
        return;
    }
    Videotape v;
    cout << "\n" << string(75, '-') << endl;
    cout << left << setw(20) << "Название" << setw(20) << "Режиссер" 
         << setw(15) << "Длительность" << setw(10) << "Цена" << endl;
    cout << string(75, '-') << endl;
    
    while (fread(&v, sizeof(Videotape), 1, f) == 1) {
        cout << left << setw(20) << v.title 
             << setw(20) << v.director 
             << setw(15) << v.duration 
             << setw(10) << v.price << endl;
    }
    cout << string(75, '-') << "\n";
    fclose(f);
}

// 3. Функция удаления элементов с ценой выше заданной
void deleteByPrice(const char* filename, double maxPrice) {
    FILE* f = fopen(filename, "rb");
    FILE* temp = fopen("temp.dat", "wb");
    if (!f || !temp) {
        cout << "Ошибка открытия файлов при удалении!\n";
        if (f) fclose(f);
        if (temp) fclose(temp);
        return;
    }

    Videotape v;
    int deletedCount = 0;
    // Читаем исходный файл
    while (fread(&v, sizeof(Videotape), 1, f) == 1) {
        if (v.price > maxPrice) {
            deletedCount++; // Пропускаем запись (удаляем)
        } else {
            fwrite(&v, sizeof(Videotape), 1, temp); // Сохраняем
        }
    }
    fclose(f);
    fclose(temp);

    // Заменяем старый файл новым
    remove(filename);
    rename("temp.dat", filename);
    cout << "Удалено записей: " << deletedCount << "\n";
}

// 4. Функция добавления элементов в конец
void appendToFile(const char* filename, int count) {
    FILE* f = fopen(filename, "ab"); // "ab" - Append Binary
    if (!f) {
        cout << "Ошибка: не удалось открыть файл для добавления!\n";
        return;
    }
    Videotape v;
    for (int i = 0; i < count; i++) {
        cout << "\n--- Добавление записи №" << i + 1 << " ---" << endl;
        inputVideotape(v);
        fwrite(&v, sizeof(Videotape), 1, f);
    }
    fclose(f);
    cout << "Добавление завершено.\n";
}

int main() {
    SetConsoleOutputCP(65001); // UTF-8 для вывода
    SetConsoleCP(65001);       // 
    const char* filename = "videotapes.dat";

    // 1. Формирование файла (создадим 5 записей)
    cout << "ФОРМИРОВАНИЕ ИСХОДНОГО ФАЙЛА" << endl;
    createFile(filename, 5);

    // 2. Печать исходного файла
    cout << "\nИСХОДНОЕ СОДЕРЖИМОЕ ФАЙЛА:" << endl;
    printFile(filename);

    // 3. Удаление элементов с ценой выше заданной
    double maxPrice;
    cout << "\nВведите максимальную цену (записи с ценой выше будут удалены): ";
    cin >> maxPrice;
    
    deleteByPrice(filename, maxPrice);
    cout << "\nФАЙЛ ПОСЛЕ УДАЛЕНИЯ:" << endl;
    printFile(filename);

    // 4. Добавление 3 элементов в конец
    cout << "\nДОБАВЛЕНИЕ 3 ЭЛЕМЕНТОВ В КОНЕЦ" << endl;
    appendToFile(filename, 3);

    // 5. Итоговая печать
    cout << "\nИТОГОВОЕ СОДЕРЖИМОЕ ФАЙЛА:" << endl;
    printFile(filename);

    return 0;
}