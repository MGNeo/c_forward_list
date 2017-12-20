#include "c_forward_list.h"

// Создает новый односвязный список.
// В случае ошибки возвращает NULL.
c_forward_list *c_forward_list_create(void)
{
    c_forward_list *new_forward_list = (c_forward_list*)malloc(sizeof(c_forward_list));
    if (new_forward_list == NULL) return NULL;

    new_forward_list->head = NULL;
    new_forward_list->nodes_count = 0;

    return new_forward_list;
}

// Удаляет односвязный список.
// В случае успеха возвращает > 0, иначе < 0.
ptrdiff_t c_forward_list_delete(c_forward_list *const _list, void (*const _del_func(void *const _data)))
{
    if (_list == NULL) return -1;

    if (_list->nodes_count > 0)
    {
        void *select_node = _list->head,
             *delete_node;

        if (_del_func != NULL)
        {
            while(select_node != NULL)
            {
                delete_node = select_node;
                select_node = *(void**)select_node;

                _del_func((uint8_t*)delete_node + sizeof(void*));

                free(delete_node);
            }
        } else {
            while(select_node != NULL)
            {
                delete_node = select_node;
                select_node = *(void**)select_node;

                free(delete_node);
            }
        }
    }

    free(_list);

    return 1;
}

// Вставляет в начало списка новый узел.
// В случае успеха возвращает указатель на неинициализированные данные.
// В случае ошибки возвращает NULL.
void *c_forward_list_push_front(c_forward_list *const _list, const size_t _data_size)
{
    if (_list == NULL) return NULL;

    void *new_node = malloc(sizeof(void*) + _data_size);
    if (new_node == NULL) return NULL;

    *( (void**)new_node ) = _list->head;
    _list->head = new_node;
    ++_list->nodes_count;

    return (uint8_t*)_list->head + sizeof(void*);
}

// Возвращает указатель на данные первого узла.
// В случае ошибки возвращает NULL.
void *c_forward_list_front_data(c_forward_list *const _list)
{
    if (_list == NULL) return NULL;
    if (_list->nodes_count == 0) return NULL;

    return (uint8_t*)_list->head + sizeof(void*);
}

// Вынимает из начала списка узел, уничтожая его.
// В случае успеха возвращает > 0, иначе < 0.
ptrdiff_t c_forward_list_pop_front(c_forward_list *const _list, void (*const _del_func(void *const _data)))
{
    if (_list == NULL) return -1;
    if (_list->nodes_count == 0) return -2;

    void *delete_node = _list->head;
    _list->head = *( (void**)(_list->head) );// Продумать этот момент.

    if (_del_func != NULL)
    {
        _del_func((uint8_t*)delete_node + sizeof(void*));
    }

    free(delete_node);

    --_list->nodes_count;

    return 1;
}
