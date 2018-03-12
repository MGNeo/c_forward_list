#include <stdio.h>
#include <stdlib.h>
#include "c_forward_list.h"

void print(void *const _data)
{
    printf("%Iu\n", *((size_t*)_data) );
}

int main()
{
    // —оздание односв€зного списка.
    c_forward_list *forward_list = c_forward_list_create();

    // «аполнение односв€зного списка.
    for (size_t i = 0; i < 10; ++i)
    {
        *( (size_t*) c_forward_list_push_back(forward_list, sizeof(size_t)) ) = i;
    }

    // «адание массива с индексами дл€ удалени€.
    size_t indexes[4] = {1, 2, 2, 8};

    // ¬ывод содержимого односв€зного списка.
    printf("forward_list:\n");
    c_forward_list_for_each(forward_list, print);

    // ”даление узлов с заданными индексами.
    c_forward_list_erase_few(forward_list, indexes, 4, NULL);

    // ¬ывод содержимого односв€зного списка.
    printf("forward_list:\n");
    c_forward_list_for_each(forward_list, print);

    getchar();

    return 0;
}
