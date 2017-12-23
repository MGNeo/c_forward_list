# Назначение c_forward_list

Контейнер **c_forward_list** реализует бестиповый односвязный список.

## Пример использования c_forward_list

Создание списка:

```c++
c_forward_list *forward_list = c_forward_list_create();
```

Вставка узла в начало списка и инициализация данных, узел содержит один элемент типа double:

```c++
*( (double*) c_forward_list_push_front(forward_list, sizeof(double) ) = 3.1415f;
```

Вставка узла по заданному индексу и инициализация данных, узел содержит один элемент типа char:

```c++
const size_t index = 3;
*( (char) c_forward_list_insert(forward_list, sizeof(char), index) ) = 'F';
```

Безопасное изменение данных узла, порядковый индекс узла = 55, тип хранящихся в узле данных size_t или аналогичной размерности:

```c++
const size_t index = 55;
*( (size_t*) c_forward_list_at(forward_list, _index) ) = 4;
```

Удаление узла с заданным индексом, узел содержит данные, для которых нет необходимости задавать специализированную функцию удаления (данные узла не содержат указателей на динамические данные, дескрипторы открытых файлов и пр.):

```c++
const size_t index = 17;
c_forward_list_erase(forward_list, index, NULL);
```

Удаление нескольких узлов списка, узлы содержат данные типа float, удаляются все узлы, данные которых > 7.f:

```c++
size_t comp(void *const _data)
{
    if ( *((float*)_data) > 7.f )
    {
        return 1;
    } else {
        return 0;
    }
}
c_forward_list_erase_few(forward_list, comp, NULL);
```

Удаление списка, все узлы которого содержат данные элементарных типов:

```c++
c_forward_list_delete(forward_list, NULL);
forward_list = NULL;
```

Работа с полиморфным списком, обработка возможных сбоев выполняется в целях упрощения кода для восприятия:

```c++
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "c_forward_list.h"

/* 0 - данные типа size_t;
 * 1 - данные типа char*;
 * 2 - данные типа person;
 *
 *
 *
 */

// Данные о человеке.
typedef struct s_person
{
	char *name;
	float mass;
} person;

// Функция полиморфного вывода данных узла. 
void poly_print(void *const _data)
{
	switch (*((uint8_t*)_data))
	{
		case(0) :
		{
			printf("Data type: size_t\n   value: %Iu\n",
				*((size_t*)((uint8_t*)_data + 1)));
			break;
		}
		case(1) :
		{
			printf("Data type: char*\n   value: %s\n", 
				*((char**)((uint8_t*)_data + 1)));
			break;
		}
		case(2) :
		{
			printf("Data type: person\n   name: %s\n   mass: %f\n",
				(*((person*)((uint8_t*)_data + 1))).name,
				(*((person*)((uint8_t*)_data + 1))).mass);
			break;
		}
	}
}

// Функция полиморфного удаления данных узла.
void poly_free(void *const _data)
{

}



int main(int argc, char **argv)
{
	// Создание списка.
	c_forward_list *forward_list = c_forward_list_create();

	// Вставка узла в начало списка, данные вставляемого узла - это uint8_t(метка) + size_t
	void * data = c_forward_list_push_front(forward_list, sizeof(uint8_t) + sizeof(size_t));
	*((uint8_t*)data) = 0;
	*((size_t*)((uint8_t*)data + 1)) = 100500;

	// Вставка узла в конец списка, данные вставляемого узла - это uint8_t(метка) + указатель на строку однобайтовых символов.
	data = c_forward_list_push_back(forward_list, sizeof(uint8_t) + sizeof(char*));
	*((uint8_t*)data) = 1;
	char *str = malloc(3);
	memcpy(str, "ok", 3);
	*((char**)((uint8_t*)data + 1)) = str;

	// Вставка узла по заданному порядковому индексу, данные вставляемого узла - это uint8_t(метка) + 1 элемент типа person.
	data = c_forward_list_insert(forward_list, (sizeof(uint8_t) + sizeof(person)), 1);
	*( (char*) data ) = 2;
	( *((person*)((uint8_t*)data + 1)) ).mass = 79.2f;
	char *name = malloc(5);
	memcpy(name, "maks", 5);
	(*((person*)((uint8_t*)data + 1))).name = name;

	printf("nodes_count: %Iu\n", forward_list->nodes_count);

	// Вывод содержимого полиморфного списка.
	c_forward_list_for_each(forward_list, poly_print);

	// Удаление полиморфного списка.
	c_forward_list_delete(forward_list, poly_free);

	getchar();
	return 0;
}
```
