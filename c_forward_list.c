/*
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

// Если расположение задано, в него помещается код.
static void error_set(size_t *const _error,
                      const size_t _code)
{
    if (_error != NULL)
    {
        *_error = _code;
    }
}

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
// В случае ошибки возвращает NULL, и если _error != NULL, в заданное расположение помещается
// код причины ошибки (> 0).
c_forward_list *c_forward_list_create(size_t *const _error)
{
    c_forward_list *const new_forward_list = malloc(sizeof(c_forward_list));
    if (new_forward_list == NULL)
    {
        error_set(_error, 1);
        return NULL;
    }

    new_forward_list->head = NULL;
    new_forward_list->nodes_count = 0;

    return new_forward_list;
}

// Удаляет односвязный список.
// В случае успеха возвращает > 0.
// В случае ошибки возвращает < 0.
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

// Добавляет данные в начало списка.
// Не позволяет добавлять NULL.
// В случае успеха возвращает > 0, данные захватываются списком.
// В случае ошибки возвращает < 0, данные не захватываются связным списком.
ptrdiff_t c_forward_list_push_front(c_forward_list *const _forward_list,
                                    const void *const _data)
{
    if (_forward_list == NULL) return -1;
    if (_data == NULL) return -2;
    if (_forward_list->nodes_count == SIZE_MAX) return -3;// Не, ну а вдруг...)

    c_forward_list_node *const new_node = malloc(sizeof(c_forward_list_node));
    if (new_node == NULL) return -4;

    new_node->next_node = _forward_list->head;
    new_node->data = (void*)_data;

    _forward_list->head = new_node;
    ++_forward_list->nodes_count;

    return 1;
}

// Убирает данные из начала списка.
// В случае успешного убирания возвращает > 0.
// Если список пуст, возвращает 0.
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

// Добавляет данные в конец списка.
// Не позволяет добавлять NULL.
// В случае успеха возвращает > 0, данные захватываются списком.
// В случае ошибки возвращает < 0, данные не захватываются списком.
ptrdiff_t c_forward_list_push_back(c_forward_list *const _forward_list,
                                   const void *const _data)
{
    if (_forward_list == NULL) return -1;
    if (_data == NULL) return -2;
    if (_forward_list->nodes_count == SIZE_MAX) return -3;// Не, ну а вдруг...)

    c_forward_list_node *const new_node = malloc(sizeof(c_forward_list_node));
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

// Убирает данные с конца связного списка.
// В случае успешного убирания возвращает > 0.
// Если список пуст, возвращает 0.
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

// Обращается к данным в начале списка.
// В случае ошибки возвращает NULL, и если _error != NULL, в заданное расположение
// помещается код причины ошибки(> 0).
// Если список пуст, возвращает NULL, это не считается ошибкой.
// Так как функция может возвращать NULL и в случае успеха, и в случае ошибки, для детектирования
// ошибки перед вызовом функции необходимо поместить 0 в заданное расположение ошибки.
void *c_forward_list_front(const c_forward_list *const _forward_list,
                           size_t *const _error)
{
    if (_forward_list == NULL)
    {
        error_set(_error, 1);
        return NULL;
    }
    if (_forward_list->nodes_count == 0) return NULL;

    return _forward_list->head->data;
}

// Обращается к данным с заданным порядковым индексом, от начала списка, от 0.
// В случае ошибки возвращает NULL, и если _error != NULL, в заданное расположение
// помещается код причины ошибки (> 0).
// Если индекс оказался >= количеству узлов, функция возвращает NULL, это не считается ошибкой.
// Так как функция может возвращать NULL и в случае успеха, и в случае ошибки, для детектирования
// ошибки перед вызовом функции необходимо поместить 0 в заданное расположение ошибки.
void *c_forward_list_at(const c_forward_list *const _forward_list,
                        const size_t _index,
                        size_t *const _error)
{
    if (_forward_list == NULL)
    {
        error_set(_error, 1);
        return NULL;
    }
    if (_index >= _forward_list->nodes_count) return NULL;

    c_forward_list_node *select_node = _forward_list->head;
    for (size_t i = 0; i < _index; ++i)
    {
        select_node = select_node->next_node;
    }

    return select_node->data;
}

// Обращается к данным в конце списка.
// В случае ошибки возвращает NULL, и если _error != NULL, в заданное расположение
// помещается код причины ошибки (> 0).
// Если список пуст, функция вернет NULL, это не считается ошибкой.
// Так как функция может возвращать NULL и в случае успеха, и в случае ошибки, для детектирования
// ошибки перед вызовом функции необходимо поместить 0 в заданное расположение ошибки.
void *c_forward_list_back(const c_forward_list *const _forward_list,
                          size_t *const _error)
{
    if (_forward_list == NULL)
    {
        error_set(_error, 1);
        return NULL;
    }
    if (_forward_list->nodes_count == 0) return NULL;

    c_forward_list_node *select_node = _forward_list->head;
    while (select_node->next_node != NULL)
    {
        select_node = select_node->next_node;
    }

    return select_node->data;
}

// Добавляет данные в заданную позицию списка, от начала, от нуля.
// В случае успеха возвращает > 0, данные захватываются списком.
// Если индекс оказался > количества узлов, функция возвращает 0, данные не захватываются списком.
// В случае ошибки возвращает < 0, данные не захватываются списком.
// Не позволяет добавлять NULL.
// Позволяет добавлять в пустой список, если _index = 0;
ptrdiff_t c_forward_list_insert(c_forward_list *const _forward_list,
                                const void *const _data,
                                const size_t _index)
{
    if (_forward_list == NULL) return -1;
    if (_data == NULL) return -2;
    if (_forward_list->nodes_count == SIZE_MAX) return -3;// Не, ну а вдруг...)
    if (_index > _forward_list->nodes_count) return 0;

    c_forward_list_node *const new_node = malloc(sizeof(c_forward_list_node));
    if (new_node == NULL) return -4;

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

// Убирает данные с заданным индексом, от начала, от 0.
// В случае успешного убирания, возвращает > 0.
// Если индекс оказался >= количеству узлов, функция возвращает 0.
// В случае ошибки возвращает < 0.
ptrdiff_t c_forward_list_erase(c_forward_list *const _forward_list,
                               const size_t _index,
                               void (*const _del_data)(void *const _data))
{
    if (_forward_list == NULL) return -1;
    if (_index >= _forward_list->nodes_count) return 0;

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

// Убирает данные с заданными порядковыми индексами, от начала, от 0.
// Массив индексов сортируется.
// Наличие несуществующих или одинаковых индексов не считается ошибкой.
// В случае успеха функция возвращает количество убранных данных.
// В случае ошибки функция возвращает 0, и если _error != NULL, в заданное расположение помещается
// код причины ошибки (> 0).
// Так как функция может возвращать 0 и в случае успеха, и в случае ошибки, для детектирования
// ошибки перед вызовом функции необходимо поместить 0 в заданное расположение ошибки.
size_t c_forward_list_erase_few(c_forward_list *const _forward_list,
                                size_t *const _indexes,
                                const size_t _indexes_count,
                                void (*const _del_data)(void *const _data),
                                size_t *const _error)
{
    if (_forward_list == NULL)
    {
        error_set(_error, 1);
        return 0;
    }
    if (_indexes == NULL)
    {
        return 0;
    }
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
    if (i_index < i_index - 1)// Не, ну а вдруг...)
    {
        error_set(_error, 2);
        return 0;
    }

    size_t count = 0;

    c_forward_list_node *prev_node = NULL,
                        *select_node = _forward_list->head,
                        *next_node = NULL;

    // Макросы дублирования кода для исключения проверки из цикла.

    // Открытие цикла.
    #define C_FORWARD_LIST_ERASE_FEW_BEGIN\
        /* Идем по списку */\
        for (size_t i = 0; (i < _forward_list->nodes_count) && (count <  i_index); ++i)\
        {\
            next_node = select_node->next_node;\
            /* Если порядковый индекс узла есть в массиве индексов */\
            if (i == _indexes[count])\
            {

    // Закрытие цикла.
    #define C_FORWARD_LIST_ERASE_FEW_END\
                free(select_node);\
                /* Сшиваем разрыв */\
                if (prev_node == NULL)\
                {\
                    _forward_list->head = next_node;\
                } else {\
                    prev_node->next_node = next_node;\
                }\
                ++count;\
            } else {\
                prev_node = select_node;\
            }\
            select_node = next_node;\
        }

    // Функция удаления данных узла задана.
    if (_del_data != NULL)
    {
       C_FORWARD_LIST_ERASE_FEW_BEGIN

       _del_data(select_node->data);

       C_FORWARD_LIST_ERASE_FEW_END
    } else {
        // Функция удаления данных узла не задана.
        C_FORWARD_LIST_ERASE_FEW_BEGIN

        C_FORWARD_LIST_ERASE_FEW_END
    }

    #undef C_FORWARD_LIST_ERASE_FEW_BEGIN
    #undef C_FORWARD_LIST_ERASE_FEW_END

    _forward_list->nodes_count -= count;

    return count;
}

