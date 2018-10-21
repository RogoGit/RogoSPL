//
// Created by dell on 19.10.18.
//
#include "LinkedList.h"
#include <stdio.h>
#include <stdlib.h>

 struct linked_list* map (struct linked_list* link_list, int (*func) (int)) {
    struct linked_list* new_list = NULL;
    while (link_list != NULL) {
        front_add(&new_list, func(link_list->element));
        link_list = link_list->prev_el;
    }
    return new_list;
}

void map_mut (struct linked_list* link_list, int (*func) (int)) {
    while (link_list != NULL) {
        link_list->element = func(link_list->element);
        link_list = link_list->prev_el;
    }
}

int foldl (struct linked_list* link_list, int acc, int (*func) (int,int)) {

    while (link_list != NULL) {
        acc = func(acc,link_list->element);
        link_list = link_list->prev_el;
    }
    return acc;
}

void foreach (struct linked_list* link_list, void (*func) (int)) {
    while (link_list->prev_el != NULL){
        link_list = link_list-> prev_el;
    }
    while (link_list != NULL) {
        func(link_list->element);
        link_list = link_list->next_el;
    }

}