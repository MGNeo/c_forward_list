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
ptrdiff_t c_forward_list_delete(c_forward_list *const _list, void (*const _del_func)(void *const _data))
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
                select_node = *((void**)select_node);

                _del_func((uint8_t*)delete_node + sizeof(void*));

                free(delete_node);
            }
        } else {
            while(select_node != NULL)
            {
                delete_node = select_node;
                select_node = *((void**)select_node);

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
    if (_data_size == 0) return NULL;

    void *new_node = malloc(sizeof(void*) + _data_size);
    if (new_node == NULL) return NULL;

    *((void**)new_node) = _list->head;
    _list->head = new_node;
    ++_list->nodes_count;

    return (uint8_t*)_list->head + sizeof(void*);
}

// Уничтожает начальный узел списка.
// В случае успеха возвращает > 0, иначе < 0.
ptrdiff_t c_forward_list_pop_front(c_forward_list *const _list, void (*const _del_func)(void *const _data))
{
    if (_list == NULL) return -1;
    if (_list->nodes_count == 0) return -2;

    void *delete_node = _list->head;
    _list->head = *((void**)_list->head);

    if (_del_func != NULL)
    {
        _del_func((uint8_t*)delete_node + sizeof(void*));
    }

    free(delete_node);

    --_list->nodes_count;

    return 1;
}

// Вставляет в конец списка новый узел.
// В случаеуспеха возвращает указатель на неинициализированные данные.
// В случае ошибки возвращает NULL.
void *c_forward_list_push_back(c_forward_list *const _list, const size_t _data_size)
{
    if (_list == NULL) return NULL;
    if (_data_size == 0) return NULL;

    void *new_node = malloc(sizeof(void*) + _data_size);
    if (new_node == NULL) return NULL;

    void *last_node = &_list->head;
    for (size_t i = 0; i < _list->nodes_count; ++i)
    {
        last_node = *((void**)last_node);
    }
    *((void**)last_node) = new_node;
    *((void**)new_node) = NULL;

    ++_list->nodes_count;

    return (uint8_t*)new_node + sizeof(void*);
}

// Уничтожает последний узел списка.
// В случае успеха возвращает > 0, иначе < 0.
ptrdiff_t c_forward_list_pop_back(c_forward_list *const _list, void (*const _del_func)(void *const _data))
{
    if (_list == NULL) return -1;
    if (_list->nodes_count == 0) return -2;

    void *prev_node = &_list->head;
    void *delete_node = _list->head;
    for (size_t i = 1; i < _list->nodes_count; ++i)
    {
        prev_node = delete_node;
        delete_node = *((void**)delete_node);
    }
    *((void**)prev_node) = NULL;

    if (_del_func != NULL)
    {
        _del_func((uint8_t*)delete_node + sizeof(void*));
    }
    free(delete_node);

    --_list->nodes_count;

    return 1;
}

// Возвращает указатель на данные первого узла.
// В случае ошибки возвращает NULL.
void *c_forward_list_front(const c_forward_list *const _list)
{
    if (_list == NULL) return NULL;
    if (_list->nodes_count == 0) return NULL;

    return (uint8_t*)_list->head + sizeof(void*);
}

// Возвращает указатель на данные узла с заданным индексом.
// В случае ошибки возвращает NULL.
void *c_forward_list_at(const c_forward_list *const _list, const size_t _index)
{
    if (_list == NULL) return NULL;
    if (_index >= _list->nodes_count) return NULL;
    void *select_node = _list->head;
    for (size_t i = 0; i < _index; ++i)
    {
        select_node = *((void**)select_node);
    }

    return (uint8_t*)select_node + sizeof(void*);
}

// Возвращает указатель на данные последнего узла.
// В случае ошибки возвращает NULL.
void *c_forward_list_back(const c_forward_list *const _list)
{
    if (_list == NULL) return NULL;
    if (_list->nodes_count == 0) return NULL;

    void *select_node = _list->head;
    for (size_t i = 1; i < _list->nodes_count; ++i)
    {
        select_node = *((void**)select_node);
    }

    return (uint8_t*)select_node + sizeof(void*);
}

// Вставка нового узла с неинициализированными данными заданного размера перед узлом с заданным порядковым номером.
// В случае успеха возвращает указатель на данные нового узла.
// В случае ошибки возвращает NULL.
// Позволяет вставлять в пустой список, если _index = 0;
void *c_forward_list_insert(c_forward_list *const _list, const size_t _data_size, const size_t _index)
{
    if (_list == NULL) return NULL;
    if (_data_size == 0) return NULL;
    if (_index > _list->nodes_count) return NULL;

    void *new_node = malloc(sizeof(void*) + _data_size);
    if (new_node == NULL) return NULL;

    void *prev_node = &_list->head,
         *select_node = _list->head;

    for (size_t i = 0; i < _index; ++i)
    {
        prev_node = select_node;
        select_node = *((void**)select_node);
    }
    *((void**)prev_node) = new_node;
    *((void**)new_node) = select_node;

    ++_list->nodes_count;

    return (uint8_t*)new_node + sizeof(void*);
}

// Удаляет узел с заданным порядковым номером.
// В случае успеха возвращает > 0, иначе < 0.
ptrdiff_t c_forward_list_erase(c_forward_list *const _list, const size_t _index, void (*const _del_func)(void *const _data))
{
    if (_list == NULL) return -1;
    if (_index >= _list->nodes_count) return -2;

    void *prev_node = &_list->head;
    void *delete_node = _list->head;

    for (size_t i = 0; i < _index; ++i)
    {
        prev_node = delete_node;
        delete_node = *((void**)delete_node);
    }
    *((void**)prev_node) = *((void**)delete_node);

    if (_del_func != NULL)
    {
        _del_func( (uint8_t*)delete_node + sizeof(void*));
    }
    free(delete_node);

    --_list->nodes_count;

    return 1;
}

// Удаляет узлы с заданными порядковыми индексами.
// Если функция удаления задана, то в массиве, на который указывает _indexes, не должно быть одинаковых индексов,
// иначе возможна ошибка повтороного освобождения ресурсов.
// Массив, на который указывает _indexes, сортируется.
// В случае успеха функция возвращает кол-во удаленных узлов.
// В случае ошибки 0.
size_t c_forward_list_erase_few(c_forward_list *const _list, size_t *const _indexes, const size_t _indexes_count,
                                void (*const _del_func)(void *const _data))
{
    if (_list == NULL) return 0;
    if (_indexes == NULL) return 0;
    if (_indexes_count == 0) return 0;
    if (_list->nodes_count == 0) return 0;
    size_t count = 0;
    // Компаратор для бинарного поиска.
    ptrdiff_t comp_bsearch(const void *const _key, const size_t * const _value)
    {
        const size_t key = *((size_t*)_key);
        const size_t value = *((size_t*)_value);
        if (key > value)
        {
            return 1;
        } else {
            if (key == value)
            {
                return 0;
            } else {
                return -1;
            }
        }
    }
    // Компаратор для сортировки массива, на который указывает _indexes.
    ptrdiff_t comp_sort(const void *const _a, const void *const _b)
    {
        const size_t a = *((size_t*)_a);
        const size_t b = *((size_t*)_b);
        if (a > b)
        {
            return 1;
        } else {
            if (a == b)
            {
                return 0;
            } else {
                return -1;
            }
        }
    }
    // Сортируем массив индексов.
    qsort(_indexes, _indexes_count, sizeof(size_t), comp_sort);
    void *prev_node = &_list->head,
         *select_node = _list->head,
         *next_node;
    if (_del_func != NULL)
    {
        for (size_t i = 0; i < _list->nodes_count; ++i)
        {
            next_node = *((void**)select_node);
            if(bsearch(&i,
                       _indexes,
                       _indexes_count,
                       sizeof(size_t),
                       comp_bsearch) != NULL)
            {
                _del_func((uint8_t*)select_node + sizeof(void*));
                free(select_node);
                ++count;
                *((void**)prev_node) = next_node;
            } else {
                prev_node = select_node;
            }
            select_node = next_node;
        }
    } else {
        for (size_t i = 0; i < _list->nodes_count; ++i)
        {
            next_node = *((void**)select_node);
            if(bsearch(&i,
                       _indexes,
                       _indexes_count,
                       sizeof(size_t),
                       comp_bsearch) != NULL)
            {
                free(select_node);
                ++count;
                *((void**)prev_node) = next_node;
            } else {
                prev_node = select_node;
            }
            select_node = next_node;
        }
    }
    _list->nodes_count -= count;
    return count;
}

// Проходит по всему списку и выполняет над данными каждого списка _func.
// В случае успеха возвращает > 0, иначе < 0.
ptrdiff_t c_forward_list_for_each(c_forward_list *const _list, void (*const _func)(void *const _data))
{
    if (_list == NULL) return -1;
    if (_func == NULL) return -2;
    if (_list->nodes_count == 0) return -3;

    void *select_node = _list->head;
    while(select_node != NULL)
    {
        _func((uint8_t*)select_node + sizeof(void*));

        select_node = *((void**)select_node);
    }

    return 1;
}

// Удаляет из списка узлы, для данных которых _comp возвращает > 0.
// Возвращает кол-во удаленных узлов.
// В случае ошибки возвращает 0.
size_t c_forward_list_remove_few(c_forward_list *const _list, size_t (*const _comp)(void *const _data),
                                void (* const _del_func)(void *const _data))
{
    if (_list == NULL) return 0;
    if (_comp == NULL) return 0;
    if (_list->nodes_count == 0) return 0;

    size_t count = 0;

    void *select_node = _list->head,
         *prev_node = &_list->head,
         *next_node = NULL;

    while(select_node != NULL)
    {
        next_node = *((void**)select_node);

        if (_comp((uint8_t*)select_node + sizeof(void*)) > 0)
        {
            if (_del_func != NULL)
            {
                _del_func((uint8_t*)select_node + sizeof(void*));
            }
            free(select_node);

            ++count;
            --_list->nodes_count;

            *((void**)prev_node) = next_node;
        } else {
            prev_node = select_node;
        }

        select_node = next_node;
    }

    return count;
}

// Очищает список ото всех узлов.
// В случае успеха возвращает > 0, иначе < 0.
ptrdiff_t c_forward_list_clear(c_forward_list *const _list, void (*const _del_func)(void *const _data))
{
    if (_list == NULL) return -1;
    if (_list->nodes_count == 0) return 1;

    void *select_node = _list->head,
         *delete_node;

    for (size_t i = 0; i < _list->nodes_count; ++i)
    {
        delete_node = select_node;
        select_node = *((void**)select_node);

        if (_del_func != NULL)
        {
            _del_func((uint8_t*)delete_node + sizeof(void*));
        }
        free(delete_node);
    }

    _list->head = NULL;
    _list->nodes_count = 0;

    return 2;
}
