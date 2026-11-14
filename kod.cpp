#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>

// Структура для хранения информации о видеокассете
struct Videocassette
{
    char title[100];
    char director[100];
    int duration;
    float price;
};

// Ввод одной видеокассеты
int inputCassette(Videocassette &v)
{
    printf("Название фильма: ");
    if (fgets(v.title, 100, stdin) == NULL)
        return 0;

    v.title[strcspn(v.title, "\n")] = '\0';

    printf("Режиссер: ");
    if (fgets(v.director, 100, stdin) == NULL)
        return 0;

    v.director[strcspn(v.director, "\n")] = '\0';

    printf("Продолжительность (мин): ");
    if (scanf("%d", &v.duration) != 1)
        return 0;

    printf("Цена: ");
    if (scanf("%f", &v.price) != 1)
        return 0;

    getchar();

    return 1;
}

// Формирование файла
int createFile(int n)
{
    FILE *f;
    Videocassette v;

    f = fopen("videocassettes.dat", "wb");

    if (f == NULL)
    {
        printf("Ошибка открытия файла!\n");
        return 0;
    }

    for (int i = 0; i < n; i++)
    {
        printf("\nВидеокассета №%d\n", i + 1);

        if (!inputCassette(v))
        {
            printf("Ошибка ввода данных!\n");
            fclose(f);
            return 0;
        }

        if (fwrite(&v, sizeof(Videocassette), 1, f) != 1)
        {
            printf("Ошибка записи в файл!\n");
            fclose(f);
            return 0;
        }
    }

    fclose(f);

    return 1;
}

// Вывод содержимого файла
int printFile()
{
    FILE *f;
    Videocassette v;
    int count = 0;

    f = fopen("videocassettes.dat", "rb");

    if (f == NULL)
    {
        printf("Ошибка открытия файла!\n");
        return 0;
    }

    printf("\nСодержимое файла:\n");

    while (fread(&v, sizeof(Videocassette), 1, f) == 1)
    {
        count++;

        printf("\nЗапись №%d\n", count);
        printf("Название: %s\n", v.title);
        printf("Режиссер: %s\n", v.director);
        printf("Продолжительность: %d мин.\n", v.duration);
        printf("Цена: %.2f\n", v.price);
    }

    if (ferror(f))
    {
        printf("Ошибка чтения файла!\n");
        fclose(f);
        return 0;
    }

    if (count == 0)
        printf("Файл пустой.\n");

    fclose(f);

    return 1;
}

// Поиск количества записей с ценой выше заданной
int findExpensive(float limit)
{
    FILE *f;
    Videocassette v;
    int count = 0;

    f = fopen("videocassettes.dat", "rb");

    if (f == NULL)
    {
        printf("Ошибка открытия файла!\n");
        return -1;
    }

    while (fread(&v, sizeof(Videocassette), 1, f) == 1)
    {
        if (v.price > limit)
            count++;
    }

    if (ferror(f))
    {
        printf("Ошибка чтения файла!\n");
        fclose(f);
        return -1;
    }

    fclose(f);

    return count;
}

// Удаление записей с ценой выше заданной
int deleteExpensive(float limit)
{
    FILE *f, *temp;
    Videocassette v;

    f = fopen("videocassettes.dat", "rb");

    if (f == NULL)
    {
        printf("Ошибка открытия исходного файла!\n");
        return 0;
    }

    temp = fopen("temp.dat", "wb");

    if (temp == NULL)
    {
        printf("Ошибка создания временного файла!\n");
        fclose(f);
        return 0;
    }

    while (fread(&v, sizeof(Videocassette), 1, f) == 1)
    {
        if (v.price <= limit)
        {
            if (fwrite(&v, sizeof(Videocassette), 1, temp) != 1)
            {
                printf("Ошибка записи во временный файл!\n");
                fclose(f);
                fclose(temp);
                return 0;
            }
        }
    }

    if (ferror(f))
    {
        printf("Ошибка чтения исходного файла!\n");
        fclose(f);
        fclose(temp);
        return 0;
    }

    fclose(f);
    fclose(temp);

    if (remove("videocassettes.dat") != 0)
    {
        printf("Ошибка удаления исходного файла!\n");
        return 0;
    }

    if (rename("temp.dat", "videocassettes.dat") != 0)
    {
        printf("Ошибка переименования временного файла!
\n");
        return 0;
    }

    return 1;
}

// Добавление трех записей в конец файла
int addThree()
{
    FILE *f;
    Videocassette v;

    f = fopen("videocassettes.dat", "ab");

    if (f == NULL)
    {
        printf("Ошибка открытия файла!\n");
        return 0;
    }

    for (int i = 0; i < 3; i++)
    {
        printf("\nНовая видеокассета №%d\n", i + 1);

        if (!inputCassette(v))
        {
            printf("Ошибка ввода данных!\n");
            fclose(f);
            return 0;
        }

        if (fwrite(&v, sizeof(Videocassette), 1, f) != 1)
        {
            printf("Ошибка записи!\n");
            fclose(f);
            return 0;
        }
    }

    fclose(f);

    return 1;
}

// Главная функция
int main()
{
    setlocale(LC_ALL, "");

    int n;
    float limit;
    int deleted;

    printf("Лабораторная работа №8\n");
    printf("Вариант 11. Видеокассеты\n");

    printf("\nВведите количество видеокассет (не менее 5): ");

    if (scanf("%d", &n) != 1 || n < 5)
    {
        printf("Неверное количество записей!\n");
        return 0;
    }

    getchar();

    // Создание файла
    if (!createFile(n))
        return 0;

    // Вывод исходного файла
    printFile();

    // Ввод предельной цены
    printf("\nВведите цену для удаления записей: ");

    if (scanf("%f", &limit) != 1)
    {
        printf("Неверное значение цены!\n");
        return 0;
    }

    // Поиск записей для удаления
    deleted = findExpensive(limit);

    if (deleted < 0)
        return 0;

    printf("\nНайдено записей для удаления: %d\n", deleted);

    // Удаление записей
    if (!deleteExpensive(limit))
        return 0;

    printf("\nФайл после удаления записей:\n");
    printFile();

    // Добавление трех записей
    printf("\nДобавление трех новых видеокассет:\n");

    if (!addThree())
        return 0;

    // Итоговый вывод
    printf("\nИтоговое содержимое файла:\n");
    printFile();

    return 0;
}