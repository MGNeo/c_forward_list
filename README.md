# Назначение c_forward_list

Контейнер **c_forward_list** реализует бестиповый односвязный список.

## Пример использования c_forward_list

Создание списка.

```c++
c_forward_list *forward_list = c_forward_list_create();
```

Вставка в начало списка и инициализация.

```c++
*( (double*) c_forward_list_push_front(forward_list, sizeof(double) ) = 3.1415f;
```

Вставка по заданному индексу и инициализация:

```c++
const size_t index = 3;
*( (char) c_forward_list_insert(forward_list, sizeof(char), index) ) = 'F';
```

Безопасное обращение к элементу с заданным индексом:

```c++
const size_t index = 0;
*( (size_t*) c_forward_list_at(forward_list, _index) ) = 4;
```

Удаление узла с заданным индексом, узел содержит данные элементарного типа:

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

Удаление списка, всеузлы которого содержат элементарные данные:

```c++
c_forward_list_delete(forward_list, NULL);
forward_list = NULL;
```
