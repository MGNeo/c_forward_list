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
    *( (float*) c_forward_list_insert(forward_list, sizeof(float), 0 ) ) = 1.f;
    *( (float*) c_forward_list_insert(forward_list, sizeof(float), 1 ) ) = 2.f;
    *( (float*) c_forward_list_insert(forward_list, sizeof(float), 0 ) ) = 3.f;
    *( (float*) c_forward_list_insert(forward_list, sizeof(float), 3 ) ) = 4.f;

    // Печать содержимого.
    print_stats(forward_list);

    // Доступ к первому элементу.
    *( (float*) c_forward_list_front(forward_list) ) = 777.f;

    // Печать содержимого.
    print_stats(forward_list);

    // Удаление списка.
    c_forward_list_delete(forward_list, NULL);
    forward_list = NULL;

    getchar();
    return 0;
}
