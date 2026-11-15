#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <windows.h>

using namespace std;

// ---------- Описание типа данных ----------
struct Video {
    char name[40];       // название фильма
    char director[40];   // режиссёр
    int  duration;       // продолжительность (мин)
    float price;         // цена
};

const char* FILENAME = "video.dat";
const char* TMPNAME  = "tmp.dat";

// ---------- Ввод одной записи ----------
void input_video(Video &v) {
    cout << "  Название фильма : "; cin >> v.name;
    cout << "  Режиссёр        : "; cin >> v.director;
    cout << "  Продолжительность (мин): "; cin >> v.duration;
    cout << "  Цена            : "; cin >> v.price;
}

// ---------- Печать одной записи ----------
void print_video(const Video &v) {
    cout << "  " << v.name << " | " << v.director
         << " | " << v.duration << " мин | " << v.price << " руб.\n";
}

// ---------- Формирование файла ----------
void create_file(const char* fname, int n) {
    FILE* fp = fopen(fname, "wb");
    if (!fp) { perror("Ошибка открытия файла для записи"); exit(1); }

    for (int i = 0; i < n; ++i) {
        cout << "Запись #" << (i + 1) << ":\n";
        Video v;
        input_video(v);
        if (fwrite(&v, sizeof(Video), 1, fp) != 1) {
            perror("Ошибка записи"); exit(2);
        }
    }
    fclose(fp);
}

// ---------- Печать содержимого файла ----------
void print_file(const char* fname) {
    FILE* fp = fopen(fname, "rb");
    if (!fp) { perror("Ошибка открытия файла для чтения"); exit(3); }

    Video v;
    cout << "--------------------------------------------------------\n";
    cout << " Название          | Режиссёр        | Мин | Цена\n";
    cout << "--------------------------------------------------------\n";
    while (fread(&v, sizeof(Video), 1, fp) == 1) {
        print_video(v);
    }
    cout << "--------------------------------------------------------\n";
    fclose(fp);
}

// ---------- Удаление записей с price > P_max ----------
void delete_expensive(const char* src, const char* tmp, float p_max) {
    FILE* fin  = fopen(src, "rb");
    if (!fin) { perror("Ошибка открытия исходного файла"); exit(4); }

    FILE* fout = fopen(tmp, "wb");
    if (!fout) { perror("Ошибка открытия временного файла"); fclose(fin); exit(5); }

    Video v;
    int kept = 0, removed = 0;
    while (fread(&v, sizeof(Video), 1, fin) == 1) {
        if (v.price <= p_max) {
            fwrite(&v, sizeof(Video), 1, fout);
            ++kept;
        } else {
            ++removed;
        }
    }
    fclose(fin);
    fclose(fout);

    if (remove(src) != 0) { perror("Ошибка удаления исходного файла"); exit(6); }
    if (rename(tmp, src) != 0) { perror("Ошибка переименования"); exit(7); }

    cout << "Удалено записей: " << removed
         << ", оставлено: " << kept << "\n";
}

// ---------- Добавление k записей в конец ----------
void add_records(const char* fname, int k) {
    FILE* fp = fopen(fname, "ab");
    if (!fp) { perror("Ошибка открытия файла для добавления"); exit(8); }

    for (int i = 0; i < k; ++i) {
        cout << "Новая запись #" << (i + 1) << ":\n";
        Video v;
        input_video(v);
        fwrite(&v, sizeof(Video), 1, fp);
    }
    fclose(fp);
}

// ---------- Главная функция ----------
int main() {
    SetConsoleOutputCP(65001);

    int n;
    cout << "Сколько записей внести в файл? "; cin >> n;
    create_file(FILENAME, n);

    cout << "\n=== Исходный файл ===\n";
    print_file(FILENAME);

    float p_max;
    cout << "\nУдалить записи с ценой выше: "; cin >> p_max;
    delete_expensive(FILENAME, TMPNAME, p_max);

    cout << "\n=== Файл после удаления ===\n";
    print_file(FILENAME);

    cout << "\nДобавляем 3 записи в конец файла:\n";
    add_records(FILENAME, 3);

    cout << "\n=== Файл после добавления ===\n";
    print_file(FILENAME);

    return 0;
}