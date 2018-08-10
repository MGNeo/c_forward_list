﻿/*
    Файл реализации односвязного списка c_forward_list
    Разработка, отладка и сборка производилась в:
    ОС: Windows 10/x64
    IDE: Code::Blocks 17.12
    Компилятор: default Code::Blocks 17.12 MinGW

    Разработчик: Глухманюк Максим
    Эл. почта: mgneo@yandex.ru
    Место: Российская Федерация, Самарская область, Сызрань
    Дата: 16.03.2018
    Лицензия: GPLv3
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "c_forward_list.h"

typedef struct s_c_forward_list_node c_forward_list_node;

struct s_c_forward_list_node
{
    struct s_c_forward_list_node *next_node;
    void *data;
};

struct s_c_forward_list
{
    c_forward_list_node *head;
    size_t nodes_count;
};

// Компаратор для сортировки массива с индексами узлов, которые необходимо удалить.
static int comp_sort(const void *const _index_a,
                     const void *const _index_b)
{
    if ( (_index_a == NULL) || (_index_b == NULL) )
    {
        return 0;
    }

    const size_t index_a = *((size_t*)_index_a);
    const size_t index_b = *((size_t*)_index_b);
    if (index_a > index_b)
    {
        return 1;
    } else {
        if (index_a == index_b)
        {
            return 0;
        } else {
            return -1;
        }
    }
}

// Создает новый односвязный список.
// В случае ошибки возвращает NULL.
c_forward_list *c_forward_list_create(void)
{
    c_forward_list *const new_forward_list = (c_forward_list*)malloc(sizeof(c_forward_list));
    if (new_forward_list == NULL) return NULL;

    new_forward_list->head = NULL;
    new_forward_list->nodes_count = 0;

    return new_forward_list;
}

// Удаляет односвязный список.
// В случае успеха возвращает > 0, иначе < 0.
ptrdiff_t c_forward_list_delete(c_forward_list *const _forward_list,
                                void (*const _del_data)(void *const _data))
{
    if (c_forward_list_clear(_forward_list, _del_data) < 0)
    {
        return -1;
    }

    free(_forward_list);

    return 1;
}

// Вставляет в начало списка новый узел.
// Не позволяет вставлять узел с указателем на NULL.
// В случае успеха возвращает > 0, иначе < 0.
ptrdiff_t c_forward_list_push_front(c_forward_list *const _forward_list,
                                    const void *const _data)
{
    if (_forward_list == NULL) return -1;
    if (_data == NULL) return -2;
    if (_forward_list->nodes_count == SIZE_MAX) return -3;// Не, ну а вдруг...)

    c_forward_list_node *const new_node = (c_forward_list_node*)malloc(sizeof(c_forward_list_node));
    if (new_node == NULL) return -4;

    new_node->next_node = _forward_list->head;
    new_node->data = (void*)_data;

    _forward_list->head = new_node;
    ++_forward_list->nodes_count;

    return 1;
}

// Уничтожает первый узел списка.
// В случае успешного уничтожения возвращает > 0.
// Если уничтожать нечего, возвращает 0.
// В случае ошибки возвращает < 0.
ptrdiff_t c_forward_list_pop_front(c_forward_list *const _forward_list,
                                   void (*const _del_data)(void *const _data))
{
    if (_forward_list == NULL) return -1;
    if (_forward_list->nodes_count == 0) return 0;

    c_forward_list_node *const delete_node = _forward_list->head;
    _forward_list->head = _forward_list->head->next_node;

    if (_del_data != NULL)
    {
        _del_data(delete_node->data);
    }

    free(delete_node);

    --_forward_list->nodes_count;

    return 1;
}

// Вставляет в конец списка новый узел.
// Не позволяет вставлять узел с указателем на NULL.
// В случае успеха возвращает > 0, иначе < 0.
ptrdiff_t c_forward_list_push_back(c_forward_list *const _forward_list,
                                   const void *const _data)
{
    if (_forward_list == NULL) return -1;
    if (_data == NULL) return -2;
    if (_forward_list->nodes_count == SIZE_MAX) return -3;// Не, ну а вдруг...)

    c_forward_list_node *const new_node = (c_forward_list_node*)malloc(sizeof(c_forward_list_node));
    if (new_node == NULL) return -4;

    new_node->next_node = NULL;
    new_node->data = (void*)_data;

    c_forward_list_node *select_node = _forward_list->head,
                        *last_node = NULL;
    while (select_node != NULL)
    {
        last_node = select_node;
        select_node = select_node->next_node;
    }

    if (last_node == NULL)
    {
        _forward_list->head = new_node;
    } else {
        last_node->next_node = new_node;
    }

    ++_forward_list->nodes_count;

    return 1;
}

// Уничтожает последний узел списка.
// В случае успешного уничтожения возвращает > 0.
// Если уничтожать нечего, возвращает 0.
// В случае ошибки возвращает < 0.
ptrdiff_t c_forward_list_pop_back(c_forward_list *const _forward_list,
                                  void (*const _del_data)(void *const _data))
{
    if (_forward_list == NULL) return -1;
    if (_forward_list->nodes_count == 0) return 0;

    c_forward_list_node *delete_node = _forward_list->head,
                        *prev_node = NULL;

    while (delete_node->next_node != NULL)
    {
        prev_node = delete_node;
        delete_node = delete_node->next_node;
    }

    if (prev_node == NULL)
    {
        _forward_list->head = NULL;
    } else {
        prev_node->next_node = NULL;
    }

    if (_del_data != NULL)
    {
        _del_data(delete_node->data);
    }
    free(delete_node);

    --_forward_list->nodes_count;

    return 1;
}

// Возвращает указатель на данные, которые связаны с первым узлом списка.
// В случае ошибки возвращает NULL.
void *c_forward_list_front(const c_forward_list *const _forward_list)
{
    if (_forward_list == NULL) return NULL;
    if (_forward_list->nodes_count == 0) return NULL;

    return _forward_list->head->data;
}

// Возвращает указатель на данные, которые связаны с узлом с заданным индексом.
// В случае ошибки возвращает NULL.
void *c_forward_list_at(const c_forward_list *const _forward_list,
                        const size_t _index)
{
    if (_forward_list == NULL) return NULL;
    if (_index >= _forward_list->nodes_count) return NULL;

    c_forward_list_node *select_node = _forward_list->head;
    for (size_t i = 0; i < _index; ++i)
    {
        select_node = select_node->next_node;
    }

    return select_node->data;
}

// Возвращает указатель на данные, связанные с последним узлом.
// В случае ошибки возвращает NULL.
void *c_forward_list_back(const c_forward_list *const _forward_list)
{
    if (_forward_list == NULL) return NULL;
    if (_forward_list->nodes_count == 0) return NULL;

    c_forward_list_node *select_node = _forward_list->head;
    while (select_node->next_node != NULL)
    {
        select_node = select_node->next_node;
    }

    return select_node->data;
}

// Вставка в заданную позицию нового узла.
// В случае успеха возвращает > 0.
// В случае ошибки возвращает < 0.
// Не позволяет вставлять узел с указателем на NULL.
// Позволяет вставлять в пустой список, если _index = 0;
ptrdiff_t c_forward_list_insert(c_forward_list *const _forward_list,
                                const void *const _data,
                                const size_t _index)
{
    if (_forward_list == NULL) return -1;
    if (_data == NULL) return -2;
    if (_forward_list->nodes_count == SIZE_MAX) return -3;// Не, ну а вдруг...)
    if (_index > _forward_list->nodes_count) return -4;

    c_forward_list_node *const new_node = (c_forward_list_node*)malloc(sizeof(c_forward_list_node));
    if (new_node == NULL) return -5;

    new_node->data = (void*)_data;

    c_forward_list_node *next_node = _forward_list->head,
                        *prev_node = NULL;

    for (size_t i = 0; i < _index; ++i)
    {
        prev_node = next_node;
        next_node = next_node->next_node;
    }

    if (prev_node == NULL)
    {
        _forward_list->head = new_node;
        new_node->next_node = next_node;
    } else {
        prev_node->next_node = new_node;
        new_node->next_node = next_node;
    }

    ++_forward_list->nodes_count;

    return 1;
}

// Удаляет узел с заданным индексом.
// В случае успеха возвращает > 0, иначе < 0.
ptrdiff_t c_forward_list_erase(c_forward_list *const _forward_list,
                               const size_t _index,
                               void (*const _del_data)(void *const _data))
{
    if (_forward_list == NULL) return -1;
    if (_index >= _forward_list->nodes_count) return -2;

    c_forward_list_node *delete_node = _forward_list->head,
                        *prev_node = NULL;

    for (size_t i = 0; i < _index; ++i)
    {
        prev_node = delete_node;
        delete_node = delete_node->next_node;
    }

    if (prev_node == NULL)
    {
        _forward_list->head = delete_node;
    } else {
        prev_node->next_node = delete_node->next_node;
    }

    if (_del_data != NULL)
    {
        _del_data( delete_node->data );
    }
    free(delete_node);

    --_forward_list->nodes_count;

    return 1;
}

// Удаляет узлы с заданными порядковыми индексами.
// Массив, на который указывает _indexes, сортируется.
// Наличие несуществующих или одинаковых индексов не считается ошибкой.
// В случае успеха функция возвращает кол-во удаленных узлов.
// В случае ошибки 0.
size_t c_forward_list_erase_few(c_forward_list *const _forward_list,
                                size_t *const _indexes,
                                const size_t _indexes_count,
                                void (*const _del_data)(void *const _data))
{
    if (_forward_list == NULL) return 0;
    if (_indexes == NULL) return 0;
    if (_indexes_count == 0) return 0;
    if (_forward_list->nodes_count == 0) return 0;

    // Сортируем массив индексов.
    qsort(_indexes, _indexes_count, sizeof(size_t), comp_sort);

    // Если корректных индексов нет, то завершаем.
    if (_indexes[0] >= _forward_list->nodes_count) return 0;

    // Избавимся от повторяющихся индексов.
    size_t i_index = 0;
    for (size_t i = 1; (i < _indexes_count) && (_indexes[i] < _forward_list->nodes_count); ++i)
    {
        if (_indexes[i] != _indexes[i - 1])
        {
            _indexes[++i_index] = _indexes[i];
        }
    }

    // Теперь i_index == количеству корректных уникальных индексов.
    i_index += 1;
    // Контроль переполнения.
    if (i_index < i_index - 1) return 0;// Не, ну а вдруг...)

    size_t count = 0;

    c_forward_list_node *prev_node = NULL,
                        *select_node = _forward_list->head,
                        *next_node = NULL;

    if (_del_data!= NULL)
    {
        for (size_t i = 0; (i < _forward_list->nodes_count) && (count <  i_index); ++i)
        {
            next_node = select_node->next_node;
            if (i == _indexes[count])
            {
                _del_data(select_node->data);
                free(select_node);

                if (prev_node == NULL)
                {
                    _forward_list->head = next_node;
                } else {
                    prev_node->next_node = next_node;
                }

                ++count;
            } else {
                prev_node = select_node;
            }
            select_node = next_node;
        }
    } else {
        // Дублирование кода, чтобы на каждом узле не проверять,
        // задана ли функция удаления данных.
        for (size_t i = 0; (i < _forward_list->nodes_count) && (count <  i_index); ++i)
        {
            next_node = select_node->next_node;
            if (i == _indexes[count])
            {
                free(select_node);

                if (prev_node == NULL)
                {
                    _forward_list->head = next_node;
                } else {
                    prev_node->next_node = next_node;
                }

                ++count;
            } else {
                prev_node = select_node;
            }
            select_node = next_node;
        }
    }

    _forward_list->nodes_count -= count;

    return count;
}

// Проходит по всему списку и выполняет над данными каждого списка заданные действия.
// В случае успешного выполнения возвращает > 0.
// Если список пуст, возвращает 0.
// В случае ошибки возвращает < 0.
ptrdiff_t c_forward_list_for_each(c_forward_list *const _forward_list,
                                  void (*const _action_data)(void *const _data))
{
    if (_forward_list == NULL) return -1;
    if (_action_data == NULL) return -2;

    if (_forward_list->nodes_count == 0) return 0;

    c_forward_list_node *select_node = _forward_list->head;
    while (select_node != NULL)
    {
        _action_data(select_node->data);

        select_node = select_node->next_node;
    }

    return 1;
}

// Удаляет из списка узлы, для данных которых _comp возвращает > 0.
// Возвращает кол-во удаленных узлов.
// В случае ошибки возвращает 0.
size_t c_forward_list_remove_few(c_forward_list *const _forward_list,
                                 size_t (*const _pred_data)(const void *const _data),
                                 void (*const _del_data)(void *const _data))
{
    if (_forward_list == NULL) return 0;
    if (_pred_data == NULL) return 0;
    if (_forward_list->nodes_count == 0) return 0;

    size_t count = 0;

    c_forward_list_node *prev_node = NULL,
                        *select_node = _forward_list->head,
                        *next_node = NULL;

    if (_del_data != NULL)
    {
        while (select_node != NULL)
        {
            next_node = select_node->next_node;

            if (_pred_data(select_node->data) > 0)
            {
                _del_data(select_node->data);
                free(select_node);

                ++count;

                if (prev_node == NULL)
                {
                    _forward_list->head = next_node;
                } else {
                    prev_node->next_node = next_node;
                }
            } else {
                prev_node = select_node;
            }

            select_node = next_node;
        }
    } else {
        // Дублирование кода, чтобы на каждом узле не проверять,
        // задана ли функция удаления.
        while (select_node != NULL)
        {
            next_node = select_node->next_node;

            if (_pred_data(select_node->data) > 0)
            {
                free(select_node);

                ++count;

                if (prev_node == NULL)
                {
                    _forward_list->head = next_node;
                } else {
                    prev_node->next_node = next_node;
                }
            } else {
                prev_node = select_node;
            }

            select_node = next_node;
        }
    }

    _forward_list->nodes_count -= count;

    return count;
}

// Очищает список ото всех узлов.
// В случае успешного очищения возвращает > 0.
// Если очищать не от чего, возвращает 0.
// В случае ошибки возвращает < 0.
ptrdiff_t c_forward_list_clear(c_forward_list *const _forward_list,
                               void (*const _del_data)(void *const _data))
{
    if (_forward_list == NULL) return -1;
    if (_forward_list->nodes_count == 0) return 0;

    c_forward_list_node *select_node = _forward_list->head,
                        *delete_node;

    if (_del_data != NULL)
    {
        while (select_node != NULL)
        {
            delete_node = select_node;
            select_node = select_node->next_node;

            _del_data( delete_node->data );
            free(delete_node);
        }
    } else {
        // Дублирование кода, чтобы на каждом узле не проверять,
        // задана ли функция удаления.
        while (select_node != NULL)
        {
            delete_node = select_node;
            select_node = select_node->next_node;

            free(delete_node);
        }
    }

    _forward_list->head = NULL;
    _forward_list->nodes_count = 0;

    return 1;
}

// Возвращает количество узлов односвязного списка.
// В случае ошибки возвращает 0.
size_t c_forward_list_nodes_count(const c_forward_list *const _forward_list)
{
    if (_forward_list == NULL)
    {
        return 0;
    }

    return _forward_list->nodes_count;
}
