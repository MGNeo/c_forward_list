#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "c_forward_list.h"

typedef struct s_object
{
    int i;
    float f;
} object;

// Функция вывода содержимого элемента односвязного списка типа object.
void print_object(void *const _data)
{
    if (_data == NULL)
    {
        return;
    }

    const object *const obj = (object*)_data;

    printf("i: %i, f: %f\n", obj->i, obj->f);
}

int main(int argc, char **argv)
{
    size_t error;
    c_forward_list *forward_list;

    // Пытаемся создать односвязный список.
    forward_list = c_forward_list_create(&error);

    // Если не удалось создать односвязный список, выводим причину ошибки.
    if (forward_list == NULL)
    {
        printf("error: %Iu\n", error);
        printf("Program end.\n");
        getchar();
        return -1;
    }

    // Вставляем в конец односвязного списка 10 элементов.
    for (size_t i = 0; i < 10; ++i)
    {
        // Пытаемся выделить память под элемент.
        object *const obj = malloc(sizeof(object));
        // Если не удалось выделить память под элемент.
        if (obj == NULL)
        {
            printf("malloc(): NULL\n");
            printf("Program end.\n");
            getchar();
            return -2;
        }
        // Инициализируем элемент.
        obj->i = i;
        obj->f = i + 3.14f;

        // Пытаемся добавить элемент в конец односвязного списка.
        const ptrdiff_t r_code = c_forward_list_push_back(forward_list, obj);
        // Если не удалось добавить элемент в конец односвязного списка, выводим причину ошибки.
        if (r_code < 0)
        {
            printf("r_code: %Id\n", r_code);
            printf("Program end.\n");
            getchar();
            return -3;
        }
    }

    // Используя функцию обхода, выведем содержимое каждого элемента.
    {
        const ptrdiff_t r_code = c_forward_list_for_each(forward_list, print_object);
        // Если произошла ошибка, покажем ее.
        if (r_code < 0)
        {
            printf("r_code: %Id\n", r_code);
            printf("Program end.\n");
            getchar();
            return -6;
        }
    }

    // Удалим узлы с индексами: 0, 1, 3, 2, 99
    //
    {
        size_t indexes[5] = {0, 1, 3, 2, 99};
        error = 0;
        const size_t d_count = c_forward_list_erase_few(forward_list, indexes, 5, free, &error);
        // Если возникла ошибка, покажем ее.
        if ( (d_count == 0) && (error > 0) )
        {
            printf("error: %Iu\n", error);
            printf("Program end.\n");
            getchar();
            return -7;
        }
        // Покажем, сколько элементов было удалено.
        printf("d_count: %Iu\n", d_count);
    }

    // Используя функцию обхода, выведем содержимое каждого элемента.
    {
        const ptrdiff_t r_code = c_forward_list_for_each(forward_list, print_object);
        // Если произошла ошибка, покажем ее.
        if (r_code < 0)
        {
            printf("r_code: %Id\n", r_code);
            printf("Program end.\n");
            getchar();
            return -8;
        }
    }

    // Удалим связный список.
    {
        const ptrdiff_t r_code = c_forward_list_delete(forward_list, free);
        // Если возникла ошибка, покажем ее.
        if (r_code < 0)
        {
            printf("r_code: %Id\n", r_code);
            printf("Program end.\n");
            getchar();
            return -9;
        }
    }

    getchar();
    return 0;
}
