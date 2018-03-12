#include <stdio.h>
#include <stdlib.h>
#include "c_forward_list.h"

void print(void *const _data)
{
    printf("%Iu\n", *((size_t*)_data) );
}

int main()
{
    // Создание односвязного списка.
    c_forward_list *forward_list = c_forward_list_create();

    // Заполнение односвязного списка.
    for (size_t i = 0; i < 10; ++i)
    {
        *( (size_t*) c_forward_list_push_back(forward_list, sizeof(size_t)) ) = i;
    }

    // Задание массива с индексами для удаления.
    size_t indexes[4] = {1, 2, 2, 8};

    // Вывод содержимого односвязного списка.
    printf("forward_list:\n");
    c_forward_list_for_each(forward_list, print);

    // Удаление узлов с заданными индексами.
    c_forward_list_erase_few(forward_list, indexes, 4, NULL);

    // Вывод содержимого односвязного списка.
    printf("forward_list:\n");
    c_forward_list_for_each(forward_list, print);

    getchar();

    return 0;
}
