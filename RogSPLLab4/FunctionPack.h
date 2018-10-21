//
// Created by dell on 19.10.18.
//
#include<stdlib.h>
#ifndef ROGSPLLAB4_FUNCTIONPACK_H
#define ROGSPLLAB4_FUNCTIONPACK_H

int square(int x) {
    return x*x;
}

int cube(int x) {
    return square(x)*x;
}

int summ(int x, int y) {
    return x+y;
}

void pr_one_line(int x) {
    printf("%d ",x);
}

void pr_few_line(int x) {
    printf("\n%d",x);
}

struct linked_list* map (struct linked_list* link_list, int (*func) (int));

struct linked_list* map_mut (struct linked_list* link_list, int (*func) (int));

int foldl (struct linked_list* link_list, int acc, int (*func) (int,int));

void foreach (struct linked_list* link_list, void (*func) (int));

#endif //ROGSPLLAB4_FUNCTIONPACK_H
