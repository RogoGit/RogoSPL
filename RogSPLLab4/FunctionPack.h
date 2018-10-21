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

int max(int x, int y) {
    int max_n;
    if (x>y) {
        max_n = x;
    } else {
        max_n = y;
    }
    return max_n;
}

int min(int x, int y) {
    int min_n;
    if (x<y) {
        min_n = x;
    } else {
        min_n = y;
    }
    return min_n;
}

int twice(int x) {
    return x*2;
}

int module(int x) {
    if (x>=0) return x;
    else return x*(-1);
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