// Проходит по всем элементам списка (от начала к концу) и выполняет над всеми данными заданные действия.
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

// Убирает все данные, для которых _pred_data возвращает > 0.
// Возвращает кол-во убранных данных.
// В случае ошибки возвращает 0, и если _error != NULL, в заданное расположение помещается
// код причины ошибки (> 0).
// Так как функция может возвращать 0 и в случае успеха, и в случае ошибки, для детектирования
// ошибки перед вызвом функции необходимо поместить 0 в заданное расположение ошибки.
size_t c_forward_list_remove_few(c_forward_list *const _forward_list,
                                 size_t (*const _pred_data)(const void *const _data),
                                 void (*const _del_data)(void *const _data),
                                 size_t *const _error)
{
    if (_forward_list == NULL)
    {
        error_set(_error, 1);
        return 0;
    }
    if (_pred_data == NULL)
    {
        error_set(_error, 2);
        return 0;
    }
    if (_forward_list->nodes_count == 0) return 0;

    size_t count = 0;

    c_forward_list_node *prev_node = NULL,
                        *select_node = _forward_list->head,
                        *next_node = NULL;

    // Макросы дублирования кода для избавления от проверок внутри цикла.

    // Открытие цикла.
    #define C_FORWARD_LIST_REMOVE_FEW_BEGIN\
    /* Обходим все узлы списка */\
    while (select_node != NULL)\
    {\
        next_node = select_node->next_node;\
        /* Если предикат говорит, что узел с такими данными необходимо удалить */\
        if (_pred_data(select_node->data) > 0)\
        {

    // Закрытие цикла.
    #define C_FORWARD_LIST_REMOVE_FEW_END\
            free(select_node);\
            ++count;\
            /* Сшиваем разрыв */\
            if (prev_node == NULL)\
            {\
                _forward_list->head = next_node;\
            } else {\
                prev_node->next_node = next_node;\
            }\
        } else {\
            prev_node = select_node;\
        }\
        select_node = next_node;\
    }

    // Закрытие циклов.

    // Функция удаления данных задана.
    if (_del_data != NULL)
    {
        C_FORWARD_LIST_REMOVE_FEW_BEGIN

        _del_data(select_node->data);

        C_FORWARD_LIST_REMOVE_FEW_END
    } else {
        // Функция удаления данных не задана.
        C_FORWARD_LIST_REMOVE_FEW_BEGIN

        C_FORWARD_LIST_REMOVE_FEW_END
    }

    #undef C_FORWARD_LIST_REMOVE_FEW_BEGIN
    #undef C_FORWARD_LIST_REMOVE_FEW_END

    _forward_list->nodes_count -= count;

    return count;
}

