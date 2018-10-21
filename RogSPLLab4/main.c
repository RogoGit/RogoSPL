#include <stdio.h>
#include "LinkedList.h"
#include "FunctionPack.h"

int main() {
    int element;
    unsigned int index;
    int sum;
    int maxim;
    int minim;
    struct linked_list *list = NULL;
    struct linked_list *map_list = NULL;
    while (scanf("%d",&element) != EOF ) {
        back_add(&list, element);
    }
    puts("Выведем с помощью foreach");
    foreach(list,pr_one_line);
    foreach(list,pr_few_line);
    printf("\nЭлементов: %d\n",list_length(list));
    printf("Cумма: %d\n",list_sum(list));
    puts("Введите индекс элемента, который хотите получить");
    if (scanf("%d",&index)) {
        printf("%d\n", list_get(list, index));
    } else puts("Нет так нет");
    ///////
    puts("Элементы в квадрате");
    map_list = map(list,square);
    foreach(map_list,pr_one_line);
    puts("\nЭлементы в кубе");
    map_list = map(list,cube);
    foreach(map_list,pr_one_line);
    ///////
    sum = foldl(list,0,summ);
    minim = foldl(list,32768,min);
    maxim = foldl(list,-32767,max);
    ///////
    puts("\nВернемся к нашему списку");
    foreach(list,pr_one_line);
    printf("\nCумма элементов: %d", sum);
    printf("\nМинимальный элемент: %d", minim);
    printf("\nМаксимальный элемент: %d\n", maxim);
    ///////
    puts("\nПерезапишем всё по модулю");
    map_mut(list, module);
    foreach(list,pr_one_line);
    ///////
    puts("\nИтерируем - степени двойки");
    map_list = iterate(1,10,twice);
    foreach(map_list,pr_one_line);
    ////////
    return 0;
}