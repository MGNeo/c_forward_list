#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "c_forward_list.h"

// Проверки возвращаемых значений не выполняются для упрощения.

// Функция печати содержимого узла.
void print_func_f(void *const _data)
{
    if (_data == NULL) return;

    const float data = *((float*)_data);
    printf("%f\n", data);
    return;
}

// Функция удаления данных узла.
void del_func_f(void *const _data)
{
    if (_data == NULL) return;

    free(_data);
    return;
}

// Функция определения необходимости удаления узла по его данным.
size_t pred_func_f(const void *const _data)
{
    if (_data == NULL) return 0;

    const float data = *((float*)_data);
    if (data > 5.f)
    {
        return 1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    // Создание односвязного списка.
    c_forward_list *forward_list = c_forward_list_create();

    // Добавление в конец десяти элементов.
    const size_t insert_count = 10;
    float *data;
    for (size_t i = 0; i < insert_count; ++i)
    {
        data = (float*)malloc(sizeof(float));
        *data = i * 1.1f;
        c_forward_list_push_back(forward_list, data);
    }

    // Вывод содержимого односвязного списка.
    c_forward_list_for_each(forward_list, print_func_f);
    printf("\n");

    // Удаление нескольких элементов с заданными индексами,
    // индексы хранятся в массиве в неупорядоченном виде.
    const size_t indexes_count = 3;
    size_t indexes[indexes_count];
    indexes[0] = 3;
    indexes[1] = 9;
    indexes[2] = 1;
    c_forward_list_erase_few(forward_list, indexes, indexes_count, del_func_f);

    // Вывод содержимого односвязного списка.
    c_forward_list_for_each(forward_list, print_func_f);
    printf("\n");

    // Удаление узлов, чьи данные удовлетворяют условиям.
    c_forward_list_remove_few(forward_list, pred_func_f, del_func_f);

    // Вывод содержимого односвязного списка.
    c_forward_list_for_each(forward_list, print_func_f);
    printf("\n");

    // Удаление односвязного списка.
    c_forward_list_delete(forward_list, del_func_f);

    getchar();
    return 0;
}