// Очищает список ото всех данных.
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

    // Макросы дублирования кода для исключения проверок внутри цикла.

    // Открытие цикла.
    #define C_FORWARD_LIST_CLEAR_BEGIN\
    /* Обходим все узлы списка и удаляем их */\
    while (select_node != NULL)\
    {\
        delete_node = select_node;\
        select_node = select_node->next_node;

    // Закрытие цикла.
    #define C_FORWARD_LIST_CLEAR_END\
        free(delete_node);\
    }

    // Функция удаления данных узла задана.
    if (_del_data != NULL)
    {
        C_FORWARD_LIST_CLEAR_BEGIN

        _del_data( delete_node->data );

        C_FORWARD_LIST_CLEAR_END
    } else {
        // Функция удаления данных узла не задана.
        C_FORWARD_LIST_CLEAR_BEGIN

        C_FORWARD_LIST_CLEAR_END
    }

    #undef C_FORWARD_LIST_CLEAR_BEGIN
    #undef C_FORWARD_LIST_CLEAR_END

    _forward_list->head = NULL;
    _forward_list->nodes_count = 0;

    return 1;
}

// Возвращает количество данных в списке.
// В случае ошибки возвращает 0, и если _error != NULL, в заданное расположение
// помещается код причины ошибки.
// Так как функция может возвращать 0 и вслучае успеха, и в случае ошибки, для детектирования
// ошибки перед вызовом функции необходимо поместить 0 в заданное расположение ошибки.
size_t c_forward_list_nodes_count(const c_forward_list *const _forward_list,
                                  size_t *const _error)
{
    if (_forward_list == NULL)
    {
        error_set(_error, 1);
        return 0;
    }

    return _forward_list->nodes_count;
}
