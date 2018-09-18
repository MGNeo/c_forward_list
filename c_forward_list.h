/*
    Заголовочный файл односвязного списка c_forward_list
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

#ifndef C_FORWARD_LIST_H
#define C_FORWARD_LIST_H

#include <stddef.h>

typedef struct s_c_forward_list c_forward_list;

c_forward_list *c_forward_list_create(size_t *const _error);

ptrdiff_t c_forward_list_delete(c_forward_list *const _forward_list,
                                void (*const _del_data)(void *const _data));

ptrdiff_t c_forward_list_push_front(c_forward_list *const _forward_list,
                                    const void *const _data);

ptrdiff_t c_forward_list_pop_front(c_forward_list *const _forward_list,
                                   void (*const _del_data)(void *const _data));

ptrdiff_t c_forward_list_push_back(c_forward_list *const _forward_list,
                                   const void *const _data);

ptrdiff_t c_forward_list_pop_back(c_forward_list *const _forward_list,
                                  void (*const _del_data)(void *const _data));

ptrdiff_t c_forward_list_insert(c_forward_list *const _forward_list,
                                const void *const _data,
                                const size_t _index);

ptrdiff_t c_forward_list_erase(c_forward_list *const _forward_list,
                               const size_t _index,
                               void (*const _del_data)(void *const _data));

size_t c_forward_list_erase_few(c_forward_list *const _forward_list,
                                size_t *const _indexes,
                                const size_t _indexes_count,
                                void (*const _del_data)(void *const _data),
                                size_t *const _error);

size_t c_forward_list_remove_few(c_forward_list *const _forward_list,
                                 size_t (*const _pred_data)(const void *const _data),
                                 void (*const _del_data)(void *const _data),
                                 size_t *const _error);

void *c_forward_list_front(const c_forward_list *const _forward_list,
                           size_t *const _error);

void *c_forward_list_at(const c_forward_list *const _forward_list,
                        const size_t _index,
                        size_t *const _error);

void *c_forward_list_back(const c_forward_list *const _forward_list,
                          size_t *const _error);

ptrdiff_t c_forward_list_for_each(c_forward_list *const _forward_list,
                                  void (*const _action_data)(void *const _data));

ptrdiff_t c_forward_list_clear(c_forward_list *const _forward_list,
                               void (*const _del_data)(void *const _data));

size_t c_forward_list_nodes_count(const c_forward_list *const _forward_list,
                                  size_t *const _error);

#endif
