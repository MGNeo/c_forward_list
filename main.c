#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#include "c_forward_list.h"

void print_stats(c_forward_list *_list)
{
    printf("forward_list->nodes_count = %Iu\n", _list->nodes_count);
    for (size_t i = 0; i < _list->nodes_count; ++i)
    {
        printf("forward_list->at(%Iu) = %f\n", i, *((float*)c_forward_list_at(_list, i)) );
    }
}

size_t comp(void *const _data)
{
    if ( *((float*)_data) < 5.f )
    {
        return 1;
    } else {
        return 0;
    }
}

int main()
{
    // Создание односвязного листа.
    c_forward_list *forward_list = c_forward_list_create();

    // Вставка в конец списка 15 элементов.
    for (size_t i = 0; i < 15; ++i)
    {
        *( (float*) c_forward_list_push_back(forward_list, sizeof(float)) ) = (i % 4) * 4;
    }

    // Печать содержимого.
    print_stats(forward_list);

    // Удаление элементов, данные которых < 5.f.
    const size_t count = c_forward_list_erase_few(forward_list, comp, NULL);
    printf("Deleted count = %Iu\n", count);

    // Печать содержимого.
    print_stats(forward_list);

    // Удаление списка.
    c_forward_list_delete(forward_list, NULL);
    forward_list = NULL;

    getchar();
    return 0;
}
