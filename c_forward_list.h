#ifndef C_FORWARD_LIST_H
#define C_FORWARD_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

typedef struct s_c_forward_list
{
    void *head;
    size_t nodes_count;
} c_forward_list;

/* Формат узла односвязного списка:
 *                                  |_____void *next_____|_____uint8_t data[_data_size]_____|
 * Указатель на узел указывает сюда ^
 */

c_forward_list *c_forward_list_create(void);
ptrdiff_t c_forward_list_delete(c_forward_list *const _list, void (*const _del_func(void *const _data)));

void *c_forward_list_push_front(c_forward_list *const _list, const size_t _data_size);
ptrdiff_t c_forward_list_pop_front(c_forward_list *const _list, void (*const _del_func(void *const _data)));

void *c_forward_list_push_back(c_forward_list *const _list, const size_t _data_size);
ptrdiff_t c_forward_list_pop_back(c_forward_list *const _list, void (*const _del_func(void *const _data)));

void *c_forward_list_insert(c_forward_list *const _list, const size_t _data_size, const size_t _index);
ptrdiff_t c_forward_list_erase(c_forward_list *const _list, const size_t _index, void (*const _del_func(void *const _data)));

void *c_forward_list_front(const c_forward_list *const _list);
void *c_forward_list_at(const c_forward_list *const _list, const size_t _index);
void *c_forward_list_back(const c_forward_list *const _list);

ptrdiff_t c_forward_list_for_each(c_forward_list *const _list, void (*const _func(void *const _data)));

size_t c_forward_list_erase_few(c_forward_list *const _list, size_t (*const _comp(void *const _data)),
                                void (*const _del_funct(void *const _data)));

#endif
