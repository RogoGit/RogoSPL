#include <stdio.h>
#include "LinkedList.h"
#include "FunctionPack.h"

int main() {
    int element;
    int demo;
    struct linked_list *list = NULL;
    struct linked_list *demolist = NULL;
    while (scanf("%d",&element) != EOF ) {
        back_add(&list, element);
    }
   // print_this_list(list);
   /* demo = list_get(list,3);                       Tests and working demo
    printf("%d\n",demo);
    front_add(&list,8);
    back_add(&list,9);
    print_this_list(list);
    linked_list_free(&list);
    print_this_list(list); */
    foreach(list,pr_one_line);
    foreach(list,pr_few_line);
    print_this_list(map(list, square));
    int ss = foldl(list,0,summ);
    printf("%d\n",ss);
    map_mut(list, cube);
    print_this_list(list);

    return 0;
}